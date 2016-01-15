#include "MitEdm/Producers/interface/ProducerConversionsKinematic.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"
#include "MitEdm/Producers/interface/HitDropperRecord.h"
#include "MitEdm/Producers/interface/HitDropper.h"
#include "MitEdm/DataFormats/interface/Types.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include "MitEdm/DataFormats/interface/StableData.h"
#include "MitEdm/VertexFitInterface/interface/MvfInterface.h"
#include "MitEdm/VertexFitInterface/interface/TrackParameters.h"
#include <TMath.h>

//#include "DataFormats/GeometrySurface/interface/ReferenceCounted.h"
//#include "RecoVertex/VertexPrimitives/interface/VertexState.h"
//#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicVertex.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticle.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicTree.h"
#include "RecoVertex/KinematicFitPrimitives/interface/TransientTrackKinematicParticle.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"

#include "RecoVertex/KinematicFitPrimitives/interface/ParticleMass.h"
#include "RecoVertex/KinematicFitPrimitives/interface/MultiTrackKinematicConstraint.h"
#include "RecoVertex/KinematicFitPrimitives/interface/KinematicParticleFactoryFromTransientTrack.h"
#include "RecoVertex/KinematicFit/interface/KinematicConstrainedVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/TwoTrackMassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleVertexFitter.h"
#include "RecoVertex/KinematicFit/interface/KinematicParticleFitter.h"
#include "RecoVertex/KinematicFit/interface/MassKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/ColinearityKinematicConstraint.h"
#include "RecoVertex/KinematicFit/interface/MultiTrackMassKinematicConstraint.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "TrackingTools/PatternTools/interface/TwoTrackMinimumDistance.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include "FWCore/Framework/interface/MakerMacros.h"

using namespace std;
using namespace edm;
using namespace reco;
using namespace mitedm;
using namespace mithep;

//--------------------------------------------------------------------------------------------------
ProducerConversionsKinematic::ProducerConversionsKinematic(const ParameterSet& cfg) :
  BaseCandProducer (cfg),
  iStables1Token_(consumes<StablePartCol>(edm::InputTag(cfg.getUntrackedParameter<string>("iStables1","")))),
  iStables2Token_(consumes<StablePartCol>(edm::InputTag(cfg.getUntrackedParameter<string>("iStables2","")))),
  convConstraint_  (cfg.getUntrackedParameter<bool>  ("convConstraint",false)),
  convConstraint3D_(cfg.getUntrackedParameter<bool>  ("convConstraint3D",true)),
  rhoMin_          (cfg.getUntrackedParameter<double>("rhoMin",0.0)),
  useRhoMin_       (cfg.getUntrackedParameter<bool>  ("useRhoMin",true)),
  useHitDropper_   (cfg.getUntrackedParameter<bool>  ("useHitDropper",true)),
  applyChargeConstraint_(cfg.getUntrackedParameter<bool>("applyChargeConstraint",false)),
  sameCollection_(cfg.getUntrackedParameter<string>("iStables1","") == cfg.getUntrackedParameter<string>("iStables2",""))
{
  // Constructor.

  produces<DecayPartCol>();
}

//--------------------------------------------------------------------------------------------------
ProducerConversionsKinematic::~ProducerConversionsKinematic()
{
  // Destructor.
}

//--------------------------------------------------------------------------------------------------
void ProducerConversionsKinematic::produce(Event &evt, const EventSetup &setup)
{
  // Produce our DecayPartCol.

  // First input collection
  Handle<StablePartCol> hStables1;
  if (!GetProduct(iStables1Token_, hStables1, evt)) {
    printf("Stable collection 1 not found in ProducerConversionsKinematic\n");
    return;  
  }
  const StablePartCol *pS1 = hStables1.product();
  // Second input collection
  Handle<StablePartCol> hStables2;
  if (!GetProduct(iStables2Token_, hStables2, evt)) {
    printf("Stable collection 2 not found in ProducerConversionsKinematic\n");
    return;
  }
  const StablePartCol *pS2 = hStables2.product();

  // Get hit dropper
  ESHandle<HitDropper> hDropper;
  setup.get<HitDropperRecord>().get("HitDropper",hDropper);
  const HitDropper *dropper = hDropper.product();

  ESHandle<TrackerTopology> hTopo;
  setup.get<TrackerTopologyRcd>().get(hTopo);
  TrackerTopology const& topo(*hTopo);
  
  // Get Magnetic Field from event setup, taking value at (0,0,0)
  edm::ESHandle<MagneticField> magneticField;
  setup.get<IdealMagneticFieldRecord>().get(magneticField);

  //const double bfield = magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
  
  edm::ESHandle<TransientTrackBuilder> hTransientTrackBuilder;
  setup.get<TransientTrackRecord>().get("TransientTrackBuilder",hTransientTrackBuilder);
  const TransientTrackBuilder *transientTrackBuilder = hTransientTrackBuilder.product();
  
  // Construct intermediate collection of TrackParameters in mvf format for vertex fit
  std::vector<reco::TransientTrack> ttrks1;
  for (UInt_t i = 0; i<pS1->size(); ++i) {
    const reco::Track *t = pS1->at(i).track();
    const reco::TransientTrack ttrk = transientTrackBuilder->build(t);
    ttrks1.push_back(ttrk);
  }
  
  std::vector<reco::TransientTrack> ttrks2;
  if (sameCollection_) {
    ttrks2 = ttrks1;
  }
  else for (UInt_t i = 0; i<pS2->size(); ++i) {
    const reco::Track *t = pS2->at(i).track();
    const reco::TransientTrack ttrk = transientTrackBuilder->build(t);
    ttrks2.push_back(ttrk);
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
//    const TrackParameters &trkPar1 = trkPars1.at(i);
    const reco::TransientTrack &ttrk1 = ttrks1.at(i);
    
    UInt_t j;
    if (sameCollection_)
      j = i+1; 
    else
      j = 0;
    
    FreeTrajectoryState initialState1 = trajectoryStateTransform::initialFreeState(*s1.track(),&*magneticField);
   
    for (; j<pS2->size(); ++j) {
      const StablePart &s2 = pS2->at(j);

      //Do fast helix fit to check if there's any hope
      const reco::Track * t2 = s2.track();
     // const TrackParameters &trkPar2 = trkPars2.at(j);
      const reco::TransientTrack &ttrk2 = ttrks2.at(j);
      
      int trackCharge = t1->charge() + t2->charge();
      
      double dR0 = 0.0;
      double dZ0 = 1e6;
      double angle1 = 0.0;
      double angle2 = 0.0;
      double angle1simple = 0.0;
      double angle2simple = 0.0;
      double phi1helix = 0.0;
      double phi2helix = 0.0;
      double eta1helix = 0.0;
      double eta2helix = 0.0;
      if (!applyChargeConstraint_ || trackCharge==0) {
	FreeTrajectoryState initialState2 = trajectoryStateTransform::initialFreeState(*s2.track(),&*magneticField);
        helixIntersector.calculate(initialState1, initialState2);
        if (helixIntersector.status()) {
          dR0 = helixIntersector.crossingPoint().perp();
          //std::pair<GlobalPoint, GlobalPoint> intPoints = helixIntersector.points();
          //dZ0 = intPoints.first.z() - intPoints.second.z();

          std::pair <GlobalTrajectoryParameters, GlobalTrajectoryParameters > trajpars = helixIntersector.trajectoryParameters();
          dZ0 = trajpars.first.position().z() - trajpars.second.position().z();
          angle1simple = trajpars.first.momentum().phi() - t1->phi();
          angle2simple = trajpars.second.momentum().phi() - t2->phi();
          phi1helix = trajpars.first.momentum().phi();
          phi2helix = trajpars.second.momentum().phi();
          eta1helix = trajpars.first.momentum().eta();
          eta2helix = trajpars.second.momentum().eta();

          TwoTrackMinimumDistance trackmin;
          trackmin.calculate(initialState1,initialState2);
          if (trackmin.status()) {
            angle1 = trackmin.firstAngle();
            angle2 = trackmin.secondAngle();
          }
          //initialFitPoint = helixIntersector.crossingPoint();
        }
      }

      double prob = -99.0;

      //int fitStatus = 0;

      RefCountedKinematicTree myTree;
      RefCountedKinematicVertex gamma_dec_vertex;
      RefCountedKinematicParticle the_photon;
      if ( (!applyChargeConstraint_ || trackCharge==0) && (!useRhoMin_ || dR0 > rhoMin_) ) {
      
        // Vertex fit now, possibly with conversion constraint
        nFits++;

        //bool cmsFitStatus = false;    
            
        float sigma = 0.00000000001;
        float chi = 0.;
        float ndf = 0.;

        edm::ParameterSet pSet;
        pSet.addParameter<double>("maxDistance", 1e-3);//0.001
        pSet.addParameter<double>("maxOfInitialValue",1.4) ;//1.4
        pSet.addParameter<int>("maxNbrOfIterations", 40);//40

        KinematicParticleFactoryFromTransientTrack pFactory;

        vector<RefCountedKinematicParticle> particles;
        
        particles.push_back(pFactory.particle (ttrk1,s1.mass(),chi,ndf,sigma));
        particles.push_back(pFactory.particle (ttrk2,s2.mass(),chi,ndf,sigma));

        MultiTrackKinematicConstraint *constr =
	  new ColinearityKinematicConstraint(ColinearityKinematicConstraint::PhiTheta);
        //MultiTrackKinematicConstraint *constr = new ColinearityKinematicConstraint(ColinearityKinematicConstraint::Phi);
        
//        ParticleMass pmass(kmass);
        //MultiTrackKinematicConstraint *constr = new TwoTrackMassKinematicConstraint(pmass);
        
        KinematicConstrainedVertexFitter kcvFitter;
        kcvFitter.setParameters(pSet);
        //RefCountedKinematicTree myTree = kcvFitter.fit(particles, constr);
        //RefCountedKinematicTree myTree = kcvFitter.fit(particles, constr, &initialFitPoint);
        //RefCountedKinematicTree myTree = kcvFitter.fit(particles);

        //if (cdfProb>1e-6) {
          myTree = kcvFitter.fit(particles, constr);
        //}


        
        

	  if (myTree->isValid()) {
            myTree->movePointerToTheTop();
            the_photon = myTree->currentParticle();                                           

            if (the_photon->currentState().isValid()) {
                //const ParticleMass photon_mass = the_photon->currentState().mass();
                gamma_dec_vertex = myTree->currentDecayVertex();

                if (gamma_dec_vertex->vertexIsValid()){         
		  //cmsFitStatus = true;

                    //const float chi2Prob = ChiSquaredProbability(gamma_dec_vertex->chiSquared(),
		    //                                             gamma_dec_vertex->degreesOfFreedom());
                    double cmsChi2 = gamma_dec_vertex->chiSquared();
                    double cmsNdof = gamma_dec_vertex->degreesOfFreedom();
                    //cmsR  = gamma_dec_vertex->position().perp();
                    //cmsPhi = gamma_dec_vertex->position().phi();
                    prob = TMath::Prob(cmsChi2,cmsNdof);
                }
            }
            

            
        }
        

        
        
      }
        
      if (prob>1e-10) {
        DecayPart *d = new DecayPart(oPid_,DecayPart::Fast);
        
        // Update temporarily some of the quantities (prob, chi2, nDoF, mass, lxy, pt, fourMomentum)
        d->setProb(prob);
        d->setChi2(gamma_dec_vertex->chiSquared());
        d->setNdof(gamma_dec_vertex->degreesOfFreedom());

        GlobalVector momphoton = the_photon->currentState().globalMomentum();
        ThreeVector gammaMom(momphoton);
        double gammamass = the_photon->currentState().mass();
        double photonenergy = sqrt(gammaMom.R()*gammaMom.R() + gammamass*gammamass);

        FourVector p4Fitted(gammaMom.x(),gammaMom.y(),gammaMom.z(),photonenergy);
        //p4Fitted += fit.getTrackP4(1);
        //p4Fitted += fit.getTrackP4(2);
        d->setFourMomentum(p4Fitted);
        ThreeVector vtxPos(gamma_dec_vertex->position());
        d->setPosition(vtxPos);
        //d->setError   (fit.getErrorMatrix(MultiVertexFitterD::VERTEX_1));
        //float mass, massErr;
        //const int trksIds[2] = { 1, 2 };
        //mass = fit.getMass(2,trksIds,massErr);

        
        ThreeVector p3Fitted(p4Fitted.px(), p4Fitted.py(), p4Fitted.pz());
        
        // Get decay length in xy plane
//         float dl, dlErr;
//         dl = fit.getDecayLength(MultiVertexFitterD::PRIMARY_VERTEX, MultiVertexFitterD::VERTEX_1,
//                p3Fitted, dlErr);
//                
//         // Get Z decay length               
//         float dlz, dlzErr;
//         dlz = fit.getZDecayLength(MultiVertexFitterD::PRIMARY_VERTEX, MultiVertexFitterD::VERTEX_1,
//                p3Fitted, dlzErr);
//                
//         // Get impact parameter               
//         float dxy, dxyErr;
//         dxy = fit.getImpactPar(MultiVertexFitterD::PRIMARY_VERTEX, MultiVertexFitterD::VERTEX_1,
//                p3Fitted, dxyErr);

        double dlz = vtxPos.z()*TMath::Abs(p4Fitted.Pz())/p4Fitted.Pz();

        ThreeVector momPerp(p4Fitted.Px(),p4Fitted.Py(),0);
        ThreeVector posPerp(vtxPos.x(),vtxPos.y(),0);
        double dl = momPerp.Dot(posPerp)/momPerp.R();

        
        double dxy = momPerp.Cross(vtxPos).Z()/p4Fitted.Pt();

        

        BasePartPtr ptr1(hStables1,i);
        BasePartPtr ptr2(hStables2,j);       
        
        std::vector< RefCountedKinematicParticle > bs_children = myTree->finalStateParticles();
      
        GlobalVector mom1 = bs_children.at(0)->currentState().globalMomentum();   
        GlobalVector mom2 = bs_children.at(1)->currentState().globalMomentum();       

        const ThreeVector trkMom1(mom1.x(),mom1.y(),mom1.z());
        const ThreeVector trkMom2(mom2.x(),mom2.y(),mom2.z());

        double deltaphi = reco::deltaPhi(trkMom1.Phi(),trkMom2.Phi());
        double etaratio = trkMom1.Eta()/trkMom2.Eta();
        
        //if (prob>1e-6 && TMath::Abs(p4Fitted.Eta()) <0.01) {
        if (prob>1e-6 && deltaphi > 0.1) {
           printf("kin track 1 : qoverp    = %5f, pt    = %5f, theta    = %5f, eta    = %5f, phi    = %5f, dxy    = %5f, dsz    = %5f, dz    = %5f\n",t1->qoverp(),t1->pt(),t1->theta(),t1->eta(),t1->phi(), t1->dxy(), t1->dsz(), t1->dz());
           printf("kin track 1 : qoverperr = %5f, pterr = %5f, thetaerr = %5f, etaerr = %5f, phierr = %5f, dxyerr = %5f, dszerr = %5f, dzerr = %5f\n",t1->qoverpError(),t1->ptError(),t1->thetaError(),t1->etaError(),t1->phiError(), t1->dxyError(), t1->dszError(), t1->dzError());
           printf("kin track 2 : qoverp    = %5f, pt    = %5f, theta    = %5f, eta    = %5f, phi    = %5f, dxy    = %5f, dsz    = %5f, dz    = %5f\n",t2->qoverp(),t2->pt(),t2->theta(),t2->eta(),t2->phi(), t2->dxy(), t2->dsz(), t2->dz());
           printf("kin track 2 : qoverperr = %5f, pterr = %5f, thetaerr = %5f, etaerr = %5f, phierr = %5f, dxyerr = %5f, dszerr = %5f, dzerr = %5f\n",t2->qoverpError(),t2->ptError(),t2->thetaError(),t2->etaError(),t2->phiError(), t2->dxyError(), t2->dszError(), t2->dzError());
           printf("kin conversion pt = %5f, eta = %5f, theta = %5f, phi = %5f\n",p4Fitted.Pt(),p4Fitted.Eta(),p4Fitted.Theta(),p4Fitted.Phi());
           printf("kin trkMom1    pt = %5f, eta = %5f, theta = %5f, phi = %5f\n",trkMom1.Rho(),trkMom1.Eta(),trkMom1.Theta(),trkMom1.Phi());
           printf("kin trkMom2    pt = %5f, eta = %5f, theta = %5f, phi = %5f\n",trkMom2.Rho(),trkMom2.Eta(),trkMom2.Theta(),trkMom2.Phi());
           printf("kin helix1               eta = %5f,              phi = %5f\n",phi1helix,eta1helix);
           printf("kin helix2               eta = %5f,              phi = %5f\n",phi2helix,eta2helix);           
           printf("kin dR0 = %5f, dZ0 = %5f, firstAngle = %5f, secondAngle = %5f, angle1simple = %5f, angle2simple = %5f, deltaphi = %5f, etaratio = %5f\n",dR0, dZ0,angle1,angle2,angle1simple,angle2simple,deltaphi,etaratio);
        } 


        StableData c1(mom1.x(),mom1.y(), mom1.z(), ptr1);
        StableData c2(mom2.x(),mom2.y(), mom2.z(), ptr2);
  
        
        // Build corrected HitPattern for StableData, removing hits before the fit vertex
        if (useHitDropper_) {
          std::pair<reco::HitPattern,uint> hits1 = dropper->CorrectedHitsAOD(s1.track(), topo, vtxPos, trkMom1, 1.0, 1.0);
          std::pair<reco::HitPattern,uint> hits2 = dropper->CorrectedHitsAOD(s2.track(), topo, vtxPos, trkMom2, 1.0, 1.0);                 
   
          c1.SetHits(hits1.first);
          c2.SetHits(hits2.first);
          c1.SetHitsFilled();
          c2.SetHitsFilled();
          c1.SetNWrongHits(hits1.second);
          c2.SetNWrongHits(hits2.second);
          
          reco::HitPattern sharedHits = dropper->SharedHits(s1.track(),s2.track(), topo);
          d->setSharedHits(sharedHits);
        }
        
        d->addStableChild(c1);
        d->addStableChild(c2);
               
               
//         d->setFittedMass     (mass);
//         d->setFittedMassError(massErr);
        
        d->setLxy(dl);
//         d->setLxyError(dlErr);
        d->setLxyToPv(dl);
//         d->setLxyToPvError(dlErr);
        
        d->setLz(dlz);
//        d->setLzError(dlzErr);
        d->setLzToPv(dlz);
//         d->setLzToPvError(dlzErr);
        
        d->setDxy(dxy);
//         d->setDxyError(dxyErr);
        d->setDxyToPv(dxy);
//         d->setDxyToPvError(dxyErr);
        
        // Put the result into our collection
        pD->push_back(*d);

        delete d;
      }
    }
  }
  
  //printf("nConversionFits = %i\n",nFits);

  // Write the collection even if it is empty
  if (0) {
    cout << " ProducerConversionsKinematic::produce - " << pD->size() << " entries collection created -"
         << " (Pid: " << oPid_ << ")\n";
  }
  evt.put(pD);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ProducerConversionsKinematic);
