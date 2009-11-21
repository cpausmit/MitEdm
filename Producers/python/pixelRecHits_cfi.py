# $Id: pixelRecHits_cfi.py,v 1.1 2009/11/05 09:33:28 loizides Exp $

import FWCore.ParameterSet.Config as cms

from RecoLocalTracker.SiPixelRecHits.PixelCPEESProducers_cff import *

siPixelRecHits = cms.EDFilter("SiPixelRecHitConverter",
    src = cms.InputTag("siPixelClusters"),
#    CPE = cms.string('PixelCPETemplateReco'),
    VerboseLevel = cms.untracked.int32(0),
)
