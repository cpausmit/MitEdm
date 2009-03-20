//--------------------------------------------------------------------------------------------------
// $Id: Types.h,v 1.8 2008/11/04 19:23:58 bendavid Exp $
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
#include "DataFormats/TrackReco/interface/Track.h"
#include "MitCommon/DataFormats/interface/Types.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Common/interface/AssociationMap.h"

namespace mitedm
{
  using mithep::ThreeVector;
  using mithep::ThreeVector32;
  using mithep::FourVector;
  using mithep::FourVector32;
  using mithep::ThreeSymMatrix;
  using mithep::SevenSymMatrix;
  
  typedef edm::Ptr<reco::Vertex> VertexPtr;
  typedef edm::Ptr<reco::Track>  TrackPtr;
  typedef edm::AssociationMap<edm::OneToManyWithQuality<edm::View<reco::Track>, 
                                                        edm::View<reco::Track>, 
                                                        double > > TrackAssociation;
}
#endif
