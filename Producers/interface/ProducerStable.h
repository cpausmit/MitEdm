//--------------------------------------------------------------------------------------------------
// $Id: ProducerStable.h,v 1.2 2008/08/29 00:27:21 loizides Exp $
//
// ProducerStable
//
// Converting tracks into ProducerStablePart.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef _PRODUCERS_STABLE_H_
#define _PRODUCERS_STABLE_H_

#include "MitEdm/Producers/interface/BaseCandidate.h"

namespace mitedm
{
  class ProducerStable : public BaseCandidate
  {
  public:
    explicit ProducerStable(const edm::ParameterSet&);
    ~ProducerStable();
    
  private:
    virtual void beginJob(const edm::EventSetup&);
    virtual void produce (edm::Event&, const edm::EventSetup&);
    virtual void endJob  ();
    
    std::string iTracks_; 
  };
}
#endif
