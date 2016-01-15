//--------------------------------------------------------------------------------------------------
// HitDropper
//
// Utility to remove (or rather flag) hits from a track which are inconsistant with a vertex fit.
//
// Authors: J.Bendavid
//--------------------------------------------------------------------------------------------------

#ifndef MITEDM_PRODUCERS_HITDROPPER_H
#define MITEDM_PRODUCERS_HITDROPPER_H

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h"
#include "Geometry/CommonDetUnit/interface/GeomDet.h"
#include "MitEdm/DataFormats/interface/Types.h"

class TrackerTopology;

namespace mitedm
{
  class HitDropper
  {
    public:
      HitDropper(const TrackerGeometry *geo, const TransientTrackBuilder *builder,
                 const GeometricSearchTracker *search) :
        trackerGeo_(geo),
        builder_(builder),
        trackerGeoSearch_(search) {}

      reco::HitPattern              CorrectedHits(const reco::TransientTrack *tTrack,
                                                  TrackerTopology const&,
                                                  const ThreeVector &vtxPos) const;
      reco::HitPattern              CorrectedHits(const reco::Track *track,
                                                  TrackerTopology const&,
                                                  const ThreeVector &vtxPos) const;
      reco::HitPattern              CorrectedHits(const reco::Track *track,
                                                  TrackerTopology const&,
                                                  const ThreeVector &vtxPos,
                                                  const ThreeVector &trkMom,
                                                  Double_t lxyError,
                                                  Double_t lzError,
                                                  Double_t sigmaTolerance=3.0) const;
      std::pair<reco::HitPattern,uint> CorrectedHitsAOD(const reco::Track *track,
                                                  TrackerTopology const&,
                                                  const ThreeVector &vtxPos,
                                                  const ThreeVector &trkMom,
                                                  Double_t lxyError,
                                                  Double_t lzError,
                                                  Double_t sigmaTolerance=1.5) const;
      reco::HitPattern              SharedHits(const reco::Track *t1, const reco::Track *t2, TrackerTopology const&) const;                                                  
      bool                          IsBarrel(const GeomDet *det) const;
      bool                          IsDisk(const GeomDet *det) const;
          
    protected:
      const DetLayer               *FindLayer(int subdet, int layer, int side) const;
      DetId                         StereoDetId(const DetId &i) const;

      const TrackerGeometry        *trackerGeo_;        //tracker geometry
      const TransientTrackBuilder  *builder_;           //track builder
      const GeometricSearchTracker *trackerGeoSearch_;  //geometrical track searcher
  };
}

//-------------------------------------------------------------------------------------------------
inline bool mitedm::HitDropper::IsBarrel(const GeomDet *det) const
{
  // Check if detector element is in a barrel layer.

  GeomDetEnumerators::SubDetector subDet = det->subDetector();
  
  if (subDet==GeomDetEnumerators::PixelBarrel || subDet==GeomDetEnumerators::TIB
           || subDet==GeomDetEnumerators::TOB || subDet==GeomDetEnumerators::DT
           || subDet==GeomDetEnumerators::RPCBarrel)
    return true;
  else
    return false;
}

//-------------------------------------------------------------------------------------------------
inline bool mitedm::HitDropper::IsDisk(const GeomDet *det) const
{
  // Check if detector element is in a barrel layer.

  GeomDetEnumerators::SubDetector subDet = det->subDetector();
  
  if (subDet==GeomDetEnumerators::PixelEndcap || subDet==GeomDetEnumerators::TID
           || subDet==GeomDetEnumerators::TEC || subDet==GeomDetEnumerators::CSC
           || subDet==GeomDetEnumerators::RPCEndcap)
    return true;
  else
    return false;
}
#endif
