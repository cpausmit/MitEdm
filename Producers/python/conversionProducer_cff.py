import FWCore.ParameterSet.Config as cms

from MitEdm.Producers.stableParts_cfi import *

from MitEdm.Producers.conversions_cfi import *

#stableParts.iTracks = 'ckfOutInTracksFromConversions'
stableParts.oPid = 11
mvfConversions.rhoMin = 1.0

conversionProducer = cms.Sequence(stableParts*mvfConversions)
