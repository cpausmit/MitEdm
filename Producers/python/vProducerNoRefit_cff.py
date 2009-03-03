# $Id: vProducerNoRefit_cff.py,v 1.3 2008/10/03 23:53:51 loizides Exp $

import FWCore.ParameterSet.Config as cms

import MitEdm.Producers.stableParts_cfi

PisStable = MitEdm.Producers.stableParts_cfi.stableParts.clone()

ProtonsStable = MitEdm.Producers.stableParts_cfi.stableParts.clone()
ProtonsStable.oPid = cms.untracked.int32(2212)

import MitEdm.Producers.v2ss_cfi
Ksh2PiPi = MitEdm.Producers.v2ss_cfi.v2ss.clone()
Ksh2PiPi.useHitDropper = cms.untracked.bool(False)

Lambda2ProtPi = MitEdm.Producers.v2ss_cfi.v2ss.clone()
Lambda2ProtPi.iStables2 = cms.untracked.string('ProtonsStable')
Lambda2ProtPi.oPid= cms.untracked.int32(3122)
Lambda2ProtPi.minMass = cms.untracked.double(1.0)
Lambda2ProtPi.maxMass = cms.untracked.double(1.3)
Lambda2ProtPi.useHitDropper = cms.untracked.bool(False)

FillKsh = cms.EDAnalyzer("FillMitTree",
    defactive = cms.untracked.bool(False),
    StableParts = cms.untracked.PSet(
        active       = cms.untracked.bool(True),
        mitName      = cms.untracked.string('PisStable'),
        edmName      = cms.untracked.string('PisStable'),
        trackMapName = cms.untracked.string('TracksMapName'),
        basePartMap  = cms.untracked.string('PisStableMapName')
    ),

    DecayParts = cms.untracked.PSet(
        active = cms.untracked.bool(True),
        mitName = cms.untracked.string('Ksh2PiPi'),
        edmName = cms.untracked.string('Ksh2PiPi'),
        basePartMaps = cms.untracked.vstring('PisStableMapName')
    )
)

FillLambda = cms.EDAnalyzer("FillMitTree",
    defactive = cms.untracked.bool(False),

    StableParts = cms.untracked.PSet(
        active      = cms.untracked.bool(True),
        mitName     = cms.untracked.string('ProtonsStable'),
        edmName     = cms.untracked.string('ProtonsStable'),
        trackMapName= cms.untracked.string('TracksMapName'),
        basePartMap = cms.untracked.string('ProtonsStableMapName')
    ),

    DecayParts = cms.untracked.PSet(
        active       = cms.untracked.bool(True),
        mitName      = cms.untracked.string('Lambda2ProtPi'),
        edmName      = cms.untracked.string('Lambda2ProtPi'),
        basePartMaps  = cms.untracked.vstring('PisStableMapName','ProtonsStableMapName')
    )
)
      

vProducer = cms.Sequence(PisStable*ProtonsStable*Ksh2PiPi*Lambda2ProtPi)
vFiller = cms.Sequence(FillKsh*FillLambda)