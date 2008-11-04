// $Id: FillerElectrons.h,v 1.11 2008/11/03 18:11:09 bendavid Exp $

#ifndef DataFormats_Common_RefToBaseToPtr_h
#define DataFormats_Common_RefToBaseToPtr_h

/*----------------------------------------------------------------------
  
Ref: A function template for conversion from RefToBase to Ptr

----------------------------------------------------------------------*/
/*
    ----------------------------------------------------------------------*/ 

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
