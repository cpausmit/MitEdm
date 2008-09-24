//--------------------------------------------------------------------------------------------------
// $Id: BasePartFwd.h,v 1.5 2008/09/24 09:00:54 bendavid Exp $
//
// BasePartFwd
//
// Edm reference typedefs for BasePart
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_BASEPARTFWD_H
#define MITEDM_BASEPARTFWD_H

#include <vector>
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"

namespace mitedm {
  class BasePart;
  typedef edm::Ptr<mitedm::BasePart>       BasePartPtr;
  typedef std::vector<BasePartPtr>         BasePartPtrVector;
}
#endif
