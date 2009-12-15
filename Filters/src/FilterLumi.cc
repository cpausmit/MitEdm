//--------------------------------------------------------------------------------------------------
// $Id: FilterLumi.cc,v 1.9 2009/12/15 13:59:40 rougny Exp $
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
       (irun!=123151) &&
       (irun!=123596) &&
       (irun!=123615) &&
       (irun!=123732) &&
       (irun!=123815) &&
       (irun!=123818) &&
       (irun!=123906) &&
       (irun!=123908) &&
       (irun!=123987) &&
       (irun!=124006) &&
       (irun!=124009) &&
       (irun!=124020) &&
       (irun!=124022) &&
       (irun!=124023) &&
       (irun!=124024) &&
       (irun!=124025) &&
       (irun!=124027) &&
       (irun!=124030) &&
       (irun!=124120) &&
       (irun!=124230) 
       
       
        )
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
  } else if (irun==123596) {
    if (/*(ilum>=4 && ilum<=26) ||*/   //pixel timing scan
        (ilum>=69 && ilum<=144) )
      accepted=true;
  } else if (irun==123615) {
    if(ilum>=71) 
      accepted=true;
  } else if (irun==123732) {
    if(ilum>=62 && ilum<=112)//though phys bit starting 57 ...
      accepted=true;
  } else if (irun==123815) {
    if(ilum>=8 && ilum<=16)
      accepted=true;
  } else if (irun==123818) {
    if(ilum>=2 && ilum<=18) //RunRegistry says lumi scan starting at lumi 19 until 42
      accepted=true;
  } else if (irun==123906) {
    if(ilum>=17 && ilum<=28)
      accepted=true;
  } else if (irun==123908) {
    if(ilum>=2 && ilum<=13)
      accepted=true;
  /*} else if (irun==123987) {//3T run ...
    if(ilum>=1 && ilum<=21)
      accepted=true;*/
  } else if (irun==124006) {
    if(ilum>=1 && ilum<=6) //though Phys bit set from lumi 6 ...
      accepted=true;
  } else if (irun==124009) {
    if(ilum>=1 && ilum<=68)
      accepted=true;
  } else if (irun==124020) {
    if(ilum>=12 && ilum<=94)
      accepted=true;
  } else if (irun==124022) {
    if(ilum>=65 && ilum<=161) //lumi scan starting lumi 161 'til 183...
      accepted=true;
  } else if (irun==124023) {
    if(ilum>=41 && ilum<=96)
      accepted=true;
  } else if (irun==124024) {
    if(ilum>=2 && ilum<=83)
      accepted=true;
  } else if (irun==124025) {
    if(ilum>=3 && ilum<=13)
      accepted=true;
  } else if (irun==124027) {
    if(ilum>=23 && ilum<=39)
      accepted=true;
  } else if (irun==124030) {
    if(ilum>=1 && ilum<=32)
      accepted=true;
  /*} else if (irun==124120) { //2.2 TeV run
    if(ilum>=1)
      accepted=true;*/
  } else if (irun==124230) {
    if(ilum>=26 && ilum<=68)//paused at 47, resumed 48
      accepted=true;
  }
  
  return accepted;
}

//define this as a plug-in
DEFINE_FWK_MODULE(FilterLumi);

