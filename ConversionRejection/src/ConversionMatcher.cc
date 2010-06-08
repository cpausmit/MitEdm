// $Id: HitDropper.cc,v 1.11 2009/12/15 23:27:34 bendavid Exp $

#include <TMath.h>
#include "MitEdm/ConversionRejection/interface/ConversionMatcher.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrackFwd.h"
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/Common/interface/RefToPtr.h"

using namespace edm;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
std::vector<edm::Ptr<DecayPart> > ConversionMatcher::allMatchedConversions(const reco::GsfElectron &ele,
                                                  const edm::Handle<std::vector<DecayPart> > &convCol) const
{
  //Return vector of ptr's to conversion candidates from the collection which match to the given
  //electron candidate (through the gsf track)
  
  std::vector<edm::Ptr<DecayPart> > outCol;
  if (!convCol.isValid())
    return outCol;
  
  const std::vector<DecayPart> *convv = convCol.product();
  
  for (std::vector<DecayPart>::const_iterator it = convv->begin(); it!=convv->end(); ++it) {
    if ( matchesConversion(ele,*it) ) {
      outCol.push_back(edm::Ptr<DecayPart>(convCol,it - convv->begin()));
    }
  }
  
  return outCol;
  
}

//--------------------------------------------------------------------------------------------------
std::vector<edm::Ptr<DecayPart> > ConversionMatcher::goodMatchedConversions(const reco::GsfElectron &ele,
                                                  const edm::Handle<std::vector<DecayPart> > &convCol) const
{
  //Return vector of ptr's to conversion candidates from the collection which both match to the given
  //electron candidate (through the gsf track) and also pass the selection cuts
  
  std::vector<edm::Ptr<DecayPart> > outCol;
  if (!convCol.isValid())
    return outCol;
  
  const std::vector<DecayPart> *convv = convCol.product();
  
  for (std::vector<DecayPart>::const_iterator it = convv->begin(); it!=convv->end(); ++it) {
    if ( matchesConversion(ele,*it) && isGoodConversion(*it)) {
      outCol.push_back(edm::Ptr<DecayPart>(convCol,it - convv->begin()));
    }
  }
  
  return outCol;
  
}

//--------------------------------------------------------------------------------------------------
bool ConversionMatcher::matchesAnyConversion(const reco::GsfElectron &ele,
                                                  const edm::Handle<std::vector<DecayPart> > &convCol) const
{
  //check if a given electron candidate matches to at least one conversion candidate in the
  //collection
  
  if (!convCol.isValid())
    return false;
  
  const std::vector<DecayPart> *convv = convCol.product();
  
  for (std::vector<DecayPart>::const_iterator it = convv->begin(); it!=convv->end(); ++it) {
    if ( matchesConversion(ele,*it) ) {
      return true;
    }
  }
  
  return false;
  
}


//--------------------------------------------------------------------------------------------------
bool ConversionMatcher::matchesGoodConversion(const reco::GsfElectron &ele,
                                                  const edm::Handle<std::vector<DecayPart> > &convCol) const
{
  //check if a given electron candidate matches to at least one conversion candidate in the
  //collection which also passes the selection cuts
  
  if (!convCol.isValid())
    return false;

  const std::vector<DecayPart> *convv = convCol.product();
  
  for (std::vector<DecayPart>::const_iterator it = convv->begin(); it!=convv->end(); ++it) {
    if ( matchesConversion(ele,*it) && isGoodConversion(*it)) {
      return true;
    }
  }
  
  return false;
  
}


//--------------------------------------------------------------------------------------------------
bool ConversionMatcher::matchesConversion(const reco::GsfElectron &ele,
                                                  const DecayPart &conv) const
{
  
  //Check if a given electron candidate matches a given conversion candidate
  
  for (int i=0; i<conv.nStableChild(); ++i) {
    const StableData &sd = conv.getStableData(i);
    const StablePart *sp = dynamic_cast<const StablePart*>(sd.originalPtr().get());
    if ( sp->trackPtr() == edm::Ptr<reco::Track>(refToPtr(ele.gsfTrack())) ) {
      return true;
    }
  }
  
  return false;
}


//--------------------------------------------------------------------------------------------------
bool ConversionMatcher::isGoodConversion(const DecayPart &conv) const
{
  
  //Check if a given conversion candidate passes the selection cuts to be used for
  //conversion rejection
  
  if (TMath::Prob(conv.chi2(),conv.ndof()) < probMin_)
    return false;
  
  if ( conv.lxy()<lxyMin_ )
    return false;
  
  if ( conv.lz()<lzMin_ )
    return false;
  
  if (conv.position().rho() < radiusMin_)
    return false;
  
  //loop through daughters
  for (int i=0; i<conv.nStableChild(); ++i) {
    const StableData &sd = conv.getStableData(i);
    
    if (sd.nWrongHits() > wrongHitsMax_)
      return false;
    
    const StablePart *sp = dynamic_cast<const StablePart*>(sd.originalPtr().get());
    const reco::Track *trk = sp->track();
    
    //don't apply the track proabability cut to gsf tracks
    if (trk->algo()!=29 && TMath::Prob(trk->chi2(),trk->ndof()) < trackProbMin_)
      return false;
    
  }
  
  return true;
}
