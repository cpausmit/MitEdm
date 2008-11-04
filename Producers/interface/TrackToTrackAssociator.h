//--------------------------------------------------------------------------------------------------
// $Id: TrackToTrackAssociator.h,v 1.1 2008/09/27 05:48:25 loizides Exp $
//
// TrackToTrackAssociator
//
// Base class for all more specific candidate producers.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_TRACKTOTRACKASSOCIATOR_H
#define MITEDM_PRODUCERS_TRACKTOTRACKASSOCIATOR_H

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
  class TrackToTrackAssociator : public edm::EDProducer
  {
    public:
      explicit TrackToTrackAssociator(const edm::ParameterSet&);
      ~TrackToTrackAssociator() {}
    
    protected:
      void beginJob(const edm::EventSetup&) {}
      void produce(edm::Event&, const edm::EventSetup&);
      void endJob() {}

      // generic accessors to make the code more simple
      void PrintErrorAndExit(const char *msg) const;
      template <typename TYPE>
      bool GetProduct(const std::string name, edm::Handle<TYPE> &product,
                      const edm::Event &event, bool ignore = true) const;

      std::string fromTracksName_; //name of tracks which are association keys
      std::string toTracksName_;   //name of tracks which are association values
  };
  
  //------------------------------------------------------------------------------------------------
  template <typename TYPE>
  inline bool TrackToTrackAssociator::GetProduct(const std::string edmName, edm::Handle<TYPE> &product,
					const edm::Event &evt, bool ignore) const
  {
    // Try to access data collection from EDM file. We check if we really get just one
    // product with the given name. If not we print an error and exit.

    try {
      evt.getByLabel(edm::InputTag(edmName),product);
      if (! product.isValid()) 
	throw edm::Exception(edm::errors::Configuration, "TrackToTrackAssociator::GetProduct()\n")
	  << "Cannot get collection with label " << edmName << std::endl;
    } catch (...) {
      if (ignore)
	return false;
      else {
	edm::LogError("TrackToTrackAssociator") << "Cannot get collection with label "
				       << edmName << std::endl;
	PrintErrorAndExit(Form("Cannot get collection with label %s", edmName.c_str()));
      }
    }
    return true;
  }
}
#endif
