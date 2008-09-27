//--------------------------------------------------------------------------------------------------
// $Id: BasePart.h,v 1.3 2008/08/29 00:27:21 loizides Exp $
//
// BasePart
//
// Implementation of base particle class. It will provide basic functionality to the DecayPart and
// StablePart class. The use of this class is also for looping purposes. See the double dispatcher
// doAction(MyAction).
//
// Authors: C.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_BASEPART_H
#define MITEDM_DATAFORMATS_BASEPART_H

#include <iostream>
#include <vector>
#include "TParticlePDG.h"
#include "TDatabasePDG.h"

namespace mitedm
{
  class BasePartAction;

  class BasePart
  {
   public:
    BasePart() : pid_(0) {}
    BasePart(int pid) : pid_(pid) {}
    virtual ~BasePart() {}
    
    int                     pid()              const { return pid_; }
    double                  mass()             const;
    TParticlePDG           *particlePdgEntry() const;
    virtual double          charge()           const { return 0.; }
    // Handle for recursive actions (have to implement it but should never be called)
    virtual void            doAction(BasePartAction *action)    const {}
    virtual void            print(std::ostream &os = std::cout) const;
    virtual int             nChild()           const { return 0; }
    virtual const BasePart *getChild(int i)    const { return 0; }
    
  protected: 
    int                     pid_;
  };
}

//--------------------------------------------------------------------------------------------------
inline TParticlePDG *mitedm::BasePart::particlePdgEntry() const 
{ 
  // Return entry to pdg database for the particle.

  return TDatabasePDG::Instance()->GetParticle(pid_); 
}
#endif
