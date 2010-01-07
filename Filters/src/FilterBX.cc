//--------------------------------------------------------------------------------------------------
// $Id: FilterBX.cc,v 1.7 2009/12/18 15:29:05 loizides Exp $
//
// FilterBX
//
// Filter to select events from specified bunch crossing numbers.
//
// See http://venturia.home.cern.ch/venturia/Collisions2009/2009_collisions_results.htm
//
// Authors: C.Loizides, E.Wenger
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
    std::vector<int> crossings_; //if not set hardcoded values will be used
    int type_;                   //==1 for collisions, ==2 for empty
  };
}

using namespace mitedm;
using namespace edm;
using namespace std;

//--------------------------------------------------------------------------------------------------
FilterBX::FilterBX(const edm::ParameterSet& iConfig)
  : crossings_(iConfig.getUntrackedParameter<std::vector<int> >("crossings")),
    type_(iConfig.getUntrackedParameter<int>("coltype",1))
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
bool FilterBX::filter( edm::Event &iEvent, const edm::EventSetup &iSetup)
{
  // Filter events based on specified bunch crossings.
  
  bool accepted = false;

  int bx = iEvent.bunchCrossing();

  if (crossings_.size()>0) {
    for(unsigned int ibunch = 0; ibunch<crossings_.size(); ibunch++) {
      if(bx == crossings_[ibunch]) {
        accepted = true;
        break;
      }
    }
    return accepted;
  }

  unsigned int irun = iEvent.id().run();

  if (irun==123596 || irun==123615) {
    if (type_==1) {
      if ((bx==51) || (bx==2724))
        accepted=true;
    } else if (type_==2) {
      if ((bx==2276) || (bx==3170))
        accepted=true;
    }
  } else if (irun==123732) {
    if (type_==1) {
      if ((bx==3487) || (bx==2596))
        accepted=true;
    } else if (type_==2) {
      if ((bx==2148) || (bx==3042))
        accepted=true;
    }
  } else if (irun==124022 || irun==124023 || irun==124024) {
    if (type_==1) {
      if ((bx==2824) || (bx==151) || (bx==51))
        accepted=true;
    } else if (type_==2) {
      if ((bx==474) || (bx==1365) || (bx==1930) || (bx==2259))
        accepted=true;
    }
  } else if (irun==124120) {
    if (type_==1) {
      if ((bx==51))
        accepted=true;
    } else if (type_==2) {
      if ((bx==1836) || (bx==942))
        accepted=true;
    }
  }

  return accepted;
}

//define this as a plug-in
DEFINE_FWK_MODULE(FilterBX);
