# $Id: FilterEvtSel_cff.py,v 1.6 2009/12/11 11:22:31 loizides Exp $

import FWCore.ParameterSet.Config as cms

###### Filter on event selection object (HF energy/timing, pixel hits, cluster shapes ######
looseEvtSelFilter = cms.EDFilter("FilterEvtSel",
    clusterPars   = cms.untracked.vdouble(0.5,0.0045),
    minHfEnergy   = cms.untracked.double(0),
    maxHfTimeDiff = cms.untracked.double(0),
    nhitsmax      = cms.untracked.int32(0),
    nhitsTrunc    = cms.untracked.int32(150),
    srcEvtSel     = cms.untracked.string("evtSelData")
)
