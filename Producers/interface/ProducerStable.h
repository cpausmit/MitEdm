//==================================================================================================
// $Id $
//
// Description: class ProducerStable
//
// Converting tracks into ProducerStablePart.
//
// Original Author:  Christoph Paus
// Created:          Mon Jul 21 14:42:57 CEST 2008
//==================================================================================================
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
