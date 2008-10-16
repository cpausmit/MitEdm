// $Id: HitDropperESProducer.cc,v 1.1 2008/10/13 10:39:23 bendavid Exp $

#include "MitEdm/Producers/interface/HitDropperESProducer.h"
#include "Geometry/Records/interface/GlobalTrackingGeometryRecord.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "boost/mpl/vector.hpp" 
#include "Geometry/CommonDetUnit/interface/GlobalTrackingGeometry.h"
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"


#include "FWCore/Framework/interface/ESHandle.h"

#include <string>
#include <memory>

using namespace mitedm;
using namespace edm;

HitDropperESProducer::HitDropperESProducer(const edm::ParameterSet & p) 
{
  std::string myname = p.getParameter<std::string>("ComponentName");
  pset_ = p;
  setWhatProduced(this,myname);
}

HitDropperESProducer::~HitDropperESProducer() {}

boost::shared_ptr<HitDropper> 
HitDropperESProducer::produce(const HitDropperRecord & iRecord){ 

  //get tracker geometry
  edm::ESHandle<TrackerGeometry> hTracker;
  iRecord.getRecord<TrackerDigiGeometryRecord>().get(hTracker);
  const TrackerGeometry *trackerGeo = hTracker.product();
  
  //get transient track builder
  edm::ESHandle<TransientTrackBuilder> hBuilder;
  iRecord.getRecord<TransientTrackRecord>().get("TransientTrackBuilder",hBuilder);
  const TransientTrackBuilder *builder = hBuilder.product();
  
  //get tracker geometry navigator
  edm::ESHandle<GeometricSearchTracker> hTrackerSearch;
  iRecord.getRecord<TrackerRecoGeometryRecord>().get(hTrackerSearch);
  const GeometricSearchTracker *trackerSearch = hTrackerSearch.product();
  
  _dropper  = boost::shared_ptr<HitDropper>(
        new HitDropper(trackerGeo, builder, trackerSearch));
        
  return _dropper;

}
