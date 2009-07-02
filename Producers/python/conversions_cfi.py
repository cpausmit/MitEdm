# $Id: conversions_cfi.py,v 1.5 2009/03/21 12:46:57 loizides Exp $

import FWCore.ParameterSet.Config as cms

mvfConversions = cms.EDProducer("ProducerConversions",
    iStables1        = cms.untracked.string('generalElectronsStable'),
    iStables2        = cms.untracked.string('generalElectronsStable'),
    iPVertexes       = cms.untracked.string('offlinePrimaryVerticesWithBS'),
    oPid             = cms.untracked.int32(22),
    usePVertex       = cms.untracked.bool(True),
    convConstraint   = cms.untracked.bool(False),
    convConstraint3D = cms.untracked.bool(True),
    rhoMin           = cms.untracked.double(0.0)
)
