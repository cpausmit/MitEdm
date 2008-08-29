//--------------------------------------------------------------------------------------------------
// $Id:$
//
// HisInterface class header file
//
// Provide a CMS specific interface for the generic HelixIntersector (His).
//
// Author: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef HISINTERFACE_H
#define HISINTERFACE_H

#include "DataFormats/TrackReco/interface/Track.h"
#include "MitVertex/Helix/interface/HelixIntersector.h"

namespace mitedm
{  
  class HisInterface
  {
  public:
    HisInterface(const reco::Track *trk1, const reco::Track *trk2, const double bField = 3.8);
    ~HisInterface();

    const HelixIntersector  *hISector() { return his_; }
  
  private:
    HelixIntersector        *his_; // the helix intersector
  };
}
#endif
