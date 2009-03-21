# $Id: HitDropper_cfi.py,v 1.2 2008/10/16 16:44:53 bendavid Exp $

import FWCore.ParameterSet.Config as cms

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *

from RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi import *

HitDropperESProducer = cms.ESProducer("HitDropperESProducer",
    ComponentName = cms.string('HitDropper')
)
