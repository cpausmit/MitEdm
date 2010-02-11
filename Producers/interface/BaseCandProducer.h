//--------------------------------------------------------------------------------------------------
// $Id: BaseCandProducer.h,v 1.2 2009/03/20 18:01:47 loizides Exp $
//
// BaseCandProducer
//
// Base class for all more specific candidate producers.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_BASECANDPRODUCER_H
#define MITEDM_PRODUCERS_BASECANDPRODUCER_H

#include <vector>
#include <iostream>
#include <TString.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
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
      bool GetProduct(const std::string name, edm::Handle<TYPE> &product,
                      const edm::Event &event, bool ignore = true) const;

      // Parameters always being used
      int         oPid_; // pid of candidate particle
  };
  
  //------------------------------------------------------------------------------------------------
  template <typename TYPE>
  inline bool BaseCandProducer::GetProduct(const std::string edmName, edm::Handle<TYPE> &product,
                                           const edm::Event &evt, bool ignore) const
  {
    // Try to access data collection from EDM file. We check if we really get just one
    // product with the given name. If not we print an error and exit.

    try {
      evt.getByLabel(edm::InputTag(edmName),product);
      if (! product.isValid()) 
	throw edm::Exception(edm::errors::Configuration, "BaseCandProducer::GetProduct()\n")
	  << "Cannot get collection with label " << edmName << std::endl;
    } catch (...) {
      if (ignore)
	return false;
      else {
	edm::LogError("BaseCandProducer") << "Cannot get collection with label "
				       << edmName << std::endl;
	PrintErrorAndExit(Form("Cannot get collection with label %s", edmName.c_str()));
      }
    }
    return true;
  }
}
#endif
