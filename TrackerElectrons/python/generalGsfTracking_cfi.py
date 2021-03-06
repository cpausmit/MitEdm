## $Id: generalGsfTracking_cfi.py,v 1.4 2009/03/21 12:46:57 loizides Exp $

#import FWCore.ParameterSet.Config as cms

#import RecoEgamma.EgammaElectronProducers.gsfElectronCkfTrackCandidateMaker_cff

#TrajectoryBuilderForGeneralGsfTracks = RecoEgamma.EgammaElectronProducers.gsfElectronCkfTrackCandidateMaker_cff.TrajectoryBuilderForPixelMatchGsfElectrons.clone()
#TrajectoryBuilderForGeneralGsfTracks.ComponentName = 'TrajectoryBuilderForGeneralGsfTracks'
#TrajectoryBuilderForGeneralGsfTracks.trajectoryFilterName = 'newTrajectoryFilter'
## TrajectoryBuilderForGeneralGsfTracks.trajectoryFilterName = 'TrajectoryFilterForGeneralGsfTracks'

## TrajectoryFilterForGeneralGsfTracks = RecoEgamma.EgammaElectronProducers.gsfElectronCkfTrackCandidateMaker_cff.TrajectoryFilterForPixelMatchGsfElectrons.clone()
## TrajectoryFilterForGeneralGsfTracks.ComponentName = 'TrajectoryFilterForGeneralGsfTracks'
## TrajectoryFilterForGeneralGsfTracks.filterPset.minPt = 0.0
## TrajectoryFilterForGeneralGsfTracks.filterPset.minPt.nSigmaMinPt = 0.0

#newTrackCandidateMakerGsf = RecoEgamma.EgammaElectronProducers.gsfElectronCkfTrackCandidateMaker_cff.egammaCkfTrackCandidates.clone()
#newTrackCandidateMakerGsf.TrajectoryBuilder = 'TrajectoryBuilderForGeneralGsfTracks'
##newTrackCandidateMakerGsf.SeedProducer = 'newCombinedSeeds'

##normal iterative tracking procedure reimplemented using gsf fits for all tracks
#import TrackingTools.GsfTracking.GsfElectronFit_cfi
#preFilterFirstStepTracksGsf = TrackingTools.GsfTracking.GsfElectronFit_cfi.GsfGlobalElectronTest.clone()
#preFilterFirstStepTracksGsf.src = 'newTrackCandidateMakerGsf'
#preFilterFirstStepTracksGsf.TrajectoryInEvent = True

#from MitEdm.TrackerElectrons.iterativeTkGsf_cff import *
#from MitEdm.TrackerElectrons.MergeTrackCollectionsGsf_cff import *

#import MitEdm.Producers.gsfTrackAssociator_cff
#generalGsfTrackAssociator = MitEdm.Producers.gsfTrackAssociator_cff.gsfTrackAssociator.clone()
#generalGsfTrackAssociator.fromTracks = 'generalTracksGsf'

#import SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi
#gsfMcMatch = SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi.trackingParticleRecoTrackAsssociation.clone()
#gsfMcMatch.label_tr = 'generalTracksGsf'

##configure tracker-seeded electron producer with very loose cuts
#import RecoEgamma.EgammaElectronProducers.gsfElectrons_cfi
#globalGeneralGsfElectrons = RecoEgamma.EgammaElectronProducers.gsfElectrons_cfi.gsfElectrons.clone()
#globalGeneralGsfElectrons.tracks = cms.InputTag("generalTracksGsf")
#globalGeneralGsfElectrons.minEOverPEndcaps = 0.0
#globalGeneralGsfElectrons.minEOverPBarrel = 0.0
#globalGeneralGsfElectrons.maxEOverPEndcaps = 99999.9
#globalGeneralGsfElectrons.maxEOverPBarrel = 99999.9
##globalGeneralGsfElectrons.maxHOverE = 99999.9
##globalGeneralGsfElectrons.maxDeltaPhi = 0.3
##globalGeneralGsfElectrons.maxDeltaEta = 0.06
##globalGeneralGsfElectrons.applyAmbResolution = True

##run isolation and electron id algorithms on new electron collection
#import RecoEgamma.EgammaIsolationAlgos.eleIsolationSequence_cff
#eleIsoDepositTkGsf = RecoEgamma.EgammaIsolationAlgos.eleIsolationSequence_cff.eleIsoDepositTk.clone()
#eleIsoDepositTkGsf.src = cms.InputTag("globalGeneralGsfElectrons")
#eleIsoDepositEcalFromHitsGsf =  RecoEgamma.EgammaIsolationAlgos.eleIsolationSequence_cff.eleIsoDepositEcalFromHits.clone()
#eleIsoDepositEcalFromHitsGsf.src = cms.InputTag("globalGeneralGsfElectrons")
##eleIsoDepositHcalFromHitsGsf =  RecoEgamma.EgammaIsolationAlgos.eleIsolationSequence_cff.eleIsoDepositHcalFromHits.clone()
##eleIsoDepositHcalFromHitsGsf.src = cms.InputTag("globalGeneralGsfElectrons")
#eleIsoFromDepsTkGsf =  RecoEgamma.EgammaIsolationAlgos.eleIsolationSequence_cff.eleIsoFromDepsTk.clone()
#eleIsoFromDepsTkGsf.deposits[0].src = cms.InputTag("eleIsoDepositEcalFromHitsGsf")
#eleIsoFromDepsEcalFromHitsGsf =  RecoEgamma.EgammaIsolationAlgos.eleIsolationSequence_cff.eleIsoFromDepsEcalFromHits.clone()
#eleIsoFromDepsEcalFromHitsGsf.deposits[0].src = cms.InputTag("eleIsoDepositEcalFromHitsGsf")
#eleIsoFromDepsHcalFromHitsGsf =  RecoEgamma.EgammaIsolationAlgos.eleIsolationSequence_cff.eleIsoFromDepsHcalFromHits.clone()
#eleIsoFromDepsHcalFromHitsGsf.deposits[0].src = cms.InputTag("eleIsoDepositHcalFromHitsGsf")

#generalGsfElectronsIso = cms.Sequence(eleIsoDepositTkGsf*eleIsoDepositEcalFromHitsGsf*eleIsoDepositHcalFromHitsGsf
                                     #*eleIsoFromDepsTkGsf*eleIsoFromDepsEcalFromHitsGsf*eleIsoFromDepsHcalFromHitsGsf)

#import RecoEgamma.ElectronIdentification.electronIdSequence_cff
#eidRobustLooseGsf = RecoEgamma.ElectronIdentification.electronIdSequence_cff.eidRobustLoose.clone()
#eidRobustLooseGsf.src = cms.InputTag("globalGeneralGsfElectrons")
#eidRobustTightGsf = RecoEgamma.ElectronIdentification.electronIdSequence_cff.eidRobustTight.clone()
#eidRobustTightGsf.src = cms.InputTag("globalGeneralGsfElectrons")
#eidLooseGsf = RecoEgamma.ElectronIdentification.electronIdSequence_cff.eidLoose.clone()
#eidLooseGsf.src = cms.InputTag("globalGeneralGsfElectrons")
#eidTightGsf = RecoEgamma.ElectronIdentification.electronIdSequence_cff.eidTight.clone()
#eidTightGsf.src = cms.InputTag("globalGeneralGsfElectrons")

#import RecoEgamma.ElectronIdentification.electronIdNeuralNet_cfi
#eidNeuralNetGsf = RecoEgamma.ElectronIdentification.electronIdNeuralNet_cfi.eidNeuralNet.clone()
#eidNeuralNetGsf.src = cms.InputTag("globalGeneralGsfElectrons")

#import RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi
#eidLikelihoodExtGsf = RecoEgamma.ElectronIdentification.electronIdLikelihoodExt_cfi.eidLikelihoodExt.clone()
#eidLikelihoodExtGsf.src = cms.InputTag("globalGeneralGsfElectrons")

#generalGsfElectronId = cms.Sequence(eidRobustLooseGsf*eidRobustTightGsf*eidLooseGsf*eidTightGsf*eidNeuralNetGsf*eidLikelihoodExtGsf)

#generalGsfTracking = cms.Sequence(newTrackCandidateMakerGsf*preFilterFirstStepTracksGsf*iterTrackingGsf*trackCollectionMergingGsf*
                                  #gsfMcMatch*generalGsfTrackAssociator*
                                  #globalGeneralGsfElectrons*generalGsfElectronsIso*generalGsfElectronId)
