//--------------------------------------------------------------------------------------------------
// $Id: StablePart.h,v 1.4 2008/08/28 22:09:16 paus Exp $
//
// StablePart
//
// Implementation of a stable particle class for use in general analyses in CMS. The contents of a
// stable particle is basically its link to the track it refers to. This will be hopefully at some
// point a useful and good implementation. See the double dispatcher doAction(MyAction).
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_STABLEPART_H
#define MITEDM_STABLEPART_H

#include <iostream>
#include <cmath>

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
    StablePart(int pid, const reco::TrackRef &trk) : BasePart(pid), track_(trk) {}
    virtual ~StablePart() {}
    
    // Override recursion helper method
    virtual void          doAction(BasePartAction *Action) const;
    // General printing method
    virtual void          print   (std::ostream &os = std::cout) const;

    // Accessors
    const reco::Track    *track   () const { return track_.get(); }
    const reco::TrackRef &trackRef() const { return track_; }
    double                charge  () const { return track()->charge(); }

  private:
    reco::TrackRef        track_;
  };
}
#endif
