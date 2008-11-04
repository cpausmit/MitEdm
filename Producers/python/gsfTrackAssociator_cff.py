# $Id: conversions_cfi.py,v 1.4 2008/10/16 16:46:49 bendavid Exp $

import FWCore.ParameterSet.Config as cms

gsfTrackAssociator = cms.EDProducer("TrackToTrackAssociator",
                      fromTracks      = cms.untracked.string('pixelMatchGsfFit'),
                      toTracks        = cms.untracked.string('generalTracks'),
                      associationName = cms.untracked.string('gsfToTrackAssociation')
                      )
