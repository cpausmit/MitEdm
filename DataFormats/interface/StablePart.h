//--------------------------------------------------------------------------------------------------
// $Id: StablePart.h,v 1.7 2008/12/01 17:25:58 bendavid Exp $
//
// StablePart
//
// Implementation of a stable particle class for use in general analyses in CMS. The contents of a
// stable particle is basically its link to the track it refers to. This will be hopefully at some
// point a useful and good implementation. See the double dispatcher doAction(MyAction).
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_STABLEPART_H
#define MITEDM_DATAFORMATS_STABLEPART_H

#include <iostream>
#include <cmath>
#include "MitEdm/DataFormats/interface/Types.h"
#include "MitEdm/DataFormats/interface/BasePart.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

namespace mitedm
{
  class BasePartAction;
  class StablePart : public BasePart
  {
    public:
      StablePart() {}
      StablePart(int pid, const mitedm::TrackPtr &trk) : BasePart(pid), track_(trk) {}
      virtual ~StablePart() {}
    
      // Override recursion helper method
      void          doAction(BasePartAction *Action)    const;
      // General printing method
      void          print(std::ostream &os = std::cout) const;

      // Accessors
      const reco::Track      *track()    const { return track_.get();      }
      const mitedm::TrackPtr &trackPtr() const { return track_;            }
      double                  charge()   const { return track()->charge(); }

    private:
      mitedm::TrackPtr        track_; //ptr to track
  };
}
#endif
