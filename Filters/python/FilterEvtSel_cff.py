# $Id:$

import FWCore.ParameterSet.Config as cms

###### Filter on event selection object (HF energy/timing, pixel hits, cluster shapes ######
looseEvtSelFilter = cms.EDFilter("FilterEvtSel",
    clusterPars = cms.untracked.vdouble(0.5,0.0045),
    minHfEnergy = cms.untracked.double(0),
    maxHfTimeDiff = cms.untracked.double(0),
    srcEvtSel = cms.untracked.string("evtSelData")
)
