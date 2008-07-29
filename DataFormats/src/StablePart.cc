// $Id: StablePart.cc,v 1.1 2008/07/29 13:16:22 loizides Exp $

#include "MitEdm/DataFormats/interface/BasePartAction.h"
#include "MitEdm/DataFormats/interface/StablePart.h"

using namespace std;
using namespace mitedm;

StablePart::StablePart() :
  BasePart(),
  hits_(0),
  stat_(0),
  phi0_(0) ,phi0Err_(0),
  d0_  (0) ,d0Err_  (0),
  pt_  (0) ,ptErr_  (0),
  z0_  (0) ,z0Err_  (0),
  cotT_(0) ,cotTErr_(0)
{
}

StablePart::StablePart(int pid) :
  BasePart(pid),
  hits_(0),
  stat_(0),
  phi0_(0) ,phi0Err_(0),
  d0_  (0) ,d0Err_  (0),
  pt_  (0) ,ptErr_  (0),
  z0_  (0) ,z0Err_  (0),
  cotT_(0) ,cotTErr_(0)
{
}

StablePart::StablePart(int pid, double mass) :
  BasePart(pid,mass),
  hits_(0),
  stat_(0),
  phi0_(0) ,phi0Err_(0),
  d0_  (0) ,d0Err_  (0),
  pt_  (0) ,ptErr_  (0),
  z0_  (0) ,z0Err_  (0),
  cotT_(0) ,cotTErr_(0)
{
}

void StablePart::print(ostream &os) const
{
  os << " StablePart::print - pid: " << pid_ << "  mass: " << mass_ << endl;
}

void StablePart::doAction(BasePartAction *action) const
{
  action->doAction(this);
  return;
}
