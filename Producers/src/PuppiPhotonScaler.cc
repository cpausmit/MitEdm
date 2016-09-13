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

#include <set>

typedef edm::ValueMap<reco::CandidatePtr> CandPtrMap;

// ------------------------------------------------------------------------------------------
PuppiPhotonScaler::PuppiPhotonScaler(const edm::ParameterSet& iConfig) :
  tokenPFCandidates_(consumes<PFView>(iConfig.getParameter<edm::InputTag>("candName"))),
  tokenPuppiCandidates_(consumes<CandidateView>(iConfig.getParameter<edm::InputTag>("puppiCandName"))),
  tokenPhotonCandidates_(consumes<CandidateView>(iConfig.getParameter<edm::InputTag>("photonName"))),
  tokenFootprints_(consumes<PFRefVectorMap>(iConfig.getParameter<edm::InputTag>("footprintsName"))),
  tokenPhotonId_(consumes<BoolMap>(iConfig.getParameter<edm::InputTag>("photonId"))),
  defaultWeight_(iConfig.getParameter<double>("defaultWeight")),
  pt_(iConfig.getParameter<double>("pt")),
  eta_(iConfig.getParameter<double>("eta")),
  matchByRef_(true)
{
  if (iConfig.exists("dRMatch")) {
    matchByRef_ = false;
    dRMatch_ = iConfig.getParameter<std::vector<double>>("dRMatch");
    pdgIds_ = iConfig.getParameter<std::vector<unsigned>>("pdgids");
  }

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

  edm::Handle<CandidateView> hPuppiProduct;
  iEvent.getByToken(tokenPuppiCandidates_, hPuppiProduct);
  const CandidateView *pupCol = hPuppiProduct.product();

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

  std::vector<std::pair<reco::PFCandidate const*, uint16_t>> fpCands;

  int iC(-1);
  for (auto& photon : *phoCol) {
    ++iC;

    if (!photon.isPhoton())
      continue;

    if (photon.pt() < pt_)
      continue;

    auto&& ptr(phoCol->ptrAt(iC));

    if (usePhotonId_ && !(*photonId)[ptr])
      continue;

    for (auto&& fpRef : (*footprints)[ptr]) {
      auto key(fpRef.key());
      auto& fpPF(*pfCol->ptrAt(key));

      if (std::abs(fpPF.eta()) < eta_)
        fpCands.emplace_back(&fpPF, key);
    }
  }

  std::vector<reco::CandidatePtr> values(hPFProduct->size());
  std::auto_ptr<PFOutputCollection> corrCandidates(new PFOutputCollection);

  std::set<reco::PFCandidate const*> matchedFPs;

  int iPup(0);
  for (auto& pupCand : *pupCol) {
    unsigned pupKey(-1);
    if (matchByRef_)
      pupKey = pupCol->refAt(iPup++).key();

    LorentzVector pVec;
    bool scaleP = (pupCand.pt() != 0.);

    for (auto& fpCand : fpCands) {
      auto& fpPF(*fpCand.first);

      if (matchByRef_) {
        if (fpCand.second != pupKey)
          continue;
      }
      else {
        if (!matchCandidate(pupCand, fpPF))
          continue;
      }

      // the lines below make no sense but that's what CMSSW does, as of 8_0_20.
      // we overwrite the candidate momentum with the last matching footprint candidate
      if (scaleP)
        pVec = pupCand.p4() * defaultWeight_ * (fpPF.pt() / pupCand.pt());
      else
        pVec = fpPF.p4() * defaultWeight_;

      matchedFPs.insert(&fpPF);
    }

    auto id = dummySinceTranslateIsNotStatic.translatePdgIdToType(pupCand.pdgId());

    reco::PFCandidate pCand(pupCand.charge(), pVec, id);
    pCand.setSourceCandidatePtr(pupCand.sourceCandidatePtr(0));
    corrCandidates->push_back(pCand);
  }

  // Add the missing pfcandidates
  for (auto& fpCand : fpCands) {
    if (matchedFPs.find(fpCand.first) != matchedFPs.end())
      continue;

    auto& fpPF(*fpCand.first);

    auto id = dummySinceTranslateIsNotStatic.translatePdgIdToType(fpPF.pdgId());

    reco::PFCandidate pCand(fpPF.charge(), fpPF.p4() * defaultWeight_, id);
    pCand.setSourceCandidatePtr(fpPF.sourceCandidatePtr(0));
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
PuppiPhotonScaler::matchCandidate(reco::Candidate const& pf, reco::Candidate const& photon) const
{
  unsigned absId(std::abs(pf.pdgId()));
  unsigned iI(0);
  for (; iI != pdgIds_.size(); ++iI) {
    if (pdgIds_[iI] == absId)
      break;
  }
  if (iI == pdgIds_.size())
    return false;

  double dr(deltaR(pf.eta(), pf.phi(), photon.eta(), photon.phi()));
  return dr < dRMatch_[iI];
}
