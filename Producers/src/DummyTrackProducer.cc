// $Id: VertexZProducer.cc,v 1.2 2011/04/23 19:13:13 bendavid Exp $

#include "MitEdm/Producers/interface/DummyTrackProducer.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"

using namespace std;
using namespace edm;


//--------------------------------------------------------------------------------------------------
DummyTrackProducer::DummyTrackProducer(const ParameterSet &parameters)
{
  // Constructor.

  produces<reco::TrackCollection>();

}

//--------------------------------------------------------------------------------------------------
DummyTrackProducer::~DummyTrackProducer()
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
void DummyTrackProducer::produce(Event &iEvent, const EventSetup &iSetup) 
{
  // Produce empty track collection.

  // Create the output collection
  auto_ptr<reco::TrackCollection> pD(new reco::TrackCollection());
  iEvent.put(pD);

}


// define this as a plug-in
DEFINE_FWK_MODULE(DummyTrackProducer);

