import FWCore.ParameterSet.Config as cms

#HIT REMOVAL
fourthClustersGsf = cms.EDFilter("TrackClusterRemover",
    oldClusterRemovalInfo = cms.InputTag("thClustersGsf"),
    trajectories = cms.InputTag("thWithMaterialTracksGsf"),
    pixelClusters = cms.InputTag("thClustersGsf"),
    Common = cms.PSet(
        maxChi2 = cms.double(30.0)
    ),
    stripClusters = cms.InputTag("thClustersGsf")
)


#TRACKER HITS
import RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi
import RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi
fourthPixelRecHitsGsf = RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi.siPixelRecHits.clone()
fourthStripRecHitsGsf = RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi.siStripMatchedRecHits.clone()
fourthPixelRecHitsGsf.src = 'fourthClustersGsf'
fourthStripRecHitsGsf.ClusterProducer = 'fourthClustersGsf'




#SEEDS
import RecoTracker.TkSeedGenerator.GlobalMixedSeeds_cfi
fourthPLSeedsGsf = RecoTracker.TkSeedGenerator.GlobalMixedSeeds_cfi.globalMixedSeeds.clone()
import RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi
fourthPLSeedsGsf.OrderedHitsFactoryPSet.SeedingLayers = 'FourthLayerPairsGsf'
fourthPLSeedsGsf.RegionFactoryPSet.RegionPSet.ptMin = 0.6
fourthPLSeedsGsf.RegionFactoryPSet.RegionPSet.originHalfLength = 10.0
fourthPLSeedsGsf.RegionFactoryPSet.RegionPSet.originRadius = 2.0


#TRAJECTORY MEASUREMENT
fourthMeasurementTrackerGsf = RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi.MeasurementTracker.clone()
import TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi
fourthMeasurementTrackerGsf.ComponentName = 'fourthMeasurementTrackerGsf'
fourthMeasurementTrackerGsf.pixelClusterProducer = 'fourthClustersGsf'
fourthMeasurementTrackerGsf.stripClusterProducer = 'fourthClustersGsf'

#TRAJECTORY FILTER
fourthCkfTrajectoryFilterGsf = TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi.trajectoryFilterESProducer.clone()
import RecoTracker.CkfPattern.GroupedCkfTrajectoryBuilderESProducer_cfi
fourthCkfTrajectoryFilterGsf.ComponentName = 'fourthCkfTrajectoryFilterGsf'
fourthCkfTrajectoryFilterGsf.filterPset.maxLostHits = 0
fourthCkfTrajectoryFilterGsf.filterPset.minimumNumberOfHits = 5
fourthCkfTrajectoryFilterGsf.filterPset.minPt = 0.3

# #TRAJECTORY BUILDER
# fourthCkfTrajectoryBuilderGsf = RecoTracker.CkfPattern.GroupedCkfTrajectoryBuilderESProducer_cfi.GroupedCkfTrajectoryBuilder.clone()
# import RecoTracker.CkfPattern.CkfTrackCandidates_cfi
# fourthCkfTrajectoryBuilderGsf.ComponentName = 'fourthCkfTrajectoryBuilderGsf'
# fourthCkfTrajectoryBuilderGsf.MeasurementTrackerName = 'fourthMeasurementTrackerGsf'
# fourthCkfTrajectoryBuilderGsf.trajectoryFilterName = 'fourthCkfTrajectoryFilterGsf'
# 
# 
# #TRACK CANDIDATES
# fourthTrackCandidatesGsf = RecoTracker.CkfPattern.CkfTrackCandidates_cfi.ckfTrackCandidates.clone()
# import RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi
# fourthTrackCandidatesGsf.SeedProducer = 'fourthPLSeedsGsf'
# fourthTrackCandidatesGsf.TrajectoryBuilder = 'fourthCkfTrajectoryBuilderGsf'

import RecoEgamma.EgammaElectronProducers.gsfElectronCkfTrackCandidateMaker_cff
#TRAJECTORY BUILDER
fourthCkfTrajectoryBuilderGsf = RecoEgamma.EgammaElectronProducers.gsfElectronCkfTrackCandidateMaker_cff.TrajectoryBuilderForPixelMatchGsfElectrons.clone()
fourthCkfTrajectoryBuilderGsf.ComponentName = 'fourthCkfTrajectoryBuilderGsf'
fourthCkfTrajectoryBuilderGsf.MeasurementTrackerName = 'fourthMeasurementTrackerGsf'
fourthCkfTrajectoryBuilderGsf.trajectoryFilterName = 'fourthCkfTrajectoryFilterGsf'


#TRACK CANDIDATES
fourthTrackCandidatesGsf = RecoEgamma.EgammaElectronProducers.gsfElectronCkfTrackCandidateMaker_cff.egammaCkfTrackCandidates.clone()
fourthTrackCandidatesGsf.SeedProducer = 'fourthPLSeedsGsf'
fourthTrackCandidatesGsf.TrajectoryBuilder = 'fourthCkfTrajectoryBuilderGsf'


import TrackingTools.GsfTracking.GsfElectronFit_cfi
#TRACKS
fourthWithMaterialTracksGsf = TrackingTools.GsfTracking.GsfElectronFit_cfi.GsfGlobalElectronTest.clone()
fourthWithMaterialTracksGsf.src = 'fourthTrackCandidatesGsf'
fourthWithMaterialTracksGsf.clusterRemovalInfo = cms.InputTag('fourthClustersGsf')
fourthWithMaterialTracksGsf.AlgorithmName = cms.string('iter4') 
fourthWithMaterialTracksGsf.TrajectoryInEvent = True

#SEEDING LAYERS
fourthlayerpairsGsf = cms.ESProducer("PixelLessLayerPairsESProducer",
    ComponentName = cms.string('FourthLayerPairsGsf'),
    layerList = cms.vstring('TIB1+TIB2',
        'TIB1+TID1_pos','TIB1+TID1_neg',
        'TID1_pos+TID2_pos','TID2_pos+TID3_pos','TID3_pos+TEC1_pos',
        'TEC1_pos+TEC2_pos','TEC2_pos+TEC3_pos','TEC3_pos+TEC4_pos','TEC3_pos+TEC5_pos','TEC4_pos+TEC5_pos',
        'TID1_neg+TID2_neg','TID2_neg+TID3_neg','TID3_neg+TEC1_neg',
        'TEC1_neg+TEC2_neg','TEC2_neg+TEC3_neg','TEC3_neg+TEC4_neg','TEC3_neg+TEC5_neg','TEC4_neg+TEC5_neg'),
    TIB = cms.PSet(
        TTRHBuilder = cms.string('WithTrackAngle'),
        matchedRecHits = cms.InputTag("fourthStripRecHitsGsf","matchedRecHit")
    ),
    TID = cms.PSet(
        matchedRecHits = cms.InputTag("fourthStripRecHitsGsf","matchedRecHit"),
        useRingSlector = cms.untracked.bool(True),
        TTRHBuilder = cms.string('WithTrackAngle'),
        minRing = cms.int32(1),
        maxRing = cms.int32(2)
    ),
    TEC = cms.PSet(
        matchedRecHits = cms.InputTag("fourthStripRecHitsGsf","matchedRecHit"),
        useRingSlector = cms.untracked.bool(True),
        TTRHBuilder = cms.string('WithTrackAngle'),
        minRing = cms.int32(1),
        maxRing = cms.int32(2)
    )
)

#import RecoTracker.FinalTrackSelectors.selectHighPurity_cfi
#pixellessStep = RecoTracker.FinalTrackSelectors.selectHighPurity_cfi.selectHighPurity.clone()
#pixellessStep.src = 'fourthWithMaterialTracks'
#pixellessStep.copyTrajectories = True
#pixellessStep.chi2n_par = 0.3
#pixellessStep.res_par = ( 0.003, 0.001 )
#pixellessStep.minNumberLayers = 5
#pixellessStep.d0_par1 = ( 1.0, 4.0 )
#pixellessStep.dz_par1 = ( 1.0, 4.0 )
#pixellessStep.d0_par2 = ( 1.0, 4.0 )
#pixellessStep.dz_par2 = ( 1.0, 4.0 )

fourthStepGsf = cms.Sequence(fourthClustersGsf*
                          fourthPixelRecHitsGsf*fourthStripRecHitsGsf*
                          fourthPLSeedsGsf*
                          fourthTrackCandidatesGsf*
                          fourthWithMaterialTracksGsf)
