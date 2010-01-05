//--------------------------------------------------------------------------------------------------
// $Id: EvtSelData.h,v 1.4 2009/12/09 12:50:02 edwenger Exp $
//
// EvtSelData
//
// Class to store information about event selection data.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_EVTSELDATA_H
#define MITEDM_DATAFORMATS_EVTSELDATA_H

namespace mitedm
{
  class EvtSelData
  {
    public:
      EvtSelData() : eHcalNeg_(0), eHcalPos_(0), 
                     eHfNeg_(0), eHfPos_(0), eHfNegTime_(0), eHfPosTime_(0), 
                     eCaNeg_(0), eCaPos_(0), eCaNegTime_(0), eCaPosTime_(0),
                     eZdcNeg_(0), eZdcPos_(0), eZdcNegTime_(0), eZdcPosTime_(0),
	             ePxbHits_(0), ePxHits_(0), eClusVtxQual_(0), eClusVtxDiff_(0), 
	             eHPTrkFrac_(0) {}
      EvtSelData(double eHcalNeg, double eHcalPos,
                 double eHfNeg, double eHfPos, double eHfNegTime, double eHfPosTime,
                 double eCaNeg, double eCaPos, double eCaNegTime, double eCaPosTime,
                 double eZdcNeg, double eZdcPos, double eZdcNegTime, double eZdcPosTime,
		 int ePxbHits, int ePxHits, double eClusVtxQual, double eClusVtxDiff, 
		 double eHPTrkFrac) : 
        eHcalNeg_(eHcalNeg), eHcalPos_(eHcalPos),
	eHfNeg_(eHfNeg), eHfPos_(eHfPos), eHfNegTime_(eHfNegTime), eHfPosTime_(eHfPosTime),
        eCaNeg_(eCaNeg), eCaPos_(eCaPos), eCaNegTime_(eCaNegTime), eCaPosTime_(eCaPosTime),
        eZdcNeg_(eZdcNeg), eZdcPos_(eZdcPos), eZdcNegTime_(eZdcNegTime), eZdcPosTime_(eZdcPosTime),
        ePxbHits_(ePxbHits), ePxHits_(ePxHits), eClusVtxQual_(eClusVtxQual), eClusVtxDiff_(eClusVtxDiff),
	eHPTrkFrac_(eHPTrkFrac) {}
      ~EvtSelData() {}

      double eHcalNeg()       const { return eHcalNeg_;     }
      double eHcalPos()       const { return eHcalPos_;     }
      double eHfNeg()         const { return eHfNeg_;       }
      double eHfPos()         const { return eHfPos_;       }
      double eHfNegTime()     const { return eHfNegTime_;   }
      double eHfPosTime()     const { return eHfPosTime_;   }
      double eCastorNeg()     const { return eCaNeg_;       }
      double eCastorPos()     const { return eCaPos_;       }
      double eCastorNegTime() const { return eCaNegTime_;   }
      double eCastorPosTime() const { return eCaPosTime_;   }
      double eZdcNeg()        const { return eZdcNeg_;      }
      double eZdcPos()        const { return eZdcPos_;      }
      double eZdcNegTime()    const { return eZdcNegTime_;  }
      double eZdcPosTime()    const { return eZdcPosTime_;  }
      int    ePxbHits()       const { return ePxbHits_;     }
      int    ePxHits()        const { return ePxHits_;      }
      double eClusVtxQual()   const { return eClusVtxQual_; }
      double eClusVtxDiff()   const { return eClusVtxDiff_; }
      double eHPTrkFrac()     const { return eHPTrkFrac_;   } 

    protected:
      double eHcalNeg_;     //energy HCAL negative side
      double eHcalPos_;     //energy HCAL positive side
      double eHfNeg_;       //energy HF negative side
      double eHfPos_;       //energy HF positive side
      double eHfNegTime_;   //energy weighted HF time on negative side 
      double eHfPosTime_;   //energy weighted HF time on positive side 
      double eCaNeg_;       //energy CASTOR negative side
      double eCaPos_;       //energy CASTOR positive side
      double eCaNegTime_;   //energy weighted CASTOR time on negative side 
      double eCaPosTime_;   //energy weighted CASTOR time on positive side 
      double eZdcNeg_;      //energy ZDC negative side
      double eZdcPos_;      //energy ZDC positive side
      double eZdcNegTime_;  //energy weighted ZDC time on negative side 
      double eZdcPosTime_;  //energy weighted ZDC time on positive side 
      int    ePxbHits_;     //number of pixel rechits in the three barrel layers
      int    ePxHits_;      //number of pixel rechits in all barrel and forward layers
      double eClusVtxQual_; //incompatibility of pixel cluster shapes with vertex (ratio)
      double eClusVtxDiff_; //incompatibility of pixel cluster shapes with vertex (difference)
      double eHPTrkFrac_;   //fraction of high-purity tracks out of all with "loose" cuts

   };
}
#endif
