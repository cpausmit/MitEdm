# $Id: generalGsfTracking_cfi.py,v 1.4 2009/03/21 12:46:57 loizides Exp $

import FWCore.ParameterSet.Config as cms

LoadTracks = cms.EDAnalyzer("LoadTracks",
    edmName = cms.untracked.string('pixelMatchGsfFit')
)
