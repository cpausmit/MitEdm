// $Id: ProducerV2SS.cc,v 1.7 2008/10/03 23:53:53 loizides Exp $

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "MitEdm/Producers/interface/HitDropperRecord.h"
#include "MitEdm/Producers/interface/HitDropper.h"
#include "MitEdm/DataFormats/interface/Types.h"
#include "MitEdm/DataFormats/interface/Collections.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "MitEdm/DataFormats/interface/StableData.h"
#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/VertexFitInterface/interface/HisInterface.h"
#include "MitEdm/Producers/interface/ProducerV2SS.h"
#include "MitCommon/VertexFit/interface/MultiVertexFitter.h"

using namespace std;
using namespace edm;
using namespace mitedm;
using namespace mithep;

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
void ProducerV2SS::produce(Event &evt, const EventSetup &setup)
{
  // Create the output collection
  auto_ptr<DecayPartCol> pD(new DecayPartCol());

  // First input collection
  Handle<StablePartCol> hStables1;
  Handle<StablePartCol> hStables2;
  if (!GetProduct(iStables1_, hStables1, evt) ) {
    cout << "Couldn't get in collection in Producer V2SS" << endl;
    evt.put(pD);
    return;  
  }
  const StablePartCol *pS1 = hStables1.product();

  // Second input collection
  if(!GetProduct(iStables2_, hStables2, evt)) {
    cout << "Couldn't get in collection in Producer V2SS" << endl;
    evt.put(pD);
    return;  
  }
  const StablePartCol *pS2 = hStables2.product();
  
  //get hit dropper
  ESHandle<HitDropper> hDropper;
  setup.get<HitDropperRecord>().get("HitDropper",hDropper);
  const HitDropper *dropper = hDropper.product();

  // -----------------------------------------------------------------------------------------------
  // Simple double loop
  // -----------------------------------------------------------------------------------------------
  if (0)
    cout << "Starting V finder loop" << endl;

  //sX_y: X= pion or proton collection.  
  //i, j = 2 loop particles. 
  //ex.: s1_i and s2_i are same particle as pion and proton
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
      // -------------------------------------------------------------------------------------------
      mithep::MultiVertexFitter fit;
      fit.init(3.8); // Reset to the MC magnetic field of 3.8 Tesla
      fit.setChisqMax(100);
      MvfInterface fitInt(&fit);
      fitInt.addTrack(s1.track(),1,s1.mass(),mithep::MultiVertexFitter::VERTEX_1);
      fitInt.addTrack(s2.track(),2,s2.mass(),mithep::MultiVertexFitter::VERTEX_1);


      if (fit.fit()){

	DecayPart *d = new DecayPart(oPid_,DecayPart::Fast);

        // Update temporarily some of the quantities (prob, chi2, nDoF, mass, lxy, pt, fourMomentum)
        d->setProb(fit.prob());
        d->setChi2(fit.chisq());
        d->setNdof(fit.ndof());

        FourVector p4Fitted(0.,0.,0.,0.);
        p4Fitted += fit.getTrackP4(1);
        p4Fitted += fit.getTrackP4(2);
        d->setFourMomentum(p4Fitted);
        d->setPosition(fit.getVertex(MultiVertexFitter::VERTEX_1));
        d->setError(fit.getErrorMatrix(MultiVertexFitter::VERTEX_1));
        float mass, massErr;
        const int trksIds[2] = { 1, 2 };
        mass = fit.getMass(2,trksIds,massErr);
        
        ThreeVector p3Fitted(p4Fitted.px(), p4Fitted.py(), p4Fitted.pz());
        
        //Get decay length in xy plane
        float dl, dlErr;
        dl = fit.getDecayLength(MultiVertexFitter::PRIMARY_VERTEX, MultiVertexFitter::VERTEX_1,
               p3Fitted, dlErr);
               
        //Get Z decay length               
        float dlz, dlzErr;
        dlz = fit.getZDecayLength(MultiVertexFitter::PRIMARY_VERTEX, MultiVertexFitter::VERTEX_1,
               p3Fitted, dlzErr);
               
        //get impact parameter               
        float dxy, dxyErr;
        dxy = fit.getImpactPar(MultiVertexFitter::PRIMARY_VERTEX, MultiVertexFitter::VERTEX_1,
               p3Fitted, dxyErr);

        BasePartPtr ptr1(hStables1,i);
        BasePartPtr ptr2(hStables2,j);       
        
        StableData c1(fit.getTrackP4(1).px(),fit.getTrackP4(1).py(), fit.getTrackP4(1).pz(), ptr1);
        StableData c2(fit.getTrackP4(2).px(),fit.getTrackP4(2).py(), fit.getTrackP4(2).pz(), ptr2);
        
        const ThreeVector vtxPos = fit.getVertex(MultiVertexFitter::VERTEX_1);
        const ThreeVector trkMom1(fit.getTrackP4(1).px(),fit.getTrackP4(1).py(), fit.getTrackP4(1).pz());
        const ThreeVector trkMom2(fit.getTrackP4(2).px(),fit.getTrackP4(2).py(), fit.getTrackP4(2).pz());
        
        //build corrected HitPattern for StableData, removing hits before the fit vertex
        reco::HitPattern hits1 = dropper->CorrectedHits(s1.track(), vtxPos, trkMom1, dlErr, dlzErr);
        reco::HitPattern hits2 = dropper->CorrectedHits(s2.track(), vtxPos, trkMom2, dlErr, dlzErr);
        
        c1.SetHits(hits1);
        c2.SetHits(hits2);
        
        d->addStableChild(c1);
        d->addStableChild(c2);               
               
        d->setFittedMass     (mass);
        d->setFittedMassError(massErr);
        
        d->setLxy(dl);
        d->setLxyError(dlErr);
        d->setLxyToPv(dl);
        d->setLxyToPvError(dlErr);
        
        d->setLz(dlz);
        d->setLzError(dlzErr);
        d->setLzToPv(dlz);
        d->setLzToPvError(dlzErr);
        
        d->setDxy(dxy);
        d->setDxyError(dxyErr);
        d->setDxyToPv(dxy);
        d->setDxyToPvError(dxyErr);
	// Put the result into our collection
	pD->push_back(*d);
      }  //done processing fit
    } //end j loop
  } //end i loop

  // -----------------------------------------------------------------------------------------------
  // Write the collection even if it is empty
  // -----------------------------------------------------------------------------------------------
  if (0)
    cout << " V2SS::produce - " << pD->size() << " entries collection created -"
         << " (Pid: " << oPid_ << ")\n";
  evt.put(pD);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProducerV2SS);

