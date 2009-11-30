// $Id: QcdLowPtDQM.h,v 1.9 2009/11/19 22:33:15 loizides Exp $

#ifndef MITEDM_PRODUCERS_VERTEXZPRODUCER_H
#define MITEDM_PRODUCERS_VERTEXZPRODUCER_H

#include "DataFormats/GeometryVector/interface/VectorUtil.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <TMath.h>
#include <vector>

class TrackerGeometry;

class VertexZProducer : public edm::EDProducer
{
  public:
    class Pixel {
      public:
        Pixel(double x=0, double y=0, double z=0, double eta=0, double phi=0, 
              double adc=0, double sx=0, double sy=0) : 
          x_(x), y_(y), z_(z), rho_(TMath::Sqrt(x_*x_+y_*y_)), 
          eta_(eta), phi_(phi), adc_(adc), sizex_(sx), sizey_(sy) {}
        Pixel(const GlobalPoint &p, double adc=0, double sx=0, double sy=0) :
          x_(p.x()), y_(p.y()), z_(p.z()), rho_(TMath::Sqrt(x_*x_+y_*y_)), 
          eta_(p.eta()), phi_(p.phi()), adc_(adc), sizex_(sx), sizey_(sy) {}
        double adc()   const { return adc_;   }
        double eta()   const { return eta_;   }
        double rho()   const { return rho_;   }
        double phi()   const { return phi_;   }
        double sizex() const { return sizex_; }
        double sizey() const { return sizey_; }
        double x()     const { return x_;     }
        double y()     const { return y_;     }
        double z()     const { return z_;     }
      protected:    
        double x_,y_,z_,rho_,eta_,phi_;
        double adc_,sizex_,sizey_;
    };
    class Tracklet {
      public:
        Tracklet() : i1_(-1), i2_(-2), deta_(0), dphi_(0) {}
        Tracklet(const Pixel &p1, const Pixel &p2) : 
          p1_(p1), p2_(p2), i1_(-1), i2_(-1), 
          deta_(p1.eta()-p2.eta()), dphi_(Geom::deltaPhi(p1.phi(),p2.phi())) {}
        double       deta()  const { return deta_;     }
        double       dphi()  const { return dphi_;     }
        int          i1()    const { return i1_;       }
        int          i2()    const { return i2_;       }
        double       eta()   const { return p1_.eta(); }
        const Pixel &p1()    const { return p1_;       }
        const Pixel &p2()    const { return p2_;       }
        void         seti1(int i1) { i1_ = i1;         }      
        void         seti2(int i2) { i2_ = i2;         }      
      protected:
        Pixel  p1_,p2_;
        int    i1_, i2_;
        double deta_,dphi_;
    };
    class Vertex {
      public:
        Vertex(double x=0, double y=0, double z=0, 
               double xs=0, double ys=0, double zs=0, 
               double chi2=0, int n=0) :
          x_(x), y_(y), z_(z), xs_(xs), ys_(ys), zs_(zs), chi2_(chi2), n_(n) {}
        double chi2() const { return chi2_; }
        int    n()    const { return n_;    }
        double x()    const { return x_;    }
        double y()    const { return y_;    }
        double z()    const { return z_;    }
        double xs()   const { return xs_;   }
        double ys()   const { return ys_;   }
        double zs()   const { return zs_;   }
        void   set(int n, double z, double zs) { n_= n; z_ = z; zs_ = zs;              }
        void   set(int n, double x,double y,double z, double xs,double ys,double zs) 
                 { n_= n; x_ = x; xs_ = xs; y_ = y; ys_ = ys; z_ = z; zs_ = zs;        }
        void   setchi2(double chi2)            { chi2_ = chi2; }
        void   setinvalid()                    { n_ = 0; z_=-9999; zs_ = 0; chi2_ = 0; }
      protected:    
        double x_,y_,z_,xs_,ys_,zs_,chi2_;
        int n_;
    };

    explicit VertexZProducer(const edm::ParameterSet &parameters);
    ~VertexZProducer();

    void                          produce(edm::Event &iEvent, const edm::EventSetup &iSetup);

  private:
    void                          fillPixels(const edm::Event &iEvent);
    template <typename TYPE>
    void                          getProduct(const std::string name, edm::Handle<TYPE> &prod,
                                             const edm::Event &event) const;    
    template <typename TYPE>
    bool                          getProductSafe(const std::string name, edm::Handle<TYPE> &prod,
                                                 const edm::Event &event) const;
    void                          print(int level, const char *msg);
    void                          print(int level, const std::string &msg)
                                    { print(level,msg.c_str()); }
    void                          reallyPrint(int level, const char *msg);
    void                          trackletVertexUnbinned(const edm::Event &iEvent, int which=12);
    void                          trackletVertexUnbinned(std::vector<Pixel> &pix1, 
                                                         std::vector<Pixel> &pix2,
                                                         Vertex &vtx);
    void                          vertexFromClusters(const edm::Event &iEvent, int which=12);
    void                          vertexFromClusters(const std::vector<Pixel> &pix, Vertex &vtx);

    std::string                   pixelName_;          //pixel reconstructed hits name
    double                        dPhiVc_;             //dPhi vertex cut for tracklet based vertex
    double                        dZVc_;               //dZ vertex cut for tracklet based vertex
    int                           verbose_;            //verbosity (0=debug,1=warn,2=error,3=throw)
    int                           pixLayers_;          //either 12,13,23
    bool                          doClusVertex_;        //if true run cluster vertex finder
    bool                          useRecHitQ_;         //if true use rec hit quality word
    bool                          usePixelQ_;          //if true use pixel hit quality word
    std::vector<Pixel>            bpix1_;              //barrel pixels layer 1
    std::vector<Pixel>            bpix2_;              //barrel pixels layer 2
    std::vector<Pixel>            bpix3_;              //barrel pixels layer 3
    Vertex                        trackletV_;          //reconstructed tracklet vertex 12
    const TrackerGeometry        *tgeo_;               //tracker geometry
};

//--------------------------------------------------------------------------------------------------
template <typename TYPE>
inline void VertexZProducer::getProduct(const std::string name, edm::Handle<TYPE> &prod,
                                    const edm::Event &event) const
{
  // Try to access data collection from EDM file. We check if we really get just one
  // product with the given name. If not we throw an exception.

  event.getByLabel(edm::InputTag(name),prod);
  if (!prod.isValid()) 
    throw edm::Exception(edm::errors::Configuration, "VertexZProducer::GetProduct()\n")
      << "Collection with label " << name << " is not valid" <<  std::endl;
}

//--------------------------------------------------------------------------------------------------
template <typename TYPE>
inline bool VertexZProducer::getProductSafe(const std::string name, edm::Handle<TYPE> &prod,
                                        const edm::Event &event) const
{
  // Try to safely access data collection from EDM file. We check if we really get just one
  // product with the given name. If not, we return false.

  if (name.size()==0)
    return false;

  try {
    event.getByLabel(edm::InputTag(name),prod);
    if (!prod.isValid()) 
      return false;
  } catch (...) {
    return false;
  }
  return true;
}

//--------------------------------------------------------------------------------------------------
inline void VertexZProducer::print(int level, const char *msg)
{
  // Print out message if it 

  if (level>=verbose_) 
    reallyPrint(level,msg);
}
#endif
