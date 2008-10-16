import FWCore.ParameterSet.Config as cms

from MitEdm.Producers.HitDropper_cfi import *

from MitEdm.Producers.stableParts_cfi import *

from MitEdm.Producers.conversions_cfi import *

#stableParts.iTracks = 'ckfOutInTracksFromConversions'
stableParts.oPid = 11
mvfConversions.rhoMin = 1.0

conversionProducer = cms.Sequence(stableParts*mvfConversions)

conversionFiller = cms.EDAnalyzer("FillMitTree",
    defactive = cms.untracked.bool(False),
    
    StableParts = cms.untracked.PSet(
      active       = cms.untracked.bool(True),
      mitName      = cms.untracked.string('ElectronsStable'),
      edmName      = cms.untracked.string('stableParts'),
      trackMapName = cms.untracked.string('TracksMapName'),
      basePartMap  = cms.untracked.string('ElectronsStableMapName')
    ),

    DecayParts = cms.untracked.PSet(
      active       = cms.untracked.bool(True),
      mitName      = cms.untracked.string('MvfConversions'),
      edmName      = cms.untracked.string('mvfConversions'),
      vertexMap    = cms.untracked.string('PrimaryVertexBSMap'),
      basePartMaps = cms.untracked.vstring('ElectronsStableMapName')
    )
)
