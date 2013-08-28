// -*- mode: c++ -*-

#ifndef ROOATGCSEMIANALYTICPDF
#define ROOATGCSEMIANALYTICPDF

#include "RooRealProxy.h"
#include "RooAbsPdf.h"
#include "TProfile2D.h"
#include "TString.h"
  
class RooATGCSemiAnalyticPdf : public RooAbsPdf {
public:
  
  enum LimitType{ dkglZ, dg1lZ, dkdg1, notype };

  RooATGCSemiAnalyticPdf ();
  RooATGCSemiAnalyticPdf (const char * name, const char * title,
			  RooAbsReal& _x, 
			  RooAbsReal& _dkg, 
			  RooAbsReal& _lZ,
			  RooAbsReal& _dg1,
			  RooAbsReal& _SM_shape,
			  const char * parFilename,
			  const unsigned& lt);
  RooATGCSemiAnalyticPdf (const RooATGCSemiAnalyticPdf& other, const char * name);
  virtual TObject * clone(const char * newname) const { 
    return new RooATGCSemiAnalyticPdf(*this, newname);
    }
  
  virtual ~RooATGCSemiAnalyticPdf ();
  
  void setLimitType(const unsigned& lt) { type_ = (LimitType)lt; }

  Int_t getAnalyticalIntegral(RooArgSet& allVars, 
			    RooArgSet& analVars, 
			    const char* rangeName = 0) const;

  Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

  void readProfiles(TDirectory& dir) const ;
  TString getProfileFilename() const { return profileFilename; }
  
protected:
  
  RooRealProxy x;
  RooRealProxy lZ;
  RooRealProxy dkg;
  RooRealProxy dg1;
  RooRealProxy SM_shape;
  
  LimitType type_;

  mutable std::map<std::string,std::vector<double> > integral_basis;

  TString profileFilename;
  
  TProfile2D ** P_dk; //!
  TProfile2D ** P_dg1; //!
  TProfile2D ** P_dkdg1; //!
  
  void initializeProfiles();
  void initializeNormalization(const std::string& rName,
			       const RooAbsReal& dep,
			       const RooAbsReal& shape) const;
  void readProfiles(RooATGCSemiAnalyticPdf const& other);
  
  virtual double evaluate() const ;
  
private:
  
  ClassDef(RooATGCSemiAnalyticPdf, 2) // aTGC function 
};

#endif
