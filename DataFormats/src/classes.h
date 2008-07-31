// $Id: classes.h,v 1.4 2008/07/30 11:43:16 bendavid Exp $

#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"
#include "MitEdm/DataFormats/interface/BasePart.h"
#include "MitEdm/DataFormats/interface/BasePartFwd.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "DataFormats/Common/interface/BaseVectorHolder.h"

namespace
{
  namespace
  {
    std::vector<mitedm::StablePart>                   dummy01;
    std::vector<mitedm::DecayPart>                    dummy02;
    edm::Wrapper<std::vector<mitedm::StablePart> >    dummy03;
    edm::Wrapper<std::vector<mitedm::DecayPart> >     dummy04;
  }
}
