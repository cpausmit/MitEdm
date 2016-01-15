//--------------------------------------------------------------------------------------------------
// BaseCandProducer
//
// Base class for all more specific candidate producers.
//
// Authors: C.Paus, Y.Iiyama
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_BASECANDPRODUCER_H
#define MITEDM_PRODUCERS_BASECANDPRODUCER_H

#include <vector>
#include <iostream>
#include <TString.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace mitedm
{
  class BaseCandProducer : public edm::EDProducer
  {
    public:
      explicit BaseCandProducer(const edm::ParameterSet&);
      ~BaseCandProducer() {}
    
    protected:
      void beginJob() {}
      void produce(edm::Event&, const edm::EventSetup&) = 0;
      void endJob() {}

      // generic accessors to make the code more simple
      void PrintErrorAndExit(const char *msg) const;
      template <typename TYPE>
      bool GetProduct(edm::EDGetTokenT<TYPE> const&, edm::Handle<TYPE>&,
                      const edm::Event &event, bool ignore = true) const;

      // Parameters always being used
      int         oPid_; // pid of candidate particle
  };
  
  //------------------------------------------------------------------------------------------------
  template <typename TYPE>
  inline bool BaseCandProducer::GetProduct(edm::EDGetTokenT<TYPE> const& token, edm::Handle<TYPE> &product,
                                           const edm::Event &evt, bool ignore) const
  {
    // Try to access data collection from EDM file. We check if we really get just one
    // product with the given name. If not we print an error and exit.

    try {
      evt.getByToken(token, product);
      if (!product.isValid()) 
        throw edm::Exception(edm::errors::Configuration, "BaseCandProducer::GetProduct()\n")
          << "Cannot get collection " << typeid(TYPE).name();
    }
    catch (cms::Exception& e) {
      if (ignore)
        return false;
      else {
        edm::LogError("BaseCandProducer") << "Cannot get collection" << typeid(TYPE).name();
        PrintErrorAndExit(Form("Cannot get collection %s", typeid(TYPE).name()));
      }
    }
    return true;
  }
}
#endif
