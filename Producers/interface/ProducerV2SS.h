//--------------------------------------------------------------------------------------------------
// $Id: ProducerV2SS.h,v 1.9 2009/07/15 20:38:24 loizides Exp $
//
// ProducerV2SS
//
// Producer for V particle reconstruction (long lived particle, decaying to two stable particles).
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
