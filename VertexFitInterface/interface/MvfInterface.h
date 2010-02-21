//--------------------------------------------------------------------------------------------------
// $Id: MvfInterface.h,v 1.9 2008/11/13 16:36:48 paus Exp $
//
// MvfInterface class header file
//
// Provide a CMS specific interface for the generic MultiVertexFitter.
// 
// Author: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_VERTEXFITINTERFACE_MVFINTERFACE_H
#define MITEDM_VERTEXFITINTERFACE_MVFINTERFACE_H

#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "MitCommon/VertexFit/interface/MultiVertexFitter.h"
#include "MitCommon/VertexFit/interface/MultiVertexFitterC.h"
#include "MitCommon/VertexFit/interface/MultiVertexFitterD.h"

namespace mitedm
{  
  class MvfInterface
  {
    public:
      MvfInterface(mithep::MultiVertexFitter  *fitter);
      MvfInterface(mithep::MultiVertexFitterC *fitter);
      MvfInterface(mithep::MultiVertexFitterD *fitter);
      ~MvfInterface() {}

      bool addTrack(const reco::Track *trk, const int id, const float mass,
                    mithep::MultiVertexFitter::vertexNumber jv);
      bool addTrack(const reco::Track *trk, const int id, const float mass,
                    mithep::MultiVertexFitterC::vertexNumber jv);
      bool addTrack(const reco::Track *trk, const int id, const float mass,
                    mithep::MultiVertexFitterD::vertexNumber jv);
      bool addTrack(const reco::TransientTrack &trk, const int id, const float mass,
                    mithep::MultiVertexFitter::vertexNumber jv);
      bool addTrack(const reco::TransientTrack &trk, const int id, const float mass,
                    mithep::MultiVertexFitterC::vertexNumber jv);
      bool addTrack(const reco::TransientTrack &trk, const int id, const float mass,
                    mithep::MultiVertexFitterD::vertexNumber jv);                    

      mithep::MultiVertexFitter  *fitter () { return mvf_;}
      mithep::MultiVertexFitterC *fitterC() { return mvfC_;}
      mithep::MultiVertexFitterD *fitterD() { return mvfD_;}
  
    private:
      mithep::MultiVertexFitter  *mvf_;  //the multi vertex fitter
      mithep::MultiVertexFitterC *mvfC_; //the multi vertex fitter
      mithep::MultiVertexFitterD *mvfD_; //the multi vertex fitter
  };
}
#endif
