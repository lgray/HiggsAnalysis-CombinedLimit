/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "HiggsAnalysis/CombinedLimit/interface/RooPhotosplinePdf.h" 

#include <math.h> 
#include "TMath.h"
 #include "RooFormulaVar.h"

#include "TFile.h"

ClassImpUnique(RooPhotosplinePdf,MAGICWORDOFSOMESORT) 

#include "photospline/splinetable.h"
#include "photospline/bspline.h"

RooPhotosplinePdf::RooPhotosplinePdf() : 
  _monodim(-1),_table(NULL)
{  
}

RooPhotosplinePdf::RooPhotosplinePdf(const char *name, 
				     const char *title, 
				     RooArgSet& params,
				     const char * splinefile,
				     int monodim 
				     ) :
  RooAbsPdf(name,title),  
  _params("deps","deps",this),
  _splinefile(splinefile),
  _monodim(monodim),
  _table(new splinetable())
{  
  int i = 0;  
  RooFIter iset; 
  RooAbsArg* var = NULL;  
  readsplinefitstable(_splinefile.Data(),_table);
  _params.add(params);
  iset = _params.fwdIterator();
  while( ( var = iset.next() ) && i < _table->ndim ) {
    if( i == _monodim ) _mononame = var->GetName();
    ++i;
  }    
} 

RooPhotosplinePdf::RooPhotosplinePdf(const RooPhotosplinePdf& other, 
				     const char* name) :  
  RooAbsPdf(other,name),
  _params("deps",this,other._params),
  _splinefile(other._splinefile),
  _mononame(other._mononame),
  _monodim(other._monodim),
  _table(new splinetable())
{ 
  readsplinefitstable(_splinefile.Data(),_table);
} 

RooPhotosplinePdf::~RooPhotosplinePdf() {
  splinetable_free(_table);
}

Double_t RooPhotosplinePdf::evaluate() const 
{ 
  int i = 0, dims_deriv = 0x0, centers[_table->ndim];
  double x[_table->ndim], ret;
  RooFIter iset = _params.fwdIterator();
  RooAbsArg* var = NULL;
  RooAbsReal* casted = NULL;
  while( ( var = iset.next() ) && i < _table->ndim ) {
    casted = reinterpret_cast<RooAbsReal*>(var);
    if( i == _monodim ) dims_deriv = 1 << i;
    x[i++] = casted->getVal();
  }
  tablesearchcenters(_table, x, centers);
  ret = ndsplineeval(_table, x, centers, dims_deriv);
  return ( ret < 0 ? 1e-6 : ret );
}

Int_t RooPhotosplinePdf::
getAnalyticalIntegral(RooArgSet& allVars,RooArgSet& analVars, 
		      const char* /*rangeName*/) const {  
  if( _monodim != -1 ) {
    RooArgSet monoset(_params[_mononame.Data()]);
    if(matchArgs(allVars, analVars, monoset)) return 1;
  }
  return 0;
}

Double_t RooPhotosplinePdf::
analyticalIntegral(Int_t code, const char* rangeName) const {  
  int i = 0, centers[_table->ndim], centers1[_table->ndim];
  double x[_table->ndim], x1[_table->ndim];
  RooFIter iset = _params.fwdIterator();
  RooAbsArg* var = NULL;
  RooAbsReal* casted = NULL;
  RooAbsRealLValue* lval = NULL;
  switch( code ) {
  case 1: // integration using directly the CDF in one dimension
    while( ( var = iset.next() ) && i < _table->ndim ) {
      if( i == _monodim ) {
	lval   = reinterpret_cast<RooAbsRealLValue*>(var);
	x[i]   = (lval->hasMin() ? lval->getMin() : _table->extents[i][0]);
	x1[i]  = (lval->hasMax() ? lval->getMax() : _table->extents[i][1]);
      } else {
	casted = reinterpret_cast<RooAbsReal*>(var);
	x[i]   = x1[i] = casted->getVal();
      }    
      ++i;
    }  
    tablesearchcenters(_table, x, centers);
    tablesearchcenters(_table, x1, centers1);
    return ( ndsplineeval(_table, x1, centers1, 0) - 
	     ndsplineeval(_table,  x,  centers, 0)   );
  default:
    assert(NULL && "only analytical integration using the CDF is supported");
  }
  return -1;
}
