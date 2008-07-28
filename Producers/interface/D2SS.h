//==================================================================================================
// $Id $
//
// Description: class D2SS
//
// Commmon two body particle decay reconstruction (e.g J/psi), special implementations using this as
// a base class exist for V and conversion.
//
// Original Author:  Christoph Paus
// Created:          Mon Jul 21 14:42:57 CEST 2008
//==================================================================================================
#ifndef _PRODUCERS_D2SS_H_
#define _PRODUCERS_D2SS_H_

#include "MitEdm/Producers/interface/BaseCandidate.h"

namespace mitedm
{
  class D2SS : public BaseCandidate
  {
  public:
    explicit D2SS(const edm::ParameterSet&);
    ~D2SS();
    
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
