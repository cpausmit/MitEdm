//--------------------------------------------------------------------------------------------------
// $Id: CollectionsEdm.h,v 1.2 2008/07/29 22:52:54 bendavid Exp $
//
// Collections
//
// This header file defines the standard types for the collections we want to use.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef DATAFORMAT_COLLECTIONSEDM_H
#define DATAFORMAT_COLLECTIONSEDM_H
 
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
