# $Id: conversionProducer_cff.py,v 1.6 2009/03/16 08:30:51 loizides Exp $

import FWCore.ParameterSet.Config as cms

from MitEdm.Producers.HitDropper_cfi import *

from MitEdm.Producers.stableParts_cfi import *

from MitEdm.Producers.conversions_cfi import *

#stableParts.iTracks = 'ckfOutInTracksFromConversions'
stableParts.oPid = 11
mvfConversions.rhoMin = 0.9

conversionProducer = cms.Sequence(stableParts*mvfConversions)

conversionFiller = cms.EDAnalyzer("FillMitTree",
    fillerOrder = cms.untracked.vstring('StableParts',
                                        'DecayParts'),

    StableParts = cms.untracked.PSet(
      active       = cms.untracked.bool(True),
      mitName      = cms.untracked.string('ElectronsStable'),
      edmName      = cms.untracked.string('stableParts'),
      trackMapName = cms.untracked.string('TracksMapName'),
      basePartMap  = cms.untracked.string('ElectronsStableMapName'),
      fillerType   = cms.untracked.string('FillerStableParts')
    ),

    DecayParts = cms.untracked.PSet(
      active       = cms.untracked.bool(True),
      mitName      = cms.untracked.string('MvfConversions'),
      edmName      = cms.untracked.string('mvfConversions'),
      vertexMap    = cms.untracked.string('PrimaryVertexBSMap'),
      basePartMaps = cms.untracked.vstring('ElectronsStableMapName'),
      fillerType   = cms.untracked.string('FillerDecayParts')
    )
)
