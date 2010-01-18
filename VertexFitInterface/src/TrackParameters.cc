// $Id: TrackParameters.cc,v 1.2 2008/09/27 05:48:26 loizides Exp $

#include "MitEdm/VertexFitInterface/interface/TrackParameters.h"
#include "TMath.h"
#include "MitEdm/DataFormats/interface/Types.h"
#include "MagneticField/Engine/interface/MagneticField.h"

using namespace std;
using namespace TMath;
using namespace reco;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
TrackParameters::TrackParameters(const TransientTrack &ttrk, const TrackConvention cv) :
  iConvention_(cv),
  bField_     (ttrk.field()->inTesla(GlobalPoint(0.,0.,0.)).z()),
  fCurv_      (0.5 * 0.0029979 * bField_ * -1.0)
{
  // Constructor from TransientTrack.  Track parameters are propagated back to PCA to (0,0,0)
  // in order to be consistent with the later transformation to the mvf helix
  // parameterization.  Magnetic field is also taken from the TransientTrack in this case.

  TrajectoryStateClosestToPoint traj = ttrk.trajectoryStateClosestToPoint(GlobalPoint(0.,0.,0.));
  
  GlobalPoint v = traj.theState().position();
  math::XYZPoint  pos( v.x(), v.y(), v.z() );
  GlobalVector p = traj.theState().momentum();
  math::XYZVector mom( p.x(), p.y(), p.z() );
 
  //construct intermediate reco track object, just so that we can be sure we are using the
  //correct accessors for the helix parameters
  reco::Track trk(ttrk.track().chi2(),
                  ttrk.track().ndof(),
                  pos, mom, traj.theState().charge(), 
                  traj.theState().curvilinearError(),
                  ttrk.track().algo());
  

  pars_.ResizeTo(5);
  cMat_.ResizeTo(5,5);
  if (cv == iCms) {
    pars_[0] = trk.qoverp();
    pars_[1] = trk.lambda();
    pars_[2] = trk.phi();
    pars_[3] = trk.dxy();
    pars_[4] = trk.dsz();
    for (int i=0; i<5; i++) {
      for (int j=0; j<5; j++)
        cMat_(i,j) = trk.covariance(i,j);
    }
  }
  else {
    cout << "TrackParameters::TrackParameters -- Requested transfer: CMS -> MVF convention"
         << " not yet implemented.\n";
  }
}


//--------------------------------------------------------------------------------------------------
TrackParameters::TrackParameters(const Track* trk, const TrackConvention cv, double bField) :
  iConvention_(cv),
  bField_     (bField),
  fCurv_      (0.5 * 0.0029979 * bField_ * -1.0)
{
  // Constructor from reco::Track.  WARNING, if the helix parameters and covariance matrix stored
  // in the track are not corresponding to the PCA to (0,0,0) (NOT the case by default) then one
  // can get inconsistent results.

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
    double phi = pars_[2];
    while (phi<0.0)
      phi += 2*Pi();
    while (phi>2*Pi())
      phi -= 2*Pi();
    outTk.setPars(4,phi);            // phi0 [0,2pi)
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
    outTk.setPars(3,pars_[3]);                              // dxy
    outTk.setPars(4,Cos(outTk.pars(1))*pars_[2]);           // dsz
    double phi = pars_[4];
    while (phi>Pi())
      phi -= 2*Pi();                     // phi0 [-pi,pi)
    while (phi<-Pi())
      phi += 2*Pi();
    outTk.setPars(2,phi);                     // phi0 [-pi,pi)
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
