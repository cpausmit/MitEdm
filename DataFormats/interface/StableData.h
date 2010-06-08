//--------------------------------------------------------------------------------------------------
// $Id: StableData.h,v 1.5 2009/03/20 17:13:33 loizides Exp $
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
      StableData() : p3_(0,0,0), hitsFilled_(false), nWrongHits_(0) {}
      StableData(double px, double py, double pz, BasePartPtr stable) :
        DaughterData(stable),
        p3_(px,py,pz),
        hitsFilled_(false),
        nWrongHits_(0)
        {}
        
      ~StableData() {}
    
      const reco::HitPattern &Hits()       const { return hits_;                          }
      bool                    HitsFilled() const { return hitsFilled_;                    }
      void                    SetHits(const reco::HitPattern &hits) { hits_=hits;         }
      void                    SetHitsFilled(bool filled=true)       { hitsFilled_=filled; }
      void                    SetNWrongHits(uint n) { nWrongHits_ = n;                    }
      const ThreeVector32    &p3()         const { return p3_;                            }
      double                  mass()       const { return originalPart_.get()->mass();    }
      uint                    nWrongHits() const { return nWrongHits_;                    }
      
    protected:
      ThreeVector32    p3_;          //vertex position
      reco::HitPattern hits_;        //hit pattern
      bool             hitsFilled_;  //=true if hits are filled
      uint             nWrongHits_;
  };
}
#endif
