//--------------------------------------------------------------------------------------------------
// $Id: FilterEvtSel.cc,v 1.4 2009/12/11 11:06:09 loizides Exp $
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
    double              minHfEnergy_;
    double              maxHfTimeDiff_;
    std::string         srcEvtSel_;
    std::vector<double> clusterPars_;
    int                 nhitsmax_;
  };
}

using namespace mitedm;
using namespace edm;
using namespace std;

//--------------------------------------------------------------------------------------------------
FilterEvtSel::FilterEvtSel(const edm::ParameterSet& iConfig)
  : minHfEnergy_(iConfig.getUntrackedParameter<double>("minHfEnergy",0)),
    maxHfTimeDiff_(iConfig.getUntrackedParameter<double>("maxHfTimeDiff",0)),
    srcEvtSel_(iConfig.getUntrackedParameter<std::string>("srcEvtSel","evtSelData")),
    clusterPars_(iConfig.getUntrackedParameter< std::vector<double> >("clusterPars")),
    nhitsmax_(iConfig.getUntrackedParameter<int>("nhitsmax",0))
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
bool FilterEvtSel::filter( edm::Event &iEvent, const edm::EventSetup &iSetup)
{
  // Filter events based on calo energy/timing and pixel clusters.
  
  Handle<EvtSelData> evtSel;
  iEvent.getByLabel(edm::InputTag(srcEvtSel_),evtSel);

  int nPxlHits = evtSel->ePxHits();
  double hfTimeDiff = evtSel->eHfPosTime() - evtSel->eHfNegTime();
  double clusVtxQual = evtSel->eClusVtxQual();
  double hfEnergyMin = min(evtSel->eHfPos(),evtSel->eHfNeg());

  // construct polynomial cut on cluster vertex quality vs. npixelhits
  double polyCut=0;
  for(unsigned int i=0; i < clusterPars_.size(); i++) {
    polyCut += clusterPars_[i]*pow((double)nPxlHits,(int)i);
  }

  bool accepted = true;
  if( (fabs(hfTimeDiff)>maxHfTimeDiff_ && maxHfTimeDiff_>0) || 
      hfEnergyMin < minHfEnergy_ || 
      clusVtxQual < polyCut ||
      (nhitsmax_ < nPxlHits && nhitsmax_>0) )
    accepted = false;

  return accepted;
}

//define this as a plug-in
DEFINE_FWK_MODULE(FilterEvtSel);
