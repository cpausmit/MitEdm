//--------------------------------------------------------------------------------------------------
// $Id: $
//
// Collections
//
// This header file defines the standard types for the collections we want to use.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef ANALYSISDATAFORMAT_COLLECTIONS_H
#define ANALYSISDATAFORMAT_COLLECTIONS_H
 
#include <vector>
#include "MitEdm/AnalysisDataFormats/interface/ObjArray.h"
#include "MitEdm/AnalysisDataFormats/interface/BasePart.h"

namespace mitedm
{
  typedef std::vector<BasePart*>        PartColl;
  typedef ObjArray<BasePart>            BasePartObjArr;
}
#endif
