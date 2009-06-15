//--------------------------------------------------------------------------------------------------
// $Id: Collections.h,v 1.1 2008/09/27 05:48:24 loizides Exp $
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
  typedef std::vector<BasePart*>        PartCol;
  typedef std::vector<StablePart>       StablePartCol;
  typedef std::vector<DecayPart>        DecayPartCol;
}
#endif
