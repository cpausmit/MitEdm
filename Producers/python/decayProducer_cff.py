import FWCore.ParameterSet.Config as cms

from MitEdm.Producers.stableParts_cfi import *

from MitEdm.Producers.d2ss_cfi import *

decayProducer = cms.Sequence(stableParts*d2ss)
