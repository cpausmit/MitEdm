//--------------------------------------------------------------------------------------------------
// $Id: ProducerV2SS.h,v 1.7 2009/03/03 21:31:08 bendavid Exp $
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

    double rhoMin_;         //minimum conversion radius
    double massMin_;        //minimum mass
    double massMax_;        //maximum mass
    double dZMax_;          //maximum dZ0
    bool   useHitDropper_;  //=true if hit dropper is to be used
  };
}
#endif
