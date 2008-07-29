#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "MitEdm/DataFormats/interface/CollectionsEdm.h"
#include "MitEdm/DataFormats/interface/StablePartEdm.h"
#include "MitEdm/Producers/interface/Stable.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
Stable::Stable(const ParameterSet& cfg) :
  BaseCandidate(cfg),
  iTracks_     (cfg.getUntrackedParameter<string>("iTracks",""))
{
    produces<StablePartCol>();
}

//--------------------------------------------------------------------------------------------------
Stable::~Stable()
{
}

//--------------------------------------------------------------------------------------------------
void Stable::produce(Event &evt, const EventSetup &setup)
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
    StablePartEdm *s = new StablePartEdm(oPid_,oMass_,theRef);
    pS->push_back(*s);
    delete s;
  }

  // -----------------------------------------------------------------------------------------------
  // Write the collection even if it is empty
  // -----------------------------------------------------------------------------------------------
  cout << " Stable::produce - " << pS->size() << " entries collection created -"
       << " (Pid: " << oPid_ << ", Mass: " << oMass_ << ")\n";
  evt.put(pS);
}

//--------------------------------------------------------------------------------------------------
void Stable::beginJob(const EventSetup &setup)
{
}

//--------------------------------------------------------------------------------------------------
void Stable::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(Stable);
