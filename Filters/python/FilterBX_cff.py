# $Id: FilterBX_cff.py,v 1.4 2009/12/11 12:21:51 loizides Exp $

import FWCore.ParameterSet.Config as cms

###### Selection on bunch pattern (collisions, empty bunches, parasitic bunch crossings) ######
collisionBunchCrossings = cms.EDFilter("FilterBX",
    crossings = cms.untracked.vint32(),
    coltype = cms.untracked.int32(1)
)

emptyBunchCrossings = cms.EDFilter("FilterBX",
    crossings = cms.untracked.vint32(),
    coltype = cms.untracked.int32(2)
)

parasiticBunchCrossings = cms.EDFilter("FilterBX",
    crossings = cms.untracked.vint32(1830,1831,1832,1833)
)
