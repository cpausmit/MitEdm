// $Id: DecayPart.cc,v 1.2 2008/07/29 22:52:55 bendavid Exp $

#include "MitEdm/DataFormats/interface/BasePartAction.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"

using namespace std;
using namespace mitedm;

DecayPart::DecayPart(int pid, DecayType decayType) :
  BasePart        (pid),
  decayType_      (decayType),
  prob_           (0),
  chi2_           (0),
  ndof_           (0),
  fittedMass_     (0),
  fittedMassError_(0),
  lxy_            (0),
  lxyError_       (0),
  lxyToPv_        (0),
  lxyToPvError_   (0),
  dxy_            (0),
  dxyError_       (0),
  dxyToPv_        (0),
  dxyToPvError_   (0),
  lz_             (0),
  lzError_        (0),
  lzToPv_         (0),
  lzToPvError_    (0),
  cTau_           (0),
  cTauError_      (0),
  pt_             (0),
  ptError_        (0),
  fourMomentum_   (0,0,0,0),
  position_       (0,0,0)
{
}

void DecayPart::print(ostream &os) const
{
  os << " DecayPart::print - pid: " << pid_ << "  mass: " << mass()
     << "  decayType: " << decayType_ << endl
     <<  "  Decays to \n";
  for (BasePartBaseRefVector::const_iterator ip=children_.begin(); ip!=children_.end(); ip++) {
    os << "   "; (*ip)->print(os);    
  }
  os <<  "  -- end decays to -------\n";
  os <<  "  Vertex fit (c2,ndof,prob): " << chi2() << ",  " << ndof() << ",  " << prob() << "\n\n" ;
}

void DecayPart::doAction(BasePartAction *action) const
{

  if (action->getActionType() == BasePartAction::NonRecursive) {
    action->doAction(this);
    return;
  }

  if (action->getActionType() == BasePartAction::TopDown)
    action->doAction(this);
  
  for (BasePartBaseRefVector::const_iterator ip=children_.begin(); ip!=children_.end(); ip++) {
    action->incLevel(); (ip->get())->doAction(action); action->decLevel();
  }

  if (action->getActionType() == BasePartAction::BottomUp)
    action->doAction(this);

  return;
}
