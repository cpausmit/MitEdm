// $Id: SealModule.cc,v 1.1 2008/10/13 10:39:24 bendavid Exp $

#include "FWCore/Framework/interface/eventsetupdata_registration_macro.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "MitEdm/Producers/interface/HitDropper.h"
#include "MitEdm/Producers/interface/HitDropperESProducer.h"
#include "MitEdm/Producers/interface/SimpleTrackListMergerTransient.h"


using namespace mitedm;

DEFINE_FWK_EVENTSETUP_MODULE(HitDropperESProducer);
EVENTSETUP_DATA_REG(HitDropper);
DEFINE_ANOTHER_FWK_MODULE(SimpleTrackListMergerTransient);
