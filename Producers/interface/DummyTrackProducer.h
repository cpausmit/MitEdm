// $Id: VertexZProducer.h,v 1.1 2009/11/30 10:11:40 loizides Exp $

#ifndef MITEDM_PRODUCERS_DummyTrackProducer_H
#define MITEDM_PRODUCERS_DummyTrackProducer_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <TMath.h>
#include <vector>


class DummyTrackProducer : public edm::EDProducer
{
  public:  
    explicit DummyTrackProducer(const edm::ParameterSet &parameters);
    ~DummyTrackProducer();

  protected:
    void                          produce(edm::Event &iEvent, const edm::EventSetup &iSetup);

};


#endif
