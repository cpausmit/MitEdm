#include "MitEdm/Producers/interface/MappingCandViewMerger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/CloneTrait.h"
#include "DataFormats/Candidate/interface/Candidate.h"

#include "FWCore/Framework/interface/MakerMacros.h"

MappingCandViewMerger::MappingCandViewMerger(edm::ParameterSet const& cfg) :
  srcTokens_()
{
  auto srcTags(cfg.getParameter<std::vector<edm::InputTag>>("src"));
  for (auto&& tag : srcTags)
    srcTokens_.push_back(consumes<reco::CandidateView>(tag));

  produces<reco::CandidateCollection>();
  produces<CandPtrMap>();
}

void
MappingCandViewMerger::produce(edm::Event& event, edm::EventSetup const&)
{
  std::auto_ptr<reco::CandidateCollection> outCol(new reco::CandidateCollection);
  std::auto_ptr<CandPtrMap> outMap(new CandPtrMap);

  std::vector<edm::Handle<reco::CandidateView>> handles(srcTokens_.size());
  std::vector<std::pair<unsigned, unsigned>> mapIntervals;

  for (unsigned iS(0); iS != srcTokens_.size(); ++iS) {
    auto& token(srcTokens_[iS]);

    event.getByToken(token, handles[iS]);
    auto& product(*handles[iS]);

    mapIntervals.emplace_back(outCol->size(), outCol->size() + product.size());

    for (auto&& cand : product)
      outCol->push_back(cand.clone());
  }

  auto orphanHandle(event.put(outCol));
  std::vector<reco::CandidatePtr> mapValues;

  for (unsigned iKey(0); iKey != orphanHandle->size(); ++iKey)
    mapValues.emplace_back(orphanHandle, iKey);

  for (unsigned iS(0); iS != srcTokens_.size(); ++iS) {
    CandPtrMap map;
    CandPtrMap::Filler filler(map);
    auto& interval(mapIntervals[iS]);

    filler.insert(handles[iS], mapValues.begin() + interval.first, mapValues.begin() + interval.second);
    filler.fill();

    *outMap += map;
  }
  
  event.put(outMap);
}

DEFINE_FWK_MODULE(MappingCandViewMerger);
