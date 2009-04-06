// $Id: FillerTracks.cc,v 1.30 2009/03/19 22:19:16 loizides Exp $

#include "MitEdm/TrackerElectrons/interface/LoadTracks.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

using namespace std;
using namespace edm;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
LoadTracks::LoadTracks(const edm::ParameterSet &cfg) : 
  edmName_(cfg.getUntrackedParameter<string>("edmName", "pixelMatchGsfFit"))
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
LoadTracks::~LoadTracks()
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
void LoadTracks::analyze(const edm::Event      &event, 
                         const edm::EventSetup &setup)
{
  // Initialize handle and get track product.

  Handle<View<reco::Track> > hTrackProduct;
  event.getByLabel(edm::InputTag(edmName_), hTrackProduct);
}

// Define this as a plug-in
DEFINE_FWK_MODULE(LoadTracks);
