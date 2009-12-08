//--------------------------------------------------------------------------------------------------
// $Id: FilterEvtSel.cc,v 1.3 2009/12/08 00:06:32 loizides Exp $
//
// FilterEvtSel
//
// Filter to select events based on event selection object,
// containing calo energy/time, pixel hits, and cluster shape.
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

#include "MitEdm/DataFormats/interface/EvtSelData.h"

namespace mitedm
{
  class FilterEvtSel : public edm::EDFilter {
  public:
    explicit FilterEvtSel(const edm::ParameterSet &ps);
    ~FilterEvtSel() {}
    
  protected:
    virtual bool filter (edm::Event &iEvent, const edm::EventSetup &iSetup);
    std::vector<double> clusterPars_;
    double minHfEnergy_;
    double maxHfTimeDiff_;
    std::string srcEvtSel_;
  };
}

using namespace mitedm;
using namespace edm;
using namespace std;

//--------------------------------------------------------------------------------------------------
FilterEvtSel::FilterEvtSel(const edm::ParameterSet& iConfig)
  : clusterPars_(iConfig.getUntrackedParameter< std::vector<double> >("clusterPars")),
    minHfEnergy_(iConfig.getUntrackedParameter<double>("minHfEnergy")),
    maxHfTimeDiff_(iConfig.getUntrackedParameter<double>("maxHfTimeDiff")),
    srcEvtSel_(iConfig.getUntrackedParameter<std::string>("srcEvtSel","evtSelData"))
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
bool FilterEvtSel::filter( edm::Event &iEvent, const edm::EventSetup &iSetup)
{
  // Filter events based on calo energy/timing and pixel clusters.
  
  Handle<EvtSelData> evtSel;
  iEvent.getByLabel(edm::InputTag(srcEvtSel_),evtSel);

  int nPxlHits = evtSel->ePxbHits();
  double hfTimeDiff = evtSel->eHfPosTime() - evtSel->eHfNegTime();
  double clusVtxQual = evtSel->eClusVtxQual();
  double hfEnergyMin = min(evtSel->eHfPos(),evtSel->eHfNeg());

  // construct polynomial cut on cluster vertex quality vs. npixelhits
  double polyCut=0;
  for(int i=0; i < (int)clusterPars_.size(); i++) {
    polyCut += clusterPars_[i]*pow((double)nPxlHits,i);
  }

  bool accepted = true;

  if( (fabs(hfTimeDiff)>maxHfTimeDiff_ && maxHfTimeDiff_>0) 
      || hfEnergyMin < minHfEnergy_ || clusVtxQual < polyCut )
    accepted = false;

  return accepted;
}

//define this as a plug-in
DEFINE_FWK_MODULE(FilterEvtSel);
