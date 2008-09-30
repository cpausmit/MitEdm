import FWCore.ParameterSet.Config as cms

import MitEdm.Producers.stableParts_cfi

from Configuration.StandardSequences.Services_cff import *
from Configuration.StandardSequences.Geometry_cff import *
from Configuration.StandardSequences.MagneticField_cff import *
from Configuration.StandardSequences.FrontierConditions_GlobalTag_cff import *
GlobalTag.globaltag = "IDEAL_V9::All"

PisStable = MitEdm.Producers.stableParts_cfi.stableParts.clone()

from RecoTracker.TrackProducer.RefitterWithMaterial_cff import *

ProtonPropagator = TrackingTools.MaterialEffects.RungeKuttaTrackerPropagator_cfi.RungeKuttaTrackerPropagator.clone()
ProtonPropagator.ComponentName = cms.string('ProtonPropagator')
ProtonPropagator.Mass = cms.double(0.93827)

TrackRefitter.Propagator = cms.string('ProtonPropagator')

ProtonsStable = MitEdm.Producers.stableParts_cfi.stableParts.clone()
ProtonsStable.iTracks = cms.untracked.string('TrackRefitter')
ProtonsStable.oPid = cms.untracked.int32(2212)

import MitEdm.Producers.v2ss_cfi
Ksh2PiPi = MitEdm.Producers.v2ss_cfi.v2ss.clone()

Lambda2ProtPi = MitEdm.Producers.v2ss_cfi.v2ss.clone()
Lambda2ProtPi.iStables2 = cms.untracked.string('ProtonsStable')
Lambda2ProtPi.oPid= cms.untracked.int32(3122)
Lambda2ProtPi.minMass = cms.untracked.double(1.0)
Lambda2ProtPi.maxMass = cms.untracked.double(1.3)

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

    GeneralTracks = cms.untracked.PSet(
        active     = cms.untracked.bool(True),
        mitName = cms.untracked.string('ProtonRefitTracks'),
        edmName = cms.untracked.string('TrackRefitter'),
        simMapName = cms.untracked.string('SimMap'),
        trackMapName = cms.untracked.string('ProtTracksMapName')
    ),
    StableParts = cms.untracked.PSet(
        active      = cms.untracked.bool(True),
        mitName     = cms.untracked.string('ProtonsStable'),
        edmName     = cms.untracked.string('ProtonsStable'),
        trackMapName= cms.untracked.string('ProtTracksMapName'),
        basePartMap = cms.untracked.string('ProtonsStableMapName')
    ),

    DecayParts = cms.untracked.PSet(
        active       = cms.untracked.bool(True),
        mitName      = cms.untracked.string('Lambda2ProtPi'),
        edmName      = cms.untracked.string('Lambda2ProtPi'),
        basePartMaps  = cms.untracked.vstring('PisStableMapName','ProtonsStableMapName')
    )
)
                            

#Sequence to PRODUCE the particles
vProducer = cms.Sequence(PisStable*TrackRefitter*ProtonsStable*Ksh2PiPi*Lambda2ProtPi)

#Sequence to fill objects - run after standard MitTreeFiller
vFiller = cms.Sequence(FillKsh*FillLambda)
