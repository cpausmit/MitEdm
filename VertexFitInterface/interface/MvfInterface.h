//--------------------------------------------------------------------------------------------------
// $Id: MvfInterface.h,v 1.4 2008/08/29 00:27:22 loizides Exp $
//
// MvfInterface class header file
//
// Provide a CMS specific interface for the generic MultiVertexFitter.
// 
// Author: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MVFINTERFACE_H
#define MVFINTERFACE_H

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
