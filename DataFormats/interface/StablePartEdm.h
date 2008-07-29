//--------------------------------------------------------------------------------------------------
// $Id: StablePartEdm.h,v 1.1 2008/07/29 13:16:22 loizides Exp $
//
// StablePartEdm
//
// Extend the stable class with just the track link. This cannot be ported to the Oak, therefore we
// have to keep it separate.
//
// Author List: Ch.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_STABLEPARTEDM_H
#define MITEDM_STABLEPARTEDM_H

#include <iostream>
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

namespace mitedm
{
  class StablePartEdm : public StablePart
  {
    public:
      // Constructors
      StablePartEdm() {}
      StablePartEdm(int pid, const reco::TrackRef &trk);
      StablePartEdm(int pid, double mass, const reco::TrackRef &trk);
      ~StablePartEdm() {}
    
      // Helpers
      void               convertFromTrack(const reco::Track *trk);
      // Accessors
      const reco::Track *track      () const { return track_.get(); }
      const reco::TrackRef &trackRef()  const { return track_; }
      virtual double     trackCharge() const { return track()->charge(); }

      // General printing method
      virtual void       printEdm   (std::ostream &os = std::cout) const;

    private:
      reco::TrackRef     track_;
  };
}
#endif
