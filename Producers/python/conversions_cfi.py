# $Id: conversions_cfi.py,v 1.10 2010/06/09 00:06:20 bendavid Exp $

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
mvfConversionRemoval = mvfConversions.clone(
    iStables1        = cms.untracked.string('gsfElectronsStable'),
    iStables2        = cms.untracked.string('mergedElectronsStable'),
    useRhoMin        = cms.untracked.bool(False),
    applyChargeConstraint = cms.untracked.bool(True),
)

kinematicConversions = cms.EDProducer("ProducerConversionsKinematic",
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
