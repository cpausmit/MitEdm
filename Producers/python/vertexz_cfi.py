# $Id: vertexz_cfi.py,v 1.1 2009/11/30 10:11:41 loizides Exp $

import FWCore.ParameterSet.Config as cms

trackletVertices = cms.EDProducer("VertexZProducer",
    pixLayerCombinations = cms.untracked.int32(12),
    doClusVertex         = cms.untracked.bool(False),
)                                

clusterVertices = cms.EDProducer("VertexZProducer",
    pixLayerCombinations = cms.untracked.int32(12),
    doClusVertex         = cms.untracked.bool(True),
)                                
