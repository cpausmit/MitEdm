//--------------------------------------------------------------------------------------------------
// $Id: MvfInterface.h,v 1.5 2008/09/04 13:55:28 loizides Exp $
//
// MvfInterface class header file
//
// Provide a CMS specific interface for the generic MultiVertexFitter.
// 
// Author: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_VERTEXFITINTERFACE_MVFINTERFACE_H
#define MITEDM_VERTEXFITINTERFACE_MVFINTERFACE_H

#include "DataFormats/TrackReco/interface/Track.h"
#include "MitVertex/Fit/interface/MultiVertexFitter.h"

namespace mitedm
{  
  class MvfInterface
  {
    public:
      MvfInterface(mithep::MultiVertexFitter *fitter);
      ~MvfInterface() {}

      bool                       addTrack(const reco::Track *trk, const int id, const float mass,
                                          mithep::MultiVertexFitter::vertexNumber jv);

      mithep::MultiVertexFitter *fitter() { return mvf_;}
  
    private:
      mithep::MultiVertexFitter *mvf_; //the multi vertex fitter
  };
}
#endif
