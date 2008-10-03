# $Id:$

import FWCore.ParameterSet.Config as cms

mvfConversions = cms.EDProducer("ProducerConversions",
                      iStables1 = cms.untracked.string('stableParts'),
                      iStables2 = cms.untracked.string('stableParts'),
                      iPVertexes = cms.untracked.string('offlinePrimaryVerticesWithBS'),
                      oPid = cms.untracked.int32(22),
                      usePVertex = cms.untracked.bool(True),
                      convConstraint3D = cms.untracked.bool(True),
                      rhoMin = cms.untracked.double(0.0)
                      )
