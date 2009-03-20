// $Id: TrackToTrackAssociator.cc,v 1.1 2008/11/04 19:25:56 bendavid Exp $

#include "MitEdm/Producers/interface/TrackToTrackAssociator.h"
#include <TSystem.h>
#include <TError.h>
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "MitEdm/DataFormats/interface/Types.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace mitedm;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
TrackToTrackAssociator::TrackToTrackAssociator(const ParameterSet& cfg) :
  fromTracksName_(cfg.getUntrackedParameter<string>("fromTracks")),
  toTracksName_(cfg.getUntrackedParameter<string>("toTracks"))
{
  // Constructor: Register your base product.

  produces<mitedm::TrackAssociation>();
}

//--------------------------------------------------------------------------------------------------
void TrackToTrackAssociator::PrintErrorAndExit(const char *msg) const
{
  // Print error message and then exit. 

  Error("PrintErrorAndExit", msg);
  gSystem->Exit(1);
}

//--------------------------------------------------------------------------------------------------
void TrackToTrackAssociator::produce(Event &evt, const EventSetup &setup)
{
  // Produce the track association.

  Handle<View<reco::Track> > hFromTrackProduct;
  GetProduct(fromTracksName_, hFromTrackProduct, evt);
  const View<reco::Track> fromTracks = *(hFromTrackProduct.product()); 
  
  Handle<View<reco::Track> > hToTrackProduct;
  GetProduct(toTracksName_, hToTrackProduct, evt);
  const View<reco::Track> toTracks = *(hToTrackProduct.product()); 
  
  auto_ptr<mitedm::TrackAssociation> association(new mitedm::TrackAssociation());
  
  //fill all for each fromTrack, fill an association for all toTracks which share some hits, 
  //using as the association quality the ratio number of matched hits/number of valid hits 
  //on toTrack
  for (View<reco::Track>::const_iterator tFrom = fromTracks.begin();
         tFrom != fromTracks.end(); ++tFrom) {
    
    reco::TrackBaseRef refFrom = fromTracks.refAt(tFrom-fromTracks.begin());
         
    for (View<reco::Track>::const_iterator tTo = toTracks.begin();
         tTo != toTracks.end(); ++tTo) {
         
      uint nShared = 0;
      for (uint i=0; i<tTo->recHitsSize(); ++i) {
        if (tTo->recHit(i)->isValid()) {
          bool matchedHit = false;
          for (uint j=0; j<tFrom->recHitsSize() && !matchedHit; ++j) {
            if ( tTo->recHit(i)->sharesInput(tFrom->recHit(j).get(), TrackingRecHit::some) ) {
              nShared++;
              matchedHit=true;
            }
          }
        }
      }
      
      if (nShared>0) {
        double rShared = (double)nShared/(double)(tTo->numberOfValidHits());
        reco::TrackBaseRef refTo = toTracks.refAt(tTo-toTracks.begin());
        std::pair<TrackBaseRef, double> assocWQuality(refTo,rShared);
        association->insert(refFrom, assocWQuality);
      }
    }
  }
  evt.put(association);
}

// define this as a plug-in
DEFINE_FWK_MODULE(TrackToTrackAssociator);
