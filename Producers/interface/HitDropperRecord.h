//--------------------------------------------------------------------------------------------------
// $Id: HitDropperRecord.h,v 1.2 2008/10/16 16:44:52 bendavid Exp $
//
// HitDropperRecord
//
// Event record for HitDropper class.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_HITDROPPERRECORD_H
#define MITEDM_PRODUCERS_HITDROPPERRECORD_H

#include "FWCore/Framework/interface/EventSetupRecordImplementation.h"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"


#include "boost/mpl/vector.hpp"

namespace mitedm {
  class HitDropperRecord : public edm::eventsetup::DependentRecordImplementation<HitDropperRecord,
                           boost::mpl::vector<TrackerDigiGeometryRecord, TransientTrackRecord,
                           TrackerRecoGeometryRecord> > {};
}
#endif
