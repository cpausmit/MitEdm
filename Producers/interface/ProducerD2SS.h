//--------------------------------------------------------------------------------------------------
// $Id $
//
// ProducerD2SS
//
// Commmon two body particle decay reconstruction (e.g J/psi), special implementations using this as
// a base class exist for V and conversion.
//
// Authors:  C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef _PRODUCERS_ProducerD2SS_H_
#define _PRODUCERS_ProducerD2SS_H_

#include "MitEdm/Producers/interface/BaseCandidate.h"

namespace mitedm
{
  class ProducerD2SS : public BaseCandidate
  {
  public:
    explicit ProducerD2SS(const edm::ParameterSet&);
    ~ProducerD2SS();
    
  protected:
    virtual void beginJob(const edm::EventSetup&);
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    // Parameters to steer the particularities of this instance
    std::string iStables1_;
    std::string iStables2_;
  };
}
#endif
