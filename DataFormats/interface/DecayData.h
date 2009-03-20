//--------------------------------------------------------------------------------------------------
// $Id: DecayData.h,v 1.2 2008/10/03 23:53:46 loizides Exp $
//
// DecayData
//
// Class to store information about decay daughters from a specific decay.
//
// Authors: C.Paus, J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_DECAYDATA_H
#define MITEDM_DATAFORMATS_DECAYDATA_H

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

      DecayData(FourVector32 &p4, double massErr, double lxy, double lxyErr, double dxy,
                                  double dxyErr, double lz, double lzErr,
                                  BasePartPtr decay) :
        DaughterData(decay),
        p4_(p4),
        massErr_(massErr),
        lxy_(lxy),
        lxyErr_(lxyErr),
        dxy_(dxy),
        dxyErr_(dxyErr),
        lz_(lz),
        lzErr_(lzErr) {}
        
      ~DecayData() {}
        
      const FourVector32 &p4()      const { return p4_; }
      double              massErr() const { return massErr_; }
      double              lxy()     const { return lxy_; }
      double              lxyErr()  const { return lxyErr_; }
      double              dxy()     const { return dxy_; }
      double              dxyErr()  const { return dxyErr_; }
      double              lz()      const { return lz_; }
      double              lzErr()   const { return lzErr_; }
        
    protected:
      FourVector32        p4_;      //momentum at vertex
      double              massErr_; //error on mass from fit
      double              lxy_;     //lxy from fit
      double              lxyErr_;  //lxy error from fit
      double              dxy_;     //dxy from fit
      double              dxyErr_;  //dxy error from fit
      double              lz_;      //lz from fit
      double              lzErr_;   //lz error from fit
  };
}
#endif
