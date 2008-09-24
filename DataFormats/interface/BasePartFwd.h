//--------------------------------------------------------------------------------------------------
// $Id: BasePartFwd.h,v 1.4 2008/08/29 00:27:21 loizides Exp $
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
  typedef edm::PtrVector<mitedm::BasePart> BasePartPtrVector;
}
#endif
