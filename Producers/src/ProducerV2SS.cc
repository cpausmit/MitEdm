#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"
#include "MitEdm/Producers/interface/HitDropperRecord.h"
#include "MitEdm/Producers/interface/HitDropper.h"
#include "MitEdm/DataFormats/interface/Types.h"
#include "MitEdm/DataFormats/interface/Collections.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "MitEdm/DataFormats/interface/StableData.h"
#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/VertexFitInterface/interface/TrackParameters.h"
#include "MitEdm/Producers/interface/ProducerV2SS.h"
#include <TMath.h>

using namespace std;
using namespace edm;
using namespace mitedm;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
ProducerV2SS::ProducerV2SS(const ParameterSet& cfg) :
  ProducerD2SS(cfg),
  rhoMin_     (cfg.getUntrackedParameter<double>("minRadius",   0.0)),
  massMin_    (cfg.getUntrackedParameter<double>("minMass",     0.0)),
  massMax_    (cfg.getUntrackedParameter<double>("maxMass",     3.0)),
  dZMax_      (cfg.getUntrackedParameter<double>("maxZDistance",5.0)),
  useHitDropper_(cfg.getUntrackedParameter<bool>("useHitDropper",true)),
  applyChargeConstraint_(cfg.getUntrackedParameter<bool>  ("applyChargeConstraint",false))
{
  // Constructor.
}

//--------------------------------------------------------------------------------------------------
ProducerV2SS::~ProducerV2SS()
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
void ProducerV2SS::produce(Event &evt, const EventSetup &setup)
{
  // Produce the output collection.

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
  const HitDropper *dropper = 0;
  if (useHitDropper_) {
    setup.get<HitDropperRecord>().get("HitDropper",hDropper);
    dropper = hDropper.product();
  }

  ESHandle<TrackerTopology> hTopo;
  setup.get<TrackerTopologyRcd>().get(hTopo);
  TrackerTopology const& topo(*hTopo);
  
  //Get Magnetic Field from event setup, taking value at (0,0,0)
  edm::ESHandle<MagneticField> magneticField;
  setup.get<IdealMagneticFieldRecord>().get(magneticField);
  const double bfield = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
  
  edm::ESHandle<TransientTrackBuilder> hTransientTrackBuilder;
  setup.get<TransientTrackRecord>().get("TransientTrackBuilder",hTransientTrackBuilder);
  const TransientTrackBuilder *transientTrackBuilder = hTransientTrackBuilder.product();
  
  //construct intermediate collection of TrackParameters in mvf format for vertex fit
  std::vector<TrackParameters> trkPars1;
  for (UInt_t i = 0; i<pS1->size(); ++i) {
    const reco::Track *t = pS1->at(i).track();
    const reco::TransientTrack ttrk = transientTrackBuilder->build(t);
    TrackParameters cmsTrk(ttrk);
    TrackParameters mvfTrk = cmsTrk.mvfTrack(); 
    trkPars1.push_back(mvfTrk);
  }
  
  std::vector<TrackParameters> trkPars2;
  if (iStables1_ == iStables2_)
    trkPars2 = trkPars1;
  else for (UInt_t i = 0; i<pS2->size(); ++i) {
    const reco::Track *t = pS2->at(i).track();
    const reco::TransientTrack ttrk = transientTrackBuilder->build(t);
    TrackParameters cmsTrk(ttrk);
    TrackParameters mvfTrk = cmsTrk.mvfTrack(); 
    trkPars2.push_back(mvfTrk);
  }

  // -----------------------------------------------------------------------------------------------
  // Simple double loop
  // -----------------------------------------------------------------------------------------------
  if (0)
    cout << "Starting V finder loop" << endl;

  ClosestApproachInRPhi helixIntersector;
  
  
  //sX_y: X= pion or proton collection.  
  //i, j = 2 loop particles. 
  //ex.: s1_i and s2_i are same particle as pion and proton
  for (UInt_t i=0; i<pS1->size(); ++i) {
    const StablePart &s1 =  pS1->at(i);
    const TrackParameters &trkPar1 = trkPars1.at(i);
   
    //const reco::Track * t1 = s1.track();
    
    UInt_t j;
    if (iStables1_ == iStables2_)
      j = i+1; 
    else
      j = 0;
    
    FreeTrajectoryState initialState1 = trajectoryStateTransform::initialFreeState(*s1.track(),&*magneticField);

    for (; j<pS2->size(); ++j) {
      const StablePart &s2 = pS2->at(j);
      const TrackParameters &trkPar2 = trkPars2.at(j);

      if( applyChargeConstraint_ && (s1.charge() + s2.charge() != 0) ) continue;

      // do fast helix fit to check if there's any hope
      //
      //const reco::Track * t2 = s2.track();
         
      double dZ0 = -999;
      double dR0 = -999;
      double mass0 = 0.0;
      
      FreeTrajectoryState initialState2 = trajectoryStateTransform::initialFreeState(*s2.track(),&*magneticField);
      helixIntersector.calculate(initialState1, initialState2);
      if (helixIntersector.status()) {
        dZ0 = fabs(helixIntersector.points().first.z() - helixIntersector.points().second.z());
        dR0 = helixIntersector.crossingPoint().perp();
        
        GlobalVector     v1, v2;
        v1 = helixIntersector.trajectoryParameters().first.momentum();
        v2 = helixIntersector.trajectoryParameters().second.momentum();

        double e1 = sqrt(v1.mag2()+s1.mass()*s1.mass());
        double x1 = v1.x();
        double y1 = v1.y();
        double z1 = v1.z();

        double e2 = sqrt(v2.mag2()+s2.mass()*s2.mass());
        double x2 = v2.x();
        double y2 = v2.y();
        double z2 = v2.z();

        FourVector sum(x1+x2, y1+y2, z1+z2, e1+e2);

        mass0 = sqrt(sum.M2());
      }
      
      
      // Basic cuts on helix intersection
      if(mass0 > massMax_ || mass0<massMin_ || fabs(dZ0) > dZMax_ || dR0 < rhoMin_) continue;

      // -------------------------------------------------------------------------------------------
      // Do vertex fit for all pairs
      // -------------------------------------------------------------------------------------------
      mithep::MultiVertexFitterD fit;
      fit.init(bfield); // Reset to the magnetic field from the event setup
      fit.setChisqMax(100);
      
      fit.addTrack(*trkPar1.pars(),*trkPar1.cMat(),1,s1.mass(),MultiVertexFitterD::VERTEX_1);
      fit.addTrack(*trkPar2.pars(),*trkPar2.cMat(),2,s2.mass(),MultiVertexFitterD::VERTEX_1);


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
        d->setPosition    (fit.getVertex     (MultiVertexFitterD::VERTEX_1));
        d->setError       (fit.getErrorMatrix(MultiVertexFitterD::VERTEX_1));
        float mass, massErr;
        const int trksIds[2] = { 1, 2 };
        mass = fit.getMass(2,trksIds,massErr);
        
        ThreeVector p3Fitted(p4Fitted.px(), p4Fitted.py(), p4Fitted.pz());
        
        //Get decay length in xy plane
        float dl, dlErr;
        dl = fit.getDecayLength  (MultiVertexFitterD::PRIMARY_VERTEX, MultiVertexFitterD::VERTEX_1,
				  p3Fitted, dlErr);
               
        //Get Z decay length               
        float dlz, dlzErr;
        dlz = fit.getZDecayLength(MultiVertexFitterD::PRIMARY_VERTEX, MultiVertexFitterD::VERTEX_1,
				  p3Fitted, dlzErr);
               
        //get impact parameter               
        float dxy, dxyErr;
        dxy = fit.getImpactPar   (MultiVertexFitterD::PRIMARY_VERTEX, MultiVertexFitterD::VERTEX_1,
				  p3Fitted, dxyErr);

        BasePartPtr ptr1(hStables1,i);
        BasePartPtr ptr2(hStables2,j);       
        
        StableData  c1(fit.getTrackP4(1).px(),fit.getTrackP4(1).py(), fit.getTrackP4(1).pz(), ptr1);
        StableData  c2(fit.getTrackP4(2).px(),fit.getTrackP4(2).py(), fit.getTrackP4(2).pz(), ptr2);
        
        const ThreeVector vtxPos = fit.getVertex(MultiVertexFitterD::VERTEX_1);
        const ThreeVector trkMom1(fit.getTrackP4(1).px(),fit.getTrackP4(1).py(),
				  fit.getTrackP4(1).pz());
        const ThreeVector trkMom2(fit.getTrackP4(2).px(),fit.getTrackP4(2).py(),
				  fit.getTrackP4(2).pz());
        
        //build corrected HitPattern for StableData, removing hits before the fit vertex
        if (useHitDropper_) {
          std::pair<reco::HitPattern,uint> hits1 = dropper->CorrectedHitsAOD(s1.track(),
                                                                             topo,
                                                          vtxPos, 
                                                          trkMom1, 
                                                          dlErr, 
                                                          dlzErr);
          std::pair<reco::HitPattern,uint> hits2 = dropper->CorrectedHitsAOD(s2.track(),
                                                                             topo,
                                                          vtxPos, 
                                                          trkMom2, 
                                                          dlErr, 
                                                          dlzErr);
          
          c1.SetHits(hits1.first);
          c2.SetHits(hits2.first);
          c1.SetHitsFilled();
          c2.SetHitsFilled();
          c1.SetNWrongHits(hits1.second);
          c2.SetNWrongHits(hits2.second);
          
          reco::HitPattern sharedHits = dropper->SharedHits(s1.track(),s2.track(), topo);
          d->setSharedHits(sharedHits);
        }
        
        d->addStableChild    (c1);
        d->addStableChild    (c2);               
	d->setFittedMass     (mass);
        d->setFittedMassError(massErr);
        d->setLxy            (dl);
        d->setLxyError       (dlErr);
        d->setLxyToPv        (dl);
        d->setLxyToPvError   (dlErr);
        d->setLz             (dlz);
        d->setLzError        (dlzErr);
        d->setLzToPv         (dlz);
        d->setLzToPvError    (dlzErr);
        d->setDxy            (dxy);
        d->setDxyError       (dxyErr);
        d->setDxyToPv        (dxy);
        d->setDxyToPvError   (dxyErr);

	// put the result into our collection
	pD->push_back(*d);
        delete d;
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

// define this as a plug-in
DEFINE_FWK_MODULE(ProducerV2SS);
