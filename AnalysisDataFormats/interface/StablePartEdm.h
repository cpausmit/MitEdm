//--------------------------------------------------------------------------------------------------
// $ Id: $
//
// Description: Class StablePartEdm
// ------------
//
// Extend the stable class with just the track link. This cannot be ported to the Oak, therefore we
// have to keep it separate.
//
// Author List: Ch.Paus
//--------------------------------------------------------------------------------------------------
#include <iostream>
#include "MitEdm/AnalysisDataFormats/interface/StablePart.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

namespace mitedm
{
  class StablePartEdm : public StablePart
  {
    public:
      // Constructors
      StablePartEdm();
      StablePartEdm(int pid, const reco::TrackRef &trk);
      StablePartEdm(int pid, double mass, const reco::TrackRef &trk);
      StablePartEdm(StablePartEdm &s);
      ~StablePartEdm() {}
    
      // Helpers
      void               convertFromTrack(const reco::Track *trk);
      // Accessors
      const reco::Track *track      () const { return track_.get(); }
      virtual double     trackCharge() const { return track()->charge(); }

      // General printing method
      virtual void       printEdm   (std::ostream &os = std::cout) const;

    private:
      reco::TrackRef     track_;
  };
}
