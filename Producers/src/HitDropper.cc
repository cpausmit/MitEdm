// $Id: BaseCandProducer.cc,v 1.1 2008/09/27 05:48:25 loizides Exp $

#include "MitEdm/Producers/interface/HitDropper.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"

using namespace edm;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
reco::HitPattern HitDropper::CorrectedHits(const reco::TransientTrack *tTrack,
                                      const ThreeVector &vtxPos,
                                      const ThreeVector &pvPos) const
{
  //return reco::HitPattern structure for the given track with all hits occuring before vtxPos
  //on the track (relative to the primary vertex if given) removed
  
  ThreeVector relDecayPos = vtxPos - pvPos;
  
  reco::HitPattern hitPattern;
  int nHits = 0;
  for (uint hi=0; hi<tTrack->recHitsSize(); ++hi) {
    const TrackingRecHit *hit = tTrack->recHit(hi).get();
    GlobalPoint surfacePoint = trackerGeo_->idToDet(hit->geographicalId())->position();
    TrajectoryStateOnSurface hitTSOS = tTrack->stateOnSurface(surfacePoint);
    ThreeVector hitPos(hitTSOS.globalPosition().x(),
                        hitTSOS.globalPosition().y(),
                        hitTSOS.globalPosition().z());
    ThreeVector relHitPos = hitPos - pvPos;
    
    //only take hit if it occurs outside of the decay radius
    if (relHitPos.R() > relDecayPos.R()) {
      hitPattern.set(*hit,nHits);
      nHits++;
    }
  }
  return hitPattern;
}

//--------------------------------------------------------------------------------------------------
reco::HitPattern HitDropper::CorrectedHits(const reco::Track *track,
                                      const ThreeVector &vtxPos,
                                      const ThreeVector &pvPos) const
{
  //build the transient track and then return the corrected HitPattern

  reco::TransientTrack tTrack = builder_->build(track);
  return CorrectedHits(&tTrack, vtxPos, pvPos);
}
