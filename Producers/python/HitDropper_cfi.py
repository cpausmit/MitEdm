# $Id: HitDropper_cfi.py,v 1.1 2008/10/13 10:39:52 bendavid Exp $

import FWCore.ParameterSet.Config as cms

from TrackingTools.TransientTrack.TransientTrackBuilder_cfi import *

from RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi import *

HitDropperESProducer = cms.ESProducer("HitDropperESProducer",
     ComponentName = cms.string('HitDropper')
)