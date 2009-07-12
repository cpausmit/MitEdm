# $Id: conversionElectronsStable_cfi.py,v 1.1 2009/07/02 15:14:52 bendavid Exp $

import FWCore.ParameterSet.Config as cms

generalElectronsStable = cms.EDProducer("ProducerStable",
    iTracks = cms.untracked.string('generalTracks'),
    oPid    = cms.untracked.int32(11)
)

ckfInOutElectronsStable = cms.EDProducer("ProducerStable",
    iTracks = cms.untracked.string('ckfInOutTracksFromConversions'),
    oPid    = cms.untracked.int32(11)
)

ckfOutInElectronsStable = cms.EDProducer("ProducerStable",
    iTracks = cms.untracked.string('ckfOutInTracksFromConversions'),
    oPid    = cms.untracked.int32(11)
)

gsfElectronsStable = cms.EDProducer("ProducerStable",
    iTracks = cms.untracked.string('electronGsfTracks'),
    oPid    = cms.untracked.int32(11)
)

import MitEdm.Producers.stablePartMerger_cfi

mergedConversionsStable = MitEdm.Producers.stablePartMerger_cfi.stablePartMerger.clone(
  StableProducer1 = 'ckfInOutElectronsStable',
  StableProducer2 = 'ckfOutInElectronsStable',
)

mergedConversionsGeneralStable = MitEdm.Producers.stablePartMerger_cfi.stablePartMerger.clone(
  StableProducer1 = 'mergedConversionsStable',
  StableProducer2 = 'generalElectronsStable',
)

mergedElectronsStable = MitEdm.Producers.stablePartMerger_cfi.stablePartMerger.clone(
  StableProducer1 = 'mergedConversionsGeneralStable',
  StableProducer2 = 'gsfElectronsStable',
  preferCollection = 2,
)

conversionElectronsStable = cms.Sequence(generalElectronsStable*
                                         ckfInOutElectronsStable*
                                         ckfOutInElectronsStable
                                         )

mvfConversionElectronsStable = cms.Sequence(gsfElectronsStable*
                                            mergedConversionsStable*
                                            mergedConversionsGeneralStable*
                                            mergedElectronsStable)
