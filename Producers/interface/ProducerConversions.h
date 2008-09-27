//--------------------------------------------------------------------------------------------------
// $Id: ProducerConversions.h,v 1.1 2008/09/17 12:49:47 bendavid Exp $
//
// ProducerConversions
//
// Conversion reconstruction using tracks and MultiVertexFitter.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_PRODUCERCONVERSIONS_H
#define MITEDM_PRODUCERS_PRODUCERCONVERSIONS_H

#include "MitEdm/Producers/interface/BaseCandProducer.h"

namespace mitedm
{
  class ProducerConversions : public BaseCandProducer
  {
    public:
      explicit ProducerConversions(const edm::ParameterSet&);
      ~ProducerConversions();
    
    protected:
      void produce(edm::Event&, const edm::EventSetup&);
      
      std::string iStables1_;        //input label first stable particle
      std::string iStables2_;        //input label second stable particle
      bool        convConstraint_;   //todo
      bool        convConstraint3D_; //todo
      double      rhoMin_;           //todo
  };
}
#endif
