//--------------------------------------------------------------------------------------------------
// $Id: ProducerConversionsKinematic.h,v 1.8 2010/01/18 14:41:33 bendavid Exp $
//
// ProducerConversionsKinematic
//
// Conversion reconstruction using tracks and MultiVertexFitter.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_PRODUCERCONVERSIONSKINEMATIC_H
#define MITEDM_PRODUCERS_PRODUCERCONVERSIONSKINEMATIC_H

#include "MitEdm/Producers/interface/BaseCandProducer.h"

namespace mitedm
{
  class ProducerConversionsKinematic : public BaseCandProducer
  {
    public:
      explicit ProducerConversionsKinematic(const edm::ParameterSet&);
      ~ProducerConversionsKinematic();
    
    protected:
      void produce(edm::Event&, const edm::EventSetup&);
      
      std::string iStables1_;        //input label first stable particle
      std::string iStables2_;        //input label second stable particle
      std::string iPVertexes_;       //input label for primary vertex collection
      bool        usePVertex_;       //compute decay parameters with respect to primary vertex
      bool        convConstraint_;   //use 2d conversion constraint for fit
      bool        convConstraint3D_; //use 3d conversion constraint for fit
      double      rhoMin_;           //minimum conversion radius
      bool        useRhoMin_;        //apply radius preselection
      bool        useHitDropper_;    //=true if hit dropper is to be used
      bool        applyChargeConstraint_; //require opposite charged tracks
  };
}
#endif
