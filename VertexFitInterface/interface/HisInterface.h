//--------------------------------------------------------------------------------------------------
// $Id: HisInterface.h,v 1.3 2008/09/04 13:55:28 loizides Exp $
//
// HisInterface class header file
//
// Provide a CMS specific interface for the generic HelixIntersector (His).
//
// Author: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_VERTEXFITINTERFACE_HISINTERFACE_H
#define MITEDM_VERTEXFITINTERFACE_HISINTERFACE_H

#include "DataFormats/TrackReco/interface/Track.h"
#include "MitVertex/Helix/interface/HelixIntersector.h"

namespace mitedm
{  
  class HisInterface
  {
    public:
      HisInterface(const reco::Track *trk1, const reco::Track *trk2, const double bField = 3.8);
      ~HisInterface();

      const mithep::HelixIntersector  *hISector() { return his_; }
  
    private:
      mithep::HelixIntersector        *his_; // the helix intersector
  };
}
#endif