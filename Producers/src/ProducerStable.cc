// $Id:$

#include "MitEdm/Producers/interface/ProducerStable.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "MitEdm/DataFormats/interface/Collections.h"
#include "MitEdm/DataFormats/interface/StablePart.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
ProducerStable::ProducerStable(const ParameterSet& cfg) :
  BaseCandProducer(cfg),
  iTracks_(cfg.getUntrackedParameter<string>("iTracks",""))
{
  // Constructor.

  produces<StablePartCol>();
}

//--------------------------------------------------------------------------------------------------
ProducerStable::~ProducerStable()
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
void ProducerStable::produce(Event &evt, const EventSetup &setup)
{
  // Produce our StablePartCol.

  // Get the track input collection
  Handle<TrackCollection> hTrks;
  if (!GetProduct(iTracks_, hTrks, evt))
    return;
  const TrackCollection iTrks = *(hTrks.product());  
  
  // Create the output collection
  auto_ptr<StablePartCol> pS(new StablePartCol());

  // Simple conversion of tracks to stable particles
  for (TrackCollection::const_iterator it = iTrks.begin(); it != iTrks.end(); ++it) {
    const reco::TrackRef theRef(hTrks, it - iTrks.begin());
    StablePart *s = new StablePart(oPid_,theRef);
    if (0)
      printf(" Track: %14.8f, %14.8f, %14.8f\n",it->pt(),it->phi(),it->eta());
    pS->push_back(*s);
    delete s;
  }

  // Write the collection even if it is empty
  if (0) {
    cout << " Stable::produce - " << pS->size() << " entries collection created -"
         << " (Pid: " << oPid_ << ")\n";
  }
  evt.put(pS);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProducerStable);
