// -*- mode: c++ -*-

#ifndef ROOPHOTOSPLINE
#define ROOPHOTOSPLINE

#include "RooRealProxy.h"
#include "RooSetProxy.h"
#include "RooAbsReal.h"
#include "TProfile2D.h"
#include "TString.h"
  
#include <vector>

struct splinetable;

class RooPhotospline : public RooAbsReal {
public:
  RooPhotospline ();
  RooPhotospline (const char * name, const char * title,
		     RooArgSet& _params,
		     const char * splinefile,
		     int monodim);
  RooPhotospline (const RooPhotospline& other, const char * name);
  virtual TObject * clone(const char * newname) const { 
    return new RooPhotospline(*this, newname);
    }
  
  virtual ~RooPhotospline ();
  
  Int_t getAnalyticalIntegral(RooArgSet& allVars, 
			      RooArgSet& analVars, 
			      const char* rangeName = 0) const;

  Double_t analyticalIntegral(Int_t code, const char* rangeName = 0) const;

  
  TString getSplineFile() const { return _splinefile; }
  
protected:
  
  RooSetProxy  _params;
    
  TString _splinefile, _mononame;

  int _monodim;
    
  splinetable* _table; //!
  
  virtual double evaluate() const ;
  
private:
  
  ClassDef(RooPhotospline, 1) // spline function
};

#endif
