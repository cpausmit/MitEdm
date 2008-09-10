//--------------------------------------------------------------------------------------------------
// $Id: ProducerV2SS.h,v 1.2 2008/08/29 00:27:21 loizides Exp $
//
// ProducerV2SS
//
// V particle reconstruction (long lived particle, decaying to two stables)
//
// Authors:  C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM__PRODUCERS_ProducerV2SS_H
#define MITEDM_PRODUCERS_ProducerV2SS_H

#include "MitEdm/Producers/interface/D2SS.h"

namespace mitedm
{
  class ProducerV2SS : public D2SS
  {
  public:
    explicit ProducerV2SS(const edm::ParameterSet &cfg);
    ~ProducerV2SS();
    
  protected:
    virtual void beginJob(const edm::EventSetup&);
    virtual void produce (edm::Event&, const edm::EventSetup&);
    virtual void endJob();
  };
}
#endif
