// $Id: classes.h,v 1.3 2008/07/30 08:41:41 loizides Exp $

#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"
#include "MitEdm/DataFormats/interface/BasePart.h"
#include "MitEdm/DataFormats/interface/BasePartFwd.h"
#include "MitEdm/DataFormats/interface/StablePartEdm.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "DataFormats/Common/interface/BaseVectorHolder.h"

namespace
{
  namespace
  {
    std::vector<mitedm::StablePartEdm>                dummy01;
    std::vector<mitedm::DecayPart>                    dummy02;
    edm::Wrapper<std::vector<mitedm::StablePartEdm> > dummy03;
    edm::Wrapper<std::vector<mitedm::DecayPart> >     dummy04;
//    edm::RefToBase<mitedm::BasePart>                  dummy05;
 //   edm::RefToBaseVector<mitedm::BasePart>            dummy06;
//    edm::Wrapper<RefToBase<mitedm::BasePart> >        dummy07
//    edm::Wrapper<edm::RefToBaseVector<mitedm::BasePart> > dummy08;
 //   edm::reftobase::BaseVectorHolder<mitedm::BasePart>    *dummy09;
//    edm::reftobase::BaseHolder<mitedm::BasePart> dummy11;

//    edm::reftobase::Holder<mitedm::BasePart, mitedm::BasePart> dummy10;

  }
}
