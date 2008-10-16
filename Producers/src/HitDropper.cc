// $Id: HitDropper.cc,v 1.1 2008/10/13 10:39:23 bendavid Exp $

#include "MitEdm/Producers/interface/HitDropper.h"
#include "DataFormats/TrackingRecHit/interface/InvalidTrackingRecHit.h"
#include "DataFormats/TrajectorySeed/interface/PropagationDirection.h"
#include "TrackingTools/GeomPropagators/interface/HelixArbitraryPlaneCrossing.h"
#include "TrackingTools/GeomPropagators/interface/StraightLinePlaneCrossing.h"

using namespace edm;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
reco::HitPattern HitDropper::CorrectedHits(const reco::TransientTrack *tTrack,
                                      const ThreeVector &vtxPos) const
{
  //return reco::HitPattern structure for the given track with all hits occuring before vtxPos
  //on the track (relative to the primary vertex if given) removed
  //This version of the function uses an iterative helix-plane intersector and does not (yet)
  //take into account the uncertainties in vertex position

  const GlobalPoint vtxPoint(vtxPos.x(),vtxPos.y(),vtxPos.z());
  const TrajectoryStateClosestToPoint vtxTSCP = tTrack->trajectoryStateClosestToPoint(vtxPoint);

  reco::HitPattern hitPattern;
  int nHits = 0;
  for (uint hi=0; hi<tTrack->recHitsSize(); ++hi) {
    const TrackingRecHit *hit = tTrack->recHit(hi).get();
    const GeomDet *det = trackerGeo_->idToDet(hit->geographicalId());
  
    HelixArbitraryPlaneCrossing crossing(HelixPlaneCrossing::PositionType(vtxTSCP.theState().position()),
                                         HelixPlaneCrossing::DirectionType(vtxTSCP.theState().momentum()),
                                         vtxTSCP.theState().transverseCurvature(),
                                         anyDirection);                                        
                                            
    std::pair<bool,double> crossResult = crossing.pathLength(det->surface());
        
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
  //build the transient track and then return the corrected HitPattern

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

  //return reco::HitPattern structure for the given track with all hits occuring before vtxPos
  //on the track (relative to the primary vertex if given) removed
  //This version of the function determines this completely analytically, and taking the
  //vertex position uncertainty into account, which might be important for particles which decay
  //within a tracker layer

  const StraightLinePlaneCrossing::PositionType vtxPosition(vtxPos);
  const StraightLinePlaneCrossing::DirectionType trkMomentum(trkMom);
  
  StraightLinePlaneCrossing crossing(vtxPosition,trkMomentum,anyDirection);
  
  reco::HitPattern hitPattern;
  int nHits = 0;
  for (uint hi=0; hi<track->recHitsSize(); ++hi) {
    const TrackingRecHit *hit = track->recHit(hi).get();
    const GeomDet *det = trackerGeo_->idToDet(hit->geographicalId());
    
    //calculate intersection of straight line with plane
    const StraightLinePlaneCrossing::PositionType crossPosition = crossing.position(det->surface()).second;
    const ThreeVector crossPos(crossPosition.x(), crossPosition.y(), crossPosition.z());
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
    
    //add the hit only if it is after the vertex, allowing for some uncertainty in the vertex position
    if ( lengthOverSigma>(-sigmaTolerance) ) {
      hitPattern.set(*hit,nHits);
      nHits++;
    } 
  }
  
  return hitPattern;
  
}
