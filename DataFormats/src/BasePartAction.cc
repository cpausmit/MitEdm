// $Id: $

#include "MitEdm/DataFormats/interface/BasePartAction.h"
#include "MitEdm/DataFormats/interface/BasePart.h"
#include "MitEdm/DataFormats/interface/DecayPart.h"
#include "MitEdm/DataFormats/interface/StablePart.h"
#include <iostream>

using namespace mitedm;

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
BasePartAction::BasePartAction(ActionType AType) :
  actionType_(AType),
  level_     (0)
{
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
BasePartAction::~BasePartAction()
{
}

//------------------------------------------------------------------------------
// Handle Generic Particles
//------------------------------------------------------------------------------
void BasePartAction::doAction(const BasePart *part)
{
  // Just override me
}
