// -*- mode: c++ -*-

#ifndef ROOATGCPROCESSSCALING
#define ROOATGCPROCESSSCALING

#include "RooRealProxy.h"
#include "RooAbsPdf.h"
#include "TProfile2D.h"
#include "TString.h"
  
class RooATGCProcessScaling : public RooAbsReal {
public:
  
  enum LimitType{ dkglZ, dg1lZ, dkdg1, notype };

  RooATGCProcessScaling ();
  RooATGCProcessScaling (const char * name, const char * title,
			 RooAbsReal& _x, RooAbsReal& _dkg,
			 RooAbsReal& _lZ, RooAbsReal& _dg1,
			 RooAbsReal& _SM_shape,
			 const char * parFilename);
  RooATGCProcessScaling (const RooATGCProcessScaling& other, const char * name);
  virtual TObject * clone(const char * newname) const { 
    return new RooATGCProcessScaling(*this, newname);
    }
  
  void setLimitType(const unsigned& lt) { type_ = (LimitType)lt; }

  virtual ~RooATGCProcessScaling ();  
  
  void readProfiles(TDirectory& dir) const ;
  TString getProfileFilename() const { return profileFilename; }
  
protected:
  
  RooRealProxy x;
  RooRealProxy lZ;
  RooRealProxy dkg;
  RooRealProxy dg1;
  
  LimitType type_;

  double SM_integral;
  std::vector<double> integral_basis;

  TString profileFilename;
  
  TProfile2D ** P_dk; //!
  TProfile2D ** P_dg1; //!
  TProfile2D ** P_dkdg1; //!
  
  void initializeProfiles();
  void initializeNormalization(const RooAbsReal& dep,
			       const RooAbsReal& shape);
  void readProfiles(RooATGCProcessScaling const& other);
  
  virtual double evaluate() const ;
  
private:
  
  ClassDef(RooATGCProcessScaling, 3) // aTGC function 
};

#endif
