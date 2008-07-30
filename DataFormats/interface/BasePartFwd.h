//--------------------------------------------------------------------------------------------------
// $Id: BasePartFwd.h,v 1.1 2008/07/29 22:56:37 bendavid Exp $
//
// BasePartFwd
//
// Edm reference typedefs for BasePart
//
// Author List: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_BASEPARTFWD_H
#define MITEDM_BASEPARTFWD_H

#include <vector>
#include "DataFormats/Common/interface/RefToBase.h"

namespace mitedm {
  class BasePart;
  typedef edm::RefToBase<mitedm::BasePart> BasePartBaseRef;
  typedef edm::RefToBaseVector<mitedm::BasePart> BasePartBaseRefVector;
}
#endif
