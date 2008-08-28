#include "MitEdm/VertexFitInterface/interface/TrackParameters.h"
#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"

using namespace reco;
using namespace mitedm;

MvfInterface::MvfInterface(MultiVertexFitter *fitter) :
  mvf_(fitter)
{
}

bool MvfInterface::addTrack(const Track *trk, const int id, const float mass,
			    MultiVertexFitter::vertexNumber jv)
{
  TrackParameters cmsTrk(trk);
  TrackParameters mvfTrk = cmsTrk.mvfTrack(); 
  return (mvf_->addTrack(*mvfTrk.pars(),*mvfTrk.cMat(),id,mass,jv));
}
