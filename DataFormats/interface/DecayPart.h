//--------------------------------------------------------------------------------------------------
// $Id: DecayPart.h,v 1.3 2008/07/31 13:29:35 bendavid Exp $
//
// DecayPart
//
// Implementation of a decay particle class for use in vertexing based analyses. The contents of a
// decay particle is basically its link to the particles it decays to. This will be hopefully at
// some point a useful and good implementation. See the double dispatcher doAction(MyAction).
//
// Author List: Ch.Paus
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DECAYPART_H
#define MITEDM_DECAYPART_H

#include <iostream>
#include <vector>
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Geometry/Point3D.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "MitEdm/DataFormats/interface/Types.h"
#include "MitEdm/DataFormats/interface/BasePart.h"
#include "MitEdm/DataFormats/interface/BasePartFwd.h"

namespace mitedm
{
  class BasePartAction;
  class DecayPart : public BasePart
  {
  
  public:
    // Decay type
    enum DecayType {Fast, Slow};
  
    DecayPart() {}
    DecayPart(int pid) {}
    DecayPart(int pid, DecayType dType);
  
    // Accessors
    DecayType           decayType    () const { return decayType_; }

    // Override recursion helper method
    virtual void        doAction     (BasePartAction *action) const;
    virtual double      charge       () const { return 0.; }
  
    // Extend the particle contents
    void                addChild     (BasePartBaseRef partRef) { children_.push_back(partRef); }
  
    const BasePart*     getChild     (int i) const    { return children_.at(i).get(); }
    const BasePartBaseRef getChildRef (int i) const { return children_.at(i); }
    int                 nChild       () const         { return children_.size();  }
  
    virtual void        print        (std::ostream& os = std::cout) const;
  
    //----------------------------------------------------------------------------------------------
    // Accessors/Setter: Base Vertex fit info from this level
    //----------------------------------------------------------------------------------------------
    // Fit quality (does not belong here)
    double    prob() const { return prob_; }
    double    chi2() const { return chi2_; }
    int       ndof() const { return ndof_; }
    void   setProb(double prob) { prob_ = prob;}
    void   setChi2(double chi2) { chi2_ = chi2;}
    void   setNdof(int    ndof) { ndof_ = ndof;}
  
    // Fitted Mass
    double    fittedMass() const { return fittedMass_; }
    void   setFittedMass(double fittedMass) { fittedMass_ = fittedMass;}
    // Fitted Mass Error
    double    fittedMassError() const { return fittedMassError_; }
    void   setFittedMassError(double fittedMassError) { fittedMassError_ = fittedMassError;}
    // Lxy
    double    lxy() const { return lxy_; }
    void   setLxy(double lxy) { lxy_ = lxy;}
    // Lxy Error
    double    lxyError() const { return lxyError_; }
    void   setLxyError(double lxyError) { lxyError_ = lxyError;}
    // LxyToPv (length to primary vertex)
    double    lxyToPv() const { return lxyToPv_; }
    void   setLxyToPv(double lxyToPv) { lxyToPv_ = lxyToPv;}
    // LxyToPv Error
    double    lxyToPvError() const { return lxyToPvError_; }
    void   setLxyToPvError(double lxyToPvError) { lxyToPvError_ = lxyToPvError;}
    // Dxy (two dimensional impact parameter)
    double    dxy() const { return dxy_; }
    void   setDxy(double dxy) { dxy_ = dxy;}
    // Dxy Error
    double    dxyError() const { return dxyError_; }
    void   setDxyError(double dxyError) { dxyError_ = dxyError;}
    // DxyToPv (two dimensional impact parameter with respect to primary vertex)
    double    dxyToPv() const { return dxyToPv_; }
    void   setDxyToPv(double dxyToPv) { dxyToPv_ = dxyToPv;}
    // DlxyToPv Error
    double    dxyToPvError() const { return dxyToPvError_; }
    void   setDxyToPvError(double dxyToPvError) { dxyToPvError_ = dxyToPvError;}
    // Lz
    double    lz() const { return lz_; }
    void   setLz(double lz) { lz_ = lz;}
    // Lz Error
    double    lzError() const { return lzError_; }
    void   setLzError(double lzError) { lzError_ = lzError;}
    // LzToPv (length to primary vertex)
    double    lzToPv() const { return lzToPv_; }
    void   setLzToPv(double lzToPv) { lzToPv_ = lzToPv;}
    // LzToPv Error
    double    lzToPvError() const { return lzToPvError_; }
    void   setLzToPvError(double lzToPvError) { lzToPvError_ = lzToPvError;}
    // CTau
    double    cTau() const { return cTau_; }
    void   setCTau(double cTau) { cTau_ = cTau;}
    // CTau Error
    double    cTauError() const { return cTauError_; }
    void   setCTauError(double cTauError) { cTauError_ = cTauError;}
    // Pt
    double    pt() const { return  pt_; }
    void   setPt(double pt) { pt_ = pt;}
    // Pt Error
    double    ptError() const { return ptError_; }
    void   setPtError(double ptError) { ptError_ = ptError;}
    // Four momentum
    const FourVector         &fourMomentum() const { return fourMomentum_; }
    void                   setFourMomentum(const FourVector &fourMomentum)
    { fourMomentum_ = fourMomentum;}
    //----------------------------------------------------------------------------------------------
    // Accessors/Setter: Extended Vertex fit info from this level
    //----------------------------------------------------------------------------------------------
    // Position
    const ThreeVector        &position() const { return position_; }
    void                   setPosition(const ThreeVector &position) { position_ = position; }
    // Error
    const ThreeSymMatrix     &error() const { return error_; }
    void                   setError(const ThreeSymMatrix &error) { error_ = error; }
    // Big 7x7 Error Matrix
    const SevenSymMatrix     &bigError() const { return bigError_; }
    void                   setBigError(const SevenSymMatrix &bigError) { bigError_ = bigError; }
    
  private:
    // Decay type (either fast of slow)
    DecayType              decayType_;
    // Fit quality
    double                 prob_;
    double                 chi2_;
    int                    ndof_;
    // Base vertex fit info
    double                 fittedMass_;
    double                 fittedMassError_;
    double                 normalizedMass_;
    double                 lxy_;
    double                 lxyError_;
    double                 lxyToPv_;
    double                 lxyToPvError_;
    double                 dxy_;
    double                 dxyError_;
    double                 dxyToPv_;
    double                 dxyToPvError_;
    double                 lz_;
    double                 lzError_;
    double                 lzToPv_;
    double                 lzToPvError_;
    double                 cTau_;
    double                 cTauError_;
    double                 pt_;
    double                 ptError_;
    FourVector             fourMomentum_;
    // Extended vertex fit info
    ThreeVector            position_;
    ThreeSymMatrix         error_;
    SevenSymMatrix         bigError_;
    
    // Contents of the decay
    BasePartBaseRefVector children_;

  };
}
#endif
