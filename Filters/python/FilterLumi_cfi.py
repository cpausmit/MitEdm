# $Id:$

import FWCore.ParameterSet.Config as cms

###### Luminosity blocks by run when tracker and pixel HV were on ######
goodLumiBlocks = cms.EDFilter("FilterLumi",
    applyLumiFilter = cms.untracked.bool(True)
)
