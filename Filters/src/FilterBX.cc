//--------------------------------------------------------------------------------------------------
// $Id: FilterBX.cc,v 1.1 2009/12/07 22:39:50 edwenger Exp $
//
// FilterBX
//
// Filter to select events from specified bunch crossing numbers
//
// Authors: E.Wenger
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
  class FilterBX : public edm::EDFilter {
  public:
    explicit FilterBX(const edm::ParameterSet &ps);
    ~FilterBX() {}
    
  protected:
    virtual bool filter (edm::Event &iEvent, const edm::EventSetup &iSetup);
    std::vector<int> crossings_;
  };
}

using namespace mitedm;
using namespace edm;
using namespace std;

//--------------------------------------------------------------------------------------------------
FilterBX::FilterBX(const edm::ParameterSet& iConfig)
  : crossings_(iConfig.getUntrackedParameter< std::vector<int> >("crossings"))
{
}

//--------------------------------------------------------------------------------------------------
bool FilterBX::filter( edm::Event &iEvent, const edm::EventSetup &iSetup)
{
  // Filter events based on specified bunch crossings
  
  bool accepted = false;
  int bx = iEvent.bunchCrossing();

  for(unsigned int ibunch = 0; ibunch<crossings_.size(); ibunch++) {
    if(bx == crossings_[ibunch]) accepted = true;
  }
  
  return accepted;
}

//define this as a plug-in
DEFINE_FWK_MODULE(FilterBX);
