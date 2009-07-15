// $Id: HitDropperESProducer.cc,v 1.3 2009/03/20 18:01:48 loizides Exp $

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

//--------------------------------------------------------------------------------------------------
HitDropperESProducer::HitDropperESProducer(const edm::ParameterSet &p) 
{
  // Constructor.

  std::string myname = p.getParameter<std::string>("ComponentName");
  pset_ = p;
  setWhatProduced(this,myname);
}

//--------------------------------------------------------------------------------------------------
HitDropperESProducer::~HitDropperESProducer() 
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
boost::shared_ptr<HitDropper> HitDropperESProducer::produce(const HitDropperRecord &iRecord)
{ 
  // Create hit dropper object.

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
  
  dropper_ = boost::shared_ptr<HitDropper>(new HitDropper(trackerGeo, builder, trackerSearch));
  return dropper_;
}
