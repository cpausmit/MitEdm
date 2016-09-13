#ifndef MitEdm_Producers_PuppiPhotonScaler_h
#define MitEdm_Producers_PuppiPhotonScaler_h

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/View.h"

#include <vector>

// Adapted from CommonTools/PileupAlgos/PuppiPhoton

// ------------------------------------------------------------------------------------------
class PuppiPhotonScaler : public edm::stream::EDProducer<> {

 public:
  explicit PuppiPhotonScaler(const edm::ParameterSet&);
  ~PuppiPhotonScaler();

 typedef math::XYZTLorentzVector LorentzVector;
 typedef std::vector<reco::PFCandidate> PFOutputCollection;
 typedef edm::View<reco::PFCandidate> PFView;
 typedef edm::View<reco::Candidate> CandidateView;
 typedef std::vector<reco::PFCandidateRef> PFRefVector;
 typedef edm::ValueMap<PFRefVector> PFRefVectorMap;
 typedef edm::ValueMap<bool> BoolMap;

 private:
 virtual void produce(edm::Event&, const edm::EventSetup&);
 bool matchCandidate(reco::Candidate const&, reco::Candidate const&) const;

 edm::EDGetTokenT<PFView> tokenPFCandidates_;
 edm::EDGetTokenT<CandidateView> tokenPuppiCandidates_;
 edm::EDGetTokenT<CandidateView> tokenPhotonCandidates_;
 edm::EDGetTokenT<PFRefVectorMap> tokenFootprints_;
 edm::EDGetTokenT<BoolMap> tokenPhotonId_; 

 double defaultWeight_;
 double pt_;
 double eta_;
 bool matchByRef_;
 std::vector<double> dRMatch_;
 std::vector<unsigned> pdgIds_;
 bool usePhotonId_;
};

#endif
