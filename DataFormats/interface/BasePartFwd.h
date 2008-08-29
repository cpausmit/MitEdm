//--------------------------------------------------------------------------------------------------
// $Id: BasePartFwd.h,v 1.3 2008/08/28 22:09:16 paus Exp $
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
#include "DataFormats/Common/interface/RefToBase.h"

namespace mitedm {
  class BasePart;
  typedef edm::RefToBase<mitedm::BasePart>       BasePartBaseRef;
  typedef edm::RefToBaseVector<mitedm::BasePart> BasePartBaseRefVector;
}
#endif
