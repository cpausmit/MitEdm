//--------------------------------------------------------------------------------------------------
// $Id: HitDropper.h,v 1.1 2008/09/27 05:48:25 loizides Exp $
//
// HitDropper
//
// Utility to remove hits from a track which are inconsistant with a vertex fit
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_HITDROPPER_H
#define MITEDM_PRODUCERS_HITDROPPER_H

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "MitEdm/DataFormats/interface/Types.h"

namespace mitedm
{
  class HitDropper
  {
    public:
       HitDropper(const TrackerGeometry *geo, const TransientTrackBuilder *builder) :
        trackerGeo_(geo),
        builder_(builder) {}
      ~HitDropper() {}

       reco::HitPattern              CorrectedHits(const reco::TransientTrack *tTrack,
                                      const ThreeVector &vtxPos,
                                      const ThreeVector &pvPos = ThreeVector(0,0,0)) const;
       reco::HitPattern              CorrectedHits(const reco::Track *track,
                                      const ThreeVector &vtxPos,
                                      const ThreeVector &pvPos = ThreeVector(0,0,0)) const;                           
          
    protected:
      const TrackerGeometry         *trackerGeo_;
      const TransientTrackBuilder   *builder_;

  };
  
  //------------------------------------------------------------------------------------------------

}
#endif
