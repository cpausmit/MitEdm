//--------------------------------------------------------------------------------------------------
// $Id: FilterLumi.cc,v 1.1 2009/12/02 20:55:28 loizides Exp $
//
// FilterLumi
//
// Filter to filter out unwanted lumi sections from given runs.
// Runs and lumi sections are hard-coded.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

namespace mitedm
{
  class FilterLumi : public edm::EDFilter {
    public:
      explicit FilterLumi(const edm::ParameterSet &ps);
      ~FilterLumi() {}

    protected:
      virtual bool filter(edm::Event &iEvent, const edm::EventSetup &iSetup);
      bool applyfilter_;
  };
}

using namespace mitedm;
using namespace edm;
using namespace std;

//--------------------------------------------------------------------------------------------------
FilterLumi::FilterLumi(const edm::ParameterSet& iConfig)
  : applyfilter_(iConfig.getUntrackedParameter<bool>("applyfilter",true))
{
}

//--------------------------------------------------------------------------------------------------
bool FilterLumi::filter(edm::Event &iEvent, const edm::EventSetup &iSetup)
{
  // Filter event based on run, lumi section numbers.

  if (!applyfilter_)
    return true;

  unsigned int irun = iEvent.id().run();
  if ( (irun!=122294) &&
       (irun!=122314) &&
       (irun!=123151) )
    return true;

  bool accepted = false;
  unsigned int ilum = iEvent.luminosityBlock();

  if (irun==122294) {
    if(ilum>=37 && ilum<=43) 
      accepted=true;
  } else if (irun==122314) {
    if(ilum>=24 && ilum<=37) 
      accepted=true;
  } else if (irun==123151) {
    if(ilum>=3 && ilum<=19) 
      accepted=true;
  }

  return accepted;
}

//define this as a plug-in
DEFINE_FWK_MODULE(FilterLumi);
