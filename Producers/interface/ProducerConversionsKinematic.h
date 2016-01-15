//--------------------------------------------------------------------------------------------------
// ProducerConversionsKinematic
//
// Conversion reconstruction using tracks and MultiVertexFitter.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_PRODUCERCONVERSIONSKINEMATIC_H
#define MITEDM_PRODUCERS_PRODUCERCONVERSIONSKINEMATIC_H

#include "MitEdm/Producers/interface/BaseCandProducer.h"
#include "MitEdm/DataFormats/interface/Collections.h"

namespace mitedm
{
  class ProducerConversionsKinematic : public BaseCandProducer
  {
    public:
      explicit ProducerConversionsKinematic(const edm::ParameterSet&);
      ~ProducerConversionsKinematic();
    
    protected:
      void produce(edm::Event&, const edm::EventSetup&);
      
      edm::EDGetTokenT<StablePartCol> iStables1Token_; //input token first stable particle
      edm::EDGetTokenT<StablePartCol> iStables2Token_; //input token second stable particle
      bool        convConstraint_;   //use 2d conversion constraint for fit
      bool        convConstraint3D_; //use 3d conversion constraint for fit
      double      rhoMin_;           //minimum conversion radius
      bool        useRhoMin_;        //apply radius preselection
      bool        useHitDropper_;    //=true if hit dropper is to be used
      bool        applyChargeConstraint_; //require opposite charged tracks
      bool        sameCollection_;
  };
}
#endif
