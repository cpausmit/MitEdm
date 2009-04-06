//--------------------------------------------------------------------------------------------------
// $Id: FillerTracks.h,v 1.21 2009/03/22 10:00:46 loizides Exp $
//
// LoadTracks
//
// Simple analyzer meant to pre-load GsfTracks.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_TRACKERELECTRONS_LOADTRACKS_H
#define MITEDM_TRACKERELECTRONS_LOADTRACKS_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"

namespace mitedm
{
  class LoadTracks : public edm::EDAnalyzer
  {
    public:
      LoadTracks(const edm::ParameterSet &cfg);
      ~LoadTracks();

      void         analyze(const edm::Event &e, const edm::EventSetup &es);

    protected:
      std::string  edmName_; //edm name of tracks collection
  };
}
#endif
