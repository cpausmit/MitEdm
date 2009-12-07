# $Id: conversions_cfi.py,v 1.7 2009/07/12 13:12:06 bendavid Exp $

import FWCore.ParameterSet.Config as cms

evtSelData = cms.EDProducer("ProducerEvtSelData",
    hfRecHits   = cms.untracked.string('hfreco'),
    hbheRecHits = cms.untracked.string('hbhereco'),
    castorRecHits = cms.untracked.string('castorreco'),
    zdcRecHits = cms.untracked.string('zdcreco'),
)
