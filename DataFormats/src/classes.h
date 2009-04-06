// $Id: classes.h,v 1.6 2008/11/04 19:25:55 bendavid Exp $

#include "DataFormats/Common/interface/Wrapper.h"
#include "MitEdm/DataFormats/interface/BasePart.h"
#include "MitEdm/DataFormats/interface/BasePartFwd.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"

namespace
{
  namespace
  {
    std::vector<mitedm::StablePart>                   dummy01;
    std::vector<mitedm::DecayPart>                    dummy02;
    edm::Wrapper<std::vector<mitedm::StablePart> >    dummy03;
    edm::Wrapper<std::vector<mitedm::DecayPart> >     dummy04;
    edm::Wrapper<mitedm::TrackAssociation>            dummy05;
  }
}
