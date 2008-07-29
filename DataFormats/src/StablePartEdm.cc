// $Id: $

#include "MitEdm/DataFormats/interface/StablePartEdm.h"
#include "MitEdm/DataFormats/interface/BasePartAction.h"
#include "DataFormats/TrackReco/interface/Track.h"

using namespace std;
using namespace mitedm;

StablePartEdm::StablePartEdm(int pid, const reco::TrackRef &trk) :
  StablePart(pid),
  track_    (trk)
{
  convertFromTrack(track());
}

StablePartEdm::StablePartEdm(int pid, double mass, const reco::TrackRef &trk) :
  StablePart(pid,mass),
  track_    (trk)
{
  convertFromTrack(track());
}

StablePartEdm::StablePartEdm(StablePartEdm &s) :
  StablePart(s),
  track_    (s.track_)
{
  convertFromTrack(track());
}

void StablePartEdm::convertFromTrack(const reco::Track *trk)
{
  setHits(0);
  setStat(0);
  setPhi0 (trk->parameter(2));         setPhi0Err(sqrt(trk->covariance(2,2)));
  setD0Raw(trk->parameter(3));         setD0Err  (sqrt(trk->covariance(3,3)));
  setPt   (trk->pt()*trk->charge());   setPtErr  (trk->ptError());
  setZ0Raw(trk->parameter(4));         setZ0Err  (sqrt(trk->covariance(4,4)));
  setCotT (trk->parameter(1));         setCotTErr(sqrt(trk->covariance(1,1)));
}

void StablePartEdm::printEdm(ostream &os) const
{
  os << " StablePartEdm::print - pid: " << pid_ << "  mass: " << mass_
     << "  track pointer: " << track_.get() << endl;
}
