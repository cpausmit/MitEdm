// $Id:$

#include <TMath.h>
#include "MitEdm/DataFormats/interface/Types.h"
#include "MitEdm/VertexFitInterface/interface/TrackParameters.h"
#include "MitEdm/VertexFitInterface/interface/HisInterface.h"

using namespace reco;
using namespace mitedm;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
HisInterface::HisInterface(const Track *trk1, const Track *trk2, const double bField) :
  his_(0)
{
  // Create the CMS track parameters
  TrackParameters cmsTrk1(trk1);
  TrackParameters cmsTrk2(trk2);
  // Extract from them the MVF track parameters
  TrackParameters hisTrk1 = cmsTrk1.mvfTrack();
  TrackParameters hisTrk2 = cmsTrk2.mvfTrack();
  // Initialize the corresponding momentum vector
  const TVector3  momTrk1(trk1->px(),trk1->py(),trk1->pz());
  const TVector3  momTrk2(trk2->px(),trk2->py(),trk2->pz());
  // Finally setup the intersection object
  his_ = new HelixIntersector(hisTrk1.pars(),&momTrk1, hisTrk2.pars(),&momTrk2);
}

HisInterface::~HisInterface()
{
  delete his_;
}
