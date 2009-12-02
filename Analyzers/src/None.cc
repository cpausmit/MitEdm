//--------------------------------------------------------------------------------------------------
// $Id: None.cc,v 1.2 2009/12/02 15:42:29 loizides Exp $
//
// None
//
// Dummy module to do nothing. Can be a place holder in sequences.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

namespace mitedm
{
  class None : public edm::EDAnalyzer {
    public:
      explicit None(const edm::ParameterSet&) {}
      ~None() {};

     private:
       virtual void beginJob() {}
       virtual void analyze(const edm::Event&, const edm::EventSetup&) {}
       virtual void endJob() {}
  };
}

using namespace mitedm;

//define this as a plug-in
DEFINE_FWK_MODULE(None);
