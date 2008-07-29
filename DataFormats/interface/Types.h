//--------------------------------------------------------------------------------------------------
// $Id: Types.h,v 1.10 2008/07/28 23:13:41 paus Exp $
//
// Types
//
// Here we define additional types.
//
// Authors: C.Loizides
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_TYPES_H
#define MITEDM_TYPES_H
 
#include <Rtypes.h>
#include <Math/GenVector/LorentzVector.h>
#include <Math/Point3Dfwd.h>

namespace mitedm
{
  typedef ::ROOT::Math::LorentzVector< ::ROOT::Math::PxPyPzE4D<double> > FourVector;
  typedef ::ROOT::Math::DisplacementVector3D<ROOT::Math::Cartesian3D<double>,
                                             ROOT::Math::DefaultCoordinateSystemTag> ThreeVector;
  typedef ::ROOT::Math::DisplacementVector3D<ROOT::Math::CylindricalEta3D<double>,
                                             ROOT::Math::DefaultCoordinateSystemTag> ThreeVectorEtaPhi;                                             
}
#endif
