// $Id: BasePart.cc,v 1.1 2008/07/29 13:16:22 loizides Exp $

#include "MitEdm/DataFormats/interface/BasePart.h"

using namespace std;
using namespace mitedm;

//--------------------------------------------------------------------------------------------------
Double_t BasePart::mass() const
{
  // Get Mass from Pdg Lookup

  TParticlePDG* pdgEntry = particlePdgEntry();
  if (pdgEntry)
    return pdgEntry->Mass();
  else {
    return -99.0;
    printf("Absolute Pdg Code %i not found in table, returning Mass=-99.0 GeV", pid_);
  }
}

//--------------------------------------------------------------------------------------------------
void BasePart::print(ostream &os) const
{
  os << " BasePart::print - pid: " << pid_ << "  mass: " << mass() << endl;
}
