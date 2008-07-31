//==================================================================================================
// $Id $
//
// Description: class BaseCandidate
//
// Base class for all more specific candidate preparation.
//
// Original Author:  Christoph Paus
// Created:          Mon Jul 21 14:42:57 CEST 2008
//==================================================================================================
#ifndef _PRODUCERS_BASECANDIDATE_H_
#define _PRODUCERS_BASECANDIDATE_H_

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
  class BaseCandidate : public edm::EDProducer
  {
  public:
    explicit BaseCandidate(const edm::ParameterSet&);
    ~BaseCandidate() {}
    
  protected:
    virtual void beginJob(const edm::EventSetup&) { }
    virtual void produce(edm::Event&, const edm::EventSetup&) = 0;
    virtual void endJob() {}

    // generic accessors to make the code more simple
    void         PrintErrorAndExit(const char *msg) const;
    template <typename TYPE>
    bool         GetProduct       (const std::string name, edm::Handle<TYPE> &product,
				   const edm::Event &event, bool ignore = true) const;

    // Parameters always being used
    int         oPid_;
  };

  //------------------------------------------------------------------------------------------------
  template <typename TYPE>
  inline bool BaseCandidate::GetProduct(const std::string edmName, edm::Handle<TYPE> &product,
					const edm::Event &evt, bool ignore) const
    {
    //----------------------------------------------------------------------------------------------
    // Try to access data collection from EDM file. We check if we really get just one
    // product with the given name. If not we print an error and exit.
    //----------------------------------------------------------------------------------------------
    try {
      evt.getByLabel(edm::InputTag(edmName),product);
      if (! product.isValid()) 
	throw edm::Exception(edm::errors::Configuration, "BaseCandidate::GetProduct()\n")
	  << "Cannot get collection with label " << edmName << std::endl;
    } catch (...) {
      if (ignore)
	return false;
      else {
	edm::LogError("BaseCandidate") << "Cannot get collection with label "
				       << edmName << std::endl;
	PrintErrorAndExit(Form("Cannot get collection with label %s", edmName.c_str()));
      }
    }
    return true;
  }
}
#endif
