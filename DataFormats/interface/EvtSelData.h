//--------------------------------------------------------------------------------------------------
// $Id: EvtSelData.h,v 1.1 2009/12/07 22:04:50 loizides Exp $
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
      EvtSelData() : eHfNeg_(0), eHfPos_(0), eHfNegTime_(0), eHfPosTime_(0), 
                     eHcalNeg_(0), eHcalPos_(0), 
                     eCaNeg_(0), eCaPos_(0), eCaNegTime_(0), eCaPosTime_(0),
                     eZdcNeg_(0), eZdcPos_(0), eZdcNegTime_(0), eZdcPosTime_(0),
	             ePxbHits_(0), eClusVtxQual_(0){}
      EvtSelData(double eHfNeg, double eHfPos, double eHfNegTime, double eHfPosTime,
                 double eHcalNeg, double eHcalPos,
                 double eCaNeg, double eCaPos, double eCaNegTime, double eCaPosTime,
                 double eZdcNeg, double eZdcPos, double eZdcNegTime, double eZdcPosTime,
		 int ePxbHits, double eClusVtxQual) : 
        eHfNeg_(eHfNeg), eHfPos_(eHfPos), eHfNegTime_(eHfNegTime), eHfPosTime_(eHfPosTime),
        eHcalNeg_(eHcalNeg), eHcalPos_(eHcalPos),
        eCaNeg_(eCaNeg), eCaPos_(eCaPos), eCaNegTime_(eCaNegTime), eCaPosTime_(eCaPosTime),
        eZdcNeg_(eZdcNeg), eZdcPos_(eZdcPos), eZdcNegTime_(eZdcNegTime), eZdcPosTime_(eZdcPosTime),
        ePxbHits_(ePxbHits), eClusVtxQual_(eClusVtxQual) {}
      ~EvtSelData() {}

      double eHfNeg()         const { return eHfNeg_; }
      double eHfPos()         const { return eHfPos_; }
      double eHfNegTime()     const { return eHfNegTime_; }
      double eHfPosTime()     const { return eHfPosTime_; }
      double eCastorNeg()     const { return eCaNeg_; }
      double eCastorPos()     const { return eCaPos_; }
      double eCastorNegTime() const { return eCaNegTime_; }
      double eCastorPosTime() const { return eCaPosTime_; }
      double eZdcNeg()        const { return eZdcNeg_; }
      double eZdcPos()        const { return eZdcPos_; }
      double eZdcNegTime()    const { return eZdcNegTime_; }
      double eZdcPosTime()    const { return eZdcPosTime_; }
      int    ePxbHits()       const { return ePxbHits_; }
      double eClusVtxQual()   const { return eClusVtxQual_; }

    protected:
      double eHfNeg_;       //energy HF negative side
      double eHfPos_;       //energy HF positive side
      double eHfNegTime_;   //energy weighted HF time on negative side 
      double eHfPosTime_;   //energy weighted HF time on positive side 
      double eHcalNeg_;     //energy HCAL negative side
      double eHcalPos_;     //energy HCAL positive side
      double eCaNeg_;       //energy CASTOR negative side
      double eCaPos_;       //energy CASTOR positive side
      double eCaNegTime_;   //energy weighted CASTOR time on negative side 
      double eCaPosTime_;   //energy weighted CASTOR time on positive side 
      double eZdcNeg_;      //energy ZDC negative side
      double eZdcPos_;      //energy ZDC positive side
      double eZdcNegTime_;  //energy weighted ZDC time on negative side 
      double eZdcPosTime_;  //energy weighted ZDC time on positive side 
      int    ePxbHits_;     //number of pixel rechits in the three barrel layers
      double eClusVtxQual_; //incompatibility of pixel cluster shapes with vertex
   };
}
#endif
