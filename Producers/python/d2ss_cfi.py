import FWCore.ParameterSet.Config as cms

d2ss = cms.EDProducer("ProducerD2SS",
                      iStables1 = cms.untracked.string('stableParts'),
                      iStables2 = cms.untracked.string('stableParts'),
                      oPid = cms.untracked.int32(310)
                      )