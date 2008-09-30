//--------------------------------------------------------------------------------------------------
// $Id: Types.h,v 1.6 2008/09/27 05:48:24 loizides Exp $
//
// Types
//
// Here we define additional types.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_TYPES_H
#define MITEDM_DATAFORMATS_TYPES_H
 
#include <Rtypes.h>
#include "MitCommon/DataFormats/interface/Types.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

namespace mitedm
{
  using mithep::ThreeVector;
  using mithep::ThreeVector32;
  using mithep::FourVector;
  using mithep::FourVector32;
  using mithep::ThreeSymMatrix;
  using mithep::SevenSymMatrix;
  
  typedef edm::Ptr<reco::Vertex> VertexPtr;
}
#endif
