#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "MitEdm/DataFormats/interface/Types.h"
#include "MitEdm/DataFormats/interface/Collections.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/VertexFitInterface/interface/HisInterface.h"
#include "MitEdm/Producers/interface/ProducerV2SS.h"
#include "MitCommon/VertexFit/interface/MultiVertexFitter.h"

using namespace std;
using namespace edm;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
ProducerV2SS::ProducerV2SS(const ParameterSet& cfg) :
  ProducerD2SS(cfg),
  rhoMin_(cfg.getUntrackedParameter<double>("minRadius",0.0)),
  massMin_(cfg.getUntrackedParameter<double>("minMass",0.0)),
  massMax_(cfg.getUntrackedParameter<double>("maxMass",3.0)),
  dZMax_(cfg.getUntrackedParameter<double>("maxZDistance",5.0))
{
}

//--------------------------------------------------------------------------------------------------
ProducerV2SS::~ProducerV2SS()
{
}

//--------------------------------------------------------------------------------------------------
void ProducerV2SS::beginJob(const EventSetup &setup)
{
}

//--------------------------------------------------------------------------------------------------
void ProducerV2SS::produce(Event &evt, const EventSetup &setup)
{
  // -----------------------------------------------------------------------------------------------
  // Create the output collection
  // -----------------------------------------------------------------------------------------------
  auto_ptr<DecayPartCol> pD(new DecayPartCol());



  // -----------------------------------------------------------------------------------------------
  // Get to the input
  // -----------------------------------------------------------------------------------------------
  // First input collection
  Handle<StablePartCol> hStables1;
  Handle<StablePartCol> hStables2;
  if (!GetProduct(iStables1_, hStables1, evt) ) {
    cout << "Couldn't get in collection in Producer V2SS" << endl;
    evt.put(pD);
    return;  
  }

  if(!GetProduct(iStables2_, hStables2, evt)) {
    cout << "Couldn't get in collection in Producer V2SS" << endl;
    evt.put(pD);
    return;  
  }

  const StablePartCol *pS1 = hStables1.product();
  // Second input collection

  const StablePartCol *pS2 = hStables2.product();



  // -----------------------------------------------------------------------------------------------
  // Simple double loop
  // -----------------------------------------------------------------------------------------------
  //cout << "Starting V finder loop" << endl;

  //sX_y: X= pion or proton collection.  i, j = 2 loop particles. ex.: s1_i and s2_i are same particle as pion and proton
  for (UInt_t i=0; i<pS1->size(); ++i) {
    const StablePart &s1 =  pS1->at(i);
   
    UInt_t j;
    if (iStables1_ == iStables2_)
      j = i+1; 
    else
      j = 0;
    
    for (; j<pS2->size(); ++j) {
      const StablePart &s2 = pS2->at(j);

      if(s1.charge() + s2.charge() != 0) continue;

      //Do fast helix fit to check if there's any hope

      const reco::Track * t1 = s1.track();
      const reco::Track * t2 = s2.track();
      HisInterface            hisInt(t1,t2);
      const mithep::HelixIntersector *his = hisInt.hISector();
      double dZ0 = -999;
      double dR0 = -999;
      dZ0 = his->IntersectionI(0).DeltaZ();
      dR0 = sqrt(his->IntersectionI(0).Location().X()*his->IntersectionI(0).Location().X()+
      		 his->IntersectionI(0).Location().Y()*his->IntersectionI(0).Location().Y());

      TVector3     v1, v2;
      v1 = his->IntersectionI(0).TrackParamsI(0).Momentum();
      v2 = his->IntersectionI(0).TrackParamsI(1).Momentum();

      double e1 = sqrt(v1.Mag()*v1.Mag() +s1.mass()*s1.mass());
      double x1 = v1.X();
      double y1 = v1.Y();
      double z1 = v1.Z();

      double e2 = sqrt(v2.Mag()*v2.Mag()+s2.mass()*s2.mass());
      double x2 = v2.X();
      double y2 = v2.Y();
      double z2 = v2.Z();

      FourVector sum(x1+x2, y1+y2, z1+z2, e1+e2);

      double mass0 = sqrt(sum.M2());

      //Basic cuts on helix intersection
      if(mass0 > massMax_ || mass0<massMin_ || fabs(dZ0) > dZMax_ || dR0 < rhoMin_) continue;

      // -------------------------------------------------------------------------------------------
      // Do vertex fit for all pairs
      mithep::MultiVertexFitter fit;
      fit.init(3.8);                                    // Reset to the MC magnetic field of 4 Tesla
      fit.setChisqMax(100);
      MvfInterface fitInt(&fit);
      fitInt.addTrack(s1.track(),1,s1.mass(),mithep::MultiVertexFitter::VERTEX_1);
      fitInt.addTrack(s2.track(),2,s2.mass(),mithep::MultiVertexFitter::VERTEX_1);


      if (fit.fit()){

	DecayPart *d = new DecayPart(oPid_,DecayPart::Fast);
        BasePartPtr ptr1(hStables1,i);
        BasePartPtr ptr2(hStables1,j);

	d->addChild(ptr1);
	d->addChild(ptr2);

        d->addChildMom(fit.getTrackP4(1));
        d->addChildMom(fit.getTrackP4(2));
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
	d->setPosition(fit.getVertex     (mithep::MultiVertexFitter::VERTEX_1));
	d->setError   (fit.getErrorMatrix(mithep::MultiVertexFitter::VERTEX_1));
	float mass, massErr;
	const int trksIds[2] = { 1, 2 };
	mass = fit.getMass(2,trksIds,massErr);
	d->setFittedMass     (mass);
	d->setFittedMassError(massErr);
	// Put the result into our collection
	pD->push_back(*d);
      }  //done processing fit
      
      


    }//end j loop
  } //end i loop

  // -----------------------------------------------------------------------------------------------
  // Write the collection even if it is empty
  // -----------------------------------------------------------------------------------------------
  cout << " V2SS::produce - " << pD->size() << " entries collection created -"
       << " (Pid: " << oPid_ << ")\n";
  evt.put(pD);
}

//--------------------------------------------------------------------------------------------------
void ProducerV2SS::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProducerV2SS);

