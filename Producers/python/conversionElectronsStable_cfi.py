# $Id: stableParts_cfi.py,v 1.3 2009/03/21 12:46:57 loizides Exp $

import FWCore.ParameterSet.Config as cms

generalElectronsStable = cms.EDProducer("ProducerStable",
    iTracks = cms.untracked.string('generalTracks'),
    oPid    = cms.untracked.int32(11)
)

ckfInOutElectronsStable = cms.EDProducer("ProducerStable",
    iTracks = cms.untracked.string('ckfInOutTracksFromConversions'),
    oPid    = cms.untracked.int32(11)
)

ckfOutInElectronsStable = cms.EDProducer("ProducerStable",
    iTracks = cms.untracked.string('ckfOutInTracksFromConversions'),
    oPid    = cms.untracked.int32(11)
)

conversionElectronsStable = cms.Sequence(generalElectronsStable*
                                         ckfInOutElectronsStable*
                                         ckfOutInElectronsStable)