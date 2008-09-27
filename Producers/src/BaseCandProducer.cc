// $Id: BaseCandProducer.cc,v 1.4 2008/09/04 13:55:28 loizides Exp $

#include "MitEdm/Producers/interface/BaseCandProducer.h"
#include <TSystem.h>
#include <TError.h>
#include "MitEdm/DataFormats/interface/Collections.h"

using namespace edm;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
BaseCandProducer::BaseCandProducer(const ParameterSet& cfg) :
  oPid_ (cfg.getUntrackedParameter<int>("oPid",0))
{
  // Constructor: Register your base product.
}

//--------------------------------------------------------------------------------------------------
void BaseCandProducer::PrintErrorAndExit(const char *msg) const
{
  // Print error message and then exit.

  Error("PrintErrorAndExit", msg);
  gSystem->Exit(1);
}
