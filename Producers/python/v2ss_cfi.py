# $Id: v2ss_cfi.py,v 1.2 2008/10/03 23:53:51 loizides Exp $

import FWCore.ParameterSet.Config as cms

v2ss = cms.EDProducer("ProducerV2SS",
                      iStables1 = cms.untracked.string('PisStable'),
                      iStables2 = cms.untracked.string('PisStable'),
                      oPid = cms.untracked.int32(310),
                      minRadius = cms.untracked.double(0.3),
                      minMass = cms.untracked.double(0.380),
                      maxMass = cms.untracked.double(0.620),
                      maxZDistance = cms.untracked.double(0.5)
                      )
