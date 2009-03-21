# $Id: d2ss_cfi.py,v 1.2 2009/03/16 08:30:51 loizides Exp $

import FWCore.ParameterSet.Config as cms

d2ss = cms.EDProducer("ProducerD2SS",
    iStables1 = cms.untracked.string('stableParts'),
    iStables2 = cms.untracked.string('stableParts'),
    oPid      = cms.untracked.int32(310)
)
