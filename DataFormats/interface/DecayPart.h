//--------------------------------------------------------------------------------------------------
// $Id: DecayPart.h,v 1.12 2008/10/03 23:53:46 loizides Exp $
//
// DecayPart
//
// Implementation of a decay particle class for use in vertexing based analyses. The contents of a
// decay particle is basically its link to the particles it decays to. This will be hopefully at
// some point a useful and good implementation. See the double dispatcher doAction(MyAction).
//
// Authors: C.Paus, J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_DATAFORMATS_DECAYPART_H
#define MITEDM_DATAFORMATS_DECAYPART_H

#include <iostream>
#include <vector>
#include "MitEdm/DataFormats/interface/Types.h"
#include "MitEdm/DataFormats/interface/BasePart.h"
#include "MitEdm/DataFormats/interface/BasePartFwd.h"
#include "MitEdm/DataFormats/interface/StableData.h"
#include "MitEdm/DataFormats/interface/DecayData.h"

namespace mitedm
{
  class BasePartAction;
  
  class DecayPart : public BasePart
  {
  
    public:
      enum DecayType {Fast, Slow};
      
    DecayPart();
    DecayPart(int pid);
    DecayPart(int pid, DecayType dType);
  
    // Accessors
    DecayType           decayType    () const { return decayType_; }

    // Override recursion helper method
    virtual void        doAction     (BasePartAction *action) const;
    virtual double      charge       () const { return 0.; }
  
    // Extend the particle contents
    void                addDecayChild  (DecayData &decay)   { decayChildren_.push_back(decay);   }
    void                addStableChild (StableData &stable) { stableChildren_.push_back(stable); }
  
    const BasePartPtr   getDaughterPtr(int i)  const { return getDaughterData(i).originalPtr(); } 
    
    const DaughterData &getDaughterData(int i) const;
    const DecayData    &getDecayData  (int i)  const { return decayChildren_.at(i);           }
    const StableData   &getStableData (int i)  const { return stableChildren_.at(i);          }
    int                 nDecayChild   ()       const { return decayChildren_.size();          }
    int                 nStableChild  ()       const { return stableChildren_.size();         }
    int                 nChild        ()       const { return nStableChild() + nDecayChild(); }
  
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
    
    VertexPtr                 primaryVertex() const { return primaryVertex_; }
    void                   setPrimaryVertex(VertexPtr ptr) { primaryVertex_ = ptr; }
    
  private:
    // Decay type (either fast of slow)
    DecayType              decayType_;        //type of decay
    // Fit quality
    double                 prob_;             //fit probability
    double                 chi2_;             //chi2 value
    int                    ndof_;             //degrees of freedom
    // Base vertex fit info
    double                 fittedMass_;       //mass from fit
    double                 fittedMassError_;  //mass error from fit
    double                 normalizedMass_;   //normalized mass
    double                 lxy_;              //lxy
    double                 lxyError_;         //lxy error
    double                 lxyToPv_;          //lxy wrt primary vertex
    double                 lxyToPvError_;     //error on  lxy wrt primary vertex
    double                 dxy_;              //dxy
    double                 dxyError_;         //dxy error
    double                 dxyToPv_;          //dxy wrt primary vertex
    double                 dxyToPvError_;     //error on dxy wrt primary vertex
    double                 lz_;               //lz
    double                 lzError_;          //lz error
    double                 lzToPv_;           //lz wrt primary vertex
    double                 lzToPvError_;      //error on lz wrt primary vertex
    double                 cTau_;             //ctau
    double                 cTauError_;        //error on ctau
    double                 pt_;               //pt
    double                 ptError_;          //error on pt
    FourVector             fourMomentum_;     //momentum at vertex
    // Extended vertex fit info
    ThreeVector            position_;         //vertex position
    ThreeSymMatrix         error_;            //error matrix
    SevenSymMatrix         bigError_;         //error matrix
     
    VertexPtr              primaryVertex_;    //primary vertex
    
    // Contents of the decay
    std::vector<StableData> stableChildren_;  //stable daughter
    std::vector<DecayData>  decayChildren_;   //decay children
  };
}

//--------------------------------------------------------------------------------------------------
inline const mitedm::DaughterData &mitedm::DecayPart::getDaughterData(int i) const
{ 
  if (i < nStableChild())
    return getStableData(i);
  else
   return getDecayData(i-nStableChild());
}
#endif
