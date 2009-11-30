# $Id: conversions_cfi.py,v 1.7 2009/07/12 13:12:06 bendavid Exp $

import FWCore.ParameterSet.Config as cms

trackletVertices = cms.EDFilter("VertexZProducer",
    pixLayerCombinations = cms.untracked.int32(12),
    doClusVertex         = cms.untracked.bool(False),
)                                

clusterVertices = cms.EDFilter("VertexZProducer",
    pixLayerCombinations = cms.untracked.int32(12),
    doClusVertex         = cms.untracked.bool(True),
)                                
