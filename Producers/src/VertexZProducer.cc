// $Id: VertexZProducer.cc,v 1.12 2009/11/29 10:19:06 loizides Exp $

#include "MitEdm/Producers/interface/VertexZProducer.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h" 
#include <TString.h>

using namespace std;
using namespace edm;

#define CP(level) \
  if (level>=verbose_)

//--------------------------------------------------------------------------------------------------
VertexZProducer::VertexZProducer(const ParameterSet &parameters) :
  pixelName_(parameters.getUntrackedParameter<string>("pixelRecHits","siPixelRecHits")),
  dPhiVc_(parameters.getUntrackedParameter<double>("dPhiVertexCut",0.08)),
  dZVc_(parameters.getUntrackedParameter<double>("dZVertexCut",0.25)),
  verbose_(parameters.getUntrackedParameter<int>("verbose",3)),
  pixLayers_(parameters.getUntrackedParameter<int>("pixLayerCombinations",12)),
  doClusVertex_(parameters.getUntrackedParameter<bool>("doClusVertex",false)),
  useRecHitQ_(parameters.getUntrackedParameter<bool>("useRecHitQualityWord",false)),
  usePixelQ_(parameters.getUntrackedParameter<bool>("usePixelQualityWord",true)),
  tgeo_(0)
{
  // Constructor.

  produces<reco::VertexCollection>();

  if ((pixLayers_!=12) && (pixLayers_!=13) && (pixLayers_!=23)) {
    print(2,Form("Value for pixLayerCombinations must be one of 12, 13, or 23. "
                 "Got %d, set value to 12", pixLayers_));
    pixLayers_ = 12;
  }
}

//--------------------------------------------------------------------------------------------------
VertexZProducer::~VertexZProducer()
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
void VertexZProducer::produce(Event &iEvent, const EventSetup &iSetup) 
{
  // Produce vertex collection.

  // get tracker geometry
  ESHandle<TrackerGeometry> trackerHandle;
  iSetup.get<TrackerDigiGeometryRecord>().get(trackerHandle);
  tgeo_ = trackerHandle.product();
  if (!tgeo_)
    print(3,"Could not obtain pointer to TrackerGeometry");

  // get pixels
  fillPixels(iEvent);

  // do vertex finding
  trackletV_.setinvalid();
  if (doClusVertex_) {
    vertexFromClusters(iEvent, pixLayers_);
  } else {
    trackletVertexUnbinned(iEvent, pixLayers_);
  }

  // publish result
  std::auto_ptr<reco::VertexCollection> vertices(new reco::VertexCollection);
  if (trackletV_.n()>0) {
    reco::Vertex::Error err;
    err(2,2) = trackletV_.zs()*trackletV_.zs();
    reco::Vertex ver(reco::Vertex::Point(0,0,trackletV_.z()), err, 
                     trackletV_.chi2(), trackletV_.n(), 0);
    vertices->push_back(ver);
  }
  iEvent.put(vertices);
}

//--------------------------------------------------------------------------------------------------
void VertexZProducer::fillPixels(const Event &iEvent) 
{
  // Fill pixel hit collections.

  bpix1_.clear();
  bpix2_.clear();
  bpix3_.clear();

  Handle<SiPixelRecHitCollection> hRecHits;
  if (!getProductSafe(pixelName_, hRecHits, iEvent)) {
    CP(2) print(2,Form("Can not obtain pixel hit collection with name %s", pixelName_.c_str()));
    return;
  }

  const SiPixelRecHitCollection *hits = hRecHits.product();
  for(SiPixelRecHitCollection::DataContainer::const_iterator hit = hits->data().begin(), 
        end = hits->data().end(); hit != end; ++hit) {

    if (!hit->isValid())
      continue;

    if (useRecHitQ_) {
      if (hit->isOnEdge() || hit->hasBadPixels())
        continue;
    }

    DetId id(hit->geographicalId());
    if(id.subdetId() != int(PixelSubdetector::PixelBarrel))
      continue;

    const PixelGeomDetUnit *pgdu = static_cast<const PixelGeomDetUnit*>(tgeo_->idToDet(id));

    if (usePixelQ_) {
      const RectangularPixelTopology *pixTopo = 
        static_cast<const RectangularPixelTopology*>(&(pgdu->specificTopology()));
      vector<SiPixelCluster::Pixel> pixels(hit->cluster()->pixels());
      bool pixelOnEdge = false;
      for(std::vector<SiPixelCluster::Pixel>::const_iterator pixel = pixels.begin(); 
          pixel != pixels.end(); ++pixel) {
        int pixelX = pixel->x;
        int pixelY = pixel->y;
        if(pixTopo->isItEdgePixelInX(pixelX) || pixTopo->isItEdgePixelInY(pixelY)) {
          pixelOnEdge = true;
          break;
        }
      }
      if (pixelOnEdge)
        continue;
    }

    LocalPoint lpos = LocalPoint(hit->localPosition().x(),
                                 hit->localPosition().y(),
                                 hit->localPosition().z());
    GlobalPoint gpos = pgdu->toGlobal(lpos);
    double adc   = hit->cluster()->charge()/135.;
    double sizex = hit->cluster()->sizeX();
    double sizey = hit->cluster()->sizeY();

    Pixel pix(gpos, adc, sizex, sizey);

    PXBDetId pid(id);
    int layer = pid.layer();

    if (layer==1) {
      bpix1_.push_back(pix);     
    } else if (layer==2) {
      bpix2_.push_back(pix);     
    } else {
      bpix3_.push_back(pix);     
    }
  }
}

//--------------------------------------------------------------------------------------------------
void VertexZProducer::reallyPrint(int level, const char *msg)
{
  // Print out information dependent on level and verbosity.

  if (level==0) {
    printf("VertexZProducer: %s\n", msg); 
  } else if (level==1) {
    LogWarning("VertexZProducer") << msg << std::endl;
  } else if (level==2) {
    LogError("VertexZProducer") << msg << std::endl;
  } else if (level==3) {
    LogError("VertexZProducer") << msg << std::endl;
    throw edm::Exception(errors::Configuration, "VertexZProducer\n") << msg << std::endl;
  }
}

//--------------------------------------------------------------------------------------------------
void VertexZProducer::trackletVertexUnbinned(const Event &iEvent, int which)
{
  // Estimate tracklet based z vertex.

  if (which==12) {
    trackletVertexUnbinned(bpix1_,bpix2_,trackletV_);
  } else if (which==13) {
    trackletVertexUnbinned(bpix1_,bpix3_,trackletV_);
  } else {
    trackletVertexUnbinned(bpix2_,bpix3_,trackletV_);
  }
}

//--------------------------------------------------------------------------------------------------
void VertexZProducer::trackletVertexUnbinned(std::vector<Pixel> &pix1, 
                                             std::vector<Pixel> &pix2,
                                             Vertex &vtx)
{
  // Calculate tracklet based z vertex position. 
  // At first build zvertex candidates from tracklet prototypes,
  // then group zvertex candidates and calculate mean position
  // from most likely cluster.

  vector<double> zvCands;
  zvCands.reserve(pix1.size()*pix2.size());

  // build candidates
  for(size_t i = 0; i<pix1.size(); ++i) {
    const Pixel &p1(pix1.at(i));
    const double r12 = p1.x()*p1.x()+p1.y()*p1.y();
    for(size_t j = 0; j<pix2.size(); ++j) {
      const Pixel &p2(pix2.at(j));
      if (TMath::Abs(Geom::deltaPhi(p1.phi(),p2.phi()))>dPhiVc_)
        continue;
      const double r22 = p2.x()*p2.x()+p2.y()*p2.y();
      const double vz = p1.z() - (p2.z()-p1.z())/(TMath::Sqrt(r22/r12)-1);
      if (TMath::IsNaN(vz))
        continue;
      if (TMath::Abs(vz)>25)
        continue;
      zvCands.push_back(vz);
    }
  }

  // sort cluster candidates
  sort(zvCands.begin(),zvCands.end());

  int    mcl=0;
  double ms2=1e10;
  double mzv=1e10;

  // cluster candidates and calculate mean z position
  for(size_t i = 0; i<zvCands.size(); ++i) {
    double z1 = zvCands.at(i);
    int    ncl   = 0;
    double mean  = 0;
    double mean2 = 0;
    for(size_t j = i; j<zvCands.size(); ++j) {
      double z2 = zvCands.at(j);
      if (TMath::Abs(z1-z2)>dZVc_)
        break;
      ++ncl;
      mean += z2;
      mean2 += z2*z2;
    }      
    if (ncl>0) {
      mean /= ncl;
      mean2 /= ncl;
    }
    double_t s2 = mean*mean - mean2;

    if ((ncl<mcl) || (ncl==mcl && s2>ms2))
      continue;

    mzv = mean;
    ms2 = s2;
    mcl = ncl;
  }

  // set the vertex
  vtx.set(mcl, mzv, ms2);
}

//--------------------------------------------------------------------------------------------------
void VertexZProducer::vertexFromClusters(const edm::Event &iEvent, int which)
{
  // Vertex from cluster positions.

  if (which==12) {
    vertexFromClusters(bpix1_,trackletV_);
  } else if (which==13) {
    vertexFromClusters(bpix3_,trackletV_);
  } else {
    vertexFromClusters(bpix2_,trackletV_);
  }
}

//--------------------------------------------------------------------------------------------------
void VertexZProducer::vertexFromClusters(const std::vector<Pixel> &pix, Vertex &vtx)
{
  // Estimate z vertex position from clusters.

  double chi_max = 1e+9;
  double z_best  = -999;
  int nhits_max  = 0;

  for(double z0 = -15.9; z0 <= 15.95; z0 += 0.1) {
    int nhits  = 0;
    double chi = 0;
    for(size_t i=0; i<pix.size(); ++i) {
      const Pixel &p = pix.at(i);

      // predicted cluster width in y direction
      double pval = 2*TMath::Abs(p.z()-z0)/p.rho() + 0.5; // FIXME
      double chitest = TMath::Abs(pval - p.sizey());
      if(chitest <= 1.) { 
        chi += chitest;
        ++nhits;
      }
    }

    if(nhits <= 0)
      continue;

    if(nhits < nhits_max)
      continue;

    if ((nhits > nhits_max) || (chi < chi_max)) { 
      z_best    = z0; 
      nhits_max = nhits; 
      chi_max   = chi; 
    }
  }

  // set the vertex
  vtx.set(nhits_max, z_best, 0.6);
  vtx.setchi2(chi_max);
}

// define this as a plug-in
DEFINE_FWK_MODULE(VertexZProducer);

