#if 0
//will be reenabled later

#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "MitEdm/DataFormats/interface/Types.h"
#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/DataFormats/interface/CollectionsEdm.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "MitEdm/DataFormats/interface/StablePartEdm.h"
#include "MitEdm/Producers/interface/V2SS.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
V2SS::V2SS(const ParameterSet& cfg) :
  D2SS(cfg)
{
}

//--------------------------------------------------------------------------------------------------
V2SS::~V2SS()
{
}

//--------------------------------------------------------------------------------------------------
void V2SS::beginJob(const EventSetup &setup)
{
}

//--------------------------------------------------------------------------------------------------
void V2SS::produce(Event &evt, const EventSetup &setup)
{
  // -----------------------------------------------------------------------------------------------
  // Get to the input
  // -----------------------------------------------------------------------------------------------
  // First input collection
  Handle<BasePartObjArr> hStables1;
  if (!GetProduct(iStables1_, hStables1, evt))
    return;
  const BasePartObjArr *pS1 = hStables1.product();
  // Second input collection
  Handle<BasePartObjArr> hStables2;
  if (!GetProduct(iStables2_, hStables2, evt))
    return;
  const BasePartObjArr *pS2 = hStables2.product();

  // -----------------------------------------------------------------------------------------------
  // Create the output collection
  // -----------------------------------------------------------------------------------------------
  auto_ptr<DecayPartCol> pD(new DecayPartCol());

  // -----------------------------------------------------------------------------------------------
  // Simple double loop
  // -----------------------------------------------------------------------------------------------
  for (UInt_t i=0; i<pS1->Entries(); ++i) {
    StablePartEdm *s1 = (StablePartEdm*) pS1->At(i);

    UInt_t j;
    if (iStables1_ == iStables2_)
      j = i+1; 
    else
      j = 0;

    for (; j<pS2->Entries(); ++j) {
      StablePartEdm *s2 = (StablePartEdm*) pS2->At(j);
      // Preparing the result
      DecayPart *d = new DecayPart(oPid_,oMass_,DecayPart::Fast);
      d->addChild(s1);
      d->addChild(s2);

      // -------------------------------------------------------------------------------------------
      // Vertex fit now (temporary screwing around)
      MultiVertexFitter fit;
      fit.init(4.0);                                    // Reset to the MC magnetic field of 4 Tesla
      MvfInterface fitInt(&fit);
      fitInt.addTrack(s1->track(),1,s1->mass(),MultiVertexFitter::VERTEX_1);
      fitInt.addTrack(s2->track(),2,s2->mass(),MultiVertexFitter::VERTEX_1);
      if (fit.fit()) {
	// -----------------------------------------------------------------------------------------
	// Update temporarily some fit quality (prob, chi2, nDoF)
	d->setProb(fit.prob());
	d->setChi2(fit.chisq());
	d->setNdof(fit.ndof());
	// -----------------------------------------------------------------------------------------
	// Update temporarily some vertex quantities (mass, lxy, pt, fourMomentum, ...)
	FourVector p4Fitted(0.,0.,0.,0.);
	p4Fitted += fit.getTrackP4(1);
	p4Fitted += fit.getTrackP4(2);
	d->setFourMomentum(p4Fitted);
	d->setPosition(fit.getVertex     (MultiVertexFitter::VERTEX_1));
	d->setError   (fit.getErrorMatrix(MultiVertexFitter::VERTEX_1));
	float mass, massErr;
	const int trksIds[2] = { 1, 2 };
	mass = fit.getMass(2,trksIds,massErr);
	d->setFittedMass     (mass);
	d->setFittedMassError(massErr);
	// Put the result into our collection
	pD->push_back(*d);
      }

    }
  }

  // -----------------------------------------------------------------------------------------------
  // Write the collection even if it is empty
  // -----------------------------------------------------------------------------------------------
  cout << " V2SS::produce - " << pD->Entries() << " entries collection created -"
       << " (Pid: " << oPid_ << ", Mass: " << oMass_ << ")\n";
  evt.put(pD);
}

//--------------------------------------------------------------------------------------------------
void V2SS::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(V2SS);

#endif
