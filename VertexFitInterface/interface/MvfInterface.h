//==================================================================================================
// MvfInterface class header file
//
// Author:      Christoph Paus, MIT
// Description: Provide a CMS specific interface for the generic MultiVertexFitter.
// Date:        Jul 18, 2008
//==================================================================================================
#ifndef MVFINTERFACE_H
#define MVFINTERFACE_H

#include "DataFormats/TrackReco/interface/Track.h"
#include "MitVertex/Fit/interface/MultiVertexFitter.h"

namespace mitedm
{  
  // Declare the class
  class MvfInterface
  {
  public:
    // ---------------------------------------------------------------------------------------------
    // *structors
    // ---------------------------------------------------------------------------------------------
    MvfInterface(MultiVertexFitter *fitter);
    ~MvfInterface() {}
  
    // ---------------------------------------------------------------------------------------------
    // Fundamental functions
    // ---------------------------------------------------------------------------------------------
    bool               addTrack(const reco::Track *trk, const int id, const float mass,
				MultiVertexFitter::vertexNumber jv);
  
    // ---------------------------------------------------------------------------------------------
    // Accessors
    // ---------------------------------------------------------------------------------------------
    MultiVertexFitter *fitter() { return mvf_;}
  
  private:
    // ---------------------------------------------------------------------------------------------
    // Data members of class
    // ---------------------------------------------------------------------------------------------
    MultiVertexFitter *mvf_;
  };
}
#endif
