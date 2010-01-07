# $Id: FilterEvtSel_cff.py,v 1.7 2009/12/12 13:37:30 edwenger Exp $

import FWCore.ParameterSet.Config as cms

###### Filter on event selection object (HF energy/timing, pixel hits, cluster shapes ######
verylooseEvtSelFilter = cms.EDFilter("FilterEvtSel",
    clusterPars   = cms.untracked.vdouble(),
    minHfEnergy   = cms.untracked.double(0),
    maxHfTimeDiff = cms.untracked.double(0),
    nhitsmax      = cms.untracked.int32(1500),
    nhitsTrunc    = cms.untracked.int32(0),
    nHfHits       = cms.untracked.int32(0),
    nHfTowers     = cms.untracked.int32(0),
    srcEvtSel     = cms.untracked.string("evtSelData")
)

looseEvtSelFilter = cms.EDFilter("FilterEvtSel",
    clusterPars   = cms.untracked.vdouble(0.5,0.0045),
    minHfEnergy   = cms.untracked.double(0),
    maxHfTimeDiff = cms.untracked.double(0),
    nhitsmax      = cms.untracked.int32(0),
    nhitsTrunc    = cms.untracked.int32(150),
    nHfHits       = cms.untracked.int32(0),
    nHfTowers     = cms.untracked.int32(0),
    srcEvtSel     = cms.untracked.string("evtSelData")
)

hfEvtSelFilter = cms.EDFilter("FilterEvtSel",
    clusterPars   = cms.untracked.vdouble(0.5,0.0045),
    minHfEnergy   = cms.untracked.double(0),
    maxHfTimeDiff = cms.untracked.double(0),
    nhitsmax      = cms.untracked.int32(0),
    nhitsTrunc    = cms.untracked.int32(150),
    nHfHits       = cms.untracked.int32(0),
    nHfTowers     = cms.untracked.int32(1),
    srcEvtSel     = cms.untracked.string("evtSelData")
)
