# $Id:$

import FWCore.ParameterSet.Config as cms

###### L1 (https://twiki.cern.ch/twiki/bin/viewauth/CMS/DNDEtaTriggerList#Relevant_L1_Bits) ######
from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff import *
import HLTrigger.HLTfilters.hltLevel1GTSeed_cfi

#tech bit 40 - BSC coincidence (threshold 1)
L1BscMB1 = HLTrigger.HLTfilters.hltLevel1GTSeed_cfi.hltLevel1GTSeed.clone()
L1BscMB1.L1TechTriggerSeeding = cms.bool(True)
L1BscMB1.L1SeedsLogicalExpression = cms.string('40') 

#tech bit 41 - BSC coincidence (threshold 2)
L1BscMB2 = HLTrigger.HLTfilters.hltLevel1GTSeed_cfi.hltLevel1GTSeed.clone()
L1BscMB2.L1TechTriggerSeeding = cms.bool(True)
L1BscMB2.L1SeedsLogicalExpression = cms.string('41')
    
#tech bit 34 - BSC OR
L1BscOR = HLTrigger.HLTfilters.hltLevel1GTSeed_cfi.hltLevel1GTSeed.clone()
L1BscOR.L1TechTriggerSeeding = cms.bool(True)
L1BscOR.L1SeedsLogicalExpression = cms.string('34')
    
#tech bits 36,37,38,39 - BSC halo bits
L1BscHalo = HLTrigger.HLTfilters.hltLevel1GTSeed_cfi.hltLevel1GTSeed.clone()
L1BscHalo.L1TechTriggerSeeding = cms.bool(True)
L1BscHalo.L1SeedsLogicalExpression = cms.string('36 OR 37 OR 38 OR 39') 

#algo bit 124 - BSC OR + BPTX OR
L1BscORBptxOR = HLTrigger.HLTfilters.hltLevel1GTSeed_cfi.hltLevel1GTSeed.clone()
L1BscORBptxOR.L1TechTriggerSeeding = cms.bool(False)
L1BscORBptxOR.L1SeedsLogicalExpression = cms.string('L1_BscMinBiasOR_BptxPlusORMinus') 

#tech bit 0 - BPTX coincidence  - (same as algo bit 0?)
L1BptxAND = HLTrigger.HLTfilters.hltLevel1GTSeed_cfi.hltLevel1GTSeed.clone()
L1BptxAND.L1TechTriggerSeeding = cms.bool(True)
L1BptxAND.L1SeedsLogicalExpression = cms.string('0') 

#algo bits 80,81 - BPTX OR - (same as tech bit 3 which is prescaled)
L1BptxOR = HLTrigger.HLTfilters.hltLevel1GTSeed_cfi.hltLevel1GTSeed.clone()
L1BptxOR.L1TechTriggerSeeding = cms.bool(False)
L1BptxOR.L1SeedsLogicalExpression = cms.string('L1_BptxMinus OR L1_BptxPlus') 

#tech bit 9 - HF coincidence
L1HFcoincidence = HLTrigger.HLTfilters.hltLevel1GTSeed_cfi.hltLevel1GTSeed.clone()
L1HFcoincidence.L1TechTriggerSeeding = cms.bool(True)
L1HFcoincidence.L1SeedsLogicalExpression = cms.string('9')
