//--------------------------------------------------------------------------------------------------
// $Id: ProducerD2SS.h,v 1.5 2008/10/24 19:57:47 paus Exp $
//
// ProducerD2SS
//
// Commmon two body particle decay reconstruction (e.g J/psi -> mu mu), special implementations
// using this as a base class exist for V and conversion.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_PRODUCERD2SS_H
#define MITEDM_PRODUCERS_PRODUCERD2SS_H

#include "MitEdm/Producers/interface/BaseCandProducer.h"

namespace mitedm
{
  class ProducerD2SS : public BaseCandProducer
  {
    public:
      explicit ProducerD2SS(const edm::ParameterSet&);
      ~ProducerD2SS();
    
    protected:
      void produce(edm::Event&, const edm::EventSetup&);

      // Parameters to steer the particularities of this instance
      std::string iStables1_; // input label first stable particle
      std::string iStables2_; // input label second stable particle
  };
}
#endif
