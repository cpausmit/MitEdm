# $Id: evtSelData_cfi.py,v 1.1 2009/12/07 22:53:00 loizides Exp $

import FWCore.ParameterSet.Config as cms

evtSelData = cms.EDProducer("ProducerEvtSelData",
    hfRecHits   = cms.untracked.string('hfreco'),
    hbheRecHits = cms.untracked.string('hbhereco'),
    castorRecHits = cms.untracked.string('castorreco'),
    zdcRecHits = cms.untracked.string('zdcreco'),
    pixelRecHits = cms.untracked.string('siPixelRecHits')
)
