# $Id: conversionElectronsStable_cfi.py,v 1.2 2009/07/12 13:12:06 bendavid Exp $

import FWCore.ParameterSet.Config as cms

from RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff import *

siPixelRecHits = cms.EDFilter("SiPixelRecHitConverter",
    src = cms.InputTag("siPixelClusters"),
    CPE = cms.string('PixelCPETemplateReco'),
    VerboseLevel = cms.untracked.int32(0),
)
