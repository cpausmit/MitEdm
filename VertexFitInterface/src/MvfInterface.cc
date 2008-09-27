// $Id: MvfInterface.cc,v 1.6 2008/09/04 13:55:29 loizides Exp $

#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/VertexFitInterface/interface/TrackParameters.h"

using namespace reco;
using namespace mitedm;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
MvfInterface::MvfInterface(MultiVertexFitter *fitter) :
  mvf_(fitter)
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
bool MvfInterface::addTrack(const Track *trk, const int id, const float mass,
			    MultiVertexFitter::vertexNumber jv)
{
  // Add a track to the track pool

  TrackParameters cmsTrk(trk);
  TrackParameters mvfTrk = cmsTrk.mvfTrack(); 
  return (mvf_->addTrack(*mvfTrk.pars(),*mvfTrk.cMat(),id,mass,jv));
}
