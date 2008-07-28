//==================================================================================================
// $Id $
//
// Description: class V2SS
//
// V particle reconstruction (long lived particle, decaying to two stables)
//
// Original Author:  Christoph Paus
// Created:          Mon Jul 21 14:42:57 CEST 2008
//==================================================================================================
#ifndef _PRODUCERS_V2SS_H_
#define _PRODUCERS_V2SS_H_

#include "MitEdm/Producers/interface/D2SS.h"

namespace mitedm
{
  class V2SS : public D2SS
  {
  public:
    explicit V2SS(const edm::ParameterSet&);
    ~V2SS();
    
  protected:
    virtual void beginJob(const edm::EventSetup&);
    virtual void produce (edm::Event&, const edm::EventSetup&);
    virtual void endJob  ();
  };
}
#endif
