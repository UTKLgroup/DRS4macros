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

#include <cstring> 
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
#include "TH1D.h"
#include "TROOT.h"
#include "TLine.h"

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

  // create canvas
   
   TCanvas *c1 = new TCanvas();
   TCanvas *c2 = new TCanvas();
   
  //open the root file
  strcpy(rootfile, filename);
  if (strchr(rootfile, '.'))
    *strchr(rootfile, '.') = 0;
  strcat(rootfile, ".root");
  TFile *outfile = new TFile(rootfile, "RECREATE");
   
  // define the rec tree

  /*TTree *rec = new TTree("rec","rec");
  rec->Branch("t1", time[0]     ,"t1[1024]/D");  
  rec->Branch("t2", time[1]     ,"t2[1024]/D");  
  rec->Branch("t3", time[2]     ,"t3[1024]/D");  
  rec->Branch("t4", time[3]     ,"t4[1024]/D");  
  rec->Branch("w1", waveform[0] ,"w1[1024]/D");
  rec->Branch("w2", waveform[1] ,"w2[1024]/D");
  rec->Branch("w3", waveform[2] ,"w3[1024]/D");
  rec->Branch("w4", waveform[3] ,"w4[1024]/D");*/
   
  // create graph for plotting individual waveforms
  
  TGraph *g = new TGraph(1024, (double *)time[0], (double *)waveform[0]);
  
  // create histograms

  TH1F *FormArea = new TH1F("FormArea","Waveform Area",1000,-1,10);
  TH2D *hAllWaveforms = new TH2D("hAllWaveforms","All Waveforms",1024,-1,210,100,-0.51,0.05);
  TH1F *TailArea = new TH1F("TailArea","Tail Area",1000,-1,10);
  TH1F *TAoverWA = new TH1F("TAoverWA","Tail Area/Waveform Area",1000,-5,5);
  TH2F *TAvsWA = new TH2F("TAvsWA","Waveform Area vs Tail Area",1000,-1,8,1000,-1,3);
  TH2F *HvsTAoverWA = new TH2F("HvsTAoverWA","Tail Area/Waveform Area vs Pulse Height",1000,-3,3,1000,0,0.55);
  TH2F *FWHMvsWA = new TH2F("FWHMvsWA","Waveform Area vs FWHM",1000,-1,8,1000,-5,25);
  TH1F *FWHMoverWA1 = new TH1F("FWHMoverWA","FWHM/Waveform Area",500,-150,500);
  TH1F *HoverWA1 = new TH1F("HoverWA","Pulse Height/Waveform Area",500,-0.1,1);
  TH2F *PHvWA = new TH2F("PHvWA","Waveform Area vs Pulse Height",50,-2,10,50,0,0.55);
  TH1F *MaxHeight = new TH1F("MaxHeight","Pulse Height",1000,0,0.55);
  TH2F *HoverWAvsWA = new TH2F("HoverWAvsWA","Waveform Area vs Height/Waveform Area",1000,-1,9,1000,0,0.25);
  TH2F *PHvFWHM = new TH2F("PHvFWHM","Pulse Width vs Pulse Height",1000,0,100,1000,0,0.55);
  TH2F *PHvDT = new TH2F("PHvDT","Discharge Time vs Pulse Height",1000,0,50,1000,0,0.55);
  TH1F *WAoverPSD1 = new TH1F("WAoverPSD1", "Waveform Area/(Pulse Height/Discharge Time)",600,-60,1000);
  TH2F *WAvsPSD1 = new TH2F("WAvsPSD1","Waveform Area vs Pulse Height/Discharge Time", 1000,-1,8,1000,0,0.03);
  TH1F *WVH = new TH1F("WVH","Waveform Height", 10000,-0.01,0.01);
  
  /*TH1F *FormArea = new TH1F("FormArea","Waveform Area",1000,-8,3);
  TH2D *hAllWaveforms = new TH2D("hAllWaveforms","All Waveforms",1024,-1,210,100,-0.51,0.05);
  TH1F *TailArea = new TH1F("TailArea","Tail Area",1000,-5,2);
  TH1F *TAoverWA = new TH1F("TAoverWA","Tail Area/Waveform Area",1000,-2,3);
  TH2F *TAvsWA = new TH2F("TAvsWA","Tail Area vs Waveform Area",1000,-8,0,1000,-3,1);
  TH2F *HvsTAoverWA = new TH2F("HvsTAoverWA","Pulse Height vs Tail Area/Waveform Area",1000,-2,3,1000,-0.55,0);
  TH2F *FWHMvsWA = new TH2F("FWHMvsWA","FWHM  vs Waveform Area",1000,-8,1,1000,-5,25);
  TH1F *FWHMoverWA1 = new TH1F("FWHMoverWA","FWHM/Waveform Area",500,-500,150);
  TH1F *HoverWA1 = new TH1F("HoverWA","Pulse Height/Waveform Area",500,-0.1,1);
  TH2F *PHvWA = new TH2F("PHvWA","Pulse Height vs Waveform Area",50,-10,2,50,-0.55,0);
  TH1F *MaxHeight = new TH1F("MaxHeight","Pulse Height",1000,-0.55,0);
  TH2F *HoverWAvsWA = new TH2F("HoverWAvsWA","Height/Waveform Area vs Waveform Area",1000,-9,1,1000,0,1);
  TH2F *PHvFWHM = new TH2F("PHvFWHM","Pulse Height vs Pulse Width",1000,0,100,1000,-0.55,0);
  TH2F *PHvDT = new TH2F("PHvDT","Pulse Height vs Discharge Time",1000,0,25,1000,-0.55,0); 
  TH1F *WAoverPSD1 = new TH1F("WAoverPSD1", "Waveform Area/(Pulse Height/Discharge Time)",600,-60,1000);
  TH2F *WAvsPSD1 = new TH2F("WAvsPSD1","Waveform Area vs Pulse Height/Discharge Time", 1000,-8,1,1000,-0.03,0);
  
  TH2D *ghistogram = new TH2D("ghistogram","ghistogram",1024,0,200,100,-0.05,0.025);*/

  
  // read time header
  fread(&th, sizeof(th), 1, f);
  printf("Found data for board #%d\n", th.board_serial_number);

  // read time bin widths
  memset(bin_width, sizeof(bin_width), 0);
  for (ch=0; ch<5 ; ch++) {
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

  for (n=0; n<1000; n++){
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
        waveform[chn_index][i] = -1*(voltage[i] / 65536. - 0.5);
            
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
    //rec->Fill();


   // Get Baseline   
 
   //double binmax = WVH->GetMaximumBin();
    
   //Get Baseline from Fit

   /*TF1 *f1 = new TF1("f1","pol0",0,5);
   g->Fit("f1","R");
   double BL1 = f1->GetParameter(0);*/
   //Fill another histogram to Find peak value of each event 
   //for (i=0; i<1024 ; i++){
         //ghistogram->Fill(time[0][i],waveform[0][i]);
   //}

    
  //Create Histogam of Voltage Values to Determine Baseline

  for (i=5; i<900; i++) {

    WVH->Fill(waveform[0][i]);

  }

  //Get Baseline

  double BL1 = WVH->GetXaxis()->GetBinCenter(WVH->GetMaximumBin());
    
  //Find Max value 

  int  maxheightindex = 0;
  double maxheight = BL1;

  for(i=5; i< 900 ; i++){

    if(abs(maxheight) < abs(waveform[0][i]-BL1)){

      maxheightindex = i;
      maxheight = waveform[0][i] - BL1;
      
    }
    
  }
  
  //Skip events with Waveforms that have higher than a certain positive voltage relative to V = 0 
  
  bool skip = true;

  for(i=0 ; i<1024; i++) {

    if (waveform[0][i] < -maxheight/8){

          skip=false;
	  
    }
	 
  }
	
  if (skip == false){

    WVH->Reset();
    continue;
    
    }
  
 //***************************************** Section for Finding Pulse Widths and Rise/Fall Times **********************************************  

  //Find the FWHM Value on the Left Side of Main Pulse

  double delta = 0;
  double old_delta = abs(maxheight/2 - (waveform[0][900]-BL1));
  int index = maxheightindex;
  
  for(i=5; i<maxheightindex ; i++){
    
    delta = abs(maxheight/2 - (waveform[0][i]-BL1));
    
	if(delta<old_delta){

	   old_delta=delta;
           index = i;

	}
	
  }

  //Find the FWHM Value on the Right Side of the Main Pulse 
  
  double delta1 = 0;
  double old_delta1 = abs(maxheight/2 - (waveform[0][900]-BL1));
  int index1=maxheightindex;

  for(i=maxheightindex; i<900; i++){

    delta1 =abs(maxheight/2 - (waveform[0][i]-BL1));

    if(delta1<old_delta1){

       old_delta1=delta1;

       index1 = i;	

    }    

  }


  //Find the Discharge Time value to left side of Max Height

  double delta2=0;
  double old_delta2 = abs(maxheight/5 - (waveform[0][900]-BL1));
  int indexDischargeMin = maxheightindex;
  
  for(i = 5; i < maxheightindex; i++){

    delta2 = abs(maxheight/5 - (waveform[0][i]-BL1));
    
    if(delta2<old_delta2){

      old_delta2 = delta2;

      indexDischargeMin = i;

    }

  }
  
  //Find the Discharge Time value to right side of Max Height

  double delta3=0;
  double old_delta3 = abs(maxheight/5 - (waveform[0][900]-BL1));
  int indexDischargeMax = maxheightindex;

  for(i = maxheightindex; i < 900; i++){

    //cout << "Delta 3 is " << delta3 << " and Old Delta 3 is " << old_delta3 << " at iteration " << i << endl;
    delta3 = abs(maxheight/5 - (waveform[0][i]-BL1));

    if(delta3<old_delta3){

      old_delta3 = delta3;

      indexDischargeMax = i;

    }

  }

  //***************************************** END OF  Section for Finding Pulse Widths and Rise/Fall Times **********************************************    


  //Define index value where the tail starts

  double TailStarts = index1;

  //ghistogram->Reset();

  //Define Integral Parameters 

  double gIntegralVBW2 = 0.0;
  double tailintegral = 0.0;

  //Waveform Integral

  for(int i = 5; i < 900; i++) {
    
    gIntegralVBW2 = gIntegralVBW2+(waveform[0][i]-BL1)*( (time[0][i]-time[0][i-1])/2.0 + (time[0][i+1]-time[0][i])/2.0 );

  }
    
  //Tail integral 

  for(int i=TailStarts ; i < 900 ; i++) {

    tailintegral=tailintegral+(waveform[0][i]-BL1)*((time[0][i]-time[0][i-1])/2+(time[0][i+1]-time[0][i])/2.0);

  }

  double DischargeTime = time[0][indexDischargeMax] - time[0][indexDischargeMin]; 
  double WaveformArea  = gIntegralVBW2;
  double TArea  = tailintegral;
  double AreaRatio = TArea/WaveformArea; 
  double PulseWidth = time[0][index1]-time[0][index];
  double FWHMoverWA = PulseWidth/WaveformArea; 
  double HoverWA = maxheight/WaveformArea;
  double PSDv1 = maxheight/DischargeTime;
  double PSDv2 = WaveformArea/PSDv1;
  
  //************************************ Draw Waveforms and Fill Other Histograms **************************************

  if(/*maxheight >= 0.055 && maxheight <= 0.065 &&*/ abs(maxheight) < 0.49 /*&& PSDv2 == 0*/  && WaveformArea > 0 /* && TArea < 0*/  && PSDv2 >= 305){ 

    // Fill Waveform Histograms
    
   for(i=0; i < 1024; i++){

     g->SetPoint(i, time[0][i], -1*waveform[0][i]);
     hAllWaveforms->Fill(time[0][i],-1*waveform[0][i]);
     
   }

   /*   
   //****** The following section can be commented out if an analysis of individual waveforms is not desired ******
   
   cout << "**********************************" << endl;
   cout << endl;
   cout << "Waveform number: " << n << endl;
   cout << "Pulse Width Max (ns): " << time[0][index] << " with index " << index << endl;
   cout << "Pulse Width Min (ns): " << time[0][index1] << " with index " << index1 << endl;
   cout<<"Pulse Width (ns): "<< PulseWidth <<endl;
   cout<<"Waveform Area (V*ns): "<< WaveformArea <<endl;
   cout<<"Tail Area (V*ns): "<< TArea <<endl;
   cout<<"Max Pulse Height (V): " << maxheight << endl;
   cout<<"Max Pulse Height Location (ns): " << time[0][maxheightindex] << " with index " << maxheightindex << endl;
   cout<<"Discharge Time (ns): " << DischargeTime << endl;
   cout<<"Waveform Value at 5 ns (V): " << waveform[0][5] - BL1 << endl;
   cout<<"Min Discharge Time (ns): " << time[0][indexDischargeMin] << " with index " << indexDischargeMin << endl;
   cout<<"Max Discharge Time (ns): " << time[0][indexDischargeMax] << " with index " << indexDischargeMax <<  endl;
   cout<<"Baseline Value (V): " << -1*BL1 << " with bin " << WVH->GetMaximumBin() << endl;
   cout << endl;

   c1->cd();
   g->Draw("acp");
   g->GetXaxis()->SetTitle("Time (ns)");
   g->GetXaxis()->CenterTitle();
   g->GetYaxis()->SetTitle("Voltage (V)");
   g->GetYaxis()->CenterTitle();
   g->GetXaxis()->SetTitleOffset(1);
   g->GetYaxis()->SetTitleOffset(1);
   g->GetXaxis()->SetTitleSize(0.05);
   g->GetYaxis()->SetTitleSize(0.05);
   //g->GetYaxis()->SetRangeUser(-0.5,0.1);
   //g->Draw("acp");
   TLine *line = new TLine(0,-1*BL1,200,-1*BL1);
   line->SetLineColor(kRed);
   line->Draw();
   c1->Update();
   //gPad->WaitPrimitive();

   c2->cd();
   WVH->GetXaxis()->SetTitle("Voltage (V)");
   WVH->GetYaxis()->SetTitle("Counts");
   WVH->GetXaxis()->CenterTitle();
   WVH->GetYaxis()->CenterTitle();
   WVH->GetXaxis()->SetTitleOffset(1);
   WVH->GetYaxis()->SetTitleOffset(1);
   WVH->GetXaxis()->SetTitleSize(0.05);
   WVH->GetYaxis()->SetTitleSize(0.05);
   WVH->Draw();
   c2->Update();

   gPad->WaitPrimitive(); 

  
  //**************** END INDIVIDAUL WAVEFORM SECTION ******************
  
  */
   
  FormArea->Fill(WaveformArea);
  TAoverWA->Fill(AreaRatio);
  TailArea->Fill(TArea);
  HvsTAoverWA->Fill(AreaRatio,maxheight);
  FWHMvsWA->Fill(WaveformArea,PulseWidth);
  TAvsWA->Fill(WaveformArea,TArea);
  FWHMoverWA1->Fill(FWHMoverWA); 
  PHvWA->Fill(WaveformArea,maxheight);

  HoverWA1->Fill(HoverWA);
  MaxHeight->Fill(maxheight);
  HoverWAvsWA->Fill(WaveformArea,HoverWA);

  PHvFWHM->Fill(PulseWidth,maxheight);
  PHvDT->Fill(DischargeTime,maxheight); 
  WAoverPSD1->Fill(PSDv2);
  WAvsPSD1->Fill(WaveformArea,PSDv1);
  
  }

  WVH->Reset();
  //cout << "WVH Histogram was reset" << endl;
  
  }
  
  // print number of events
   printf("\n%d events processed, \"%s\" written.\n", n, rootfile);
   
  // save and close root file
  //Save histograms

  hAllWaveforms->Write();
  
  FormArea->Write();
  TailArea->Write();
  TAvsWA->Write();
  TAoverWA->Write();
  HvsTAoverWA->Write();
  FWHMvsWA->Write();
  FWHMoverWA1->Write();

  MaxHeight->Write();
  PHvWA->Write();
  PHvFWHM->Write();
  HoverWA1->Write();
  HoverWAvsWA->Write();
  
  PHvDT->Write();
  WAoverPSD1->Write();
  WAvsPSD1->Write();
  
  outfile->Close();
  
}

/*int main() {
  //char dummy[22];
  std::string dummy = "07-20-16-drs4run5.dat";
  char *cstr = &dummy[0];
  decode(cstr); //ADD FILE NAME 
  return 0;
  }*/

