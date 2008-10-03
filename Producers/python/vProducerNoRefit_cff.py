# $Id:$

import FWCore.ParameterSet.Config as cms

import MitEdm.Producers.stableParts_cfi

PisStable = MitEdm.Producers.stableParts_cfi.stableParts.clone()

ProtonsStable = MitEdm.Producers.stableParts_cfi.stableParts.clone()
ProtonsStable.oPid = cms.untracked.int32(2212)

import MitEdm.Producers.v2ss_cfi
Ksh2PiPi = MitEdm.Producers.v2ss_cfi.v2ss.clone()

Lambda2ProtPi = MitEdm.Producers.v2ss_cfi.v2ss.clone()
Lambda2ProtPi.iStables2 = cms.untracked.string('ProtonsStable')
Lambda2ProtPi.oPid= cms.untracked.int32(3122)
Lambda2ProtPi.minMass = cms.untracked.double(1.0)
Lambda2ProtPi.maxMass = cms.untracked.double(1.3)


vProducer = cms.Sequence(PisStable*ProtonsStable*Ksh2PiPi*Lambda2ProtPi)
