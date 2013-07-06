/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "HiggsAnalysis/CombinedLimit/interface/RooATGCProcessScaling.h" 

#include <math.h> 
#include "TMath.h" 
#include "RooFormulaVar.h"

#include "TFile.h"

ClassImpUnique(RooATGCProcessScaling,MAGICWORDOFSOMESORT) 

RooATGCProcessScaling::RooATGCProcessScaling() : 
  P_dk(0), P_dg1(0)
{
  initializeProfiles();
}

RooATGCProcessScaling::RooATGCProcessScaling(const char *name, 
					     const char *title,
					     RooAbsReal& _x,
					     RooAbsReal& _dkg,
					     RooAbsReal& _lZ,
					     RooAbsReal& _dg1,
					     RooAbsReal& _SM_shape,
					     const char * parFilename) :
   RooAbsReal(name,title),
   lZ("lZ","lZ",this,_lZ),
   dkg("dkg","dkg",this,_dkg),
   dg1("dg1","dg1",this,_dg1),
   profileFilename(parFilename),
   P_dk(0), P_dg1(0)
{ 
  initializeProfiles();
  initializeNormalization(_x,_SM_shape);
  const char* pwd = gDirectory->GetPath();
  TFile *f = TFile::Open(parFilename,"READ");  
  gDirectory->cd(pwd);
  std::cout << "opened the input file!" << std::endl;
  readProfiles(*f);
  f->Close();
} 

RooATGCProcessScaling::
RooATGCProcessScaling(const RooATGCProcessScaling& other, 
		      const char* name) :  
  RooAbsReal(other,name),
  lZ("lZ",this,other.lZ),
  dkg("dkg",this,other.dkg),
  dg1("dg1",this,other.dg1),
  SM_integral(other.SM_integral),
  integral_basis(other.integral_basis),
  profileFilename(other.profileFilename),
  P_dk(0), P_dg1(0)
{ 
  initializeProfiles();
  const char* pwd = gDirectory->GetPath();
  TFile *f = TFile::Open(profileFilename,"READ");  
  gDirectory->cd(pwd);  
  readProfiles(*f);
  f->Close();
} 

void RooATGCProcessScaling::initializeProfiles() {
  P_dk = new TProfile2D*[7]();
  P_dg1 = new TProfile2D*[7]();
}

void RooATGCProcessScaling::initializeNormalization(const RooAbsReal& dep,
						    const RooAbsReal& shape) {
  RooAbsReal* integral = shape.createIntegral(RooArgSet(dep),RooArgSet());
  SM_integral = integral->getVal();
  delete integral;
  for( int i = 0; i<=6; ++i ) {
    RooFormulaVar temp("temp","integral of x^i * shape",
		       Form("@1*@0**%i",i),RooArgList(dep,shape));
    integral = temp.createIntegral(RooArgSet(dep),RooArgSet());
    integral_basis.push_back(integral->getVal());
    delete integral;
  }
}

void RooATGCProcessScaling::readProfiles(TDirectory& dir) const {

  int i;
  for(i=0; i<=6; ++i) {
    
    if (P_dk[i]) delete P_dk[i];
    TString dkname = TString::Format("p%i_lambda_dkg", i);
    P_dk[i] = dynamic_cast<TProfile2D *>(dir.Get(dkname)->Clone(dkname+"new"));
    P_dk[i]->SetDirectory(0);
    if (P_dg1[i]) delete P_dg1[i];
    TString dg1name = TString::Format("p%i_lambda_dg1", i);
    P_dg1[i] = dynamic_cast<TProfile2D *>(dir.Get(dg1name)->Clone(dg1name+"new"));
    P_dg1[i]->SetDirectory(0);
  }

  // for (i=0; i<=6; i++) {
  //   std::cout << 'P' << i << "_dk " << P_dk[i]->GetName() << '\n';
  // }
}

void RooATGCProcessScaling::readProfiles(RooATGCProcessScaling const& other) {

  for (int i = 0; i<=6; ++i) {
    std::cout << other.P_dk[i] << std::endl;
    std::cout << other.P_dg1[i] << std::endl;
    TString dkname = TString::Format("p%i_lambda_dkg", i);
    P_dk[i] = dynamic_cast<TProfile2D *>(other.P_dk[i]->Clone(dkname+"new"));
    P_dk[i]->SetDirectory(0);
    TString dg1name = TString::Format("p%i_lambda_dg1", i);
    P_dg1[i] = dynamic_cast<TProfile2D *>(other.P_dg1[i]->Clone(dg1name+"new"));
    P_dg1[i]->SetDirectory(0);
  }
}

RooATGCProcessScaling::~RooATGCProcessScaling() {
  for(int i = 0; i<7; ++i) {
    if (P_dk[i])
      delete P_dk[i];
    if (P_dg1[i])
      delete P_dg1[i];
  }
  delete[] P_dk;
  delete[] P_dg1;
}

Double_t RooATGCProcessScaling::evaluate() const 
{ 
  TProfile2D ** P = P_dg1;
  double v1(lZ), v2(dg1);
  if(TMath::Abs(dg1)<0.000001) {
    P = P_dk;
    v2 = dkg;
  }

  if (not P[0]) {
    TFile f(profileFilename);
    readProfiles(f);
    f.Close();
  }

  if (v1 < P[0]->GetXaxis()->GetXmin())
    v1 = P[0]->GetXaxis()->GetXmin();
  if (v1 > P[0]->GetXaxis()->GetXmax())
    v1 = P[0]->GetXaxis()->GetXmax();
  if (v2 < P[0]->GetYaxis()->GetXmin())
    v2 = P[0]->GetYaxis()->GetXmin();
  if (v2 > P[0]->GetYaxis()->GetXmax())
    v2 = P[0]->GetYaxis()->GetXmax();

  double ret(0.);
  for(int i = 0; i<= 6; i++) {
    ret += P[i]->Interpolate(v1,v2)*integral_basis[i];
  }
  return ret/SM_integral;
}

