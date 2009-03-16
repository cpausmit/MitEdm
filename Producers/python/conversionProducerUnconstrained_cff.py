# $Id: vProducer_cff.py,v 1.7 2009/03/13 21:00:54 loizides Exp $

import FWCore.ParameterSet.Config as cms

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *

from MitEdm.Producers.HitDropper_cfi import *

from MitEdm.Producers.conversions_cfi import *

mvfConversionsUnconstrained = mvfConversions.clone()

mvfConversionsUnconstrained.rhoMin = 1.0
mvfConversionsUnconstrained.convConstraint3D = False

conversionProducerUnconstrained = cms.Sequence(mvfConversionsUnconstrained)

conversionFillerUnconstrained = cms.EDAnalyzer("FillMitTree",
    defactive = cms.untracked.bool(False),

    fillerOrder = cms.untracked.vstring('DecayParts'),

    DecayParts = cms.untracked.PSet(
      active       = cms.untracked.bool(True),
      mitName      = cms.untracked.string('MvfConversionsUnconstrained'),
      edmName      = cms.untracked.string('mvfConversionsUnconstrained'),
      vertexMap    = cms.untracked.string('PrimaryVertexBSMap'),
      basePartMaps = cms.untracked.vstring('ElectronsStableMapName'),
      fillerType   = cms.untracked.string('FillerDecayParts')
    )
)
