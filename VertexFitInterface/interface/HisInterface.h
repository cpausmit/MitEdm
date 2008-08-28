//==================================================================================================
// HisInterface class header file
//
// Author:      Christoph Paus, MIT
// Description: Provide a CMS specific interface for the generic HelixIntersector (His).
// Date:        Jul 18, 2008
//==================================================================================================
#ifndef HISINTERFACE_H
#define HISINTERFACE_H

#include "DataFormats/TrackReco/interface/Track.h"
#include "MitVertex/Helix/interface/HelixIntersector.h"

namespace mitedm
{  
  class HisInterface
  {
  public:
    // ---------------------------------------------------------------------------------------------
    // *structors
    // ---------------------------------------------------------------------------------------------
    HisInterface(const reco::Track *trk1,const reco::Track *trk2, const double bField = 3.8);
    ~HisInterface();
    
    // ---------------------------------------------------------------------------------------------
    // Accessors
    // ---------------------------------------------------------------------------------------------
    const HelixIntersector  *hISector() { return his_; }
  
  private:
    // ---------------------------------------------------------------------------------------------
    // Data members of class
    // ---------------------------------------------------------------------------------------------
    HelixIntersector        *his_;
  };
}
#endif
