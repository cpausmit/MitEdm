//--------------------------------------------------------------------------------------------------
// $Id: BasePart.h,v 1.1 2008/07/29 13:16:22 loizides Exp $
//
// BasePart
//
// Implementation of base particle class. It will provide basic functionality to the DecayPart and
// StablePart class. The use of this class is also for looping purposes. See the double dispatcher
// doAction(MyAction).
//
// Author List: Ch.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_BASEPART_H
#define MITEDM_BASEPART_H

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
    // Constructors
    BasePart() : pid_(0) {}
    BasePart(int pid) : pid_(pid) {}
    virtual ~BasePart() {}
    
    int                  pid        () const { return pid_; }
    double               mass       () const;

    TParticlePDG        *particlePdgEntry() const;
     
    // Handle for recursive actions (have to implement it but should never be called)
    virtual void         doAction   (BasePartAction *action) const {}
    //virtual double       charge     () const = 0;
  
    virtual void         print      (std::ostream &os = std::cout) const;
  
    virtual int          nChild     () const { return 0; }
    virtual
      const BasePart*    getChild   (int i)  const { return 0; }
    
  protected: 
    // General stuff
    int                  pid_;
  };
}

//--------------------------------------------------------------------------------------------------
inline TParticlePDG *mitedm::BasePart::particlePdgEntry() const 
{ 
  // Return entry to pdg database for the PARTICLE.

  return TDatabasePDG::Instance()->GetParticle(pid_); 
}
#endif
