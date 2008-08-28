//==================================================================================================
// $Id $
//
// Description: class TrackParameters
//
// Class to manage track parameters of different conventions and convert one into the other. This is
// not really done very well and could be done more efficient but this is what it is for the moment.
//
// Original Author:  Christoph Paus
// Created:          Thu Aug 21 02:29:00 CEST 2008
//
// -------------------------------------------------------------------------------------------------
// CMS parameter ordering for the vector/matrix, which is assumed here:
//
//   qoverp, lambda, phi0, dxy, dsz;  (lambda = pi/2 - theta)
//   mapping to CDF is therefore { 1*, 0*, 4, 3, 2* }, where * indicates that a transformation is
//                                                     needed
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
//==================================================================================================

#ifndef _VERTEXFITINTERFACE_TRACKPARAMETERS_H_
#define _VERTEXFITINTERFACE_TRACKPARAMETERS_H_

#include "TMatrixDSym.h"
#include "TVectorD.h"
#include "DataFormats/TrackReco/interface/Track.h"

namespace mitedm
{
  // Define existing track parameter conventions
  enum   TrackConvention { iCms, iMvf };

  // Declare the track parameter class
  class TrackParameters
  {
  public:
    // *structors
    TrackParameters() {}
    TrackParameters(const reco::Track     *trk, TrackConvention tcv = iCms, double bField = 3.8);
    TrackParameters(const TrackParameters &trk);
    ~TrackParameters() {};

    // Access the specific contents
    const TVectorD    *pars    ()             const { return &pars_; };
    double             pars    (int i)        const { return pars_[i]; }; 
    const TMatrixDSym *cMat    ()             const { return &cMat_; };
    double             cMat    (int i, int j) const { return cMat_(i,j); };
    
    // Access the different parametrizations independently of the local storage
    TrackParameters    cmsTrack() const;
    TrackParameters    mvfTrack() const;
    
    void               setPars (int i, double v)         { pars_(i)    = v; }
    void               setCMat (int i, int j, double v)  { cMat_(i,j)  = v; }
    void               addCMat (int i, int j, double v)  { cMat_(i,j) += v; }

    // Utilities
    void               print   () const;
    
  private:
    TrackConvention    iConvention_; // track parameter convention (def. CMS)
    double             bField_;      // magnetic field in Tesla
    double             fCurv_;       // combined curvature constant
    TVectorD           pars_;        // track paramters (helix)
    TMatrixDSym        cMat_;        // corresponding covariance matrix (sig_i * sig_j) 
  };
}
#endif
