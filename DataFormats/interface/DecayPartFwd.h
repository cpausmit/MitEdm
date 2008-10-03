//--------------------------------------------------------------------------------------------------
// $Id: DecayPartFwd.h,v 1.1 2008/09/30 12:57:42 bendavid Exp $
//
// BasePartFwd
//
// Edm reference typedefs for BasePart.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_DECAYPARTFWD_H
#define MITEDM_DATAFORMATS_DECAYPARTFWD_H

#include "DataFormats/Common/interface/Ptr.h"

namespace mitedm {
  class BasePart;
  typedef edm::Ptr<mitedm::DecayPart>       DecayPartPtr;
}
#endif
