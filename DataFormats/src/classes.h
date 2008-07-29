// $Id: $

#include "DataFormats/Common/interface/Wrapper.h"
#include "MitEdm/DataFormats/interface/BasePart.h"

namespace
{
  namespace
  {
    std::vector<mitedm::BasePart*>                 dummy01;
    edm::Wrapper<std::vector<mitedm::BasePart*> >  dummy02;
  }
}
