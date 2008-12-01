import FWCore.ParameterSet.Config as cms

from MitEdm.TrackerElectrons.SecStepGsf_cff import *
from MitEdm.TrackerElectrons.ThStepGsf_cff import *
from MitEdm.TrackerElectrons.PixelLessStepGsf_cff import *
iterTrackingGsf = cms.Sequence(secondStepGsf*thirdStepGsf*fourthStepGsf)
#iterTracking = cms.Sequence(firstfilter*secondStep*thirdStep)

