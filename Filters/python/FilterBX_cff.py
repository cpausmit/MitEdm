# $Id:$

import FWCore.ParameterSet.Config as cms

###### Selection on bunch pattern (collisions, empty bunches, parasitic bunch crossings) ######
collisionBunchCrossings = cms.EDFilter("FilterBX",
    crossings = cms.untracked.vint32(51,2724)
)

emptyBunchCrossings = cms.EDFilter("FilterBX",
    crossings = cms.untracked.vint32(3170,2276)
)

parasiticBunchCrossings = cms.EDFilter("FilterBX",
    crossings = cms.untracked.vint32(1830,1831,1832,1833)
)

