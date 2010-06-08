//--------------------------------------------------------------------------------------------------
// $Id: HitDropper.h,v 1.9 2009/12/15 23:27:34 bendavid Exp $
//
// ConversionMatcher
//
// Utility to match electrons to conversions and perform the final conversion selection for
// electron rejection.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_CONVERSIONREJECTION_CONVERSIONMATCHER_H
#define MITEDM_CONVERSIONREJECTION_CONVERSIONMATCHER_H

#include "DataFormats/TrackReco/interface/Track.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "MitEdm/DataFormats/interface/Types.h"

namespace mitedm
{
  class ConversionMatcher
  {
    public:
      ConversionMatcher() :
      radiusMin_(2.0),
      lxyMin_(0.0),
      lzMin_(0.0),
      probMin_(1e-6),
      trackProbMin_(1e-6),
      wrongHitsMax_(0)
      {}
      
      ConversionMatcher(double radiusMin, double lxyMin, double lzMin,
                 double probMin, double trackProbMin, uint wrongHitsMax) :
      radiusMin_(radiusMin),
      lxyMin_(lxyMin),
      lzMin_(lzMin),
      probMin_(probMin),
      trackProbMin_(trackProbMin),
      wrongHitsMax_(wrongHitsMax)
      {}
           

      std::vector<edm::Ptr<DecayPart> >  allMatchedConversions(const reco::GsfElectron &ele,
                                                  const edm::Handle<std::vector<DecayPart> > &convCol) const;

      std::vector<edm::Ptr<DecayPart> >  goodMatchedConversions(const reco::GsfElectron &ele,
                                                  const edm::Handle<std::vector<DecayPart> > &convCol) const;

      bool                               matchesAnyConversion(const reco::GsfElectron &ele,
                                                  const edm::Handle<std::vector<DecayPart> > &convCol) const;
                                                  
      bool                               matchesGoodConversion(const reco::GsfElectron &ele,
                                                  const edm::Handle<std::vector<DecayPart> > &convCol) const;                                                  
                                                  
      bool                               isGoodConversion(const DecayPart &conv) const;
      
      bool                               matchesConversion(const reco::GsfElectron &ele,
                                                  const DecayPart &conv) const;
                                                  
    private:
      double radiusMin_;   //minimum conversion radius
      double lxyMin_;      //minimum transverse decay length (wrt PV)
      double lzMin_;       //minimum z decay length (wrt PV)
      double probMin_;     //minimum conversion fit probability
      double trackProbMin_; //minimum track fit probability
      uint   wrongHitsMax_; //maximum number of hits before the vertex for each track
      
                                                  
  };
}
#endif
