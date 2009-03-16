# $Id: vProducer_cff.py,v 1.7 2009/03/13 21:00:54 loizides Exp $

import FWCore.ParameterSet.Config as cms

from MitEdm.Producers.stableParts_cfi import *

from MitEdm.Producers.d2ss_cfi import *

decayProducer = cms.Sequence(stableParts*d2ss)
