//--------------------------------------------------------------------------------------------------
// $Id: HitDropper.h,v 1.3 2008/10/16 18:39:09 bendavid Exp $
//
// SimpleTrackListMergerGsf
//
// Merges collections of GsfTracks, adapted from SimpleTrackListMerger from CMSSW
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_TRACKERELECTRONS_SimpleTrackListMergerGsf_h
#define MITEDM_TRACKERELECTRONS_SimpleTrackListMergerGsf_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/EDProduct.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackBase.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtraFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackExtra.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace mitedm
{
  class SimpleTrackListMergerGsf : public edm::EDProducer
  {
  public:

    explicit SimpleTrackListMergerGsf(const edm::ParameterSet& conf);

    virtual ~SimpleTrackListMergerGsf();

    virtual void produce(edm::Event& e, const edm::EventSetup& c);

  private:
    edm::ParameterSet conf_;

    std::auto_ptr<reco::GsfTrackCollection> outputTrks;
    std::auto_ptr<reco::TrackExtraCollection> outputTrkExtras;
    std::auto_ptr<reco::GsfTrackExtraCollection> outputGsfTrkExtras;
    std::auto_ptr< TrackingRecHitCollection>  outputTrkHits;
    std::auto_ptr< std::vector<Trajectory> > outputTrajs;

    reco::GsfTrackRefProd refTrks;
    reco::TrackExtraRefProd refTrkExtras;
    reco::GsfTrackExtraRefProd refGsfTrkExtras;
    TrackingRecHitRefProd refTrkHits;
    edm::RefProd< std::vector<Trajectory> > refTrajs;
    std::vector<reco::GsfTrackRef> trackRefs;

  };
}


#endif
