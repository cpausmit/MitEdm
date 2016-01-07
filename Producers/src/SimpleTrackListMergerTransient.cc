#include <memory>
#include <string>
#include <iostream>
#include <cmath>
#include <vector>
#include "MitEdm/Producers/interface/SimpleTrackListMergerTransient.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/ProjectedSiStripRecHit2D.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

using namespace mitedm;

namespace mitedm
{
  edm::ProductID clusterProduct(const TrackingRecHit *hit)
  {
    edm::ProductID pID;
    //cast it into the proper class	and find productID
    DetId detid = hit->geographicalId(); 
    uint32_t subdet = detid.subdetId();
    if ((subdet == PixelSubdetector::PixelBarrel) || (subdet == PixelSubdetector::PixelEndcap)) {
      pID=reinterpret_cast<const SiPixelRecHit *>(hit)->cluster().id();
    }
    else {
      const std::type_info &type = typeid(*hit);
      if (type == typeid(SiStripRecHit2D)) {
	pID=reinterpret_cast<const SiStripRecHit2D *>(hit)->cluster().id();
      }
      else if (type == typeid(SiStripMatchedRecHit2D)) {
	const SiStripMatchedRecHit2D *mhit = reinterpret_cast<const SiStripMatchedRecHit2D *>(hit);
	pID=mhit->monoHit().cluster().id();
      }
      else if (type == typeid(ProjectedSiStripRecHit2D)) {
	const ProjectedSiStripRecHit2D *phit = 
          reinterpret_cast<const ProjectedSiStripRecHit2D *>(hit);

	pID=phit->omniClusterRef().cluster_strip().id();
      }
      else
	throw cms::Exception("Unknown RecHit Type")
	  << "RecHit of type " << type.name() << " not supported. (use c++filt to demangle the name)";
    }
        
    return pID;
  }
}  

SimpleTrackListMergerTransient::SimpleTrackListMergerTransient(edm::ParameterSet const& conf) : 
  trackCollection1Tag_(consumes<mitedm::StablePartCol>(edm::InputTag(conf.getParameter<std::string>("StableProducer1")))),
  trackCollection2Tag_(consumes<mitedm::StablePartCol>(edm::InputTag(conf.getParameter<std::string>("StableProducer2")))),
  maxNormalizedChisq_(conf.getParameter<double>("MaxNormalizedChisq")),
  minPT_(conf.getParameter<double>("MinPT")),
  minFound_(conf.getParameter<unsigned>("MinFound")),
  epsilon_(conf.getParameter<double>("Epsilon")),
  shareFrac_(conf.getParameter<double>("ShareFrac")),
  removeDuplicates_(conf.getParameter<bool>("removeDuplicates")),
  preferCollection_(conf.getParameter<uint>("preferCollection")),
  copyExtras_(conf.getUntrackedParameter<bool>("copyExtras", true))
{
  produces<mitedm::StablePartCol >();
}

SimpleTrackListMergerTransient::~SimpleTrackListMergerTransient() 
{ 
  // Virtual destructor needed.
}  

// Functions that gets called by framework every event
void SimpleTrackListMergerTransient::produce(edm::Event& e, const edm::EventSetup& es)
{
  bool use_sharesInput = (epsilon_ <= 0.0);

  // CP(2014/08/14)> this code is not used anywhere ?!
  //
  //// New track quality should be read from the file
  //std::string qualityStr = conf_.getParameter<std::string>("newQuality");
  //reco::TrackBase::TrackQuality qualityToSet;
  //if (qualityStr != "") {
  //  qualityToSet = reco::TrackBase::qualityByName(conf_.getParameter<std::string>("newQuality"));
  //}
  //else 
  //  qualityToSet = reco::TrackBase::undefQuality;
  // CP<

  // extract tracker geometry
  //printf(" - Geometry\n");
  edm::ESHandle<TrackerGeometry> theG;
  es.get<TrackerDigiGeometryRecord>().get(theG);

  // get Inputs
  //
  // if one input list does not exist, make an empty list, issue a warning, and continue this allows
  // SimpleTrackListMergerTransient to be used as a cleaner only if handed just one list if both
  // input lists don't exist, will issue 2 warnings and generate an empty output collection

  const mitedm::StablePartCol *TC1 = 0;
  static const mitedm::StablePartCol s_empty1, s_empty2;
  edm::Handle<mitedm::StablePartCol> trackCollection1;
  e.getByToken(trackCollection1Tag_, trackCollection1);
  if (trackCollection1.isValid()) {
    TC1 = trackCollection1.product();
  }
  else {
    TC1 = &s_empty1;
    edm::LogWarning("SimpleTrackListMergerTransient") 
      << "1st TrackCollection not found; will only clean 2nd TrackCollection";
  }
  const mitedm::StablePartCol tC1 = *TC1;

  const mitedm::StablePartCol *TC2 = 0;
  edm::Handle<mitedm::StablePartCol> trackCollection2;
  e.getByToken(trackCollection2Tag_, trackCollection2);
  if (trackCollection2.isValid()) {
    TC2 = trackCollection2.product();
  }
  else {
    TC2 = &s_empty2;
    edm::LogWarning("SimpleTrackListMergerTransient") 
      << "2nd TrackCollection not found; will only clean 1st TrackCollection";
  }
  const mitedm::StablePartCol tC2 = *TC2;

  // Step B: create empty output collection
  std::auto_ptr<mitedm::StablePartCol> outputTrks(new mitedm::StablePartCol);

  int i;
  std::vector<int> selected1; 
  for (unsigned int jj=0; jj<tC1.size(); ++jj)
    selected1.push_back(1);

  //printf(" - C1 - 1\n");
  if (0<tC1.size()) {
    i=-1;
    for (mitedm::StablePartCol::const_iterator stable=tC1.begin(); stable!=tC1.end(); stable++) {
      i++;
      const reco::Track *track = stable->track();
      if ((short unsigned)track->ndof() < 1) {
        selected1[i]=0; 

        continue;
      }
      if (track->normalizedChi2() > maxNormalizedChisq_) {
        selected1[i]=0; 

        continue;
      }
      if (track->found() < minFound_) {
        selected1[i]=0; 

        continue;
      }
      if (track->pt() < minPT_) {
        selected1[i]=0; 

        continue;
      }
    } //end loop over tracks
  } //end more than 0 track

  std::vector<int> selected2; 
  for (unsigned int jj=0; jj<tC2.size(); ++jj) 
    selected2.push_back(1);

  if (0<tC2.size()) {
    i=-1;
    for (mitedm::StablePartCol::const_iterator stable=tC2.begin(); stable!=tC2.end(); stable++) {
      i++;
      const reco::Track *track = stable->track();
      if ((short unsigned)track->ndof() < 1) {
        selected2[i]=0; 

        continue;
      }
      if (track->normalizedChi2() > maxNormalizedChisq_) {
        selected2[i]=0; 

        continue;
      }
      if (track->found() < minFound_) {
        selected2[i]=0; 

        continue;
      }
      if (track->pt() < minPT_) {
        selected2[i]=0; 

        continue;
      }
    }
  }


  //  L1 and L2 both have > 0 track - try merging

  std::map<mitedm::StablePartCol::const_iterator, std::vector<const TrackingRecHit*> > rh1;
  for (mitedm::StablePartCol::const_iterator stable=tC1.begin(); stable!=tC1.end(); ++stable) {
    const reco::Track *track = stable->track();
    if (track->extra().isAvailable()) {
      trackingRecHit_iterator itB = track->recHitsBegin();
      trackingRecHit_iterator itE = track->recHitsEnd();
      for (trackingRecHit_iterator it = itB;  it != itE; ++it) { 
	const TrackingRecHit* hit = &(**it);
	rh1[stable].push_back(hit);
      }
    }
  }

  std::map<mitedm::StablePartCol::const_iterator, std::vector<const TrackingRecHit*> > rh2;
  for (mitedm::StablePartCol::const_iterator stable=tC2.begin(); stable!=tC2.end(); ++stable) {
    const reco::Track *track = stable->track();
    if (removeDuplicates_ && track->extra().isAvailable()) {
      trackingRecHit_iterator jtB = track->recHitsBegin();
      trackingRecHit_iterator jtE = track->recHitsEnd();
      for (trackingRecHit_iterator jt = jtB;  jt != jtE; ++jt) { 
	const TrackingRecHit* hit = &(**jt);
	rh2[stable].push_back(hit);
      }
    }
  }

  if (0<tC1.size() && 0<tC2.size()) {
    i=-1;
    for (mitedm::StablePartCol::const_iterator stable=tC1.begin(); stable!=tC1.end(); ++stable) {
      i++; 
      const reco::Track *track = stable->track();
      if (!selected1[i] || !removeDuplicates_)
	continue;
      std::vector<const TrackingRecHit*>& iHits = rh1[stable]; 
      unsigned nh1 = iHits.size();
      int qualityMaskT1 = track->qualityMask();
      int j=-1;
      for (mitedm::StablePartCol::const_iterator stable2=tC2.begin(); stable2!=tC2.end(); ++stable2) {
        j++;
	const reco::Track *track2 = stable2->track();
        if ((!selected2[j])||(!selected1[i])||(stable->pid()!=stable2->pid())||(!removeDuplicates_))
          continue;
	std::vector<const TrackingRecHit*>& jHits = rh2[stable2]; 
	unsigned nh2 = jHits.size();
        int noverlap=0;
	for ( unsigned ih=0; ih<nh1; ++ih ) { 
	  const TrackingRecHit* it = iHits[ih];
          if (it->isValid()) {
            int jj=-1;
	    for ( unsigned jh=0; jh<nh2; ++jh ) { 
	      const TrackingRecHit* jt = jHits[jh];
              jj++;
	      if (jt->isValid()) {
                if (!use_sharesInput) {
                  float delta = fabs ( it->localPosition().x()-jt->localPosition().x() ); 
                  if ((it->geographicalId()==jt->geographicalId())&&(delta<epsilon_))noverlap++;
                }
		else {
                  const TrackingRecHit* kt = jt;
                  if (it->sharesInput(kt,TrackingRecHit::some))
		    noverlap++;
                }
              }
            }
          }
        }
	int newQualityMask = (qualityMaskT1 | track2->qualityMask()); // take OR of trackQuality 
        float fi=float(noverlap)/float(track->numberOfValidHits()); 
        float fj=float(noverlap)/float(track2->numberOfValidHits());

        if ((fi>shareFrac_)||(fj>shareFrac_)) {
	  //collection preference overrides
	  if (preferCollection_==1) {
	    selected2[j]=0;
	    selected1[i]=10+newQualityMask;
	  }
	  else if (preferCollection_==2) {
	    selected1[i]=0; 
	    selected2[j]=10+newQualityMask;
	  }
	  else {
	    if (fi<fj) {
	      selected2[j]=0; 
	      selected1[i]=10+newQualityMask; // add 10 to avoid the case where mask = 1
	    }
	    else {
	      if (fi>fj) {
		selected1[i]=0; 
		selected2[j]=10+newQualityMask;  // add 10 to avoid the case where mask = 1
	      }
	      else {
		const double almostSame = 1.001;
		if (track->normalizedChi2() > almostSame * track2->normalizedChi2()) {
		  selected1[i]=0;
		  selected2[j]=10+newQualityMask; // add 10 to avoid the case where mask = 1
		}
		else if (track2->normalizedChi2() > almostSame * track->normalizedChi2()) {
		  selected2[j]=0;
		  selected1[i]=10+newQualityMask; // add 10 to avoid the case where mask = 1
		}
		else {
		  // If tracks from both iterations are virtually identical, choose the one from the first iteration.
		  if (track->algo() <= track2->algo()) {
		    selected2[j]=0;
		    selected1[i]=10+newQualityMask; // add 10 to avoid the case where mask = 1
		  }
		  else {
		    selected1[i]=0;
		    selected2[j]=10+newQualityMask; // add 10 to avoid the case where mask = 1
		  }
		}
	      }
	    }
	  }
        }
      }
    }
  }

  //  output selected tracks - if any

  size_t current = 0;

  if ( 0<tC1.size() ) {
    i=0;
    for (mitedm::StablePartCol::const_iterator stable=tC1.begin(); stable!=tC1.end(); 
         ++stable, ++current, ++i) {
      if (!selected1[i]) {
	continue;
      }
      outputTrks->push_back(*stable);
    } //end faux loop over tracks
  } //end more than 0 track

  //short offset = current; //save offset into trackRefs
  if (0<tC2.size()) {
    i=0;
    for (mitedm::StablePartCol::const_iterator stable=tC2.begin(); stable!=tC2.end();
         ++stable, ++current, ++i) {
      if (!selected2[i]) {
        continue;
      }
      //fill the TrackCollection
      outputTrks->push_back( *stable );
    } //end faux loop over tracks
  } //end more than 0 track
  
  e.put(outputTrks);
}
