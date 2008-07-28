#include <TSystem.h>
#include <TError.h>

#include "MitEdm/AnalysisDataFormats/interface/CollectionsEdm.h"
#include "MitEdm/Producers/interface/BaseCandidate.h"

using namespace edm;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
BaseCandidate::BaseCandidate(const ParameterSet& cfg) :
  oPid_ (cfg.getUntrackedParameter<int>   ("oPid", 0  )),
  oMass_(cfg.getUntrackedParameter<double>("oMass",0.0))
{
  // register your base product
  produces<BasePartObjArr>();
}

//--------------------------------------------------------------------------------------------------
void BaseCandidate::PrintErrorAndExit(const char *msg) const
{
  Error("PrintErrorAndExit", msg);
  gSystem->Exit(1);
}
