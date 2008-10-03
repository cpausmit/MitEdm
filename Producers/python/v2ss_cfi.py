# $Id:$

import FWCore.ParameterSet.Config as cms

v2ss = cms.EDProducer("ProducerV2SS",
                      iStables1 = cms.untracked.string('PisStable'),
                      iStables2 = cms.untracked.string('PisStable'),
                      oPid = cms.untracked.int32(310),
                      minRadius = cms.untracked.double(0.1),
                      minMass = cms.untracked.double(0.350),
                      maxMass = cms.untracked.double(0.650),
                      maxZDistance = cms.untracked.double(1.0)
                      )
