// $Id: BasePartAction.cc,v 1.1 2008/07/29 13:16:22 loizides Exp $

#include "MitEdm/DataFormats/interface/BasePartAction.h"
#include "MitEdm/DataFormats/interface/BasePart.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include <iostream>

using namespace mitedm;

//------------------------------------------------------------------------------
BasePartAction::BasePartAction(ActionType AType) :
  actionType_(AType),
  level_     (0)
{
  // Constructor
}

//------------------------------------------------------------------------------
BasePartAction::~BasePartAction()
{
  // Destructor
}

//------------------------------------------------------------------------------
void BasePartAction::doAction(const BasePart *part)
{
  // Handle generic particles. Just override this.
}
