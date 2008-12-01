import FWCore.ParameterSet.Config as cms

import RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi
#TRACKER HITS
thPixelRecHitsGsf = RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi.siPixelRecHits.clone()
import RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi
thStripRecHitsGsf = RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi.siStripMatchedRecHits.clone()
thPixelRecHitsGsf.src = 'thClustersGsf'
thStripRecHitsGsf.ClusterProducer = 'thClustersGsf'



import RecoTracker.TkSeedGenerator.GlobalMixedSeeds_cfi
#SEEDS
thPLSeedsGsf = RecoTracker.TkSeedGenerator.GlobalMixedSeeds_cfi.globalMixedSeeds.clone()
import RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi
thPLSeedsGsf.OrderedHitsFactoryPSet.SeedingLayers = 'ThLayerPairsGsf'
thPLSeedsGsf.RegionFactoryPSet.RegionPSet.ptMin = 0.6
thPLSeedsGsf.RegionFactoryPSet.RegionPSet.originHalfLength = 7.0
thPLSeedsGsf.RegionFactoryPSet.RegionPSet.originRadius = 0.7

#TRAJECTORY MEASUREMENT
thMeasurementTrackerGsf = RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi.MeasurementTracker.clone()
import TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi
thMeasurementTrackerGsf.ComponentName = 'thMeasurementTrackerGsf'
thMeasurementTrackerGsf.pixelClusterProducer = 'thClustersGsf'
thMeasurementTrackerGsf.stripClusterProducer = 'thClustersGsf'

#TRAJECTORY FILTER
thCkfTrajectoryFilterGsf = TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi.trajectoryFilterESProducer.clone()
import RecoTracker.CkfPattern.GroupedCkfTrajectoryBuilderESProducer_cfi
thCkfTrajectoryFilterGsf.ComponentName = 'thCkfTrajectoryFilterGsf'
thCkfTrajectoryFilterGsf.filterPset.maxLostHits = 0
thCkfTrajectoryFilterGsf.filterPset.minimumNumberOfHits = 3
thCkfTrajectoryFilterGsf.filterPset.minPt = 0.3

#TRAJECTORY BUILDER
thCkfTrajectoryBuilderGsf = RecoTracker.CkfPattern.GroupedCkfTrajectoryBuilderESProducer_cfi.GroupedCkfTrajectoryBuilder.clone()
import RecoTracker.CkfPattern.CkfTrackCandidates_cfi
thCkfTrajectoryBuilderGsf.ComponentName = 'thCkfTrajectoryBuilderGsf'
thCkfTrajectoryBuilderGsf.MeasurementTrackerName = 'thMeasurementTrackerGsf'
thCkfTrajectoryBuilderGsf.trajectoryFilterName = 'thCkfTrajectoryFilterGsf'


#TRACK CANDIDATES
thTrackCandidatesGsf = RecoTracker.CkfPattern.CkfTrackCandidates_cfi.ckfTrackCandidates.clone()
import RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi
thTrackCandidatesGsf.SeedProducer = 'thPLSeedsGsf'
thTrackCandidatesGsf.TrajectoryBuilder = 'thCkfTrajectoryBuilderGsf'
thTrackCandidatesGsf.doSeedingRegionRebuilding = True
thTrackCandidatesGsf.useHitsSplitting = True

import TrackingTools.GsfTracking.GsfElectronFit_cfi
#TRACKS
thWithMaterialTracksGsf = TrackingTools.GsfTracking.GsfElectronFit_cfi.GsfGlobalElectronTest.clone()
thWithMaterialTracksGsf.src = 'thTrackCandidatesGsf'
thWithMaterialTracksGsf.clusterRemovalInfo = cms.InputTag('thClustersGsf')
thWithMaterialTracksGsf.TrajectoryInEvent = True



#HIT REMOVAL
thClustersGsf = cms.EDFilter("TrackClusterRemover",
    oldClusterRemovalInfo = cms.InputTag("secClustersGsf"),
    trajectories = cms.InputTag("secWithMaterialTracksGsf"),
    pixelClusters = cms.InputTag("secClustersGsf"),
    Common = cms.PSet(
        maxChi2 = cms.double(30.0)
    ),
    stripClusters = cms.InputTag("secClustersGsf")
)

#SEEDING LAYERS
thlayerpairsGsf = cms.ESProducer("MixedLayerPairsESProducer",
    ComponentName = cms.string('ThLayerPairsGsf'),
    layerList = cms.vstring('BPix1+BPix2', 
        'BPix2+BPix3',
        'BPix1+FPix1_pos',
        'BPix1+FPix1_neg',
        'FPix1_pos+FPix2_pos',
        'FPix1_neg+FPix2_neg',
        'FPix2_pos+TEC2_pos',
        'FPix2_neg+TEC2_neg'),
    TEC = cms.PSet(
        matchedRecHits = cms.InputTag("thStripRecHitsGsf","matchedRecHit"),
        useRingSlector = cms.untracked.bool(True),
        TTRHBuilder = cms.string('WithTrackAngle'),
        minRing = cms.int32(1),
        maxRing = cms.int32(2)
    ),
    BPix = cms.PSet(
        useErrorsFromParam = cms.untracked.bool(True),
        hitErrorRPhi = cms.double(0.0027),
        TTRHBuilder = cms.string('TTRHBuilderWithoutAngle4MixedPairs'),
        HitProducer = cms.string('thPixelRecHitsGsf'),
        hitErrorRZ = cms.double(0.006)
    ),
    FPix = cms.PSet(
        useErrorsFromParam = cms.untracked.bool(True),
        hitErrorRPhi = cms.double(0.0051),
        TTRHBuilder = cms.string('TTRHBuilderWithoutAngle4MixedPairs'),
        HitProducer = cms.string('thPixelRecHitsGsf'),
        hitErrorRZ = cms.double(0.0036)
    )
)

##from RecoTracker.IterativeTracking.ThVxFilter_cff import *
#import RecoTracker.FinalTrackSelectors.selectHighPurity_cfi
#thStepVtx = RecoTracker.FinalTrackSelectors.selectHighPurity_cfi.selectHighPurity.clone()
#thStepVtx.src = 'thWithMaterialTracks'
#thStepVtx.copyTrajectories = True
#thStepVtx.chi2n_par = 0.9
#thStepVtx.res_par = ( 0.003, 0.001 )
#thStepVtx.d0_par1 = ( 0.9, 3.0 )
#thStepVtx.dz_par1 = ( 0.9, 3.0 )
#thStepVtx.d0_par2 = ( 1.0, 3.0 )
#thStepVtx.dz_par2 = ( 1.0, 3.0 )

#thStepTrk = RecoTracker.FinalTrackSelectors.selectHighPurity_cfi.selectHighPurity.clone()
#thStepTrk.src = 'thWithMaterialTracks'
#thStepTrk.copyTrajectories = True
#thStepTrk.chi2n_par = 0.5
#thStepTrk.res_par = ( 0.003, 0.001 )
#thStepTrk.minNumberLayers = 5
#thStepTrk.d0_par1 = ( 1.0, 4.0 )
#thStepTrk.dz_par1 = ( 1.0, 4.0 )
#thStepTrk.d0_par2 = ( 1.0, 4.0 )
#thStepTrk.dz_par2 = ( 1.0, 4.0 )
#import RecoTracker.FinalTrackSelectors.ctfrsTrackListMerger_cfi

#thStep = RecoTracker.FinalTrackSelectors.ctfrsTrackListMerger_cfi.ctfrsTrackListMerger.clone()
#thStep.TrackProducer1 = 'thStepVtx'
#thStep.TrackProducer2 = 'thStepTrk'

thirdStepGsf = cms.Sequence(thClustersGsf*
                         thPixelRecHitsGsf*thStripRecHitsGsf*
                         thPLSeedsGsf*
                         thTrackCandidatesGsf*
                         thWithMaterialTracksGsf
                         )
