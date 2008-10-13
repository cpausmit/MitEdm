// $Id: HitDropperESProducer.h,v 1.1 2008/09/27 05:48:25 loizides Exp $

#include "FWCore/Framework/interface/eventsetupdata_registration_macro.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "MitEdm/Producers/interface/HitDropper.h"
#include "MitEdm/Producers/interface/HitDropperESProducer.h"

using namespace mitedm;

DEFINE_FWK_EVENTSETUP_MODULE(HitDropperESProducer);
EVENTSETUP_DATA_REG(HitDropper);
