//--------------------------------------------------------------------------------------------------
// $Id: StableData.h,v 1.3 2008/10/13 10:36:41 bendavid Exp $
//
// StableData
//
// Class to store information about stable daughters from a specific decay
//
// Authors: C.Paus, J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_STABLEDATA_H
#define MITEDM_DATAFORMATS_STABLEDATA_H

#include "MitEdm/DataFormats/interface/DaughterData.h"

namespace mitedm
{
  class StableData : public DaughterData
  {
    public:
      StableData() : p3_(0,0,0), hitsFilled_(false) {}
      StableData(float px, float py, float pz, BasePartPtr stable) :
        DaughterData(stable),
        p3_(px,py,pz),
        hitsFilled_(false)
        {}
        
      ~StableData() {}
    
      const reco::HitPattern &Hits()    const { return hits_; }
      bool                    HitsFilled() const { return hitsFilled_; }
      void                    SetHits(const reco::HitPattern &hits) { hits_=hits; }
      void                    SetHitsFilled(bool filled=true) { hitsFilled_=filled; }
      const ThreeVector32    &p3()      const { return p3_; }
      double                  mass()    const { return originalPart_.get()->mass(); }
      
    protected:
      ThreeVector32    p3_;
      reco::HitPattern hits_;
      bool             hitsFilled_;
  };
}
#endif
