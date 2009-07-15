// $Id: HitDropper.cc,v 1.6 2009/07/12 20:53:07 bendavid Exp $

#include "MitEdm/Producers/interface/HitDropper.h"
#include "DataFormats/TrackingRecHit/interface/InvalidTrackingRecHit.h"
#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"
#include "TrackingTools/GeomPropagators/interface/HelixArbitraryPlaneCrossing.h"
#include "TrackingTools/GeomPropagators/interface/StraightLinePlaneCrossing.h"
#include "TrackingTools/GeomPropagators/interface/StraightLineBarrelCylinderCrossing.h"

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

  const StraightLinePlaneCrossing::PositionType vtxPosition(vtxPos);
  const StraightLinePlaneCrossing::DirectionType trkMomentum(trkMom);
  
  StraightLinePlaneCrossing crossing(vtxPosition,trkMomentum,anyDirection);
  
  reco::HitPattern hitPattern;
  int nHits = 0;
  for (uint hi=0; hi<track->recHitsSize(); ++hi) {
    const TrackingRecHit *hit = track->recHit(hi).get();
    DetId geoId = hit->geographicalId();
    if(geoId == uint32_t(0)) continue;
    const GeomDet *det = trackerGeo_->idToDet(geoId);
    
    //calculate intersection of straight line with plane
    const StraightLinePlaneCrossing::PositionType cPos = crossing.position(det->surface()).second;
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
reco::HitPattern HitDropper::CorrectedHitsAOD(const reco::Track *track,
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

  // *** This function is not working yet ***
  
  return reco::HitPattern();
  
  if (0) {
  
    const StraightLinePlaneCrossing::PositionType vtxPosition(vtxPos);
    const StraightLinePlaneCrossing::DirectionType trkMomentum(trkMom);
    
    const GlobalPoint cVtxPosition(vtxPos.x(), vtxPos.y(), vtxPos.z());
    const GlobalVector cVtxMomentum(trkMom.x(), trkMom.y(), trkMom.z());
    
    StraightLinePlaneCrossing planeCrossing(vtxPosition,trkMomentum,anyDirection);
    StraightLineBarrelCylinderCrossing cylinderCrossing(cVtxPosition,cVtxMomentum,anyDirection);
    
    reco::HitPattern hitPattern;
    int nHits = 0;
    const reco::HitPattern &inhits = track->hitPattern();
    for (Int_t hi=0; hi<inhits.numberOfHits(); hi++) {
      uint32_t hit = inhits.getHitPattern(hi);
      uint32_t layerid = inhits.getLayer(hit);
      //if(layerid == uint32_t(0)) continue;
      //const GeomDet *detg = trackerGeo_->idToDet(layerid);
      //if (!detg) continue;
      //const DetLayer *det = detg->layer();
      const DetLayer *det = trackerGeoSearch_->detLayer(DetId(layerid));
      if (!det) continue;
      
      //calculate intersection of straight line with plane
      //const StraightLinePlaneCrossing::PositionType crossPosition = 
      //  crossing.position(det->surface()).second;
      //const ThreeVector crossPos(crossPosition.x(), crossPosition.y(), crossPosition.z());
      //const ThreeVector delta = crossPos - vtxPos;
      
      Double_t lengthOverSigma = 0;
      
      //calculate distance between vertex and approximate hit position, projected into
      //the appropriate plane/axis and compared to the uncertainty in vertex position
      //in that plane/axis
      if (det->location()==GeomDetEnumerators::barrel) {
        const BarrelDetLayer *barrelDet = static_cast<const BarrelDetLayer*>(det);
        double pathLength = cylinderCrossing.pathLength(barrelDet->specificSurface()).second;
        const GlobalPoint crossPosition = cylinderCrossing.position(pathLength);
        const ThreeVector crossPos(crossPosition.x(), crossPosition.y(), crossPosition.z());
        const ThreeVector delta = crossPos - vtxPos;
        const ThreeVector trkMomXY(trkMom.x(), trkMom.y(), 0.0);
        Double_t deltaXY = delta.Dot(trkMomXY)/trkMomXY.R();
        lengthOverSigma = deltaXY/lxyError;
      }
      else if (det->location()==GeomDetEnumerators::endcap) {
        const ForwardDetLayer *forwardDet = static_cast<const ForwardDetLayer*>(det);
        const StraightLinePlaneCrossing::PositionType cPos = 
          planeCrossing.position(forwardDet->specificSurface()).second;
        const ThreeVector crossPos(cPos.x(), cPos.y(), cPos.z());
        const ThreeVector delta = crossPos - vtxPos;
        Double_t deltaZ = delta.z()*trkMom.z()/fabs(trkMom.z());
        lengthOverSigma = deltaZ/lzError;
      }
      else
        throw edm::Exception(edm::errors::Configuration, "HitDropper::CorrectedHits\n")
          << "Error! Detector element not in a valid barrel or disk layer." << std::endl; 
      
      //add the hit only if it is after the vertex, 
      //allowing for some uncertainty in the vertex position
      if ( lengthOverSigma>(-sigmaTolerance) ) {
        TrackingRecHit::Type hitType = static_cast<TrackingRecHit::Type>(inhits.getHitType(hit));
        InvalidTrackingRecHit dummyhit(layerid, hitType);
        hitPattern.set(dummyhit,nHits);
        printf("inhit = %i, outhit = %i\n",hit,hitPattern.getHitPattern(nHits));
        nHits++;
      } 
    }
    return hitPattern;
  }
}
