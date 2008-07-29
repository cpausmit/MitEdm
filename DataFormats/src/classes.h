// $Id: classes.h,v 1.1 2008/07/29 13:16:22 loizides Exp $

#include "DataFormats/Common/interface/Wrapper.h"
#include "MitEdm/DataFormats/interface/BasePart.h"
#include "MitEdm/DataFormats/interface/BasePartFwd.h"
#include "MitEdm/DataFormats/interface/StablePartEdm.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "DataFormats/Common/interface/BaseVectorHolder.h"

namespace
{
  namespace
  {
    std::vector<mitedm::BasePart*>                 dummy01;
    edm::Wrapper<std::vector<mitedm::BasePart*> >  dummy02;
    std::vector<mitedm::StablePartEdm>                dummy03;
    std::vector<mitedm::DecayPart>                 dummy04;
    edm::Wrapper<std::vector<mitedm::StablePartEdm> >  dummy05;
    edm::Wrapper<std::vector<mitedm::DecayPart> >  dummy06;
    edm::Wrapper<std::vector<mitedm::DecayPart> >  dummy07;
    edm::RefToBase<mitedm::BasePart>               dummy08;
    edm::RefToBaseVector<mitedm::BasePart>         dummy09;
    //edm::reftobase::BaseVectorHolder<mitedm::BasePart>  dummy10;
  }
}
