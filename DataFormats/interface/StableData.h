//--------------------------------------------------------------------------------------------------
// $Id: StableData.h,v 1.2 2008/10/03 23:53:47 loizides Exp $
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
      StableData() : p3_(0,0,0) {}
      StableData(float px, float py, float pz, BasePartPtr stable) :
        DaughterData(stable),
        p3_(px,py,pz)
        {}
        
      ~StableData() {}
    
      const reco::HitPattern &Hits()    const { return hits_; }
      void                    SetHits(const reco::HitPattern &hits) { hits_=hits; }
      const ThreeVector32    &p3()      const { return p3_; }
      double                  mass()    const { return originalPart_.get()->mass(); }
      
    protected:
      ThreeVector32    p3_;
      reco::HitPattern hits_;
  };
}
#endif
