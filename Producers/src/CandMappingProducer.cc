#include "MitEdm/Producers/interface/CandMappingProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "FWCore/Framework/interface/MakerMacros.h"

CandMappingProducer::CandMappingProducer(edm::ParameterSet const& cfg) :
  sourceToken_(consumes<reco::CandidateView>(cfg.getParameter<edm::InputTag>("source"))),
  targetToken_(consumes<reco::CandidateView>(cfg.getParameter<edm::InputTag>("target")))
{
  produces<CandPtrMap>();
}

void
CandMappingProducer::produce(edm::Event& event, edm::EventSetup const&)
{
  edm::Handle<reco::CandidateView> hSource;
  edm::Handle<reco::CandidateView> hTarget;

  event.getByToken(sourceToken_, hSource);
  event.getByToken(targetToken_, hTarget);

  std::auto_ptr<CandPtrMap> outMap(new CandPtrMap);
  std::vector<reco::CandidatePtr> mapValues;

  for (auto& source : *hSource) {
    unsigned iKey(0);
    for (; iKey != hTarget->size(); ++iKey) {
      if (&hTarget->at(iKey) == &source) {
        mapValues.emplace_back(hTarget, iKey);
        break;
      }
    }
    if (iKey == hTarget->size())
      mapValues.push_back(reco::CandidatePtr());
  }

  CandPtrMap::Filler filler(*outMap);
  filler.insert(hSource, mapValues.begin(), mapValues.end());
  filler.fill();

  event.put(outMap);
}

DEFINE_FWK_MODULE(CandMappingProducer);
