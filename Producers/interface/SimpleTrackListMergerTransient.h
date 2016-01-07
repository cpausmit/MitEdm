//--------------------------------------------------------------------------------------------------
// Original package was:
//  Package:         RecoTracker/FinalTrackSelectors
//  Class:           SimpleTrackListMergerTransient
//  Author: Steve Wagner, stevew@pizero.colorado.edu
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_SiMPLETRACKLISTMERGERTRANSIENT_H
#define MITEDM_PRODUCERS_SIMPLETRACKLISTMERGERTRANSIENT_H


#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
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
      edm::EDGetTokenT<mitedm::StablePartCol> trackCollection1Tag_;
      edm::EDGetTokenT<mitedm::StablePartCol> trackCollection2Tag_;
      double maxNormalizedChisq_;
      double minPT_;
      unsigned minFound_;
      double epsilon_;
      double shareFrac_;
      bool removeDuplicates_;
      unsigned preferCollection_;

      bool copyExtras_;
      bool makeReKeyedSeeds_;
  };
}
#endif
