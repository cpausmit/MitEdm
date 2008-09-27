//--------------------------------------------------------------------------------------------------
// $Id: BasePartFwd.h,v 1.6 2008/09/24 09:47:22 bendavid Exp $
//
// BasePartFwd
//
// Edm reference typedefs for BasePart
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_BASEPARTFWD_H
#define MITEDM_DATAFORMATS_BASEPARTFWD_H

#include <vector>
#include "DataFormats/Common/interface/Ptr.h"
#include "DataFormats/Common/interface/PtrVector.h"

namespace mitedm {
  class BasePart;
  typedef edm::Ptr<mitedm::BasePart>       BasePartPtr;
  typedef std::vector<BasePartPtr>         BasePartPtrVector;
}
#endif
