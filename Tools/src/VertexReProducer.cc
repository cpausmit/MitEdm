//--------
// Code copied from /UserCode/Mangano/WWAnalysis/Tools/src/VertexReProducer.cc
//--------

#include "MitEdm/Tools/interface/VertexReProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Common/interface/Provenance.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "FWCore/ParameterSet/interface/Registry.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Common/interface/Provenance.h"

VertexReProducer::VertexReProducer(const edm::Handle<reco::VertexCollection> &handle,
				   const edm::Event &iEvent) 
{
  const edm::Provenance *prov = handle.provenance();
  if (prov == 0)
    throw cms::Exception("CorruptData") << "Vertex handle doesn't have provenance.";

  if (edm::moduleName(*prov) != "PrimaryVertexProducer") 
    throw cms::Exception("Configuration")
      << "Vertices to re-produce don't come from a PrimaryVertexProducer, but from a "
      << edm::moduleName(*prov) <<".\n";

  // get paramter set id and then the parameter set
  //edm::ParameterSetID psid = prov->psetID();
  //edm::pset::Registry *psregistry = edm::pset::Registry::instance();
  //edm::ParameterSet /;
  //if (!psregistry->getMapped(psid, psetFromProvenance)) 
  //  throw cms::Exception("CorruptData") << "Vertex handle parameter set ID id = " << psid;

  config_ = edm::parameterSet(*prov);

  //// test whether this is the proper collection
  //if (moduleName(*prov) != "PrimaryVertexProducer") 
  //  throw cms::Exception("Configuration")
  //    << "Vertices to re-produce don't come from a PrimaryVertexProducer, but from a "
  //    << moduleName(*prov) <<".\n";

  algo_.reset(new PrimaryVertexProducerAlgorithm(config_));

  tracksTag_ = config_.getParameter<edm::InputTag>("TrackLabel");
  beamSpotTag_ = config_.getParameter<edm::InputTag>("beamSpotLabel");
}

VertexReProducer::VertexReProducer(const edm::ParameterSet & iConfig) :
  config_(iConfig),
  tracksTag_(iConfig.getParameter<edm::InputTag>("TrackLabel")),
  beamSpotTag_(iConfig.getParameter<edm::InputTag>("beamSpotLabel")),
  algo_(new PrimaryVertexProducerAlgorithm(iConfig))
{
}

std::vector<TransientVertex> 
VertexReProducer::makeVertices(const reco::TrackCollection &tracks, 
                               const reco::BeamSpot &bs, 
                               const edm::EventSetup &iSetup) const 
{
  edm::ESHandle<TransientTrackBuilder> theB;
  iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
  
  std::vector<reco::TransientTrack> t_tks; t_tks.reserve(tracks.size());
  for (reco::TrackCollection::const_iterator it = tracks.begin(), ed = tracks.end();
       it != ed; ++it) {
    t_tks.push_back((*theB).build(*it));
    t_tks.back().setBeamSpot(bs);
  }
  
  return algo_->vertices(t_tks, bs, "");
}
//
//std::string
//VertexReProducer::moduleName(const edm::Provenance &provenance) const 
//{
//  if (!provenance.product().moduleName().empty()) {
//    return provenance.product().moduleName();
//  }
//  if (!provenance.product().moduleNames().size() == 1) {
//    return provenance.product().moduleNames().begin()->second;
//  }
//  std::map<ProcessConfigurationID, std::string>::const_iterator it =
//    provenance.product().moduleNames().find(provenance.product().processConfigurationID());
//  if (it == provenance.product().moduleNames().end()) {
//    return std::string();
//  }
//  return it->second;
//}
