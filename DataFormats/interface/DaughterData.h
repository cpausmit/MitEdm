//--------------------------------------------------------------------------------------------------
// $Id: BasePart.h,v 1.3 2008/08/29 00:27:21 loizides Exp $
//
// DaughterData
//
// Base class to store information about daughters from a specific decay
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DAUGHTERDATA_H
#define MITEDM_DAUGHTERDATA_H

#include "MitEdm/DataFormats/interface/BasePartFwd.h"

namespace mitedm
{
  class DaughterData
  {
   public:
    // Constructors
    DaughterData() {}
    DaughterData(BasePartPtr ptr) : originalPart_(ptr) {}
    ~DaughterData() {}
    
    BasePartPtr                  originalPtr() const { return originalPart_; }
    void                      setOriginalPtr(BasePartPtr ptr) { originalPart_ = ptr; }
    
  protected: 
    // General stuff
    BasePartPtr                  originalPart_;
  };
}

#endif
