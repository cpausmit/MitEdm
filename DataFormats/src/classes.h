// $Id: classes.h,v 1.8 2009/04/07 08:06:41 loizides Exp $

#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DataFormats/Common/interface/RefToBaseVector.h"
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
    std::vector<edm::Ptr<reco::Track> >               dummy06;
    edm::Wrapper<std::vector<edm::Ptr<reco::Track> > > dummy07;
  }
}
