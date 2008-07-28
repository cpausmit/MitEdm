#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"

using namespace reco;
using namespace mitedm;

const int cms2GenMap[5] = { 1, 0, 4, 3, 2 }; // parameter map: transfer CMS to generic coordinates

MvfInterface::MvfInterface(MultiVertexFitter *fitter) :
  _mvf   (fitter),
  _fCurv0(0.5 * 0.0029979),
  _fCurv (_fCurv0 * fitter->bField())
{
}

bool MvfInterface::addTrack(const reco::Track* trk, const int id, const float mass,
			    MultiVertexFitter::vertexNumber jv)
{
  // Fill the parameters and matrix compliant with the generic fitter this involves remapping and a
  // simple parameter transformation
  HepVector    params(5);                                               // track parameters
  HepSymMatrix covmat(5,0);                                             // 5x5 matrix filled with 0
  double       fcCosTheta   = _fCurv * cos(trk->theta());
  double       fcCosThetaSq = fcCosTheta * fcCosTheta;

  // double loop to fill covariance matrix
  for (int i=0; i<5; i++) {
    params[i] = trk->parameter(cms2GenMap[i]);

    // apply q/p correction
    if (i == 1)
      params[i] = params[i] * fcCosTheta;

    for (int j=i; j<5; j++) {
      covmat[i][j] = trk->covariance(cms2GenMap[i],cms2GenMap[j]);

      // apply q/p correction
      if      (1 == 1 && j == 1)
	covmat[i][j] = covmat[i][j] * fcCosThetaSq;
      else if (i == 1 || j == 1)
	covmat[i][j] = covmat[i][j] * fcCosTheta;

      // force symmetry (should not be needed but well, better apply)
      if (i != j)
	covmat[j][i] = covmat[i][j];
    }
  }

  return (_mvf->addTrack(params,covmat,id,mass,jv));
}
