#define AnaRun2_cxx
#include "AnaRun2.h"

AnaRun2::AnaRun2(TTree * tree) : fChain(0) {
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
  if (tree == 0) {
    TFile * f = (TFile *) gROOT->GetListOfFiles()->FindObject("./example.root");
    if (!f || !f->IsOpen()) {
      f = new TFile("./example.root");
    }
    f->GetObject("DataTree", tree);

  }
  Init(tree);
}

AnaRun2::~AnaRun2() {
  if (!fChain) {
    return;
  }
  delete fChain->GetCurrentFile();
}

Int_t AnaRun2::GetEntry(Long64_t entry) {
// Read contents of entry.
  if (!fChain) {
    return 0;
  }
  return fChain->GetEntry(entry);
}

Long64_t AnaRun2::LoadTree(Long64_t entry) {
// Set the environment to read one entry
  if (!fChain) {
    return -5;
  }
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) {
    return centry;
  }
  if (fChain->GetTreeNumber() != fCurrent) {
    fCurrent = fChain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void AnaRun2::Init(TTree * tree) {
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).

  // Set branch addresses and branch pointers
  if (!tree) {
    return;
  }
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  TObjArray *list = fChain->GetListOfBranches();

  std::vector<std::string> BranchName;

  /*for(int i = 0; i < list->GetEntries(); ++i){

    BranchName.push_back(list->At(i)->GetName());
    fChain->SetBranchAddress((list->At(i)->GetName()).Data(), TimeChannel1, &b_TimeChannel1);

  }*/

  fChain->SetBranchAddress("TimeChannel1", TimeChannel1, &b_TimeChannel1);
  fChain->SetBranchAddress("TimeChannel2", TimeChannel2, &b_TimeChannel2);
  fChain->SetBranchAddress("TimeChannel3", TimeChannel3, &b_TimeChannel3);
  fChain->SetBranchAddress("TimeChannel4", TimeChannel4, &b_TimeChannel3);
  fChain->SetBranchAddress("WaveformChannel1", WaveformChannel1, &b_WaveformChannel1);
  fChain->SetBranchAddress("WaveformChannel2", WaveformChannel2, &b_WaveformChannel2);
  fChain->SetBranchAddress("WaveformChannel3", WaveformChannel3, &b_WaveformChannel3);
  fChain->SetBranchAddress("WaveformChannel4", WaveformChannel4, &b_WaveformChannel4);
  Notify();

}

Bool_t AnaRun2::Notify() {
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.

  return kTRUE;
}

void AnaRun2::Show(Long64_t entry) {
// Print contents of entry.
// If entry is not specified, print current entry
  if (!fChain) {
    return;
  }
  fChain->Show(entry);
}

Int_t AnaRun2::Cut(Long64_t entry) {
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
  return 1;
}

void AnaRun2::Loop() {
//   In a ROOT session, you can do:
//      Root > .L AnaRun2.C
//      Root > AnaRun2 t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
// by  b_branchname->GetEntry(ientry); //read only this branch
  if (fChain == 0) {
    return;
  }

  // Initiate variables
  TimeData.clear();
  WaveData.clear();

  ofstream myfile;
  myfile.open("example.txt");
  int count = 0;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry = 0; jentry < nentries; jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) {
      break;
    }
    nb = fChain->GetEntry(jentry);
    nbytes += nb;

    // if (Cut(ientry) < 0) continue;

    // Event loop - Code goes here
    std::vector<double> dummyVec1;
    dummyVec1.clear();

    std::vector<double> dummyVec2;
    dummyVec2.clear();

    for (int i = 0; i < 1024; i++) {
      dummyVec1.push_back(TimeChannel1[i]);
      dummyVec2.push_back(WaveformChannel1[i]);
    }

    TimeData.push_back(dummyVec1);
    WaveData.push_back(dummyVec2);

  }
  return;
}

void AnaRun2::NormalizeByWaveformArea() {
  std::cout << "Size: " << TimeData.size() << std::endl;
  for (int i = 0; i < TimeData.size(); i++) {
    double waveformArea = 0;
    for (int j = 0; j < 1023; j++) {
      waveformArea += WaveData.at(i).at(j) * (TimeData.at(i).at(j + 1) - TimeData.at(i).at(j));
    }
    for (int j = 0; j < 1024; j++) {
      WaveData.at(i).at(j) = WaveData.at(i).at(j) / waveformArea;
    }
  }

  return;
}

void AnaRun2::FeatureScaling() {
  for (int i = 0; i < 1024; i++) {
    double waveMean = 0;
    double waveSigma = 0;
    for (int j = 0; j < TimeData.size(); j++) {
      waveMean += WaveData.at(j).at(i);
    }
    waveMean = waveMean / TimeData.size();
    for (int j = 0; j < TimeData.size(); j++) {
      waveSigma += std::pow(WaveData.at(j).at(i) - waveMean, 2);
    }
    waveSigma = std::sqrt(waveSigma / TimeData.size());
    for (int j = 0; j < TimeData.size(); j++) {
      WaveData.at(j).at(i) = (WaveData.at(j).at(i) - waveMean) / waveSigma;
    }
  }

  return;

}

std::pair<double,double> AnaRun2::GetBaseline(std::vector<double> waveformArray) {

    for (int j = 0; j < 1024; j++) {

      hBaseline->Fill(waveformArray.at(j));

    }

  double BL = hBaseline->GetBinCenter(hBaseline->GetMaximumBin());
  double BLRMS = hBaseline->GetRMS();

  return std::make_pair(BL,BLRMS);

}

bool AnaRun2::PeakFinder(std::vector<double> waveformArray, std::vector<double> timeArray) {
  bool FoundPeak = false;

  for (int i = 2; i < waveformArray.size() - 2; i++) {
    if (FivePointStencil(waveformArray, timeArray, i) < -3.5E-3) {
      if (waveformArray.at(i) < (BaselineMean - 20 * BaselineRMS)) {
        if ((i > 300) && (i < 800)) {

          FoundPeak = true;
          CurrentPeakLocation = i;
          return FoundPeak;

        }
      }
    }
  }

  return FoundPeak;

}

bool AnaRun2::CutOffWaveformFinder(std::vector<double> waveformArray) {
  bool NoCutOff = true;

  for (int i = 200; i < 800; i++) {
    if (waveformArray.at(i) <= -0.49) {
      // std::cout << "Waveform being cut-off." << std::endl;
      NoCutOff = false;
      return NoCutOff;
    }
  }

  return NoCutOff;
}

double AnaRun2::FivePointStencil(std::vector<double> aVector, std::vector<double> bVector, int i) {

  double dev = (-aVector.at(i + 2) + 8 * aVector.at(i + 1) - 8 * aVector.at(i - 1) + aVector.at(i - 2)) / (3 * (bVector.at(i + 2) - bVector.at(i - 2)));

  return dev;

}

double AnaRun2::WaveformArea(std::vector<double> aVector, std::vector<double> bVector, int ia) {
  double waveformArea = 0;

  for (int i = ia - 10; i < 1023; i++) {
    waveformArea += (aVector.at(i) - BaselineMean) * (bVector.at(i + 1) - bVector.at(i));
  }

  return waveformArea;
}

double AnaRun2::WavetailArea(std::vector<double> aVector, std::vector<double> bVector, int ia) {

  double wavetailArea = 0;

  for (int i = ia + 80; i < 1023; i++) {

    wavetailArea += (aVector.at(i) - BaselineMean) * (bVector.at(i + 1) - bVector.at(i));

  }

  return wavetailArea;

}

double AnaRun2::WaveAmplitude(std::vector<double> aVector, std::vector<double> bVector, int ia) {
  double waveAmplitude = 0;

  for (int i = ia; i < ia + 200; i++) {
    if (aVector.at(i) < waveAmplitude) {
      if ((FivePointStencil(aVector, bVector, i - 5) * FivePointStencil(aVector, bVector, i + 5)) < 0) {

        waveAmplitude = aVector.at(i);
        return waveAmplitude;

      }
    }
  }

  return 0;

}

void AnaRun2::PSD(int nentries) {

  if(nentries == -1){

    nentries = TimeData.size();

  }

  //gStyle->SetOptStat(0);
  TH1D * hAreaRatio = new TH1D("hAreaRatio", "Neutron/Gamma PSD using Charge Comparison method", 500,0.0, 0.8);
  TH1D * hAmplitudeRatio = new TH1D("hAmplitudeRatio", "Neutron/Gamma PSD using Amplitude Comparison method", 500, 0.0, 0.6);
  TH2D * hPSD = new TH2D("hPSD", "Neutron/Gamma PSD using Multivariate method", 100, 0, 0.6, 100, 0., 0.15);
  hBaseline = new TH1D("hBaseline", "Baseline", 10000, -0.01, 0.01);

  std::pair<double,double> bl;

  for (int i = 0; i < nentries; i++) { //i < TimeData.size()

    bl = GetBaseline(WaveData.at(i));
    BaselineMean = bl.first;
    BaselineRMS = bl.second;

    if (PeakFinder(WaveData.at(i), TimeData.at(i)) && CutOffWaveformFinder(WaveData.at(i))) {
      hAreaRatio->Fill(WavetailArea(WaveData.at(i), TimeData.at(i), CurrentPeakLocation) / WaveformArea(WaveData.at(i), TimeData.at(i), CurrentPeakLocation));
      hAmplitudeRatio->Fill(WaveAmplitude(WaveData.at(i), TimeData.at(i), CurrentPeakLocation) / WaveformArea(WaveData.at(i), TimeData.at(i), CurrentPeakLocation));
      hPSD->Fill(WavetailArea(WaveData.at(i), TimeData.at(i), CurrentPeakLocation) / WaveformArea(WaveData.at(i), TimeData.at(i), CurrentPeakLocation), WaveAmplitude(WaveData.at(i), TimeData.at(i), CurrentPeakLocation) / WaveformArea(WaveData.at(i), TimeData.at(i), CurrentPeakLocation));

      /*std::cout << std::endl;
      std::cout << "Baseline is " << BaselineMean << std::endl;
      std::cout << "Baseline RMS is" << BaselineRMS << std::endl;
      std::cout << "CurrentPeakLocation is " << CurrentPeakLocation << std::endl;
      std::cout << "Wave Amplitude is " << WaveAmplitude(WaveData.at(i), TimeData.at(i), CurrentPeakLocation) << std::endl;
      std::cout << "Waveform Area is " << WaveformArea(WaveData.at(i), TimeData.at(i), CurrentPeakLocation) << std::endl;
      std::cout << "Tail Area is " << WavetailArea(WaveData.at(i), TimeData.at(i), CurrentPeakLocation) << std::endl;
      std::cout << "********************************" << std::endl;*/

    }

    hBaseline->Reset();

  }

  std::cout << nentries << " events analyzed." << std::endl;

  TFile * OutFile = new TFile("./example_Analyzed.root", "RECREATE");
  hAreaRatio->Write();
  hAmplitudeRatio->Write();
  hPSD->Write();
  OutFile->Write();

  return;
}
