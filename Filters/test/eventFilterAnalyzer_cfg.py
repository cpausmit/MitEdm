import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("Configuration.StandardSequences.Services_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.Geometry_cff")

#global tags: https://twiki.cern.ch/twiki/bin/view/CMS/SWGuideFrontierConditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'STARTUP3X_V8I::All'

#message logger
process.load("FWCore.MessageService.MessageLogger_cfi")

# input source
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(

    # PYTHIA MB RelVal
    '/store/relval/CMSSW_3_3_5/RelValMinBias/GEN-SIM-RECO/STARTUP3X_V8D-v1/0008/90DE9F86-12DC-DE11-A010-001731AF67B5.root',
    
    # Non-collision events (Run 123515)
    #'file:/d100/data/ExpressPhysics/123151/bscskim/BSCskim_123151_Express.root',

    # Collision events (Run 123592)
    #'file:/d100/data/ExpressPhysics/123592/bscskim/BSCskim_123592_Express_bit40-41.root'
        
    )
)

### EDFilters ###
process.load("MitEdm.Filters.FilterLumi_cfi")
process.load("MitEdm.Filters.FilterBX_cff")
process.load("MitEdm.Filters.FilterEvtSel_cff")
process.load("MitEdm.Filters.FilterL1_cff")
process.load("MitEdm.Filters.FilterHLT_cff")

# load reconstruction for re-running rechits
process.load("Configuration.StandardSequences.Reconstruction_cff")

# load event selection data object producer
process.load("MitEdm.Producers.evtSelData_cfi")

# run extra reco
process.p = cms.Path(process.siPixelRecHits * process.evtSelData)

# define filter
process.myFilter = cms.Path(process.goodLumiBlocks 
		            #* process.collisionBunchCrossings
                            #* process.L1BscOR
                            * ~process.L1BscHalo # tilde means NOT
                            * process.L1BscMB1
                            #* process.L1BscMB2
                            * process.looseEvtSelFilter)

# write output that passes filter
process.output = cms.OutputModule("PoolOutputModule",
                                  compressionLevel = cms.untracked.int32(2),
                                  commitInterval = cms.untracked.uint32(1),
                                  fileName = cms.untracked.string("test.root"),
                                  outputCommands = cms.untracked.vstring('keep *'),
                                  dataset = cms.untracked.PSet(
                                        dataTier = cms.untracked.string('RAW-RECO'),
                                        filterName = cms.untracked.string('event filter')),
                                  SelectEvents = cms.untracked.PSet(
                                        SelectEvents = cms.vstring('myFilter')
                                        )
                                  )

process.save = cms.EndPath(process.output)
