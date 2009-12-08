import FWCore.ParameterSet.Config as cms

###### Filter on event selection object (HF energy/timing, pixel hits, cluster shapes ######

looseEvtSelFilter = cms.EDFilter("FilterEvtSel",
                                 clusterPars = cms.untracked.vdouble(0.0,0.008),
                                 minHfEnergy = cms.untracked.double(10.0),
                                 maxHfTimeDiff = cms.untracked.double(100.0),
                                 srcEvtSel = cms.untracked.string("evtSelData")
                                 )
