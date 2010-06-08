# $Id: conversionProducer_cff.py,v 1.4 2009/11/03 15:13:18 bendavid Exp $

import FWCore.ParameterSet.Config as cms

from MitEdm.Producers.HitDropper_cfi import *

from MitEdm.Producers.conversionElectronsStable_cfi import *

from MitEdm.Producers.conversions_cfi import *

conversionRejection = cms.Sequence(electronsStable*mvfConversionRemoval)

