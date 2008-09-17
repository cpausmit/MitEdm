import FWCore.ParameterSet.Config as cms

mvfConversions = cms.EDProducer("ProducerConversions",
                      iStables1 = cms.untracked.string('stableParts'),
                      iStables2 = cms.untracked.string('stableParts'),
                      oPid = cms.untracked.int32(22),
                      convConstraint3D = cms.untracked.bool(True),
                      rhoMin = cms.untracked.double(0.0)
                      )
