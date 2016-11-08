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
// create histograms
  TH1F *WVH = new TH1F("WVH","Waveoform histogram", 100,-0.5,0.02);
  TH2D *hAllWaveforms = new TH2D("hAllWaveforms","hAllWaveforms",1024,0,1024,1024,-0.05,0.5);
  TH2F *PSD1 = new TH2F("MH","Height/DecayTime vs TotalArea",1000,-0.012,0,1000,-11,-4);
  TH1F *FOMPSD2 = new TH1F("FOMPSD2","TotalArea/(Log(PulseHeight/TotalArea - Constnat))",10000,0.15,0.8); 
  TH1F *FOMPSD1 = new TH1F("FOMPSD1","TotalArea/(PulseHeight/DecayTime)",1000,20,80);
  TH2F *PSD2 = new TH2F("PSD2","PulseHeight/TotalArea vs Total Area",1000,-11,0,1000,0,0.05);
  TH2F *LogPSD2 = new TH2F("LogPSD2","Log(PulseHeight/TotalArea - Constant) vs Total Area",1000,-11,0,1000,-8,-3); 

 //read time header
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
  for (n=0 ; n<150000; n++) {
    // read event header
    i = fread(&eh, sizeof(eh), 1, f);
    if (i < 1)
      break;
         
   //printf("\nFound event #%d\n", eh.event_serial_number);

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
        temp=(waveform[0][i]);
}
}
  //Skip events with Overshoot
	bool skip= true;
        for(i=0 ; i<1024; i++) {
                if ((waveform[0][i])> -temp/100){
		skip=false;
		break;
}}
   if (skip == false) continue;
   
   // fill graph and Create Histogams
    for (i=0 ; i<1024 ; i++) {
      hAllWaveforms->Fill(time[0][i],(waveform[0][i]));
      WVH->Fill(waveform[0][i]); 
 }
 /*//Find the peak width
  double delta = 0;
  double old_delta = abs(temp/2 - (waveform[0][0]));
  int index;
  for(i=Maxx ; i<1024 ; i++){
	delta =abs(temp/2 - (waveform[0][i]));
	if(delta<old_delta){
	old_delta=delta;
	index = i;
}
}
*/  //Find the Base value on the other side of Maxx
  double delta1;
  double old_delta1 = abs(temp/10 -  waveform[0][0]);
  int index1;
  for(i=Maxx; i<1024 ; i++){
        delta1 =abs(temp/10 - (waveform[0][i]));
        if(delta1<old_delta1){
        old_delta1=delta1;
        index1 = i;
}
}
 int binmax = WVH->GetMaximumBin();
 double BL = WVH->GetXaxis()->GetBinCenter(binmax); 
 double TailStarts = index1;
  double tailintegral=0.0;
  double Total =0.0;
  double sum[900];
  //Compute each integral rectangle
  for(int i = 0 ; i < 900 ; i++){
    sum[i] = (waveform[0][i]-BL)*((time[0][i]-time[0][i-1])/2.0+(time[0][i+1]-time[0][i])/2.0);
  }
  //Sum all before the tail
  for(int i = 0 ; i < TailStarts ; i++){
    Total += sum[i];
  } 
  //Sum the tail
  for(int i = TailStarts ; i < 900 ; i++){
    tailintegral += sum[i];
  }
  //Finish total computation
  Total += tailintegral;
        
//Skip positive Charge Count
	if(Total <-11) continue;
	if(Total > 0) continue;
  //double ChargeCount = Total;
  //double TailCharge = tailintegral;
  //double ChargeRatio=  (tailintegral/Total); 
  double DT = index1 - Maxx;
  //double FWHMoverTA = PulseWidth/ChargeCount; 
  //double HoverPA = temp/ChargeCount;
  double HoverTA = temp/Total;
  double MHValue = temp/DT;
  //double FastCharge = ChargeCount - TailCharge ;
  //double SFRatio = TailCharge/FastCharge ;
  //double FCRatio = FastCharge/ChargeCount ; 
  double lHoverTA = log(abs(HoverTA-0.0447282));
  //If Charge Ratio > 1 skip
	//if(ChargeRatio > 1) continue;
  double FOM1 = Total/HoverTA;
  double FOM2 = Total/lHoverTA ; 
  PSD1->Fill(MHValue,Total);
  LogPSD2->Fill(Total,lHoverTA);
  PSD2->Fill(Total,HoverTA);
  FOMPSD1->Fill(FOM1);
  FOMPSD2->Fill(FOM2);
  /*//Distinguish Particle
  //Obtain the line that divides the two populations 
  double a1=-0.03365;
  double b1=-0.01295;
  double c1=-0.00062;
  double a2=-0.0377124;
  double b2=-0.0128;
  double c2=-0.000624129;
  double a3=-0.041;
  double b3=-0.0124;
  double c3=-0.0006243;
  double CutValue1 = a1 + a1*ChargeCount + c1*(ChargeCount)*(ChargeCount);
  double CutValue2 = a2 + b2*ChargeCount + c2*(ChargeCount)*(ChargeCount);
  double CutValue3 = a3 + b3*ChargeCount + c3*(ChargeCount)*(ChargeCount);
  double Charge = 0.0;
	bool Neutron = false; 
 	if(CutValue1>HoverTA && CutValue2<HoverTA){ 
		Neutron = true;
}
	if(Neutron == true){
		HRN->Fill(HoverTA);
		PHvPAN->Fill(ChargeCount,HoverTA);
     	   for (i=0 ; i<950 ; i++) {
      hAllNeutrons->Fill(time[0][i],waveform[0][i]);
      Charge = Charge +((waveform[0][i]-BL)*(waveform[0][i]-BL))*((time[0][i]-time[0][i-1])/2.0+(time[0][i+1]-time[0][i])/2.0);
}   
             CCN->Fill(Charge);
                CC->Fill(Charge);
}
	
	bool Gamma = false;
	if(CutValue2>HoverTA && CutValue3<HoverTA){
        	Gamma = true;
}
	if(Gamma == true){ 

		HRG->Fill(HoverTA);
		PHvPAG->Fill(ChargeCount,HoverTA);
	   for (i=0 ; i<950 ; i++) {
      hAllGammas->Fill(time[0][i],waveform[0][i]);
      Charge = Charge + ((waveform[0][i]-BL)*(waveform[0][i]-BL))*((time[0][i]-time[0][i-1])/2.0+(time[0][i+1]-time[0][i])/2.0);
} 
		double ChargeMEV = 90.625*Charge; 
              CCG->Fill(ChargeMEV);
                CC->Fill(Charge);
}
*/	}
  //print number of events
  printf("\n%d events processed, \"%s\" written.\n", n, rootfile);
   
  // save and close root file
  //Save histograms 
  PSD1->Write();
  hAllWaveforms->Write();
  FOMPSD2->Write();
  PSD2->Write();
  LogPSD2->Write();
  FOMPSD1->Write();
  rec->Write();
  outfile->Close();
  
}
