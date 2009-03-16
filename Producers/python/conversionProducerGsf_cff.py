# $Id: vProducer_cff.py,v 1.7 2009/03/13 21:00:54 loizides Exp $

import FWCore.ParameterSet.Config as cms

from MitEdm.Producers.HitDropper_cfi import *

import MitEdm.Producers.stableParts_cfi
stablePartsGsf = MitEdm.Producers.stableParts_cfi.stableParts.clone()

from MitEdm.Producers.conversions_cfi import *
mvfConversionsGsf = MitEdm.Producers.conversions_cfi.mvfConversions.clone()

#stableParts.iTracks = 'ckfOutInTracksFromConversions'
stablePartsGsf.iTracks = 'generalTracksGsf'
stablePartsGsf.oPid = 11
mvfConversionsGsf.rhoMin = 0.9
mvfConversionsGsf.iStables1 = 'stablePartsGsf'
mvfConversionsGsf.iStables2 = 'stablePartsGsf'

conversionProducerGsf = cms.Sequence(stablePartsGsf*mvfConversionsGsf)

conversionFillerGsf = cms.EDAnalyzer("FillMitTree",
    defactive = cms.untracked.bool(False),

    fillerOrder = cms.untracked.vstring('StableParts',
                                        'DecayParts'),
    
    StableParts = cms.untracked.PSet(
      active       = cms.untracked.bool(True),
      mitName      = cms.untracked.string('ElectronsStableGsf'),
      edmName      = cms.untracked.string('stablePartsGsf'),
      trackMapName = cms.untracked.string('GeneralGsfTracksMapName'),
      basePartMap  = cms.untracked.string('ElectronsStableGsfMapName'),
      fillerType   = cms.untracked.string('FillerStableParts')
    ),

    DecayParts = cms.untracked.PSet(
      active       = cms.untracked.bool(True),
      mitName      = cms.untracked.string('MvfConversionsGsf'),
      edmName      = cms.untracked.string('mvfConversionsGsf'),
      vertexMap    = cms.untracked.string('PrimaryVertexBSMap'),
      basePartMaps = cms.untracked.vstring('ElectronsStableGsfMapName'),
      fillerType   = cms.untracked.string('FillerDecayParts')
    )
)
