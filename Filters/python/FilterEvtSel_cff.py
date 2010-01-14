# $Id: FilterEvtSel_cff.py,v 1.8 2010/01/07 17:07:54 loizides Exp $

import FWCore.ParameterSet.Config as cms

###### Filter on event selection object (HF energy/timing, pixel hits, cluster shapes) ######
verylooseEvtSelFilter = cms.EDFilter("FilterEvtSel",
    clusterPars   = cms.untracked.vdouble(),
    minHfEnergy   = cms.untracked.double(0),
    maxHfTimeDiff = cms.untracked.double(0),
    nhitsmax      = cms.untracked.int32(1500),
    nhitsTrunc    = cms.untracked.int32(0),
    clusterTrunc  = cms.untracked.double(0.0),                             
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
    clusterTrunc  = cms.untracked.double(0.0),                             
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
    clusterTrunc  = cms.untracked.double(0.0),                             
    nHfHits       = cms.untracked.int32(0),
    nHfTowers     = cms.untracked.int32(1),
    srcEvtSel     = cms.untracked.string("evtSelData")
)

## looser cuts being tested ##
#    clusterPars   = cms.untracked.vdouble(0.0,0.0045),
#    clusterTrunc  = cms.untracked.double(2.0),                             
