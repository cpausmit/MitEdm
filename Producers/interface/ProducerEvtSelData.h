//--------------------------------------------------------------------------------------------------
// $Id: ProducerStable.h,v 1.5 2009/07/15 20:38:24 loizides Exp $
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

namespace mitedm
{
  class ProducerEvtSelData : public edm::EDProducer {
    public:
      explicit ProducerEvtSelData(const edm::ParameterSet&);
      ~ProducerEvtSelData();
    
    private:
      void produce (edm::Event&, const edm::EventSetup&);

      std::string srcHF_;     //hf rec hits
      std::string srcHBHE_;   //hbhe rec hits
      std::string srcCastor_; //castor rec hits
      std::string srcZDC_;    //zdc rec hits
  };
}
#endif
