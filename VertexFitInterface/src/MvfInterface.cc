// $Id: MvfInterface.cc,v 1.9 2009/12/07 16:27:37 mrudolph Exp $

#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/VertexFitInterface/interface/TrackParameters.h"

using namespace reco;
using namespace mitedm;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
MvfInterface::MvfInterface(MultiVertexFitter *fitter) :
  mvf_(fitter)
{
  // Constructor
}

//--------------------------------------------------------------------------------------------------
MvfInterface::MvfInterface(MultiVertexFitterC *fitter) :
  mvfC_(fitter)
{
  // Constructor
}

//--------------------------------------------------------------------------------------------------
MvfInterface::MvfInterface(MultiVertexFitterD *fitter) :
  mvfD_(fitter)
{
  // Constructor
}

//--------------------------------------------------------------------------------------------------
bool MvfInterface::addTrack(const Track *trk, const int id, const float mass,
			    MultiVertexFitter::vertexNumber jv)
{
  // Add a track to the track pool

  TrackParameters cmsTrk(trk,iCms, mvf_->bField());
  TrackParameters mvfTrk = cmsTrk.mvfTrack(); 
  return (mvf_->addTrack(*mvfTrk.pars(),*mvfTrk.cMat(),id,mass,jv));
}

//--------------------------------------------------------------------------------------------------
bool MvfInterface::addTrack(const Track *trk, const int id, const float mass,
			    MultiVertexFitterC::vertexNumber jv)
{
  // Add a track to the track pool

  TrackParameters cmsTrk(trk,iCms, mvfC_->bField());
  TrackParameters mvfTrk = cmsTrk.mvfTrack(); 
  return (mvfC_->addTrack(*mvfTrk.pars(),*mvfTrk.cMat(),id,mass,jv));
}

//--------------------------------------------------------------------------------------------------
bool MvfInterface::addTrack(const Track *trk, const int id, const float mass,
			    MultiVertexFitterD::vertexNumber jv)
{
  // Add a track to the track pool

  TrackParameters cmsTrk(trk,iCms, mvfD_->bField());
  TrackParameters mvfTrk = cmsTrk.mvfTrack(); 
  return (mvfD_->addTrack(*mvfTrk.pars(),*mvfTrk.cMat(),id,mass,jv));
}

//--------------------------------------------------------------------------------------------------
bool MvfInterface::addTrack(const TransientTrack &trk, const int id, const float mass,
                            MultiVertexFitter::vertexNumber jv)
{
  // Add a track to the track pool

  TrackParameters cmsTrk(trk,iCms);
  TrackParameters mvfTrk = cmsTrk.mvfTrack(); 
  return (mvf_->addTrack(*mvfTrk.pars(),*mvfTrk.cMat(),id,mass,jv));
}

//--------------------------------------------------------------------------------------------------
bool MvfInterface::addTrack(const TransientTrack &trk, const int id, const float mass,
                            MultiVertexFitterC::vertexNumber jv)
{
  // Add a track to the track pool

  TrackParameters cmsTrk(trk,iCms);
  TrackParameters mvfTrk = cmsTrk.mvfTrack(); 
  return (mvfC_->addTrack(*mvfTrk.pars(),*mvfTrk.cMat(),id,mass,jv));
}

//--------------------------------------------------------------------------------------------------
bool MvfInterface::addTrack(const TransientTrack &trk, const int id, const float mass,
                            MultiVertexFitterD::vertexNumber jv)
{
  // Add a track to the track pool

  TrackParameters cmsTrk(trk,iCms);
  TrackParameters mvfTrk = cmsTrk.mvfTrack(); 
  return (mvfD_->addTrack(*mvfTrk.pars(),*mvfTrk.cMat(),id,mass,jv));
}
