//--------------------------------------------------------------------------------------------------
// ProducerStable
//
// Converting tracks into StableParts.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_PRODUCERSTABLE_H
#define MITEDM_PRODUCERS_PRODUCERSTABLE_H

#include "MitEdm/Producers/interface/BaseCandProducer.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/TrackReco/interface/Track.h"

namespace mitedm
{
  class ProducerStable : public BaseCandProducer
  {
    public:
      explicit ProducerStable(const edm::ParameterSet&);
      ~ProducerStable();
    
    private:
      void produce(edm::Event&, const edm::EventSetup&);

      edm::EDGetTokenT<edm::View<reco::Track>> tracksToken_;
  };
}
#endif
