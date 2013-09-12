// -*- mode: c++ -*-

#ifndef ROOPHOTOSPLINEPDF
#define ROOPHOTOSPLINEPDF

#include "RooRealProxy.h"
#include "RooSetProxy.h"
#include "RooAbsPdf.h"
#include "TProfile2D.h"
#include "TString.h"
  
struct splinetable;

class RooPhotosplinePdf : public RooAbsPdf {
public:
  RooPhotosplinePdf ();
  RooPhotosplinePdf (const char * name, const char * title,
		     RooArgSet& _params,
		     const char * splinefile,
		     int monodim);
  RooPhotosplinePdf (const RooPhotosplinePdf& other, const char * name);
  virtual TObject * clone(const char * newname) const { 
    return new RooPhotosplinePdf(*this, newname);
    }
  
  virtual ~RooPhotosplinePdf ();
  
  Int_t getAnalyticalIntegral(RooArgSet& allVars, 
			      RooArgSet& analVars, 
			      const char* rangeName = 0) const;

  Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

  
  TString getSplineFile() const { return _splinefile; }
  
protected:
  
  RooSetProxy  _params;
  
  TString _splinefile;

  int _monodim;
    
  splinetable* _table; //!
  
  virtual double evaluate() const ;
  
private:
  
  ClassDef(RooPhotosplinePdf, 1) // aTGC function 
};

#endif
