#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/DataFormats/interface/Types.h"
#include "TMath.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"

using namespace TMath;
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
  FiveMatrix  dPidPj;
  TMatrixDSym covmat(5);
  
  params[0] = 1.0/Tan(PiOver2() - trk->lambda());               //cotTheta
  params[1] = _fCurv*trk->qoverp()/Cos(trk->lambda());          //curvature
  params[2] = trk->dsz()/Cos(trk->lambda());                    //z0
  params[3] = -trk->dxy();                                      //d0
  params[4] = trk->phi();                                       //phi0
  
  // dPidPj(i,j) gives partial dx_i/dy_j where x are the new parameters and y are the old ones
  dPidPj(0,0) = 0;
  dPidPj(0,1) = -1.0/(Sin(PiOver2() - trk->lambda())*Sin(PiOver2() - trk->lambda()));
  dPidPj(0,2) = 0;
  dPidPj(0,3) = 0;
  dPidPj(0,4) = 0;
  dPidPj(1,0) = _fCurv/Cos(trk->lambda());
  dPidPj(1,1) = _fCurv*Abs(trk->qoverp())*Tan(trk->lambda())/Cos(trk->lambda());
  dPidPj(1,2) = 0;
  dPidPj(1,3) = 0;
  dPidPj(1,4) = 0;
  dPidPj(2,0) = 0;
  dPidPj(2,1) = -trk->dsz()*Tan(trk->lambda())/Cos(trk->lambda());
  dPidPj(2,2) = 0;
  dPidPj(2,3) = 0;
  dPidPj(2,4) = 1.0/Cos(trk->lambda());
  dPidPj(3,0) = 0;
  dPidPj(3,1) = 0;
  dPidPj(3,2) = 0;
  dPidPj(3,3) = -1.0;
  dPidPj(3,4) = 0;
  dPidPj(4,0) = 0;
  dPidPj(4,1) = 0;
  dPidPj(4,2) = 1.0;
  dPidPj(4,3) = 0;
  dPidPj(4,4) = 0;

  for (Int_t i=0; i<5; i++)
    for (Int_t j=0; j<5; j++) {
      covmat(i,j)=0;
      for (Int_t k=0; k<5; k++)
        for (Int_t l=0; l<5; l++)
          covmat(i,j) += dPidPj(i,k)*dPidPj(j,l)*trk->covariance(k,l);
    }

  return (_mvf->addTrack(params,covmat,id,mass,jv));
}
