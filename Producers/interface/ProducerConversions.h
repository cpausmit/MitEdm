//--------------------------------------------------------------------------------------------------
// $Id: ProducerConversions.h,v 1.3 2008/09/04 13:55:28 loizides Exp $
//
// ProducerConversions
//
// Conversion reconstruction using tracks and MultiVertexFitter
//
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef _PRODUCERS_PRODUCERCONVERSIONS_H_
#define _PRODUCERS_PRODUCERCONVERSIONS_H_

#include "MitEdm/Producers/interface/BaseCandidate.h"

namespace mitedm
{
  class ProducerConversions : public BaseCandidate
  {
  public:
    explicit ProducerConversions(const edm::ParameterSet&);
    ~ProducerConversions();
    
  protected:
    virtual void beginJob(const edm::EventSetup&);
    virtual void produce(edm::Event&, const edm::EventSetup&);
    virtual void endJob();

    // Parameters to steer the particularities of this instance
    std::string iStables1_;
    std::string iStables2_;
    bool convConstraint_;
    bool convConstraint3D_;
    double rhoMin_;
  };
}
#endif
