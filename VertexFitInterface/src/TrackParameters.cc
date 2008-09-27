// $Id:$

#include "MitEdm/VertexFitInterface/interface/TrackParameters.h"
#include "TMath.h"
#include "MitEdm/DataFormats/interface/Types.h"

using namespace std;
using namespace TMath;
using namespace reco;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
TrackParameters::TrackParameters(const Track* trk, const TrackConvention cv, double bField) :
  iConvention_(cv),
  bField_     (bField),
  fCurv_      (0.5 * 0.0029979 * bField_ * -1.0)
{
  // Constructor.

  pars_.ResizeTo(5);
  cMat_.ResizeTo(5,5);
  if (cv == iCms) {
    pars_[0] = trk->qoverp();
    pars_[1] = trk->lambda();
    pars_[2] = trk->phi();
    pars_[3] = trk->dxy();
    pars_[4] = trk->dsz();
    for (int i=0; i<5; i++) {
      for (int j=0; j<5; j++)
	cMat_(i,j) = trk->covariance(i,j);
    }
  }
  else {
    cout << "TrackParameters::TrackParameters -- Requested transfer: CMS -> MVF convention"
	 << " not yet implemented.\n";
  }
}

//--------------------------------------------------------------------------------------------------
TrackParameters::TrackParameters(const TrackParameters &trk) :
  iConvention_(trk.iConvention_),
  bField_     (trk.bField_),
  fCurv_      (trk.fCurv_)
{
  // Constructor.

  pars_.ResizeTo(5);
  cMat_.ResizeTo(5,5);
  for (int i=0; i<5; i++) {
    pars_(i) = trk.pars(i);
    for (int j=0; j<5; j++)
      cMat_(i,j) = trk.cMat(i,j);
  }
}

//--------------------------------------------------------------------------------------------------
TrackParameters TrackParameters::mvfTrack() const
{
  // Return a new set of track parameters in the required MVF convention.

  TrackParameters outTk;
  outTk.pars_.ResizeTo(5);
  outTk.cMat_.ResizeTo(5,5);

  if (iConvention_ == iCms) {
    outTk.setPars(0,1.0/Tan(PiOver2() - pars_[1]));  // cotTheta
    outTk.setPars(1,fCurv_*pars_[0]/Cos(pars_[1]));  // curvature
    outTk.setPars(2,pars_[4]/Cos(pars_[1]));         // z0
    outTk.setPars(3,pars_[3]);                       // d0
    outTk.setPars(4,pars_[2]);                       // phi0
    if (outTk.pars()[4]<0.0)
      outTk.setPars(4,pars_[2] + 2*Pi());            // phi0 [0,2pi)
    // dPidQj(i,j) gives partial dP_i/dQ_j where Pi are the new parameters and Qj are the old ones
    TMatrixD dPidQj(5,5);
    dPidQj(0,1) = 1.0/(Sin(PiOver2() - pars_[1])*Sin(PiOver2() - pars_[1]));
    dPidQj(1,0) = fCurv_/Cos(pars_[1]);
    dPidQj(1,1) = fCurv_*pars_[0]*Tan(pars_[1])/Cos(pars_[1]);
    dPidQj(2,1) = pars_[4]*Tan(pars_[1])/Cos(pars_[1]);
    dPidQj(2,4) = 1.0/Cos(pars_[1]);
    dPidQj(3,3) = 1.0;
    dPidQj(4,2) = 1.0;
    for (int i=0; i<5; i++)
      for (int j=0; j<5; j++) {
	outTk.setCMat(i,j,0.0);
	for (int k=0; k<5; k++)
	  for (int l=0; l<5; l++)
	    outTk.addCMat(i,j,dPidQj(i,k)*dPidQj(j,l)*cMat_(k,l));
      }
  }
  else {
    outTk.setPars(0,pars_[0]);
    outTk.setPars(1,pars_[1]);
    outTk.setPars(2,pars_[2]);
    outTk.setPars(3,pars_[3]);
    outTk.setPars(4,pars_[4]);
    for (int i=0; i<5; i++)
      for (int j=0; j<5; j++) {
	outTk.setCMat(i,j,cMat_(i,j));
      }
  }
  return outTk;
}

TrackParameters TrackParameters::cmsTrack() const
{
  // Return a new set of track parameters in the required CMS convention.

  TrackParameters outTk;
  outTk.pars_.ResizeTo(5);
  outTk.cMat_.ResizeTo(5,5);

  if (iConvention_ == iCms) {
    outTk.setPars(0,pars_[0]);
    outTk.setPars(1,pars_[1]);
    outTk.setPars(2,pars_[2]);
    outTk.setPars(3,pars_[3]);
    outTk.setPars(4,pars_[4]);
    for (int i=0; i<5; i++)
      for (int j=0; j<5; j++) {
	outTk.setCMat(i,j,cMat_(i,j));
      }
  }
  else {
    // Apply MVF to CMS conversion
    outTk.setPars(1,ATan(pars_[0]));                        // lambda
    outTk.setPars(0,pars_[1]*Cos(outTk.pars(1))/fCurv_);    // qoverp
    outTk.setPars(2,pars_[4]);                              // phi
    outTk.setPars(3,pars_[3]);                              // dxy
    outTk.setPars(4,Cos(outTk.pars(1))*pars_[2]);           // dsz
    if (outTk.pars(2)>Pi())
      outTk.setPars(2,pars_[4]-2*Pi());                     // phi0 [-pi,pi)
    // dPidQj(i,j) gives partial dP_i/dQ_j where Pi are the new parameters and Qj are the old ones
    TMatrixD dPidQj(5,5);
    dPidQj(1,0) = 1.0/(1.0+pars_[0]*pars_[0]);
    dPidQj(0,0) = pars_[1]*Sin(outTk.pars(1))*dPidQj(1,0)/fCurv_;
    dPidQj(0,1) = Cos(outTk.pars(1))/fCurv_;
    dPidQj(2,4) = 1.0;
    dPidQj(3,3) = 1.0;
    dPidQj(4,0) = -pars_[2]*Sin(outTk.pars(1))*dPidQj(1,0);
    dPidQj(4,2) = Cos(outTk.pars(1));
    for (int i=0; i<5; i++)
      for (int j=0; j<5; j++) {
	outTk.setCMat(i,j,0.0);
	for (int k=0; k<5; k++)
	  for (int l=0; l<5; l++)
	    outTk.addCMat(i,j,dPidQj(i,k)*dPidQj(j,l)*cMat_(k,l));
      }
  }
  return outTk;
}

//--------------------------------------------------------------------------------------------------
void TrackParameters::print() const
{
  // Print track parameters.

  printf("\n==== Show track ====\n Track parameters with convention %1d.\n",int(iConvention_));
  for (int i=0; i<5; i++)
    printf(" Parameter [%1d]: %8.4f\n",i,pars_[i]);
  printf("Track covariance matrix");
  for (int i=0; i<5; i++) {
    printf("\n CV[%1d,0-4] ",i);
    for (int j=0; j<5; j++)
      printf(" %8.4f",cMat_(i,j));
  }
  printf("\n");
}
