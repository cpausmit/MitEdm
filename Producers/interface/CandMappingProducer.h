//--------------------------------------------------------------------------------------------------
// CandMappingProducer
//
// Produces a ValueMap<CandidatePtr> to connect two collections of same candidates
//
// Authors: Y.Iiyama
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_CANDMAPPINGPRODUCER_H
#define MITEDM_PRODUCERS_CANDMAPPINGPRODUCER_H

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"

class CandMappingProducer : public edm::EDProducer {
  typedef edm::EDGetTokenT<reco::CandidateView> CandViewToken;
  typedef edm::ValueMap<reco::CandidatePtr> CandPtrMap;

 public:
  CandMappingProducer(edm::ParameterSet const&);
  ~CandMappingProducer() {}

 private:
  void produce(edm::Event&, edm::EventSetup const&) override;

  CandViewToken sourceToken_;
  CandViewToken targetToken_;
};

#endif
