# $Id: gsfTrackAssociator_cff.py,v 1.1 2008/11/04 19:25:56 bendavid Exp $

import FWCore.ParameterSet.Config as cms

gsfTrackAssociator = cms.EDProducer("TrackToTrackAssociator",
                      fromTracks      = cms.untracked.string('pixelMatchGsfFit'),
                      toTracks        = cms.untracked.string('generalTracks')
                      )
