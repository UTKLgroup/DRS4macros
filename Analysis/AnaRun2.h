// ////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Dec 14 15:47:51 2016 by ROOT version 5.34/36
// from TTree DataTree/DataTree
// found on file: ./07-14-16-drs4run2.root
// ////////////////////////////////////////////////////////

#ifndef AnaRun2_h
#define AnaRun2_h

#include <iostream>
#include <vector>
#include <fstream>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2.h>
#include <TH1.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TLegend.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class AnaRun2 {
public:
  TTree * fChain;            // !pointer to the analyzed TTree or TChain
  Int_t fCurrent;            // !current Tree number in a TChain

  // Declaration of leaf types
  Double_t TimeChannel1[1024];
  Double_t TimeChannel2[1024];
  Double_t TimeChannel3[1024];
  Double_t TimeChannel4[1024];
  Double_t WaveformChannel1[1024];
  Double_t WaveformChannel2[1024];
  Double_t WaveformChannel3[1024];
  Double_t WaveformChannel4[1024];

  // List of branches
  TBranch * b_TimeChannel1;          // !
  TBranch * b_TimeChannel2;          // !
  TBranch * b_TimeChannel3;          // !
  TBranch * b_TimeChannel4;          // !
  TBranch * b_WaveformChannel1;          // !
  TBranch * b_WaveformChannel2;          // !
  TBranch * b_WaveformChannel3;          // !
  TBranch * b_WaveformChannel4;          // !

  AnaRun2(TTree * tree = 0);
  virtual ~AnaRun2();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree * tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);
  virtual void     NormalizeByWaveformArea();
  virtual void     FeatureScaling();
  virtual bool     PeakFinder(std::vector<double>, std::vector<double>);
  virtual double   FivePointStencil(std::vector<double>, std::vector<double>, int);
  virtual void     PSD(int);
  virtual double   WaveformArea(std::vector<double>, std::vector<double>, int);
  virtual double   WavetailArea(std::vector<double>, std::vector<double>, int);
  virtual bool     CutOffWaveformFinder(std::vector<double>);
  virtual double   WaveAmplitude(std::vector<double>, std::vector<double>, int);
  virtual std::pair<double,double> GetBaseline(std::vector<double>);

  std::vector<std::vector<double> > TimeData;
  std::vector<std::vector<double> > WaveData;
  double BaselineMean;
  double BaselineRMS;
  int CurrentPeakLocation;

  TH1D *hBaseline;

};

#endif /* ifndef AnaRun2_h */
