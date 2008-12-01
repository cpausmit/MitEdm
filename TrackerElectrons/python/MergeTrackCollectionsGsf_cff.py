import FWCore.ParameterSet.Config as cms

import MitEdm.TrackerElectrons.ctfrsTrackListMergerGsf_cfi
# Track filtering and quality.
#   input:    firstStepTracksWithQuality,secStep,thStep,pixellessStep
#   output:   generalTracks
#   sequence: trackCollectionMerging

#
merge2nd3rdTracksGsf = MitEdm.TrackerElectrons.ctfrsTrackListMergerGsf_cfi.ctfrsTrackListMergerGsf.clone()
merge2nd3rdTracksGsf.TrackProducer1 = 'secWithMaterialTracksGsf'
merge2nd3rdTracksGsf.TrackProducer2 = 'thWithMaterialTracksGsf'
merge2nd3rdTracksGsf.promoteTrackQuality = True

iterTracksGsf = MitEdm.TrackerElectrons.ctfrsTrackListMergerGsf_cfi.ctfrsTrackListMergerGsf.clone()
iterTracksGsf.TrackProducer1 = 'merge2nd3rdTracksGsf'
iterTracksGsf.TrackProducer2 = 'fourthWithMaterialTracksGsf'
iterTracksGsf.promoteTrackQuality = True

generalTracksGsf = MitEdm.TrackerElectrons.ctfrsTrackListMergerGsf_cfi.ctfrsTrackListMergerGsf.clone()
generalTracksGsf.TrackProducer1 = 'preFilterFirstStepTracksGsf'
generalTracksGsf.TrackProducer2 = 'iterTracksGsf'
generalTracksGsf.promoteTrackQuality = True

#generalTracks = RecoTracker.FinalTrackSelectors.ctfrsTrackListMerger_cfi.ctfrsTrackListMerger.clone()
#generalTracks.TrackProducer1 = 'firstStepTracksWithQuality'
#generalTracks.TrackProducer2 = 'merge2nd3rdTracks'
#generalTracks.promoteTrackQuality = True

trackCollectionMergingGsf = cms.Sequence(merge2nd3rdTracksGsf*
                                      iterTracksGsf*
                                      generalTracksGsf)
