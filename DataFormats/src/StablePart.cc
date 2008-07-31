// $Id: StablePart.cc,v 1.2 2008/07/29 22:52:55 bendavid Exp $

#include "MitEdm/DataFormats/interface/BasePartAction.h"
#include "MitEdm/DataFormats/interface/StablePart.h"

using namespace std;
using namespace mitedm;

void StablePart::print(ostream &os) const
{
  os << " StablePartEdm::print - pid: " << pid_ << "  mass: " << mass()
     << "  track pointer: " << track_.get() << endl;
}

void StablePart::doAction(BasePartAction *action) const
{
  action->doAction(this);
  return;
}
