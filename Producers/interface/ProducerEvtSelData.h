//--------------------------------------------------------------------------------------------------
// $Id: ProducerEvtSelData.h,v 1.1 2009/12/07 22:52:27 loizides Exp $
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

  class VertexHit {
    public:
      float z;
      float r;
      float w;
  };

  class ProducerEvtSelData : public edm::EDProducer {
    public:
      explicit ProducerEvtSelData(const edm::ParameterSet&);
      ~ProducerEvtSelData();
    
    private:
      void produce (edm::Event&, const edm::EventSetup&);
      void beginJob (const edm::EventSetup&);
      int  getContainedHits (std::vector<VertexHit> hits, float z0, float & chi);

      std::string srcHF_;     //hf rec hits
      std::string srcHBHE_;   //hbhe rec hits
      std::string srcCastor_; //castor rec hits
      std::string srcZDC_;    //zdc rec hits
      std::string srcPixels_; //pixel rec hits

      const TrackerGeometry* theTracker;
  };

}
#endif
