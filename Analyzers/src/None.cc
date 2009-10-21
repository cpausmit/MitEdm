// $Id$

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

class None : public edm::EDAnalyzer {
   public:
    explicit None(const edm::ParameterSet&) {}
    ~None() {};

   private:
    virtual void beginJob() {}
    virtual void analyze(const edm::Event&, const edm::EventSetup&) {}
    virtual void endJob() {}
};

//define this as a plug-in
DEFINE_FWK_MODULE(None);
