//--------------------------------------------------------------------------------------------------
// MappingCandViewMerger
//
// Extension of CandViewMerger (CommonTools/CandAlgos/plugins/CandViewMerger.cc) which
// produces the mapping from the original input collections to the output (clone) collection
//
// Authors: Y.Iiyama
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_MAPPINGCANDVIEWMERGER_H
#define MITEDM_PRODUCERS_MAPPINGCANDVIEWMERGER_H

#include "FWCore/Framework/interface/EDProducer.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Common/interface/ValueMap.h"

class MappingCandViewMerger : public edm::EDProducer {
  typedef edm::EDGetTokenT<reco::CandidateView> CandViewToken;
  typedef std::vector<CandViewToken> VCandViewToken;
  typedef edm::ValueMap<reco::CandidatePtr> CandPtrMap;

 public:
  MappingCandViewMerger(edm::ParameterSet const&);
  ~MappingCandViewMerger() {}

 private:
  void produce(edm::Event&, edm::EventSetup const&) override;

  VCandViewToken srcTokens_;
};

#endif
