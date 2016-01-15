import FWCore.ParameterSet.Config as cms

stablePartMerger = cms.EDProducer(
  "SimpleTrackListMergerTransient",
  # minimum shared fraction to be called duplicate
  ShareFrac = cms.double(0.66),
  # minimum pT in GeV/c
  MinPT = cms.double(0.0),
  # minimum difference in rechit position in cm negative Epsilon uses sharedInput for comparison
  Epsilon = cms.double(-0.001),
  # maximum chisq/dof
  MaxNormalizedChisq = cms.double(1e9),
  # minimum number of RecHits used in fit
  MinFound = cms.uint32(0),
  StableProducer1 = cms.string(''),
  StableProducer2 = cms.string(''),
  # set new quality for confirmed tracks
  promoteTrackQuality = cms.bool(False),
  newQuality = cms.string('confirmed'),
  copyExtras = cms.untracked.bool(False),
  removeDuplicates = cms.bool(True),
  preferCollection = cms.uint32(0),
)
