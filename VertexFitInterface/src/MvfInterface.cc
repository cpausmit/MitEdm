#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/DataFormats/interface/Types.h"
#include "TMath.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"

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
  TVectorD    params(5);                                               // track parameters  
  FiveMatrix derivatives;
  TMatrixDSym covmat(5);
  
  
  params[0] = 1.0/TMath::Tan(TMath::PiOver2() - trk->lambda());
  params[1] = _fCurv*trk->qoverp()/TMath::Cos(trk->lambda());
  params[2] = trk->dsz()/TMath::Cos(trk->lambda());
  params[3] = -trk->dxy();
  params[4] = trk->phi();
  
  //derivatives(i,j) gives partial dx_i/dy_j where x are the new parameters and y are the old ones
  derivatives(0,0) = 0;
  derivatives(0,1) = -1.0/(TMath::Sin(TMath::PiOver2() - trk->lambda())*TMath::Sin(TMath::PiOver2() - trk->lambda()));
  derivatives(0,2) = 0;
  derivatives(0,3) = 0;
  derivatives(0,4) = 0;
  derivatives(1,0) = _fCurv/TMath::Cos(trk->lambda());
  derivatives(1,1) = _fCurv*TMath::Abs(trk->qoverp())*TMath::Tan(trk->lambda())/TMath::Cos(trk->lambda());
  derivatives(1,2) = 0;
  derivatives(1,3) = 0;
  derivatives(1,4) = 0;
  derivatives(2,0) = 0;
  derivatives(2,1) = -trk->dsz()*TMath::Tan(trk->lambda())/TMath::Cos(trk->lambda());
  derivatives(2,2) = 0;
  derivatives(2,3) = 0;
  derivatives(2,4) = 1.0/TMath::Cos(trk->lambda());
  derivatives(3,0) = 0;
  derivatives(3,1) = 0;
  derivatives(3,2) = 0;
  derivatives(3,3) = -1.0;
  derivatives(3,4) = 0;
  derivatives(4,0) = 0;
  derivatives(4,1) = 0;
  derivatives(4,2) = 1.0;
  derivatives(4,3) = 0;
  derivatives(4,4) = 0;

  for (Int_t i=0; i<5; i++)
    for (Int_t j=0; j<5; j++) {
      covmat(i,j)=0;
      for (Int_t k=0; k<5; k++)
        for (Int_t l=0; l<5; l++)
          covmat(i,j) += derivatives(i,k)*derivatives(j,l)*trk->covariance(k,l);
    }

  
//   double       fcCosTheta   = _fCurv * cos(trk->theta());
//   double       fcCosThetaSq = fcCosTheta * fcCosTheta;
// 
//   // double loop to fill covariance matrix
//   for (int i=0; i<5; i++) {
//     params[i] = trk->parameter(cms2GenMap[i]);
// 
//     // apply q/p correction
//     if (i == 1)
//       params[i] = params[i] * fcCosTheta;
// 
//     for (int j=i; j<5; j++) {
//       covmat(i,j) = trk->covariance(cms2GenMap[i],cms2GenMap[j]);
// 
//       // apply q/p correction
//       if      (1 == 1 && j == 1)
// 	covmat(i,j) = covmat(i,j) * fcCosThetaSq;
//       else if (i == 1 || j == 1)
// 	covmat(i,j) = covmat(i,j) * fcCosTheta;
// 
//       // force symmetry (should not be needed but well, better apply)
//       if (i != j)
// 	covmat[j][i] = covmat(i,j);
//     }
//   }

  return (_mvf->addTrack(params,covmat,id,mass,jv));
}
