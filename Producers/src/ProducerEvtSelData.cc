// $Id: ProducerEvtSelData.cc,v 1.1 2009/12/07 22:52:30 loizides Exp $

#include "MitEdm/Producers/interface/ProducerEvtSelData.h"
#include "MitEdm/DataFormats/interface/EvtSelData.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"

using namespace std;
using namespace edm;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
ProducerEvtSelData::ProducerEvtSelData(const ParameterSet& cfg)
  : srcHF_(cfg.getUntrackedParameter<string>("hfRecHits","hfreco")),
    srcHBHE_(cfg.getUntrackedParameter<string>("hbheRecHits","hbhereco")),
    srcCastor_(cfg.getUntrackedParameter<string>("castorRecHits","castorreco")),
    srcZDC_(cfg.getUntrackedParameter<string>("zdcRecHits","zdcreco")),
    srcPixels_(cfg.getUntrackedParameter<string>("pixelRecHits","siPixelRecHits"))
{
  // Constructor.

  produces<EvtSelData>();
}

//--------------------------------------------------------------------------------------------------
ProducerEvtSelData::~ProducerEvtSelData()
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
void ProducerEvtSelData::produce(Event &evt, const EventSetup &setup)
{
  // Produce event selection data for this event.

  double eHfNeg     = 0;
  double eHfPos     = 0;
  double eHfPosTime = 0;
  double eHfNegTime = 0;
  double eHcalNeg   = 0;
  double eHcalPos   = 0;
  double eCaNeg     = 0;
  double eCaPos     = 0;
  double eCaPosTime = 0;
  double eCaNegTime = 0;
  double eZdNeg     = 0;
  double eZdPos     = 0;
  double eZdPosTime = 0;
  double eZdNegTime = 0;
  int    ePxbHits   = 0;
  double eClusVtxQual = 0;

  Handle<HFRecHitCollection> hfhits;
  try {
    evt.getByLabel(edm::InputTag(srcHF_),hfhits);
  } catch (...) {}  
  if (hfhits.isValid()) {
    for(size_t ihit = 0; ihit<hfhits->size(); ++ihit){
      const HFRecHit h = (*hfhits)[ihit];
      double energy = h.energy();
      double time = h.time();
      const HcalDetId id(h.id()); 
      if (id.zside()<0) {
        eHfNeg     += energy;
        eHfNegTime += energy * time;
      } else {
        eHfPos     += energy;
        eHfPosTime += energy * time;
      }
    }
  }

  Handle<HBHERecHitCollection> hbhehits;
  try {
    evt.getByLabel(edm::InputTag(srcHBHE_),hbhehits);
  } catch (...) {}  
  if (hbhehits.isValid()) {
    for(size_t ihit = 0; ihit<hbhehits->size(); ++ihit){
      const HBHERecHit h = (*hbhehits)[ihit];
      double energy = h.energy();
      const HcalDetId id(h.id()); 
      if (id.zside()<0) {
        eHcalNeg   += energy;
      } else {
        eHcalPos   += energy;
      }
    }
  }

  Handle<CastorRecHitCollection> castorhits;
  try {
    evt.getByLabel(edm::InputTag(srcCastor_),castorhits);
  } catch (...) {}  
  if (castorhits.isValid()) {
    for(size_t ihit = 0; ihit<castorhits->size(); ++ihit){
      const CastorRecHit h = (*castorhits)[ihit];
      double energy = h.energy();
      double time = h.time();
      const HcalCastorDetId id(h.id()); 
      if (id.zside()<0) {
        eCaNeg     += energy;
        eCaNegTime += energy * time;
      } else {
        eCaPos     += energy;
        eCaPosTime += energy * time;
      }
    }
  }

  Handle<ZDCRecHitCollection> zdchits;
  try {
    evt.getByLabel(edm::InputTag(srcZDC_),zdchits);
  } catch (...) {}  
  if (zdchits.isValid()) {
    for(size_t ihit = 0; ihit<zdchits->size(); ++ihit){
      const ZDCRecHit h = (*zdchits)[ihit];
      double energy = h.energy();
      double time = h.time();
      const HcalZDCDetId id(h.id()); 
      if (id.zside()<0) {
        eZdNeg     += energy;
        eZdNegTime += energy * time;
      } else {
        eZdPos     += energy;
        eZdPosTime += energy * time;
      }
    }
  }

  if(eHfPos>0)
    eHfPosTime /= eHfPos;
  if(eHfNeg>0)
    eHfNegTime /= eHfNeg;
  if(eCaPos>0)
    eCaPosTime /= eCaPos;
  if(eCaNeg>0)
    eCaNegTime /= eCaNeg;
  if(eZdPos>0)
    eZdPosTime /= eZdPos;
  if(eZdNeg>0)
    eZdNegTime /= eZdNeg;
  
  cout <<"now it is time for pixels" << endl;


  Handle<SiPixelRecHitCollection> pixelhits;
  try {
    evt.getByLabel(edm::InputTag(srcPixels_),pixelhits);
  } catch (...) {}
  if (pixelhits.isValid()) {
    
    cout << "valid pixel hits" << endl;

    const SiPixelRecHitCollection* thePixelHits = pixelhits.product();
    ePxbHits = thePixelHits->size();

    if(ePxbHits > 0) {
      
      vector<VertexHit> hits;
      
      for(SiPixelRecHitCollection::DataContainer::const_iterator
	    recHit = thePixelHits->data().begin(),
	    recHitEnd = thePixelHits->data().end();
	  recHit != recHitEnd; ++recHit)
	{
	  if(recHit->isValid())
	    {
	      //      if(!(recHit->isOnEdge() || recHit->hasBadPixels()))
	      DetId id = recHit->geographicalId();
	      const PixelGeomDetUnit* pgdu =
		dynamic_cast<const PixelGeomDetUnit*>(theTracker->idToDetUnit(id));
	      const RectangularPixelTopology* theTopol =
		dynamic_cast<const RectangularPixelTopology*>( &(pgdu->specificTopology()) );
	      vector<SiPixelCluster::Pixel> pixels = recHit->cluster()->pixels();
	      
	      bool pixelOnEdge = false;
	      for(vector<SiPixelCluster::Pixel>::const_iterator
		    pixel = pixels.begin(); pixel!= pixels.end(); pixel++)
		{
		  int pos_x = (int)pixel->x;
		  int pos_y = (int)pixel->y;
		  
		  if(theTopol->isItEdgePixelInX(pos_x) ||
		     theTopol->isItEdgePixelInY(pos_y))
		    { pixelOnEdge = true; break; }
		}
	      
	      if(!pixelOnEdge)
		if(id.subdetId() == int(PixelSubdetector::PixelBarrel))  
		  {
		    PXBDetId pid(id);
		    
		    LocalPoint lpos = LocalPoint(recHit->localPosition().x(),
						 recHit->localPosition().y(),
						 recHit->localPosition().z());
		    
		    GlobalPoint gpos = theTracker->idToDet(id)->toGlobal(lpos);
		    
		    VertexHit hit;
		    hit.z = gpos.z(); 
		    hit.r = gpos.perp(); 
		    hit.w = recHit->cluster()->sizeY();
		    
		    hits.push_back(hit);
		  }
	    }
	}
      
      int nhits; int nhits_max = 0;
      float chi; float chi_max = 1e+9;
      
      float zest = 0.0;
      
      for(float z0 = -15.9; z0 <= 15.95; z0 += 0.1)
	{
	  nhits = getContainedHits(hits, z0, chi);
	  
	  if(nhits > 0)
	    {
	      if(nhits >  nhits_max)
		{ chi_max = 1e+9; nhits_max = nhits; }
	      
	      if(nhits >= nhits_max)
		if(chi < chi_max)
		  { chi_max = chi; zest = z0; }
	    }
	}
      
      int nbest=0, nminus=0, nplus=0;
      nbest = getContainedHits(hits,zest,chi);
      nminus = getContainedHits(hits,zest-10.,chi);
      nplus = getContainedHits(hits,zest+10.,chi);
      

      cout << "  [vertex position] estimated = " << zest 
	   << " | contained hits = " << nbest 
	   << " | contained hits + 10 = " << nplus
	   << " | contained hits - 10 = " << nminus
	   << " | pixel barrel hits = " << ePxbHits;
     
      eClusVtxQual = (nminus+nplus)/(2.0*nbest);
 
    }
    
  }
  
  
  std::auto_ptr<EvtSelData> output(new EvtSelData(eHfNeg,eHfPos,eHfNegTime,eHfPosTime,
                                                  eHcalNeg, eHcalPos,
                                                  eCaNeg,eCaPos,eCaNegTime,eCaPosTime,
                                                  eZdNeg,eZdPos,eZdNegTime,eZdPosTime,
						  ePxbHits,eClusVtxQual));
  evt.put(output);
}

//--------------------------------------------------------------------------------------------------
void ProducerEvtSelData::beginJob (const edm::EventSetup& es) {

  cout << "begin job" << endl;

  // Get tracker geometry
  edm::ESHandle<TrackerGeometry> trackerHandle;
  es.get<TrackerDigiGeometryRecord>().get(trackerHandle);
  theTracker = trackerHandle.product();

}

//--------------------------------------------------------------------------------------------------
int ProducerEvtSelData::getContainedHits (std::vector<VertexHit> hits, float z0, float & chi) {

  // Calculate number of hits contained in V-shaped window in cluster y-width vs. z-position

  int n = 0;
  chi = 0.;

  for(vector<VertexHit>::const_iterator hit = hits.begin();
                                        hit!= hits.end(); hit++)
  {
    // Predicted cluster width in y direction
    float p = 2 * fabs(hit->z - z0)/hit->r + 0.5; // FIXME

    if(fabs(p - hit->w) <= 1.)
    { 
      chi += fabs(p - hit->w);
      n++;
    }
  }

  return n;

}


//define this as a plug-in
DEFINE_FWK_MODULE(ProducerEvtSelData);
