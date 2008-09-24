// $Id: ProducerConversions.cc,v 1.3 2008/09/24 08:35:00 bendavid Exp $

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "MitEdm/DataFormats/interface/Types.h"
#include "MitEdm/DataFormats/interface/CollectionsEdm.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/Producers/interface/ProducerConversions.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace mitedm;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
ProducerConversions::ProducerConversions(const ParameterSet& cfg) :
  BaseCandidate(cfg),
  iStables1_   (cfg.getUntrackedParameter<string>("iStables1","" )),
  iStables2_   (cfg.getUntrackedParameter<string>("iStables2","" )),
  convConstraint_ (cfg.getUntrackedParameter<bool>("convConstraint",false )),
  convConstraint3D_ (cfg.getUntrackedParameter<bool>("convConstraint3D",true )),
  rhoMin_ (cfg.getUntrackedParameter<double>("rhoMin",0.0 ))
{
  produces<DecayPartCol>();
}

//--------------------------------------------------------------------------------------------------
ProducerConversions::~ProducerConversions()
{
}

//--------------------------------------------------------------------------------------------------
void ProducerConversions::produce(Event &evt, const EventSetup &setup)
{
  // -----------------------------------------------------------------------------------------------
  // Get the input
  // -----------------------------------------------------------------------------------------------
  // First input collection
  Handle<StablePartCol> hStables1;
  if (!GetProduct(iStables1_, hStables1, evt))
    return;  
  const StablePartCol *pS1 = hStables1.product();
  // Second input collection
  Handle<StablePartCol> hStables2;
  if (!GetProduct(iStables2_, hStables2, evt))
    return;
  const StablePartCol *pS2 = hStables2.product();

  // -----------------------------------------------------------------------------------------------
  // Create the output collection
  // -----------------------------------------------------------------------------------------------
  auto_ptr<DecayPartCol> pD(new DecayPartCol());

  // -----------------------------------------------------------------------------------------------
  // Simple double loop
  // -----------------------------------------------------------------------------------------------
  for (UInt_t i = 0; i<pS1->size(); ++i) {
    const StablePart &s1 =  pS1->at(i);
    
    UInt_t j;
    if (iStables1_ == iStables2_)
      j = i+1; 
    else
      j = 0;
    
    for (; j<pS2->size(); ++j) {
      const StablePart &s2 = pS2->at(j);

      // Vertex fit now, possibly with conversion constraint
      MultiVertexFitter fit;
      fit.init(3.8);                                    // Reset to the MC magnetic field of 4 Tesla
      MvfInterface fitInt(&fit);
      fitInt.addTrack(s1.track(),1,s1.mass(),MultiVertexFitter::VERTEX_1);
      fitInt.addTrack(s2.track(),2,s2.mass(),MultiVertexFitter::VERTEX_1);
      if (convConstraint3D_) {
        fit.conversion_3d(MultiVertexFitter::VERTEX_1);
        //printf("applying 3d conversion constraint\n");
      }
      else if (convConstraint_) {
        fit.conversion_2d(MultiVertexFitter::VERTEX_1);
        //printf("applying 2d conversion constraint\n");
      }
      if (fit.fit()) {
        DecayPart *d = new DecayPart(oPid_,DecayPart::Fast);
        
        BasePartPtr ptr1(hStables1,i);
        BasePartPtr ptr2(hStables2,j);
        d->addChild(ptr1);
        d->addChild(ptr2);
        d->addChildMom(fit.getTrackP4(1));
        d->addChildMom(fit.getTrackP4(2));
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
        
        if(0) {
          const reco::Track *p1 = s1.track();
          const reco::Track *p2 = s2.track();
        
          //// create the dimuon system
          FourVector mu1(p1->px(),p1->py(),p1->pz(),sqrt(p1->p()*p1->p()+0.105658357*0.105658357));
          FourVector mu2(p2->px(),p2->py(),p2->pz(),sqrt(p2->p()*p2->p()+0.105658357*0.105658357));
          FourVector diMu = mu1+mu2;

          //// for convenience and economy
          double mass4Vec = sqrt(diMu.M2());
        
          printf(" Generated mass:   ....\n");
          printf(" Four vector mass: %14.6f\n",mass4Vec);
          printf(" Fitted mass:      %14.6f +- %14.6f\n",mass,massErr);
        }

        d->setFittedMass     (mass);
        d->setFittedMassError(massErr);
        // Put the result into our collection
        if (d->position().rho() > rhoMin_)
         pD->push_back(*d);
        delete d;
      }
    }
  }

  // -----------------------------------------------------------------------------------------------
  // Write the collection even if it is empty
  // -----------------------------------------------------------------------------------------------
 // cout << " ProducerConversions::produce - " << pD->size() << " entries collection created -"
  //     << " (Pid: " << oPid_ << ")\n";
  evt.put(pD);
}

//--------------------------------------------------------------------------------------------------
void ProducerConversions::beginJob(const EventSetup &setup)
{
}

//--------------------------------------------------------------------------------------------------
void ProducerConversions::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProducerConversions);
