//--------------------------------------------------------------------------------------------------
// $Id: CollectionsEdm.h,v 1.3 2008/07/31 13:29:35 bendavid Exp $
//
// Collections
//
// This header file defines the standard types for the collections we want to use.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_COLLECTIONS_H
#define MITEDM_DATAFORMATS_COLLECTIONS_H
 
#include <vector>
#include "MitEdm/DataFormats/interface/BasePart.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"

namespace mitedm
{
  typedef std::vector<BasePart*>        PartColl;
  typedef std::vector<StablePart>       StablePartCol;
  typedef std::vector<DecayPart>        DecayPartCol;
}
#endif
