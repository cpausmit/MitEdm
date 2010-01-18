# $Id: conversions_cfi.py,v 1.7 2009/07/12 13:12:06 bendavid Exp $

import FWCore.ParameterSet.Config as cms

mvfConversions = cms.EDProducer("ProducerConversions",
    iStables1        = cms.untracked.string('mergedElectronsStable'),
    iStables2        = cms.untracked.string('mergedElectronsStable'),
    iPVertexes       = cms.untracked.string('offlinePrimaryVerticesWithBS'),
    oPid             = cms.untracked.int32(22),
    usePVertex       = cms.untracked.bool(True),
    convConstraint   = cms.untracked.bool(False),
    convConstraint3D = cms.untracked.bool(True),
    rhoMin           = cms.untracked.double(0.9),
    useRhoMin        = cms.untracked.bool(True),
)

mvfTrackerConversions = mvfConversions.clone(
    iStables1        = cms.untracked.string('generalElectronsStable'),
    iStables2        = cms.untracked.string('generalElectronsStable'),
)