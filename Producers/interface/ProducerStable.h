//--------------------------------------------------------------------------------------------------
// $Id: ProducerStable.h,v 1.5 2009/07/15 20:38:24 loizides Exp $
//
// ProducerStable
//
// Converting tracks into StableParts.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_PRODUCERSTABLE_H
#define MITEDM_PRODUCERS_PRODUCERSTABLE_H

#include "MitEdm/Producers/interface/BaseCandProducer.h"

namespace mitedm
{
  class ProducerStable : public BaseCandProducer
  {
    public:
      explicit ProducerStable(const edm::ParameterSet&);
      ~ProducerStable();
    
    private:
      void produce (edm::Event&, const edm::EventSetup&);
    
      std::string iTracks_; //input label of tracks
  };
}
#endif
