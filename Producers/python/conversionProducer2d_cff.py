# $Id: conversionProducer2d_cff.py,v 1.4 2009/03/17 14:26:24 loizides Exp $

import FWCore.ParameterSet.Config as cms

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *

from MitEdm.Producers.HitDropper_cfi import *

from MitEdm.Producers.conversions_cfi import *

mvfConversions2d = mvfConversions.clone()

mvfConversions2d.rhoMin = 0.9
mvfConversions2d.convConstraint3D = False
mvfConversions2d.convConstraint = True

conversionProducer2d = cms.Sequence(mvfConversions2d)

conversionFiller2d = cms.EDAnalyzer("FillMitTree",
    fillers = cms.untracked.vstring('DecayParts'),

    DecayParts = cms.untracked.PSet(
      active       = cms.untracked.bool(True),
      mitName      = cms.untracked.string('MvfConversions2d'),
      edmName      = cms.untracked.string('mvfConversions2d'),
      vertexMap    = cms.untracked.string('PrimaryVertexBSMap'),
      basePartMaps = cms.untracked.vstring('ElectronsStableMapName'),
      fillerType   = cms.untracked.string('FillerDecayParts')
    )
)
