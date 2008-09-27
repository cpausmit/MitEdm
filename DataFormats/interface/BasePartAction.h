//--------------------------------------------------------------------------------------------------
// $Id: BasePartAction.h,v 1.2 2008/08/29 00:27:21 loizides Exp $
//
// BasePartAction
//
// This abstract base class has been created to help people with recursive descent of a Particle
// Decay Graph. Instead of writing a recursive subroutine or member function, define a subclass of
// this class. The subclass should provide a handler for both decay particles and stable particles.
//
// Recursion occurs when a client invokes:
//
//    DecayPart *particle;
//    particle = ....
//    particle->doAction(Action);
//
// Then, the handle functions are called once per particle as the action is passed up or down the
// Particle Decay Graph. A non-recursive action has also been implemented.
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_BASEPARTACTION_H
#define MITEDM_DATAFORMATS_BASEPARTACTION_H

namespace mitedm
{
  class BasePart;
  class DecayPart;
  class StablePart;

  class BasePartAction
  {
    public:
      enum ActionType { // type of the action
        BottomUp, 
        TopDown, 
        NonRecursive 
      };

      BasePartAction(ActionType aType);
      virtual ~BasePartAction();

      // Handle generic particles
      virtual void doAction(const BasePart* part);
      // Handle decaying particles
      virtual void doAction(const DecayPart* part) = 0;
      // Handle stable particles
      virtual void doAction(const StablePart* part) = 0;

      // Recursion level
      void         incLevel()            { level_++; }
      void         decLevel()            { level_--; }
      int          getLevel()      const { return level_; }
      ActionType   getActionType() const { return actionType_; }

  private:
    
    ActionType   actionType_; //type of action
    int          level_;      //remember how deep we are in there
  };
}
#endif
