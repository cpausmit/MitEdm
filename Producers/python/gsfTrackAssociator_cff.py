# $Id: gsfTrackAssociator_cff.py,v 1.2 2008/12/01 17:25:09 bendavid Exp $

import FWCore.ParameterSet.Config as cms

gsfTrackAssociator = cms.EDProducer("TrackToTrackAssociator",
    fromTracks = cms.untracked.string('pixelMatchGsfFit'),
    toTracks   = cms.untracked.string('generalTracks')
)
