# $Id: stableParts_cfi.py,v 1.2 2009/03/16 08:30:51 loizides Exp $

import FWCore.ParameterSet.Config as cms

stableParts = cms.EDProducer("ProducerStable",
    iTracks = cms.untracked.string('generalTracks'),
    oPid    = cms.untracked.int32(211)
)
