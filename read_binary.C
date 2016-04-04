/*
 
  This macro was adapted from a macro written by Stefan Ritt of PSI which can be
  found here:
  https://midas.psi.ch/elogs/DRS4+Forum/361

  Primary Author: Saad Khan saadakhan@utexas.edu
  Other Author(s): Will Flanagan will.flanagan@utexas.edu

  You can always pull the latest version of this code with:
  git clone https://github.com/UTKLgroup/DRS4macros

  root [0] .L read_binary.C+
  root [1] decode("test.dat");
  - The Histogram disappears once the file is saved. Type in the following to
  make the histogram again.
  root [2] TFile MyFile("test.root")
  root [3] h1->Draw()

  To do:
  [ ] Change variable names to something more illuminating
  [ ] Continue to add comments
  [ ] Track down seg fault behavior
  [ ] Show 'all waveform' plot on a separate canvas

  Thou shalt not use tabs.

*/
 
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include <string.h>
#include <stdio.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "Getline.h"
#include <stdlib.h>
#include <iostream>
using namespace std;

typedef struct {
  char           time_header[4];
  char           bn[2];
  unsigned short board_serial_number;
} THEADER;

typedef struct {
  char           event_header[4];
  unsigned int   event_serial_number;
  unsigned short year;
  unsigned short month;
  unsigned short day;
  unsigned short hour;
  unsigned short minute;
  unsigned short second;
  unsigned short millisecond;
  unsigned short reserved1;
  char           bs[2];
  unsigned short board_serial_number;
  char           tc[2];
  unsigned short trigger_cell;
} EHEADER;

/*-----------------------------------------------------------------------------*/

void decode(char *filename) {
  THEADER th;
  EHEADER eh;
  char hdr[4];
  unsigned short voltage[1024];
  double waveform[4][1024], time[4][1024];
  float bin_width[4][1024];
  char rootfile[256];
  int i, j, ch, n, chn_index;
  double t1, t2, dt, BL;

  // open the binary waveform file
  FILE *f = fopen(Form("%s", filename), "r");
  if (f == NULL) {
     printf("Cannot find file \'%s\'\n", filename);
  return;
  }

  //open the root file
  strcpy(rootfile, filename);
  if (strchr(rootfile, '.'))
    *strchr(rootfile, '.') = 0;
  strcat(rootfile, ".root");
  TFile *outfile = new TFile(rootfile, "RECREATE");
   
  // define the rec tree
  TTree *rec = new TTree("rec","rec");
  rec->Branch("t1", time[0]     ,"t1[1024]/D");  
  rec->Branch("t2", time[1]     ,"t2[1024]/D");  
  rec->Branch("t3", time[2]     ,"t3[1024]/D");  
  rec->Branch("t4", time[3]     ,"t4[1024]/D");  
  rec->Branch("w1", waveform[0] ,"w1[1024]/D");
  rec->Branch("w2", waveform[1] ,"w2[1024]/D");
  rec->Branch("w3", waveform[2] ,"w3[1024]/D");
  rec->Branch("w4", waveform[3] ,"w4[1024]/D");
   
  // create canvas
  TCanvas *c1 = new TCanvas();
   
  // create graph
  TGraph *g = new TGraph(1024, (double *)time[0], (double *)waveform[0]);

  // create histograms

  TH1F *h1 = new TH1F("h1","Integral V.B.W",1000,-50,200);
  TH2D *hAllWaveforms = new TH2D("hAllWaveforms","hAllWaveforms",1024,0,1463,100,-0.1,0.05);
  TH2D *ghistogram = new TH2D("ghistogram","ghistogram",1024,0,200,100,-0.05,0.025);
  TH1F *WVH = new TH1F("WVH","Maximum Waveform Height", 10000,-0.02,0.02);
  TH1F *Tail = new TH1F("Tail","Area of the Tail",1000,-1,1);
  TH1F *TT = new TH1F("Tail/Total","Ratio of Tail over total area",1000,-1,3);
  TH2F *MH = new TH2F("MH","Tail vs Total",1000,-20,4,1000,-20,5);
  TH2F *WVA = new TH2F("WVA","Width vs Total Area",1000,-30,4,1000,0,150);
  TH1F *FWHMTA = new TH1F("FWHMTA","Full Width Half Mass/Total Area",400,-100,100);
  TH1F *HR = new TH1F("HR","Height ratio",180,-0.3,0.3);
  TH2F *PHvPA = new TH2F("PHvPA","Pulse height vs Pulse Area",1000,-20,0,1000,-0.3,0);
  // read time header
  fread(&th, sizeof(th), 1, f);
  printf("Found data for board #%d\n", th.board_serial_number);

  // read time bin widths
  memset(bin_width, sizeof(bin_width), 0);
  for (ch=0 ; ch<5 ; ch++) {
    fread(hdr, sizeof(hdr), 1, f);
    if (hdr[0] != 'C') {
      // event header found
      fseek(f, -4, SEEK_CUR);
      break;      
    }
    i = hdr[3] - '0' - 1;
    printf("Found timing calibration for channel #%d\n", i+1);
    fread(&bin_width[i][0], sizeof(float), 1024, f);
  }

  // loop over all events in data file

  for (n=0 ; n<50000 ; n++) {
    // read event header
    i = fread(&eh, sizeof(eh), 1, f);
    if (i < 1)
      break;
         
    printf("\nFound event #%d\n", eh.event_serial_number);

    // reach channel data
    for (ch=0 ; ch<5 ; ch++) {
      i = fread(hdr, sizeof(hdr), 1, f);
      if (i < 1)
        break;
      if (hdr[0] != 'C') {
        // event header found
        fseek(f, -4, SEEK_CUR);
        break;      
      }
      chn_index = hdr[3] - '0' - 1;
      fread(voltage, sizeof(short), 1024, f);
         
      for (i=0 ; i<1024 ; i++) {
        // convert data to volts
        waveform[chn_index][i] = (voltage[i] / 65536. - 0.5);
            
        // calculate time for this cell
        for (j=0,time[chn_index][i]=0 ; j<i ; j++)
          time[chn_index][i] += bin_width[chn_index][(j+eh.trigger_cell) % 1024];            
      }
    }
    
    // align cell #0 of all channels
    t1 = time[0][(1024-eh.trigger_cell) % 1024];
    for (ch=1 ; ch<4 ; ch++) {
      t2 = time[ch][(1024-eh.trigger_cell) % 1024];
      dt = t1 - t2;
      for (i=0 ; i<1024 ; i++)
        time[ch][i] += dt;
    }

    // fill root tree
    rec->Fill();
  //Find Max value 
  int  Maxx;
  double temp=0.0;
  for(i=0; i< 900 ; i++){
        if(temp>waveform[0][i]){
        Maxx = i;
        temp=waveform[0][i];
}
}
  //Skip events with Bipolar waveforms
	bool skip= true;
        for(i=0 ; i<1024; i++) {
                if (waveform[0][i] > -temp/8){
		skip=false;
}}
   if (skip == false) continue;
    // fill graph and Create Histogams
    for (i=0 ; i<1024 ; i++) {
      g->SetPoint(i, time[0][i], waveform[0][i]);
      hAllWaveforms->Fill(time[0][i],waveform[0][i]);
      WVH->Fill(waveform[0][i]);
       
 }
  // Get Baseline   
  BL = WVH->GetMean();
  //Get Baseline from Fit
  TF1 *f1 = new TF1("f1","pol0",0,20);
  g->Fit("f1","R");
  double BL1 = f1->GetParameter(0);
  //Fill another histogram to Find peak value of each event 
  for (i=0; i<1024 ; i++){
    ghistogram->Fill(time[0][i],waveform[0][i]);
  }
  //Find the peak width
  double delta = 0;
  double old_delta = abs(temp/2 - waveform[0][0]);
  int index;
  for(i=Maxx ; i<1024 ; i++){
	delta =abs(temp/2 - waveform[0][i]);
	if(delta<old_delta){
	old_delta=delta;
	index = i;
}
}
  //Find the Base value on the other side of Maxx
  double delta1;
  double old_delta1 = abs(temp/2 - waveform[0][0]);
  int index1;
  for(i=0 ; i<Maxx ; i++){
        delta1 =abs(temp/2 - waveform[0][i]);
        if(delta1<old_delta1){
        old_delta1=delta1;
        index1 = i;
}
}

  //Print Max value and corresponding index
  cout<<"index1: "<<index1 <<endl;
  cout<<"index: "<<index <<endl;
  cout<<"temp: "<<temp <<endl;
  cout<<"Max x-value: "<<Maxx <<endl;
  //Define index value where the tail starts
  double TailStarts = index;
  //Print Tail Start point
  WVH->Reset();
  ghistogram->Reset();
  //Define Integral Parameters 
  double gIntegralVBW2=0.0;
  double tailintegral=0.0;
    for(int i = 1 ; i < 900 ; i++) {
      gIntegralVBW2=gIntegralVBW2+(waveform[0][i]-BL1)*((time[0][i]-time[0][i-1])/2.0+(time[0][i+1]-time[0][i])/2.0);
             }
  //Tail integral 
    for(int i=TailStarts ; i < 900 ; i++) {
      tailintegral=tailintegral+(waveform[0][i]-BL1)*((time[0][i]-time[0][i-1])/2+(time[0][i+1]-time[0][i])/2.0);
	}
	
  double ChargeCount = gIntegralVBW2;
  double TailCharge = tailintegral;
  double ChargeRatio =0.0;
  ChargeRatio= (TailCharge/ChargeCount); 
  double PulseWidth = index-index1;
  double FWHMoverTA = PulseWidth/ChargeCount; 
  double HoverTA = temp/ChargeCount;
  cout<<"PulseWidth: "<<PulseWidth <<endl;
  cout<<"ChargeCount: "<<ChargeCount <<endl;
  cout<<"TailCharge: "<<TailCharge <<endl;
  cout<<"ChargeRatio: "<<ChargeRatio <<endl;

  // Fill Histogram
  h1->Fill(ChargeCount);
  TT->Fill(ChargeRatio);
  Tail->Fill(TailCharge);
  WVA->Fill(ChargeCount,PulseWidth);
  MH->Fill(ChargeCount,TailCharge);
  FWHMTA->Fill(FWHMoverTA); 
  PHvPA->Fill(ChargeCount,temp);
  HR->Fill(HoverTA);  
}
  // print number of events
  printf("\n%d events processed, \"%s\" written.\n", n, rootfile);
   
  // save and close root file
  //Save histograms 
  h1->Write();
  TT->Write();
  Tail->Write();
  MH->Write();
  hAllWaveforms->Write();
  WVA->Write();
  HR->Write();
  PHvPA->Write();
  FWHMTA->Write();
  rec->Write();
  outfile->Close();
  
}
