//--------------------------------------------------------------------------------------------------
// $Id:$
//
// Original package was:
//  Package:         RecoTracker/FinalTrackSelectors
//  Class:           SimpleTrackListMergerTransient
//  Author: Steve Wagner, stevew@pizero.colorado.edu
//
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_SiMPLETRACKLISTMERGERTRANSIENT_H
#define MITEDM_PRODUCERS_SIMPLETRACKLISTMERGERTRANSIENT_H


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/EDProduct.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "MitEdm/DataFormats/interface/Collections.h"

namespace mitedm
{
  class SimpleTrackListMergerTransient : public edm::EDProducer
  {
    public:
      explicit SimpleTrackListMergerTransient(const edm::ParameterSet& conf);
      ~SimpleTrackListMergerTransient();

      void produce(edm::Event &e, const edm::EventSetup &c);

  private:
    edm::ParameterSet conf_;

    std::auto_ptr<mitedm::StablePartCol > outputTrks;

    reco::TrackRefProd refTrks;
    reco::TrackExtraRefProd refTrkExtras;
    TrackingRecHitRefProd refTrkHits;
    //edm::RefProd< std::vector<Trajectory> > refTrajs;
    //std::vector<reco::TrackRef> trackRefs;
    //edm::RefProd< TrajectorySeedCollection > refTrajSeeds;
    bool copyExtras_;
    bool makeReKeyedSeeds_;
  };
}
#endif
