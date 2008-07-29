//--------------------------------------------------------------------------------------------------
// $Id: CollectionsEdm.h,v 1.1 2008/07/28 23:05:36 paus Exp $
//
// Collections
//
// This header file defines the standard types for the collections we want to use.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef DATAFORMAT_COLLECTIONS_H
#define DATAFORMAT_COLLECTIONS_H
 
#include <vector>
#include "MitEdm/AnalysisDataFormats/interface/BasePart.h"

namespace mitedm
{
  typedef std::vector<BasePart*>        PartColl;
}
#endif
