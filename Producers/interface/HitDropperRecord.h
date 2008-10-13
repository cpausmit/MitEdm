//--------------------------------------------------------------------------------------------------
// $Id: HitDropperESProducer.h,v 1.1 2008/09/27 05:48:25 loizides Exp $
//
// HitDropperRecord
//
// Event record for HitDropper class
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_HITDROPPERRECORD_H
#define MITEDM_PRODUCERS_HITDROPPERRECORD_H

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"


#include "boost/mpl/vector.hpp"

namespace mitedm {
  class HitDropperRecord : public edm::eventsetup::DependentRecordImplementation<HitDropperRecord,
    boost::mpl::vector<TrackerDigiGeometryRecord, TransientTrackRecord> > {};
}
#endif
