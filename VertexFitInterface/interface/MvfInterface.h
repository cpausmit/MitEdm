//--------------------------------------------------------------------------------------------------
// $Id:$
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
    MvfInterface(MultiVertexFitter *fitter);
    ~MvfInterface() {}

    bool               addTrack(const reco::Track *trk, const int id, const float mass,
				MultiVertexFitter::vertexNumber jv);

    MultiVertexFitter *fitter() { return mvf_;}
  
  private:
    MultiVertexFitter *mvf_; //the multi vertex fitter
  };
}
#endif
