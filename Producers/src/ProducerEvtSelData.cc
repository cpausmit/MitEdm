// $Id: ProducerStable.cc,v 1.6 2008/12/01 17:25:58 bendavid Exp $

#include "MitEdm/Producers/interface/ProducerEvtSelData.h"
#include "MitEdm/DataFormats/interface/EvtSelData.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

using namespace std;
using namespace edm;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
ProducerEvtSelData::ProducerEvtSelData(const ParameterSet& cfg)
  : srcHF_(cfg.getUntrackedParameter<string>("hfRecHits","hfreco")),
    srcHBHE_(cfg.getUntrackedParameter<string>("hbheRecHits","hbhereco")),
    srcCastor_(cfg.getUntrackedParameter<string>("castorRecHits","castorreco")),
    srcZDC_(cfg.getUntrackedParameter<string>("zdcRecHits","zdcreco"))
{
  // Constructor.

  produces<EvtSelData>();
}

//--------------------------------------------------------------------------------------------------
ProducerEvtSelData::~ProducerEvtSelData()
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
void ProducerEvtSelData::produce(Event &evt, const EventSetup &setup)
{
  // Produce event selection data for this event.

  double eHfNeg     = 0;
  double eHfPos     = 0;
  double eHfPosTime = 0;
  double eHfNegTime = 0;
  double eHcalNeg   = 0;
  double eHcalPos   = 0;
  double eCaNeg     = 0;
  double eCaPos     = 0;
  double eCaPosTime = 0;
  double eCaNegTime = 0;
  double eZdNeg     = 0;
  double eZdPos     = 0;
  double eZdPosTime = 0;
  double eZdNegTime = 0;

  Handle<HFRecHitCollection> hfhits;
  try {
    evt.getByLabel(edm::InputTag(srcHF_),hfhits);
  } catch (...) {}  
  if (hfhits.isValid()) {
    for(size_t ihit = 0; ihit<hfhits->size(); ++ihit){
      const HFRecHit h = (*hfhits)[ihit];
      double energy = h.energy();
      double time = h.time();
      const HcalDetId id(h.id()); 
      if (id.zside()<0) {
        eHfNeg     += energy;
        eHfNegTime += energy * time;
      } else {
        eHfPos     += energy;
        eHfPosTime += energy * time;
      }
    }
  }

  Handle<HBHERecHitCollection> hbhehits;
  try {
    evt.getByLabel(edm::InputTag(srcHBHE_),hbhehits);
  } catch (...) {}  
  if (hbhehits.isValid()) {
    for(size_t ihit = 0; ihit<hbhehits->size(); ++ihit){
      const HBHERecHit h = (*hbhehits)[ihit];
      double energy = h.energy();
      const HcalDetId id(h.id()); 
      if (id.zside()<0) {
        eHcalNeg   += energy;
      } else {
        eHcalPos   += energy;
      }
    }
  }

  Handle<CastorRecHitCollection> castorhits;
  try {
    evt.getByLabel(edm::InputTag(srcCastor_),castorhits);
  } catch (...) {}  
  if (castorhits.isValid()) {
    for(size_t ihit = 0; ihit<castorhits->size(); ++ihit){
      const CastorRecHit h = (*castorhits)[ihit];
      double energy = h.energy();
      double time = h.time();
      const HcalCastorDetId id(h.id()); 
      if (id.zside()<0) {
        eCaNeg     += energy;
        eCaNegTime += energy * time;
      } else {
        eCaPos     += energy;
        eCaPosTime += energy * time;
      }
    }
  }

  Handle<ZDCRecHitCollection> zdchits;
  try {
    evt.getByLabel(edm::InputTag(srcZDC_),zdchits);
  } catch (...) {}  
  if (castorhits.isValid()) {
    for(size_t ihit = 0; ihit<zdchits->size(); ++ihit){
      const ZDCRecHit h = (*zdchits)[ihit];
      double energy = h.energy();
      double time = h.time();
      const HcalZDCDetId id(h.id()); 
      if (id.zside()<0) {
        eZdNeg     += energy;
        eZdNegTime += energy * time;
      } else {
        eZdPos     += energy;
        eZdPosTime += energy * time;
      }
    }
  }

  if(eHfPos>0)
    eHfPosTime /= eHfPos;
  if(eHfNeg>0)
    eHfNegTime /= eHfNeg;
  if(eCaPos>0)
    eCaPosTime /= eCaPos;
  if(eCaNeg>0)
    eCaNegTime /= eCaNeg;
  if(eZdPos>0)
    eZdPosTime /= eZdPos;
  if(eZdNeg>0)
    eZdNegTime /= eZdNeg;
  
  std::auto_ptr<EvtSelData> output(new EvtSelData(eHfNeg,eHfPos,eHfNegTime,eHfPosTime,
                                                  eHcalNeg, eHcalPos,
                                                  eCaNeg,eCaPos,eCaNegTime,eCaPosTime,
                                                  eZdNeg,eZdPos,eZdNegTime,eZdPosTime));
  evt.put(output);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProducerEvtSelData);
