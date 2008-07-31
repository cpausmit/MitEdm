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
  class MvfInterface {
    // ---------------------------------------------------------------------------------------------
    // CMS parameter ordering for the vector/matrix, which is assumed here:
    //
    //   qoverp, lambda = pi/2 - theta, phi0, dxy, dsz;
    //   mapping to CDF is therefore { 1, 0*, 4, 3, 2 }, where * indicates that a transformation is
    //                                                   needed
    //
    // Note that the radius of curvature (in cm) is then:
    //
    //   Rc = cos(theta) / [ 0.0029979.... * (q/p) * B ],
    //
    // where B is the magnetic field in Tesla and tht is the angle between the field and the
    // direction. With p * cos(theta) = pT it follows:
    //
    //   Rc = pT / [ 0.0029979.... * q * B ],
    //   fullCurvature = 1 / Rc = 0.0029979 * q * B / pT = - 0.0029979 * B / pT.
    //
    // see the conventions for the MultiVertexFitter in its own header file.
    // ---------------------------------------------------------------------------------------------
  
  public:
    // ---------------------------------------------------------------------------------------------
    // *structors
    // ---------------------------------------------------------------------------------------------
    MvfInterface(MultiVertexFitter *fitter);
    ~MvfInterface() {}
  
    // ---------------------------------------------------------------------------------------------
    // Fundamental funtions
    // ---------------------------------------------------------------------------------------------
    bool addTrack(const reco::Track* trk, const int id, const float mass,
		  MultiVertexFitter::vertexNumber jv);
  
    // ---------------------------------------------------------------------------------------------
    // Accessors
    // ---------------------------------------------------------------------------------------------
    MultiVertexFitter *fitter() { return _mvf; }
  
  private:
    // ---------------------------------------------------------------------------------------------
    // Data members of class
    // ---------------------------------------------------------------------------------------------
    MultiVertexFitter *_mvf;
    double             _fCurv0;
    double             _fCurv;
  };
}
#endif
