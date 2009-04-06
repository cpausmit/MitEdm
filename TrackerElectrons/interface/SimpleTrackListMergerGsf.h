//--------------------------------------------------------------------------------------------------
// $Id: SimpleTrackListMergerGsf.h,v 1.2 2009/03/20 17:13:34 loizides Exp $
//
// SimpleTrackListMergerGsf
//
// Merges collections of GsfTracks, adapted from SimpleTrackListMerger from CMSSW.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_TRACKERELECTRONS_SIMPLETRACKLISTMERGERGSF_H
#define MITEDM_TRACKERELECTRONS_SIMPLETRACKLISTMERGERGSF_H

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/EDProduct.h"
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
    edm::ParameterSet                            conf_;               //configuration
    std::auto_ptr<reco::GsfTrackCollection>      outputTrks_;         //output tracks
    std::auto_ptr<reco::TrackExtraCollection>    outputTrkExtras_;    //output track extras
    std::auto_ptr<reco::GsfTrackExtraCollection> outputGsfTrkExtras_; //output gsf tracks
    std::auto_ptr<TrackingRecHitCollection>      outputTrkHits_;      //output track hits
    std::auto_ptr<std::vector<Trajectory> >      outputTrajs_;        //output trajectories
    reco::GsfTrackRefProd                        refTrks_;            //ref tracks
    reco::TrackExtraRefProd                      refTrkExtras_;       //ref track extras
    reco::GsfTrackExtraRefProd                   refGsfTrkExtras_;    //ref gsf track extras
    TrackingRecHitRefProd                        refTrkHits_;         //ref track hits
    edm::RefProd< std::vector<Trajectory> >      refTrajs_;           //ref trajectories
    std::vector<reco::GsfTrackRef>               trackRefs_;          //ref gsf tracks
  };
}
#endif
