// $Id: ProducerConversions.cc,v 1.22 2009/12/15 23:27:34 bendavid Exp $

#include "MitEdm/Producers/interface/ProducerConversions.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
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
#include <TMath.h>

using namespace std;
using namespace edm;
using namespace reco;
using namespace mitedm;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
ProducerConversions::ProducerConversions(const ParameterSet& cfg) :
  BaseCandProducer (cfg),
  iStables1_       (cfg.getUntrackedParameter<string>("iStables1","")),
  iStables2_       (cfg.getUntrackedParameter<string>("iStables2","")),
  iPVertexes_      (cfg.getUntrackedParameter<string>("iPVertexes","offlinePrimaryVerticesWithBS")),
  usePVertex_      (cfg.getUntrackedParameter<bool>  ("usePVertex",true)),
  convConstraint_  (cfg.getUntrackedParameter<bool>  ("convConstraint",false)),
  convConstraint3D_(cfg.getUntrackedParameter<bool>  ("convConstraint3D",true)),
  rhoMin_          (cfg.getUntrackedParameter<double>("rhoMin",0.0)),
  useRhoMin_       (cfg.getUntrackedParameter<bool>  ("useRhoMin",true)),
  useHitDropper_   (cfg.getUntrackedParameter<bool>  ("useHitDropper",true)),
  applyChargeConstraint_(cfg.getUntrackedParameter<bool>  ("applyChargeConstraint",false))
{
  // Constructor.

  produces<DecayPartCol>();
}

//--------------------------------------------------------------------------------------------------
ProducerConversions::~ProducerConversions()
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
void ProducerConversions::produce(Event &evt, const EventSetup &setup)
{
  // Produce our DecayPartCol.

  // First input collection
  Handle<StablePartCol> hStables1;
  if (!GetProduct(iStables1_, hStables1, evt)) {
    printf("Stable collection 1 not found in ProducerConversions\n");
    return;  
  }
  const StablePartCol *pS1 = hStables1.product();
  // Second input collection
  Handle<StablePartCol> hStables2;
  if (!GetProduct(iStables2_, hStables2, evt)) {
    printf("Stable collection 2 not found in ProducerConversions\n");
    return;
  }
  const StablePartCol *pS2 = hStables2.product();

  const reco::Vertex *vertex = 0;
  mitedm::VertexPtr vPtr;
  if (usePVertex_) {
    // Primary vertex collection
    Handle<reco::VertexCollection> hVertexes;
    if (!GetProduct(iPVertexes_, hVertexes, evt))
      return;
    const reco::VertexCollection *pV = hVertexes.product();
  
    //Choose the primary vertex with the largest number of tracks
    UInt_t maxTracks=0;
    for (UInt_t i=0; i<pV->size(); ++i) {
      const reco::Vertex &v = pV->at(i);
      UInt_t nTracks = v.tracksSize();
      if (nTracks >= maxTracks) {
        maxTracks = nTracks;
        vertex = &v;
        vPtr = mitedm::VertexPtr(hVertexes,i);
      }
    }
  }
  
  // Get hit dropper
  ESHandle<HitDropper> hDropper;
  setup.get<HitDropperRecord>().get("HitDropper",hDropper);
  const HitDropper *dropper = hDropper.product();
  
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

  
  // Create the output collection
  auto_ptr<DecayPartCol> pD(new DecayPartCol());
  
  ClosestApproachInRPhi helixIntersector;
  
  int nFits = 0;
  
  //printf("S1 size = %i\n", pS1->size());
  
  // Simple double loop
  for (UInt_t i = 0; i<pS1->size(); ++i) {
    const StablePart &s1 =  pS1->at(i);
    
    const reco::Track * t1 = s1.track();
    const TrackParameters &trkPar1 = trkPars1.at(i);
    
    UInt_t j;
    if (iStables1_ == iStables2_)
      j = i+1; 
    else
      j = 0;
    
    TrajectoryStateTransform tsTransform;
    
    FreeTrajectoryState initialState1 = tsTransform.initialFreeState(*s1.track(),&*magneticField);
    
    for (; j<pS2->size(); ++j) {
      const StablePart &s2 = pS2->at(j);

      //Do fast helix fit to check if there's any hope
      const reco::Track * t2 = s2.track();
      const TrackParameters &trkPar2 = trkPars2.at(j);
      
      int trackCharge = t1->charge() + t2->charge();
      
      double dR0 = 0.0;
      
      if (!applyChargeConstraint_ || trackCharge==0) {
        FreeTrajectoryState initialState2 = tsTransform.initialFreeState(*s2.track(),&*magneticField);
        helixIntersector.calculate(initialState1, initialState2);
        if (helixIntersector.status())
          dR0 = helixIntersector.crossingPoint().perp();
      }

      int fitStatus = 0;
      MultiVertexFitterD fit;      
      if ( (!applyChargeConstraint_ || trackCharge==0) && (!useRhoMin_ || dR0 > rhoMin_) ) {
      
        // Vertex fit now, possibly with conversion constraint
        nFits++;
  
        fit.init(bfield); // Reset to the magnetic field from the event setup
        
        fit.addTrack(*trkPar1.pars(),*trkPar1.cMat(),1,s1.mass(),MultiVertexFitterD::VERTEX_1);
        fit.addTrack(*trkPar2.pars(),*trkPar2.cMat(),2,s2.mass(),MultiVertexFitterD::VERTEX_1);
        if (convConstraint3D_) {
          fit.conversion_3d(MultiVertexFitterD::VERTEX_1);
          //printf("applying 3d conversion constraint\n");
        }
        else if (convConstraint_) {
          fit.conversion_2d(MultiVertexFitterD::VERTEX_1);
          //printf("applying 2d conversion constraint\n");
        }
        //initialize primary vertex parameters in the fitter
        if (usePVertex_) {
          float vErr[3][3];
          for (UInt_t vi=0; vi<3; ++vi)
            for (UInt_t vj=0; vj<3; ++vj)
              vErr[vi][vj] = vertex->covariance(vi,vj);
              
          fit.setPrimaryVertex(vertex->x(),vertex->y(),vertex->z());
          fit.setPrimaryVertexError(vErr);
        }
        
        fitStatus = fit.fit();     
        
      }
        
      if (fitStatus) {
        DecayPart *d = new DecayPart(oPid_,DecayPart::Fast);
        
        // Update temporarily some of the quantities (prob, chi2, nDoF, mass, lxy, pt, fourMomentum)
        d->setProb(fit.prob());
        d->setChi2(fit.chisq());
        d->setNdof(fit.ndof());

        FourVector p4Fitted(0.,0.,0.,0.);
        p4Fitted += fit.getTrackP4(1);
        p4Fitted += fit.getTrackP4(2);
        d->setFourMomentum(p4Fitted);
        d->setPosition(fit.getVertex     (MultiVertexFitterD::VERTEX_1));
        d->setError   (fit.getErrorMatrix(MultiVertexFitterD::VERTEX_1));
        float mass, massErr;
        const int trksIds[2] = { 1, 2 };
        mass = fit.getMass(2,trksIds,massErr);
        
        ThreeVector p3Fitted(p4Fitted.px(), p4Fitted.py(), p4Fitted.pz());
        
        // Get decay length in xy plane
        float dl, dlErr;
        dl = fit.getDecayLength(MultiVertexFitterD::PRIMARY_VERTEX, MultiVertexFitterD::VERTEX_1,
               p3Fitted, dlErr);
               
        // Get Z decay length               
        float dlz, dlzErr;
        dlz = fit.getZDecayLength(MultiVertexFitterD::PRIMARY_VERTEX, MultiVertexFitterD::VERTEX_1,
               p3Fitted, dlzErr);
               
        // Get impact parameter               
        float dxy, dxyErr;
        dxy = fit.getImpactPar(MultiVertexFitterD::PRIMARY_VERTEX, MultiVertexFitterD::VERTEX_1,
               p3Fitted, dxyErr);

        BasePartPtr ptr1(hStables1,i);
        BasePartPtr ptr2(hStables2,j);       
        
        StableData c1(fit.getTrackP4(1).px(),fit.getTrackP4(1).py(), fit.getTrackP4(1).pz(), ptr1);
        StableData c2(fit.getTrackP4(2).px(),fit.getTrackP4(2).py(), fit.getTrackP4(2).pz(), ptr2);
        
        const ThreeVector vtxPos = fit.getVertex(MultiVertexFitterD::VERTEX_1);
        const ThreeVector trkMom1(fit.getTrackP4(1).px(),
                                  fit.getTrackP4(1).py(), 
                                  fit.getTrackP4(1).pz());
        const ThreeVector trkMom2(fit.getTrackP4(2).px(),
                                  fit.getTrackP4(2).py(), 
                                  fit.getTrackP4(2).pz());
        
        // Build corrected HitPattern for StableData, removing hits before the fit vertex
        if (useHitDropper_) {
          reco::HitPattern hits1 = dropper->CorrectedHitsAOD(s1.track(), vtxPos, trkMom1, dlErr, dlzErr);
          reco::HitPattern hits2 = dropper->CorrectedHitsAOD(s2.track(), vtxPos, trkMom2, dlErr, dlzErr);                 
   
          c1.SetHits(hits1);
          c2.SetHits(hits2);
          c1.SetHitsFilled();
          c2.SetHitsFilled();
          
          reco::HitPattern sharedHits = dropper->SharedHits(s1.track(),s2.track());
          d->setSharedHits(sharedHits);
        }
        
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
        
        if (usePVertex_)
          d->setPrimaryVertex(vPtr);
        
        // Put the result into our collection
        pD->push_back(*d);

        delete d;
      }
    }
  }
  
  //printf("nConversionFits = %i\n",nFits);

  // Write the collection even if it is empty
  if (0) {
    cout << " ProducerConversions::produce - " << pD->size() << " entries collection created -"
         << " (Pid: " << oPid_ << ")\n";
  }
  evt.put(pD);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProducerConversions);
