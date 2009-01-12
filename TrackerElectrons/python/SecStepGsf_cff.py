import FWCore.ParameterSet.Config as cms

# new hit collection
secClustersGsf = cms.EDFilter("TrackClusterRemover",
    trajectories = cms.InputTag("preFilterFirstStepTracksGsf"),
    pixelClusters = cms.InputTag("siPixelClusters"),
    Common = cms.PSet(
        maxChi2 = cms.double(30.0)
    ),
    stripClusters = cms.InputTag("siStripClusters")
)

import RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi
secPixelRecHitsGsf = RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi.siPixelRecHits.clone()
import RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi
secStripRecHitsGsf = RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi.siStripMatchedRecHits.clone()

secPixelRecHitsGsf.src = cms.InputTag("secClustersGsf")
secStripRecHitsGsf.ClusterProducer = 'secClustersGsf'


# seeding

import RecoTracker.TkSeedingLayers.PixelLayerTriplets_cfi
seclayertripletsGsf = RecoTracker.TkSeedingLayers.PixelLayerTriplets_cfi.pixellayertriplets.clone()
import RecoTracker.TkSeedGenerator.GlobalSeedsFromTripletsWithVertices_cfi
secTripletsGsf = RecoTracker.TkSeedGenerator.GlobalSeedsFromTripletsWithVertices_cfi.globalSeedsFromTripletsWithVertices.clone()

seclayertripletsGsf.ComponentName = 'SecLayerTripletsGsf'
seclayertripletsGsf.BPix.HitProducer = 'secPixelRecHitsGsf'
seclayertripletsGsf.FPix.HitProducer = 'secPixelRecHitsGsf'
secTripletsGsf.RegionFactoryPSet.RegionPSet.originHalfLength = 17.5
secTripletsGsf.OrderedHitsFactoryPSet.SeedingLayers = 'SecLayerTripletsGsf'
secTripletsGsf.RegionFactoryPSet.RegionPSet.ptMin = 0.3


# building 
import RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi
secMeasurementTrackerGsf = RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi.MeasurementTracker.clone()
import TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi
secCkfTrajectoryFilterGsf = TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi.trajectoryFilterESProducer.clone()
# import RecoTracker.CkfPattern.GroupedCkfTrajectoryBuilderESProducer_cfi
# secCkfTrajectoryBuilderGsf = RecoTracker.CkfPattern.GroupedCkfTrajectoryBuilderESProducer_cfi.GroupedCkfTrajectoryBuilder.clone()
# import RecoTracker.CkfPattern.CkfTrackCandidates_cfi
# secTrackCandidatesGsf = RecoTracker.CkfPattern.CkfTrackCandidates_cfi.ckfTrackCandidates.clone()
import RecoEgamma.EgammaElectronProducers.gsfElectronCkfTrackCandidateMaker_cff
secCkfTrajectoryBuilderGsf = RecoEgamma.EgammaElectronProducers.gsfElectronCkfTrackCandidateMaker_cff.TrajectoryBuilderForPixelMatchGsfElectrons.clone()
secTrackCandidatesGsf = RecoEgamma.EgammaElectronProducers.gsfElectronCkfTrackCandidateMaker_cff.egammaCkfTrackCandidates.clone()

secMeasurementTrackerGsf.ComponentName = 'secMeasurementTrackerGsf'
secMeasurementTrackerGsf.pixelClusterProducer = 'secClustersGsf'
secMeasurementTrackerGsf.stripClusterProducer = 'secClustersGsf'

secCkfTrajectoryFilterGsf.ComponentName = 'secCkfTrajectoryFilterGsf'
secCkfTrajectoryFilterGsf.filterPset.maxLostHits = 1
secCkfTrajectoryFilterGsf.filterPset.minimumNumberOfHits = 3
secCkfTrajectoryFilterGsf.filterPset.minPt = 0.3
secCkfTrajectoryBuilderGsf.ComponentName = 'secCkfTrajectoryBuilderGsf'
secCkfTrajectoryBuilderGsf.MeasurementTrackerName = 'secMeasurementTrackerGsf'
secCkfTrajectoryBuilderGsf.trajectoryFilterName = 'secCkfTrajectoryFilterGsf'

secTrackCandidatesGsf.SeedProducer = 'secTripletsGsf'
secTrackCandidatesGsf.TrajectoryBuilder = 'secCkfTrajectoryBuilderGsf'
secTrackCandidatesGsf.doSeedingRegionRebuilding = True
secTrackCandidatesGsf.useHitsSplitting = True


# fitting
import TrackingTools.GsfTracking.GsfElectronFit_cfi
secWithMaterialTracksGsf = TrackingTools.GsfTracking.GsfElectronFit_cfi.GsfGlobalElectronTest.clone()
secWithMaterialTracksGsf.src = 'secTrackCandidatesGsf'
secWithMaterialTracksGsf.clusterRemovalInfo = cms.InputTag('secClustersGsf')
secWithMaterialTracksGsf.TrajectoryInEvent = True

secondStepGsf = cms.Sequence(secClustersGsf*secPixelRecHitsGsf*secStripRecHitsGsf*secTripletsGsf*secTrackCandidatesGsf*secWithMaterialTracksGsf)
