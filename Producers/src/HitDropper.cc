// $Id: HitDropper.cc,v 1.11 2009/12/15 23:27:34 bendavid Exp $

#include "MitEdm/Producers/interface/HitDropper.h"
#include "DataFormats/TrackingRecHit/interface/InvalidTrackingRecHit.h"
#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/MuonDetId/interface/DTLayerId.h"
#include "DataFormats/MuonDetId/interface/CSCDetId.h"
#include "DataFormats/MuonDetId/interface/RPCDetId.h"
#include "TrackingTools/GeomPropagators/interface/HelixArbitraryPlaneCrossing.h"

using namespace edm;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
reco::HitPattern HitDropper::CorrectedHits(const reco::TransientTrack *tTrack,
                                           const ThreeVector &vtxPos) const
{
  // Return reco::HitPattern structure for the given track with all hits occuring before vtxPos
  // on the track (relative to the primary vertex if given) removed.
  // This version of the function uses an iterative helix-plane intersector and does not (yet)
  // take into account the uncertainties in vertex position.

  const GlobalPoint vtxPoint(vtxPos.x(),vtxPos.y(),vtxPos.z());
  const TrajectoryStateClosestToPoint vtxTSCP = tTrack->trajectoryStateClosestToPoint(vtxPoint);

  reco::HitPattern hitPattern;
  int nHits = 0;
  for (uint hi=0; hi<tTrack->recHitsSize(); ++hi) {
    const TrackingRecHit *hit = tTrack->recHit(hi).get();
    DetId geoId = hit->geographicalId();
    if(geoId == uint32_t(0)) continue;
    const GeomDet *det = trackerGeo_->idToDet(geoId);
  
    HelixArbitraryPlaneCrossing c(HelixPlaneCrossing::PositionType(vtxTSCP.theState().position()),
                                  HelixPlaneCrossing::DirectionType(vtxTSCP.theState().momentum()),
                                  vtxTSCP.theState().transverseCurvature(),
                                  anyDirection);                                        
                                            
    std::pair<bool,double> crossResult = c.pathLength(det->surface());
    if ( crossResult.first && crossResult.second >= 0 ) {
      hitPattern.set(*hit,nHits);
      nHits++;
    }
  }

  return hitPattern;
}

//--------------------------------------------------------------------------------------------------
reco::HitPattern HitDropper::CorrectedHits(const reco::Track *track,
                                           const ThreeVector &vtxPos) const
{
  // Build the transient track and then return the corrected HitPattern.

  reco::TransientTrack tTrack = builder_->build(track);
  return CorrectedHits(&tTrack, vtxPos);
}

//--------------------------------------------------------------------------------------------------
reco::HitPattern HitDropper::CorrectedHits(const reco::Track *track,
                                           const ThreeVector &vtxPos,
                                           const ThreeVector &trkMom,
                                           Double_t lxyError,
                                           Double_t lzError,
                                           Double_t sigmaTolerance) const
{
  // Return reco::HitPattern structure for the given track with all hits occuring before vtxPos
  // on the track (relative to the primary vertex if given) removed.
  // This version of the function determines this completely analytically, and taking the
  // vertex position uncertainty into account, which might be important for particles which decay
  // within a tracker layer.

  const LocalPoint nullLocal(0,0,0);
  
  reco::HitPattern hitPattern;
  int nHits = 0;
  for (uint hi=0; hi<track->recHitsSize(); ++hi) {
    const TrackingRecHit *hit = track->recHit(hi).get();
    DetId geoId = hit->geographicalId();
    if(geoId == uint32_t(0)) continue;
    const GeomDet *det = trackerGeo_->idToDet(geoId);
  
    const GlobalPoint cPos = det->toGlobal(nullLocal);
    
    const ThreeVector crossPos(cPos.x(), cPos.y(), cPos.z());
    const ThreeVector delta = crossPos - vtxPos;
       
    Double_t lengthOverSigma = 0;
    
    //calculate distance between vertex and approximate hit position, projected into
    //the appropriate plane/axis and compared to the uncertainty in vertex position
    //in that plane/axis
    if (IsBarrel(det)) {
      const ThreeVector trkMomXY(trkMom.x(), trkMom.y(), 0.0);
      Double_t deltaXY = delta.Dot(trkMomXY)/trkMomXY.R();
      lengthOverSigma = deltaXY/lxyError;
    }
    else if (IsDisk(det)) {
      Double_t deltaZ = delta.z()*trkMom.z()/fabs(trkMom.z());
      lengthOverSigma = deltaZ/lzError;
    }
    else
      throw edm::Exception(edm::errors::Configuration, "HitDropper::CorrectedHits\n")
         << "Error! Detector element not in a valid barrel or disk layer." << std::endl; 
    
    //add the hit only if it is after the vertex, 
    //allowing for some uncertainty in the vertex position
    if ( lengthOverSigma>(-sigmaTolerance) ) {
      hitPattern.set(*hit,nHits);
      nHits++;
    } 
  }
  return hitPattern;
}


//--------------------------------------------------------------------------------------------------
std::pair<reco::HitPattern,uint> HitDropper::CorrectedHitsAOD(const reco::Track *track,
                                           const ThreeVector &vtxPos,
                                           const ThreeVector &trkMom,
                                           Double_t lxyError,
                                           Double_t lzError,
                                           Double_t sigmaTolerance) const
{
  // Return reco::HitPattern structure for the given track with all hits occuring before vtxPos
  // on the track (relative to the primary vertex if given) removed.
  // This version of the function determines this completely analytically, and taking the
  // vertex position uncertainty into account, which might be important for particles which decay
  // within a tracker layer.  In order to function in AOD, the tracker geometry is used in an
  // approximate way, at the level of barrel and disk layers.  The layer thickness is taken
  // into account in the uncertainty tolerance.
  

  //Figure out whether we should use the +z or -z side in the case of a disk layer
  int side = 0;
  if (track->pz() < 0)
    side = -1;
  else
    side = 1;
  
  int nHits = 0;
  reco::HitPattern hitPattern;
  const reco::HitPattern* inHitPatterns[3];
  inHitPatterns[0] = &track->hitPattern();
  inHitPatterns[1] = &track->trackerExpectedHitsInner();
  inHitPatterns[2] = &track->trackerExpectedHitsOuter();
  
  uint nWrongHits = 0;
  
  for (Int_t hp=0; hp<3; ++hp) {
    const reco::HitPattern &inhits = *inHitPatterns[hp];
    for (Int_t hi=0; hi<inhits.numberOfHits(); hi++) {
      uint32_t hit = inhits.getHitPattern(hi);
      uint32_t subdet = inhits.getSubStructure(hit);
      uint32_t layerid = inhits.getLayer(hit);
      const DetLayer *det = FindLayer(subdet,layerid,side);
      if (!det) continue;
          
      bool goodHit = false;
      
      //calculate distance between vertex and approximate hit position, projected into
      //the appropriate plane/axis and compared to the uncertainty in vertex position
      //in that plane/axis
      if (det->location()==GeomDetEnumerators::barrel) {
        const BarrelDetLayer *barrelDet = static_cast<const BarrelDetLayer*>(det);
        
        const double barrelRho = barrelDet->specificSurface().radius();
        const double barrelHalfThickness = barrelDet->specificSurface().bounds().thickness()/2.0;
        const ThreeVector crossPos(barrelRho*cos(track->phi()),barrelRho*sin(track->phi()),0.0);
        
        const ThreeVector delta = crossPos - vtxPos;
        const ThreeVector trkMomXY(trkMom.x(), trkMom.y(), 0.0);
        const double deltaXY = delta.Dot(trkMomXY)/trkMomXY.R();
        goodHit = deltaXY>(-sigmaTolerance*lxyError - barrelHalfThickness);
      }
      else if (det->location()==GeomDetEnumerators::endcap) {
        const ForwardDetLayer *forwardDet = static_cast<const ForwardDetLayer*>(det);
        const double diskZ = forwardDet->specificSurface().position().z();
        const double diskHalfThickness = forwardDet->specificSurface().bounds().thickness()/2.0;
        const double deltaZ = (diskZ - vtxPos.z())*trkMom.z()/fabs(trkMom.z());;
        goodHit = deltaZ>(-sigmaTolerance*lzError - diskHalfThickness);
      }
      else
        throw edm::Exception(edm::errors::Configuration, "HitDropper::CorrectedHitsAOD\n")
          << "Error! Detector element not in a valid barrel or disk layer." << std::endl; 
      
      //add the hit only if it is after the vertex, 
      //allowing for some uncertainty in the vertex position
      if ( goodHit ) {
        bool isStereo = inhits.getSide(hit);
        DetId dummyid = det->basicComponents().front()->geographicalId();
        if (isStereo) {
          dummyid = StereoDetId(dummyid);
        }
          
        const TrackingRecHit::Type hitType = static_cast<TrackingRecHit::Type>(inhits.getHitType(hit));
        InvalidTrackingRecHit dummyhit(dummyid, hitType);
        hitPattern.set(dummyhit,nHits);
        if ( hit != hitPattern.getHitPattern(nHits) )
          throw edm::Exception(edm::errors::Configuration, "HitDropper::CorrectedHitsAOD\n")
          << "Error! Mismatch in copying hit pattern." << std::endl; 
          
        nHits++;
      }
      else {
        if (inhits.validHitFilter(hit)) {
          if (inhits.trackerHitFilter(hit)) {
            nWrongHits++;
          }
        }
      }
    }
  }

  return std::pair<reco::HitPattern,uint>(hitPattern,nWrongHits);
}

reco::HitPattern HitDropper::SharedHits(const reco::Track *t1, const reco::Track *t2) const
{
  //Return a hit pattern corresponding to the hits on the two tracks which share clusters
  
  int nHits = 0;
  reco::HitPattern sharedHits;
  
  if (!t1->extra().isAvailable() || !t2->extra().isAvailable())
    return sharedHits;
  
  for (trackingRecHit_iterator iHit1 = t1->recHitsBegin();  iHit1 != t1->recHitsEnd(); ++iHit1) { 
    const TrackingRecHit *hit1 = iHit1->get();
    if (hit1->isValid()) {
      for (trackingRecHit_iterator iHit2 = t2->recHitsBegin();  iHit2 != t2->recHitsEnd(); ++iHit2) { 
        const TrackingRecHit *hit2 = iHit2->get();
        if (hit2->isValid() && hit1->sharesInput(hit2,TrackingRecHit::some)) {
          sharedHits.set(*hit1,nHits);
          nHits++;
        }
      }    
    }
  }
  
  return sharedHits;
  
}

//--------------------------------------------------------------------------------------------------
const DetLayer *HitDropper::FindLayer(int subdet, int layer, int side) const
{
  // Find a given layer.

  switch(subdet) {
    case StripSubdetector::TIB:
      //edm::LogInfo(TkDetLayers) << "TIB layer n: " << TIBDetId(id).layer() ;
      return trackerGeoSearch_->tibLayers().at(layer-1);
      break;

    case StripSubdetector::TOB:
      //edm::LogInfo(TkDetLayers) << "TOB layer n: " << TOBDetId(id).layer() ;
      return trackerGeoSearch_->tobLayers().at(layer-1);
      break;

    case StripSubdetector::TID:
      //edm::LogInfo(TkDetLayers) << "TID wheel n: " << TIDDetId(id).wheel() ;
      if( side == -1 ) {
        return trackerGeoSearch_->negTidLayers().at(layer-1);
      }else if( side == 1 ) {
        return trackerGeoSearch_->posTidLayers().at(layer-1);
      }
      break;

    case StripSubdetector::TEC:
      //edm::LogInfo(TkDetLayers) << "TEC wheel n: " << TECDetId(id).wheel() ;
      if( side == -1 ) {
        return trackerGeoSearch_->negTecLayers().at(layer-1);
      } else if( side == 1 ) {
        return trackerGeoSearch_->posTecLayers().at(layer-1);
      }
      break;

    case PixelSubdetector::PixelBarrel:
      //edm::LogInfo(TkDetLayers) << "PixelBarrel layer n: " << PXBDetId(id).layer() ;
      return trackerGeoSearch_->pixelBarrelLayers().at(layer-1);
      break;

    case PixelSubdetector::PixelEndcap:
      //edm::LogInfo(TkDetLayers) << "PixelEndcap disk n: " << PXFDetId(id).disk() ;
      if(side == -1 ) {
        return trackerGeoSearch_->negPixelForwardLayers().at(layer-1);
      }else if( side == 1 ) {
        return trackerGeoSearch_->posPixelForwardLayers().at(layer-1);
      }
      break;

    default:    
      return 0;
      // throw(something);
  }
  return 0; //just to avoid compile warnings
}

//--------------------------------------------------------------------------------------------------
DetId HitDropper::StereoDetId(const DetId &i) const
{
  // Deal with stereo det id.

  switch (i.det()) {
    case DetId::Tracker:
      switch (i.subdetId()) {
        case PixelSubdetector::PixelBarrel:
        case PixelSubdetector::PixelEndcap:
          return 0;
        case StripSubdetector::TIB:
        {
          TIBDetId id = i;
          if (id.isStereo())
            return id;
          else {
            const std::vector<unsigned int> idString = id.string();
            return TIBDetId(id.layer(),idString[0],idString[1],idString[2],id.module(),1);
          }
        }
        case StripSubdetector::TID:
        {
          TIDDetId id = i;
          if (id.isStereo())
            return id;
          else {
            const std::vector<unsigned int> idModule = id.module();
            return TIDDetId(id.side(),id.wheel(),id.ring(),idModule[0],idModule[1],1);
          }
        }
      case StripSubdetector::TOB:
      {
        TOBDetId id = i;
        if (id.isStereo())
          return id;
        else {
          const std::vector<unsigned int> idRod = id.rod();
          return TOBDetId(id.layer(),idRod[0],idRod[1],id.module(),1);
        }
      }
        case StripSubdetector::TEC:
        {
            TECDetId id = i;
            if (id.isStereo())
              return id;
            else {
              const std::vector<unsigned int> idPetal = id.petal();
              return TECDetId(id.side(),id.wheel(),idPetal[0],idPetal[1],id.ring(),id.module(),1);
            }
        }
        default:
          return 0;
      }
      break;
    default:
      return 0;
  }
}
