//--------------------------------------------------------------------------------------------------
// $Id: BasePart.h,v 1.3 2008/08/29 00:27:21 loizides Exp $
//
// DecayData
//
// Class to store information about decay daughters from a specific decay
//
// Authors: C.Paus, J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DECAYDATA_H
#define MITEDM_DECAYDATA_H

#include "MitEdm/DataFormats/interface/DaughterData.h"

namespace mitedm
{
  class DecayData : public DaughterData
  {
    public:
      DecayData() :
        p4_(0,0,0,0),
        massErr_(0),
        lxy_(0),
        lxyErr_(0),
        dxy_(0),
        dxyErr_(0),
        lz_(0),
        lzErr_(0) {}
      DecayData(FourVector32 &p4, float massErr, float lxy, float lxyErr, float dxy,
                                  float dxyErr, float lz, float lzErr,
                                  BasePartPtr decay) :
        DaughterData(decay),
        p4_(p4),
        massErr_(massErr),
        lxy_(lxy),
        lxyErr_(lxyErr),
        dxy_(dxy),
        dxyErr_(dxyErr),
        lz_(lz),
        lzErr_(lzErr)
        {}
        
      ~DecayData() {}
        
      const FourVector32 &p4()      const { return p4_; }
      float            massErr() const { return massErr_; }
      float            lxy()     const { return lxy_; }
      float            lxyErr()  const { return lxyErr_; }
      float            dxy()     const { return dxy_; }
      float            dxyErr()  const { return dxyErr_; }
      float            lz()      const { return lz_; }
      float            lzErr()   const { return lzErr_; }
        
    protected:
      FourVector32    p4_;
      float           massErr_;
      float           lxy_;
      float           lxyErr_;
      float           dxy_;
      float           dxyErr_;
      float           lz_;
      float           lzErr_;
  };
}

#endif
