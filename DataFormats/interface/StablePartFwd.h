//--------------------------------------------------------------------------------------------------
// $Id: StablePartFwd.h,v 1.1 2008/09/30 12:57:42 bendavid Exp $
//
// BasePartFwd
//
// Edm reference typedefs for BasePart.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_STABLEPARTFWD_H
#define MITEDM_DATAFORMATS_STABLEPARTFWD_H

#include "DataFormats/Common/interface/Ptr.h"

namespace mitedm {
  class BasePart;
  typedef edm::Ptr<mitedm::StablePart>       StablePartPtr;
}
#endif
