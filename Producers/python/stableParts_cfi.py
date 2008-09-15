import FWCore.ParameterSet.Config as cms

stableParts = cms.EDProducer("ProducerStable",
                           iTracks = cms.untracked.string('generalTracks'),
                           oPid = cms.untracked.int32(211)
                           )
