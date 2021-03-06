# $Id:$

import FWCore.ParameterSet.Config as cms

#
# ctf tracks parameter-set entries for module
#
# SimpleTrackListMerger
#
# located in
#
# RecoTracker/FinalTrackSelectors
#
# 
# sequence dependency:
#
# - ctfWithMaterialTracks: include "RecoTracker/TrackProducer/data/CTFFinalFitWithMaterial.cff"
# - rsWithMaterialTracks: include "RecoTracker/TrackProducer/data/RSFinalFitWithMaterial.cff"
#
#
# service dependency:
#
# - geometry:          include "Geometry/CMSCommonData/data/cmsIdealGeometryXML.cfi"
# - tracker geometry:  include "Geometry/TrackerGeometryBuilder/data/trackerGeometry.cfi"
# - tracker numbering: include "Geometry/TrackerNumberingBuilder/data/trackerNumberingGeometry.cfi"
#
# function:
#
# cleans and merges ctf and rs Track lists and put new list back in Event
ctfrsTrackListMergerGsf = cms.EDFilter("SimpleTrackListMergerGsf",
    # minimum shared fraction to be called duplicate
    ShareFrac = cms.double(0.66),
    # minimum pT in GeV/c
    MinPT = cms.double(0.05),
    # minimum difference in rechit position in cm
    # negative Epsilon uses sharedInput for comparison
    Epsilon = cms.double(-0.001),
    # maximum chisq/dof
    MaxNormalizedChisq = cms.double(1000.0),
    # minimum number of RecHits used in fit
    MinFound = cms.int32(3),
    TrackProducer2 = cms.string(''),
    TrackProducer1 = cms.string(''),
    # set new quality for confirmed tracks
    promoteTrackQuality = cms.bool(False),
    newQuality = cms.string('confirmed')
)
