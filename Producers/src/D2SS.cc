#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "MitEdm/AnalysisDataFormats/interface/Types.h"
#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/AnalysisDataFormats/interface/CollectionsEdm.h"
#include "MitEdm/AnalysisDataFormats/interface/DecayPart.h"
#include "MitEdm/AnalysisDataFormats/interface/StablePartEdm.h"
#include "MitEdm/Producers/interface/D2SS.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
D2SS::D2SS(const ParameterSet& cfg) :
  BaseCandidate(cfg),
  iStables1_   (cfg.getUntrackedParameter<string>("iStables1","" )),
  iStables2_   (cfg.getUntrackedParameter<string>("iStables2","" ))
{
}

//--------------------------------------------------------------------------------------------------
D2SS::~D2SS()
{
}

//--------------------------------------------------------------------------------------------------
void D2SS::produce(Event &evt, const EventSetup &setup)
{
  // -----------------------------------------------------------------------------------------------
  // Get the input
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
  auto_ptr<BasePartObjArr> pD(new BasePartObjArr(10));

  // -----------------------------------------------------------------------------------------------
  // Simple double loop
  // -----------------------------------------------------------------------------------------------
  for (UInt_t i = 0; i<pS1->Entries(); ++i) {
    StablePartEdm *s1 = (StablePartEdm*) pS1->At(i);
    
    UInt_t j;
    if (iStables1_ == iStables2_)
      j = i+1; 
    else
      j = 0;
    
    for (; j<pS2->Entries(); ++j) {
      StablePartEdm *s2 = (StablePartEdm*) pS2->At(j);

      // Vertex fit now (temporary screwing around)
      MultiVertexFitter fit;
      fit.init(4.0);                                    // Reset to the MC magnetic field of 4 Tesla
      MvfInterface fitInt(&fit);
      fitInt.addTrack(s1->track(),1,s1->mass(),MultiVertexFitter::VERTEX_1);
      fitInt.addTrack(s2->track(),2,s2->mass(),MultiVertexFitter::VERTEX_1);
      if (fit.fit()) {
	DecayPart *d = new DecayPart(oPid_,oMass_,DecayPart::Fast);
	d->addChild(s1);
	d->addChild(s2);
	// Update temporarily some of the quantities (prob, chi2, nDoF, mass, lxy, pt, fourMomentum)
	d->setProb(fit.prob());
	d->setChi2(fit.chisq());
	d->setNdof(fit.ndof());

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
	pD->Add(d);
      }

    }
  }

  // -----------------------------------------------------------------------------------------------
  // Write the collection even if it is empty
  // -----------------------------------------------------------------------------------------------
  cout << " D2SS::produce - " << pD->Entries() << " entries collection created -"
       << " (Pid: " << oPid_ << ", Mass: " << oMass_ << ")\n";
  evt.put(pD);
}

//--------------------------------------------------------------------------------------------------
void D2SS::beginJob(const EventSetup &setup)
{
}

//--------------------------------------------------------------------------------------------------
void D2SS::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(D2SS);
