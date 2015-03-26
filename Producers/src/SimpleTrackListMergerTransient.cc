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
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "RecoTracker/TrackProducer/interface/ClusterRemovalRefSetter.h"

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
	pID=(&phit->originalHit())->cluster().id();
      }
      else
	throw cms::Exception("Unknown RecHit Type")
	  << "RecHit of type " << type.name() << " not supported. (use c++filt to demangle the name)";
    }
        
    return pID;
  }
}  

SimpleTrackListMergerTransient::SimpleTrackListMergerTransient(edm::ParameterSet const& conf) : 
  conf_(conf)
{
  // Constructor.

  copyExtras_ = conf_.getUntrackedParameter<bool>("copyExtras", true);
  produces<mitedm::StablePartCol >();
}


SimpleTrackListMergerTransient::~SimpleTrackListMergerTransient() 
{ 
  // Virtual destructor needed.
}  

// Functions that gets called by framework every event
void SimpleTrackListMergerTransient::produce(edm::Event& e, const edm::EventSetup& es)
{
  // retrieve producer name of input TrackCollection(s)
  std::string trackProducer1 = conf_.getParameter<std::string>("StableProducer1");
  std::string trackProducer2 = conf_.getParameter<std::string>("StableProducer2");

  //std::cout
  //  << "Collections " << trackProducer1 << " " << trackProducer2 << std::endl;

  double maxNormalizedChisq =  conf_.getParameter<double>("MaxNormalizedChisq");
  double minPT =  conf_.getParameter<double>("MinPT");
  unsigned int minFound = (unsigned int)conf_.getParameter<int>("MinFound");
  double epsilon =  conf_.getParameter<double>("Epsilon");
  bool use_sharesInput = true;
  if ( epsilon > 0.0 )
    use_sharesInput = false;
  double shareFrac =  conf_.getParameter<double>("ShareFrac");
  
  //bool promoteQuality = conf_.getParameter<bool>("promoteTrackQuality");

  uint removeDuplicates = conf_.getParameter<bool>("removeDuplicates");
  uint preferCollection = conf_.getParameter<uint>("preferCollection");

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

  //printf(" - TC1\n");
  const mitedm::StablePartCol *TC1 = 0;
  static const mitedm::StablePartCol s_empty1, s_empty2;
  edm::Handle<mitedm::StablePartCol> trackCollection1;
  e.getByLabel(trackProducer1, trackCollection1);
  if (trackCollection1.isValid()) {
    TC1 = trackCollection1.product();
    //std::cout
    //  << "1st collection " << trackProducer1 << " has " << TC1->size() << " tracks" << std::endl;
  }
  else {
    TC1 = &s_empty1;
    edm::LogWarning("SimpleTrackListMergerTransient") 
      << "1st TrackCollection " << trackProducer1
      << " not found; will only clean 2nd TrackCollection " << trackProducer2 ;
  }
  const mitedm::StablePartCol tC1 = *TC1;

  //printf(" - TC2\n");
  const mitedm::StablePartCol *TC2 = 0;
  edm::Handle<mitedm::StablePartCol> trackCollection2;
  e.getByLabel(trackProducer2, trackCollection2);
  if (trackCollection2.isValid()) {
    TC2 = trackCollection2.product();
    //std::cout << "2nd collection " << trackProducer2 << " has " 
    //          << TC2->size() << " tracks" << std::endl ;
  }
  else {
    TC2 = &s_empty2;
    edm::LogWarning("SimpleTrackListMergerTransient") 
      << "2nd TrackCollection " << trackProducer2 << " not found; will only clean 1st TrackCollection " 
      << trackProducer1 ;
  }
  const mitedm::StablePartCol tC2 = *TC2;

  // Step B: create empty output collection
  //printf(" - OUTPUT\n");
  outputTrks = std::auto_ptr<mitedm::StablePartCol>(new mitedm::StablePartCol);
  //refTrks = e.getRefBeforePut<std::vector<edm::Ptr<reco::Track> > >();      

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
        //std::cout << "L1Track "<< i << " rejected in SimpleTrackListMergerTransient; ndof() < 1" << std::endl ;
        continue;
      }
      if (track->normalizedChi2() > maxNormalizedChisq) {
        selected1[i]=0; 
        //std::cout << "L1Track "<< i 
        //          << " rejected in SimpleTrackListMergerTransient; normalizedChi2() > maxNormalizedChisq " 
        //          << track->normalizedChi2() << " " << maxNormalizedChisq << std::endl ;
        continue;
      }
      if (track->found() < minFound) {
        selected1[i]=0; 
        //std::cout << "L1Track "<< i << " rejected in SimpleTrackListMergerTransient; found() < minFound " 
        //          << track->found() << " " << minFound << std::endl ;
        continue;
      }
      if (track->pt() < minPT) {
        selected1[i]=0; 
        //std::cout << "L1Track "<< i << " rejected in SimpleTrackListMergerTransient; pt() < minPT " 
        //          << track->pt() << " " << minPT << std::endl ;
        continue;
      }
    } //end loop over tracks
  } //end more than 0 track

  std::vector<int> selected2; 
  for (unsigned int jj=0; jj<tC2.size(); ++jj) 
    selected2.push_back(1);

  //printf(" - C2 - 1\n");
  if (0<tC2.size()) {
    i=-1;
    for (mitedm::StablePartCol::const_iterator stable=tC2.begin(); stable!=tC2.end(); stable++) {
      i++;
      const reco::Track *track = stable->track();
      if ((short unsigned)track->ndof() < 1) {
        selected2[i]=0; 
        //std::cout << "L2Track "<< i << " rejected in SimpleTrackListMergerTransient; ndof() < 1" << std::endl ;
        continue;
      }
      if (track->normalizedChi2() > maxNormalizedChisq) {
        selected2[i]=0; 
        //std::cout << "L2Track " << i 
        //          << " rejected in SimpleTrackListMergerTransient; normalizedChi2() > maxNormalizedChisq " 
        //          << track->normalizedChi2() << " " << maxNormalizedChisq << std::endl ;
        continue;
      }
      if (track->found() < minFound) {
        selected2[i]=0; 
        //std::cout << "L2Track "<< i << " rejected in SimpleTrackListMergerTransient; found() < minFound " 
        //          << track->found() << " " << minFound << std::endl ;
        continue;
      }
      if (track->pt() < minPT) {
        selected2[i]=0; 
        //std::cout << "L2Track "<< i << " rejected in SimpleTrackListMergerTransient; pt() < minPT " 
        //          << track->pt() << " " << minPT << std::endl ;
        continue;
      }
    }
  }


  //  L1 and L2 both have > 0 track - try merging

  //printf(" - MERGING - 1\n");
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

  //printf(" - MERGING - 2\n");
  std::map<mitedm::StablePartCol::const_iterator, std::vector<const TrackingRecHit*> > rh2;
  for (mitedm::StablePartCol::const_iterator stable=tC2.begin(); stable!=tC2.end(); ++stable) {
    //printf(" - TRACK TRY\n");
    const reco::Track *track = stable->track();
    //printf(" - TRACK ASSIGNED\n");
    if (removeDuplicates && track->extra().isAvailable()) {
      //printf(" - TRACK - HIT - 2\n");
      trackingRecHit_iterator jtB = track->recHitsBegin();
      trackingRecHit_iterator jtE = track->recHitsEnd();
      for (trackingRecHit_iterator jt = jtB;  jt != jtE; ++jt) { 
	const TrackingRecHit* hit = &(**jt);
	rh2[stable].push_back(hit);
      }
    }
  }

  //printf(" - MERGING - 3\n");
  if (0<tC1.size() && 0<tC2.size()) {
    i=-1;
    for (mitedm::StablePartCol::const_iterator stable=tC1.begin(); stable!=tC1.end(); ++stable) {
      i++; 
      const reco::Track *track = stable->track();
      if (!selected1[i] || !removeDuplicates)
	continue;
      std::vector<const TrackingRecHit*>& iHits = rh1[stable]; 
      unsigned nh1 = iHits.size();
      int qualityMaskT1 = track->qualityMask();
      int j=-1;
      for (mitedm::StablePartCol::const_iterator stable2=tC2.begin(); stable2!=tC2.end(); ++stable2) {
        j++;
	const reco::Track *track2 = stable2->track();
        if ((!selected2[j])||(!selected1[i])||(stable->pid()!=stable2->pid())||(!removeDuplicates))
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
                  if ((it->geographicalId()==jt->geographicalId())&&(delta<epsilon))noverlap++;
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
	//std::cout << " trk1 trk2 nhits1 nhits2 nover " << i << " " << j << " " << track->numberOfValidHits() 
        //          << " "  << track2->numberOfValidHits() << " " << noverlap << " " << fi << " " << fj  <<std::endl;
        if ((fi>shareFrac)||(fj>shareFrac)) {
	  //collection preference overrides
	  if (preferCollection==1) {
	    selected2[j]=0;
	    selected1[i]=10+newQualityMask;
	  }
	  else if (preferCollection==2) {
	    selected1[i]=0; 
	    selected2[j]=10+newQualityMask;
	  }
	  else {
	    if (fi<fj) {
	      selected2[j]=0; 
	      selected1[i]=10+newQualityMask; // add 10 to avoid the case where mask = 1
	      //std::cout << " removing L2 trk in pair " << std::endl;
	    }
	    else {
	      if (fi>fj) {
		selected1[i]=0; 
		selected2[j]=10+newQualityMask;  // add 10 to avoid the case where mask = 1
		//std::cout << " removing L1 trk in pair " << std::endl;
	      }
	      else {
		//std::cout << " removing worst chisq in pair " << track->normalizedChi2() << " " 
                //          << track2->normalizedChi2() << std::endl;
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
		  //std::cout<<"MERGE "<<track->algo()<<" "<<track2->algo()<<" "
                  //         <<track->normalizedChi2()<<" "<<track2->normalizedChi2()<<" "
                  //         <<(track->normalizedChi2()-track2->normalizedChi2())/track->normalizedChi2()<<std::endl;
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

  //printf(" - OUTPUTTING\n");

  //trackRefs.resize(tC1.size()+tC2.size());
  //std::vector<edm::RefToBase<TrajectorySeed> > seedsRefs(tC1.size()+tC2.size());
  size_t current = 0;

  if ( 0<tC1.size() ) {
    i=0;
    for (mitedm::StablePartCol::const_iterator stable=tC1.begin(); stable!=tC1.end(); 
         ++stable, ++current, ++i) {
      if (!selected1[i]) {
	//trackRefs[current] = reco::TrackRef();
	continue;
      }
      //const reco::Track & theTrack = * track;
      //fill the TrackCollection
      outputTrks->push_back(*stable);
      //trackRefs[current] = reco::TrackRef(refTrks, outputTrks->size() - 1);
    } //end faux loop over tracks
  } //end more than 0 track


  //short offset = current; //save offset into trackRefs
  if (0<tC2.size()) {
    i=0;
    for (mitedm::StablePartCol::const_iterator stable=tC2.begin(); stable!=tC2.end();
         ++stable, ++current, ++i) {
      if (!selected2[i]) {
        //trackRefs[current] = reco::TrackRef();
        continue;
      }
      //const reco::Track & theTrack = * track;
      //fill the TrackCollection
      outputTrks->push_back( *stable );
      //trackRefs[current] = reco::TrackRef(refTrks, outputTrks->size() - 1);
    } //end faux loop over tracks
  } //end more than 0 track
  
  e.put(outputTrks);
  return;
}
