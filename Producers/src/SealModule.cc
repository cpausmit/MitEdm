#include "FWCore/Utilities/interface/typelookup.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "MitEdm/Producers/interface/HitDropper.h"
#include "MitEdm/Producers/interface/HitDropperESProducer.h"
#include "MitEdm/Producers/interface/SimpleTrackListMergerTransient.h"
#include "MitEdm/Producers/interface/PuppiPhotonScaler.h"

using namespace mitedm;

DEFINE_FWK_EVENTSETUP_MODULE(HitDropperESProducer);
TYPELOOKUP_DATA_REG(HitDropper);
DEFINE_FWK_MODULE(SimpleTrackListMergerTransient);
DEFINE_FWK_MODULE(PuppiPhotonScaler);
