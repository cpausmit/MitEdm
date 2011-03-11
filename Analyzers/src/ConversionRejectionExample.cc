//--------------------------------------------------------------------------------------------------
// $Id: ConversionRejectionExample.cc,v 1.1 2010/06/08 23:30:47 bendavid Exp $
//
// ConversionRejectionExample
//
// Example analyzer for using the ConversionMatcher tool
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#include <TMath.h>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "MitEdm/ConversionRejection/interface/ConversionMatcher.h"

namespace mitedm
{
  class ConversionRejectionExample : public edm::EDAnalyzer {
    public:
      explicit ConversionRejectionExample(const edm::ParameterSet&) {}
      ~ConversionRejectionExample() {};

     private:
       virtual void beginJob() {}
       virtual void analyze(const edm::Event &e, const edm::EventSetup &es);
       virtual void endJob() {}
  };
}

using namespace mitedm;
using namespace edm;

//define this as a plug-in
DEFINE_FWK_MODULE(ConversionRejectionExample);

void ConversionRejectionExample::analyze(const edm::Event &e, const edm::EventSetup &es)
{
  
  //get electron collection
  Handle<reco::GsfElectronCollection> hElectrons;
  e.getByLabel("gsfElectrons", hElectrons);
  const reco::GsfElectronCollection *electronCol = hElectrons.product();
  
  //get collection of reconstructed conversions
  edm::Handle<std::vector<mitedm::DecayPart> > hConversions;
  e.getByLabel("mvfConversionRemoval", hConversions);
  
  //initialize ConversionMatcher with default cuts
  mitedm::ConversionMatcher convMatcher;

  for (reco::GsfElectronCollection::const_iterator it = electronCol->begin(); it!=electronCol->end(); ++it) {
    
    //check if electron matches a conversion passing all of the cuts (then the electron should be rejected)
    bool matchesGoodConversion = convMatcher.matchesGoodConversion(*it,hConversions);
    printf("matchesGoodConversion = %i\n", matchesGoodConversion);
    
    //dump some info on the good conversions
    std::vector<edm::Ptr<DecayPart> > goodConversions = convMatcher.goodMatchedConversions(*it,hConversions);
    for (std::vector<edm::Ptr<DecayPart> >::const_iterator jt = goodConversions.begin(); jt!=goodConversions.end(); ++jt) {
      const DecayPart *conv = jt->get();
      printf("Good Matched Conversion %i:\n",(int)(jt-goodConversions.begin()));
      printf("  radius = %5f\n",conv->position().rho());
      printf("  lxy    = %5f\n",conv->lxy());
      printf("  lz     = %5f\n",conv->lz());
      printf("  chi2   = %5f\n",conv->chi2());
      printf("  ndof   = %i\n",conv->ndof());
      printf("  prob   = %5f\n",TMath::Prob(conv->chi2(),conv->ndof()));

      //loop through daughters
      for (int i=0; i<conv->nStableChild(); ++i) {
        printf("  Daughter %i\n",i);
        const StableData &sd = conv->getStableData(i); 
        const StablePart *sp = dynamic_cast<const StablePart*>(sd.originalPtr().get());
        const reco::Track *trk = sp->track();
        
        printf("    Hits before vertex = %i\n", sd.nWrongHits());
        printf("    Track Algo         = %i\n", trk->algo());
        printf("    Track chi2         = %5f\n",trk->chi2());
        printf("    Track ndof         = %5f\n", trk->ndof());
        printf("    Track prob         = %5f\n", TMath::Prob(trk->chi2(),trk->ndof()));             
      }
    }
    
    //dump some info on all matched conversions
    std::vector<edm::Ptr<DecayPart> > allConversions = convMatcher.allMatchedConversions(*it,hConversions);
    for (std::vector<edm::Ptr<DecayPart> >::const_iterator jt = allConversions.begin(); jt!=allConversions.end(); ++jt) {
      const DecayPart *conv = jt->get();
      printf("Matched Conversion %i:\n",(int)(jt-allConversions.begin()));
      printf("  radius = %5f\n",conv->position().rho());
      printf("  prob   = %5f\n",TMath::Prob(conv->chi2(),conv->ndof()));
    }
  }
  
}