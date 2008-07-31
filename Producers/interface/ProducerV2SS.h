//==================================================================================================
// $Id $
//
// Description: class ProducerV2SS
//
// V particle reconstruction (long lived particle, decaying to two stables)
//
// Original Author:  Christoph Paus
// Created:          Mon Jul 21 14:42:57 CEST 2008
//==================================================================================================
#ifndef _PRODUCERS_ProducerV2SS_H_
#define _PRODUCERS_ProducerV2SS_H_

#include "MitEdm/Producers/interface/D2SS.h"

namespace mitedm
{
  class ProducerV2SS : public D2SS
  {
  public:
    explicit ProducerV2SS(const edm::ParameterSet&);
    ~ProducerV2SS();
    
  protected:
    virtual void beginJob(const edm::EventSetup&);
    virtual void produce (edm::Event&, const edm::EventSetup&);
    virtual void endJob  ();
  };
}
#endif
