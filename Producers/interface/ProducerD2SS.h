//--------------------------------------------------------------------------------------------------
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
#include "MitEdm/DataFormats/interface/Collections.h"

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
      edm::EDGetTokenT<StablePartCol> iStables1Token_; // input token first stable particle
      edm::EDGetTokenT<StablePartCol> iStables2Token_; // input token second stable particle
      bool   sameCollection_; // true if iStables1 and 2 are the same collection
  };
}
#endif
