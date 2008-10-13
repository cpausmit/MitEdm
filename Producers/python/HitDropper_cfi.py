# $Id: conversions_cfi.py,v 1.3 2008/10/03 23:53:51 loizides Exp $

import FWCore.ParameterSet.Config as cms

HitDropperESProducer = cms.ESProducer("HitDropperESProducer",
     ComponentName = cms.string('HitDropper')
)