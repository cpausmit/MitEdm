//--------------------------------------------------------------------------------------------------
// $Id: DaughterData.h,v 1.1 2008/09/30 12:57:42 bendavid Exp $
//
// DaughterData
//
// Base class to store information about daughters from a specific decay.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_DAUGHTERDATA_H
#define MITEDM_DATAFORMATS_DAUGHTERDATA_H

#include "MitEdm/DataFormats/interface/BasePartFwd.h"

namespace mitedm
{
  class DaughterData
  {
    public:
      DaughterData() {}
      DaughterData(BasePartPtr ptr) : originalPart_(ptr) {}
      ~DaughterData() {}
    
      BasePartPtr                  originalPtr() const          { return originalPart_; }
      void                      setOriginalPtr(BasePartPtr ptr) { originalPart_ = ptr; }
    
    protected: 
      BasePartPtr                  originalPart_; //edm ptr
  };
}
#endif
