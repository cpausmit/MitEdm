//--------------------------------------------------------------------------------------------------
// $Id: ProducerEvtSelData.h,v 1.2 2009/12/08 00:31:46 edwenger Exp $
//
// ProducerEvtSelData
//
// Produce event selection data.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_PRODUCEREVTSELDATA_H
#define MITEDM_PRODUCERS_PRODUCEREVTSELDATA_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class TrackerGeometry;

namespace mitedm
{
  class ProducerEvtSelData : public edm::EDProducer {
    public:
      explicit ProducerEvtSelData(const edm::ParameterSet &cfg);
      ~ProducerEvtSelData();
    
    private:
      struct VertexHit {
        float z;
        float r;
        float w;
      };

      void produce(edm::Event &evt, const edm::EventSetup &setup);
      int  getContainedHits(const std::vector<VertexHit> &hits, double z0, double &chi);

      std::string srcHF_;     //hf rec hits
      std::string srcHBHE_;   //hbhe rec hits
      std::string srcCastor_; //castor rec hits
      std::string srcZDC_;    //zdc rec hits
      std::string srcPixels_; //pixel rec hits
      std::string srcVertex_; //vertex (if not set will use pixel counting vertex)
  };
}
#endif
