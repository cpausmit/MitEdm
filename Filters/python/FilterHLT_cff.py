# $Id:$

import FWCore.ParameterSet.Config as cms

###### HLT (https://twiki.cern.ch/twiki/bin/viewauth/CMS/DNDEtaTriggerList#Relevant_HLT_Paths) ######
import HLTrigger.HLTfilters.hltHighLevel_cfi

#MinBiasBSC
hltMinBiasBSC = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
hltMinBiasBSC.HLTPaths = cms.vstring('HLT_MinBiasBSC')

#MinBiasPixel
hltMinBiasPixel1 = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
hltMinBiasPixel1.HLTPaths = cms.vstring('HLT_MinBiasPixel_SingleTrack')
