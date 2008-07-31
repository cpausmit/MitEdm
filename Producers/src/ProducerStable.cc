#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "MitEdm/DataFormats/interface/CollectionsEdm.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "MitEdm/Producers/interface/ProducerStable.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
ProducerStable::ProducerStable(const ParameterSet& cfg) :
  BaseCandidate(cfg),
  iTracks_     (cfg.getUntrackedParameter<string>("iTracks",""))
{
    produces<StablePartCol>();
}

//--------------------------------------------------------------------------------------------------
ProducerStable::~ProducerStable()
{
}

//--------------------------------------------------------------------------------------------------
void ProducerStable::produce(Event &evt, const EventSetup &setup)
{
  // -----------------------------------------------------------------------------------------------
  // Get the track input collection
  // -----------------------------------------------------------------------------------------------
  Handle<TrackCollection> hTrks;
  if (!GetProduct(iTracks_, hTrks, evt))
    return;
  const TrackCollection iTrks = *(hTrks.product());  
  
  // -----------------------------------------------------------------------------------------------
  // Create the output collection
  // -----------------------------------------------------------------------------------------------
  auto_ptr<StablePartCol> pS(new StablePartCol());

  // -----------------------------------------------------------------------------------------------
  // Simple conversion of tracks to stable particles
  // -----------------------------------------------------------------------------------------------
  for (TrackCollection::const_iterator it = iTrks.begin(); it != iTrks.end(); ++it) {
    const reco::TrackRef theRef(hTrks, it - iTrks.begin());
    StablePart *s = new StablePart(oPid_,theRef);
    pS->push_back(*s);
    delete s;
  }

  // -----------------------------------------------------------------------------------------------
  // Write the collection even if it is empty
  // -----------------------------------------------------------------------------------------------
  cout << " Stable::produce - " << pS->size() << " entries collection created -"
       << " (Pid: " << oPid_ << ")\n";
  evt.put(pS);
}

//--------------------------------------------------------------------------------------------------
void ProducerStable::beginJob(const EventSetup &setup)
{
}

//--------------------------------------------------------------------------------------------------
void ProducerStable::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProducerStable);
