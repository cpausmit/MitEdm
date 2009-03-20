//--------------------------------------------------------------------------------------------------
// $Id: BaseCandProducer.h,v 1.1 2008/09/27 05:48:25 loizides Exp $
//
// A function template for conversion from RefToBase to Ptr. 
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_REFTOBASETOPTR_H
#define MITEDM_PRODUCERS_REFTOBASETOPTR_H

#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/Common/interface/Ptr.h"

namespace mitedm {
  template <typename T>
  edm::Ptr<T> refToBaseToPtr(
    edm::RefToBase<T> const& ref) {
    if (ref.get() == 0 && ref.productGetter() != 0) {
      return edm::Ptr<T>(ref.id(), ref.key(), ref.productGetter());
    }
    else
      return edm::Ptr<T>(ref.id(), ref.get(), ref.key());
  }
}
#endif
