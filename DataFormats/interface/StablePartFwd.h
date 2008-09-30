//--------------------------------------------------------------------------------------------------
// $Id: BasePartFwd.h,v 1.6 2008/09/24 09:47:22 bendavid Exp $
//
// BasePartFwd
//
// Edm reference typedefs for BasePart
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_STABLEPARTFWD_H
#define MITEDM_STABLEPARTFWD_H

#include "DataFormats/Common/interface/Ptr.h"

namespace mitedm {
  class BasePart;
  typedef edm::Ptr<mitedm::StablePart>       StablePartPtr;
}
#endif
