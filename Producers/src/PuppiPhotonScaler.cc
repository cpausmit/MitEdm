#include "MitEdm/Producers/interface/PuppiPhotonScaler.h"

// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/RefToPtr.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
//#include "DataFormats/Common/interface/Association.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
//Main File

typedef edm::ValueMap<reco::CandidatePtr> CandPtrMap;

// ------------------------------------------------------------------------------------------
PuppiPhotonScaler::PuppiPhotonScaler(const edm::ParameterSet& iConfig) :
  tokenPFCandidates_(consumes<PFView>(iConfig.getParameter<edm::InputTag>("candName"))),
  tokenPuppiCandidates_(consumes<PFView>(iConfig.getParameter<edm::InputTag>("puppiCandName"))),
  tokenPhotonCandidates_(consumes<CandidateView>(iConfig.getParameter<edm::InputTag>("photonName"))),
  tokenFootprints_(consumes<PFRefVectorMap>(iConfig.getParameter<edm::InputTag>("footprintsName"))),
  tokenPhotonId_(consumes<BoolMap>(iConfig.getParameter<edm::InputTag>("photonId"))),
  pt_(iConfig.getParameter<double>("pt")),
  dRMatch_(iConfig.getParameter<std::vector<double>>("dRMatch")),
  pdgIds_(iConfig.getParameter<std::vector<unsigned>>("pdgids"))
{
  usePhotonId_ = !tokenPhotonId_.isUninitialized();

  produces<PFOutputCollection>();
  produces<CandPtrMap>(); 
}

// ------------------------------------------------------------------------------------------
PuppiPhotonScaler::~PuppiPhotonScaler()
{
}

// ------------------------------------------------------------------------------------------
void
PuppiPhotonScaler::produce(edm::Event& iEvent, const edm::EventSetup&)
{
  static const reco::PFCandidate dummySinceTranslateIsNotStatic;

  edm::Handle<CandidateView> hPhoProduct;
  iEvent.getByToken(tokenPhotonCandidates_, hPhoProduct);
  const CandidateView *phoCol = hPhoProduct.product();

  edm::Handle<PFRefVectorMap> hFPProduct;
  iEvent.getByToken(tokenFootprints_, hFPProduct);
  const PFRefVectorMap *footprints = hFPProduct.product();

  edm::Handle<PFView> hPuppiProduct;
  iEvent.getByToken(tokenPuppiCandidates_, hPuppiProduct);
  const PFView *pupCol = hPuppiProduct.product();

  edm::Handle<PFView> hPFProduct;
  iEvent.getByToken(tokenPFCandidates_, hPFProduct);
  const PFView *pfCol = hPFProduct.product();

  if (pfCol->size() != pupCol->size())
    throw cms::Exception("InvalidInput") << "Puppi collection size does not match the PF Collection size";

  BoolMap const* photonId(0);
  if(usePhotonId_) {
    edm::Handle<BoolMap> hPhotonId;
    iEvent.getByToken(tokenPhotonId_, hPhotonId);
    photonId = hPhotonId.product();
  }

  std::vector<reco::PFCandidate const*> matchedPFs;
  std::vector<uint16_t> matchedPFKeys;

  int iC(0);
  for (auto& photon : *phoCol) {
    auto&& ptr(phoCol->ptrAt(iC++));

    if (!photon.isPhoton())
      continue;

    if (photon.pt() < pt_)
      continue;

    if (usePhotonId_) {
      if (!(*photonId)[ptr])
        continue;
    }

    for (auto&& fpRef : (*footprints)[ptr]) {
      auto key(fpRef.key());
      auto& fpPF(*pfCol->ptrAt(key));

      if (matchPFCandidate(fpPF, photon)) {
        matchedPFKeys.push_back(key);
        matchedPFs.push_back(&fpPF);
      }
    }
  }

  std::vector<reco::CandidatePtr> values(hPFProduct->size());
  std::auto_ptr<PFOutputCollection> corrCandidates(new PFOutputCollection);

  int iPF(0);
  for (auto& pupCand : *pupCol) {
    auto&& pupKey(pupCol->refAt(iPF++).key());

    reco::PFCandidate pCand(pupCand);

    float pWeight = 1.;

    for (unsigned iKey(0); iKey != matchedPFKeys.size(); ++iKey) {
      auto& key(matchedPFKeys[iKey]);

      if (key != pupKey)
        continue;

      // are we sure we want just the last matched candidate??
      if (pupCand.pt() != 0)
        pWeight = matchedPFs[iKey]->pt() / pupCand.pt();

      matchedPFs[iKey] = 0; // candidate matched by key; not used any more
    }

    LorentzVector pVec(pupCand.p4());
    if(pupCand.pt() != 0.)
      pVec.SetPxPyPzE(pupCand.px() * pWeight, pupCand.py() * pWeight, pupCand.pz() * pWeight, pupCand.energy() * pWeight);

    pCand.setP4(pVec);
    pCand.setSourceCandidatePtr(pupCand.sourceCandidatePtr(0));
    corrCandidates->push_back(pCand);
  }

  // Add the missing pfcandidates
  for (auto* pf : matchedPFs) {
    if (!pf)
      continue;

    auto id = dummySinceTranslateIsNotStatic.translatePdgIdToType(pf->pdgId());

    reco::PFCandidate pCand(pf->charge(), pf->p4(), id);
    pCand.setSourceCandidatePtr(pf->sourceCandidatePtr(0));
    corrCandidates->push_back(pCand);
  }

  // Fill it into the event
  edm::OrphanHandle<reco::PFCandidateCollection> oh(iEvent.put(corrCandidates));
  for (unsigned iC(0); iC != pupCol->size(); ++iC)
    values[iC] = reco::CandidatePtr(oh, iC);

  std::auto_ptr<CandPtrMap> pfMap_p(new CandPtrMap());
  CandPtrMap::Filler filler(*pfMap_p);
  filler.insert(hPFProduct, values.begin(), values.end());
  filler.fill();
  iEvent.put(pfMap_p);
}

// ------------------------------------------------------------------------------------------
bool
PuppiPhotonScaler::matchPFCandidate(reco::PFCandidate const& pf, reco::Candidate const& photon) const
{
  unsigned absId(std::abs(pf.pdgId()));
  double dr(deltaR(pf.eta(), pf.phi(), photon.eta(), photon.phi()));
  
  for (unsigned iI(0); iI != pdgIds_.size(); ++iI) {
    if (pdgIds_[iI] == absId && dr < dRMatch_[iI])
      return true;
  }
  return false;
}
