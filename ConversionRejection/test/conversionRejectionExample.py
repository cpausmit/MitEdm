# $Id:#

import FWCore.ParameterSet.Config as cms

process = cms.Process('ANALYZER')

# import of standard configurations
process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load('Configuration/StandardSequences/GeometryIdeal_cff')
process.load('Configuration/StandardSequences/MagneticField_38T_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.load('Configuration/EventContent/EventContent_cff')


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.options = cms.untracked.PSet(
   Rethrow = cms.untracked.vstring('ProductNotFound'),
   fileMode =  cms.untracked.string('NOMERGE'),
)

# input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/relval/CMSSW_3_6_1/RelValTTbar/GEN-SIM-RECO/START36_V7-v1/0020/60E04BD6-095D-DF11-92CD-001A92971BD8.root'),
)
process.source.inputCommands = cms.untracked.vstring("keep *", "drop *_MEtoEDMConverter_*_*")

# other statements

process.GlobalTag.globaltag = 'START36_V9::All'


process.load("MitEdm.Producers.conversionRejection_cff")
process.load("MitEdm.ConversionRejection.conversionRejectionExample_cfi")

process.conv_step  = cms.Path(process.conversionRejection*process.conversionRejectionAnalyzer)

# schedule definition
process.schedule = cms.Schedule(process.conv_step)
