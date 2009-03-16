# $Id: vProducer_cff.py,v 1.7 2009/03/13 21:00:54 loizides Exp $

import FWCore.ParameterSet.Config as cms

stableParts = cms.EDProducer("ProducerStable",
                           iTracks = cms.untracked.string('generalTracks'),
                           oPid = cms.untracked.int32(211)
                           )
