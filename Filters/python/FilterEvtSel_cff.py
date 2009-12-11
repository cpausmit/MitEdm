# $Id: FilterEvtSel_cff.py,v 1.5 2009/12/10 15:31:04 loizides Exp $

import FWCore.ParameterSet.Config as cms

###### Filter on event selection object (HF energy/timing, pixel hits, cluster shapes ######
looseEvtSelFilter = cms.EDFilter("FilterEvtSel",
    clusterPars   = cms.untracked.vdouble(0.5,0.0045),
    minHfEnergy   = cms.untracked.double(0),
    maxHfTimeDiff = cms.untracked.double(0),
    nhitsmax      = cms.untracked.int32(0),
    srcEvtSel     = cms.untracked.string("evtSelData")
)
