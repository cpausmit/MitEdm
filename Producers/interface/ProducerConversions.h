//--------------------------------------------------------------------------------------------------
// $Id: ProducerConversions.h,v 1.2 2008/09/27 05:48:25 loizides Exp $
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
      std::string iPVertexes_;       //input label for primary vertex collection
      bool        usePVertex_;       //compute decay parameters with respect to primary vertex
      bool        convConstraint_;   //use 2d conversion constraint for fit
      bool        convConstraint3D_; //use 3d conversion constraint for fit
      double      rhoMin_;           //minimum conversion radius
  };
}
#endif
