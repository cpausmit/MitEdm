//--------------------------------------------------------------------------------------------------
// $Id: ProducerV2SS.h,v 1.6 2008/10/03 23:53:50 loizides Exp $
//
// ProducerV2SS
//
// V particle reconstruction (long lived particle, decaying to two stables)
//
// Authors: C.Paus, M.Rudolph
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_PRODUCERV2SS_H
#define MITEDM_PRODUCERS_PRODUCERV2SS_H

#include "MitEdm/Producers/interface/ProducerD2SS.h"

namespace mitedm
{
  class ProducerV2SS : public ProducerD2SS
  {
  public:
    explicit ProducerV2SS(const edm::ParameterSet &cfg);
    ~ProducerV2SS();
    
  protected:
    virtual void produce (edm::Event&, const edm::EventSetup&);

    double rhoMin_;
    double massMin_;
    double massMax_;
    double dZMax_;
    bool   useHitDropper_;
  };
}
#endif
