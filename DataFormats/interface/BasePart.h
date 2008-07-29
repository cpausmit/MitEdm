//--------------------------------------------------------------------------------------------------
// $Id: $
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

namespace mitedm
{
  class BasePartAction;
  class BasePart
  {
   public:
    // Constructors
    BasePart() : pid_(0), mass_(0) {}
    BasePart(int pid);
    BasePart(int pid, double mass);
    BasePart(const BasePart &b);
    // Destructor
    virtual ~BasePart() {}
    
    int                  pid        () const { return pid_; }
    double               mass       () const { return mass_; }
     
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
    double               mass_;
  };
}
#endif
