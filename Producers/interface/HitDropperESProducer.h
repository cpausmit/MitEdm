//--------------------------------------------------------------------------------------------------
// $Id: HitDropperESProducer.h,v 1.1 2008/09/27 05:48:25 loizides Exp $
//
// HitDropperESProducer
//
// Produces HitDropper helper class for dropping hits from tracks after vertex fit
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_HITDROPPERESPRODUCER_H
#define MITEDM_PRODUCERS_HITDROPPERESPRODUCER_H

#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"


#include "MitEdm/Producers/interface/HitDropperRecord.h"
#include "MitEdm/Producers/interface/HitDropper.h"

#include <boost/shared_ptr.hpp>

namespace mitedm {
  class  HitDropperESProducer: public edm::ESProducer{
  public:
    HitDropperESProducer(const edm::ParameterSet & p);
    virtual ~HitDropperESProducer(); 
    boost::shared_ptr<HitDropper> produce(const HitDropperRecord &);
  private:
    boost::shared_ptr<HitDropper> _dropper;
    edm::ParameterSet pset_;
  };

}
#endif
