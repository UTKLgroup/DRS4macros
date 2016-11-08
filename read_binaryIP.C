/*

  This macro was adapted from a macro written by Stefan Ritt of PSI which can be
  found here:
  https://midas.psi.ch/elogs/DRS4+Forum/361

  Primary Author: Saad Khan saadakhan@utexas.edu
  Other Author(s): Will Flanagan will.flanagan@utexas.edu
                 : Abhishek Rajput abhi_rajput5@utexas.edu

  You can always pull the latest version of this code with:
  git clone https://github.com/UTKLgroup/DRS4macros

  root [0] .L read_binary.C+
  root [1] decode("filename.dat");
  - The Histogram disappears once the file is saved. Type in the following to
  make the histogram again.
  root [2] TFile MyFile("filename.root")
  root [3] h1->Draw()

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
#include <sstream>
#include <vector>

using namespace std;

typedef struct {
  char           time_header[4];
  char           bn[2];
  unsigned short board_serial_number;

}THEADER;

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

}EHEADER;

typedef struct {

  char hdr[4];
  unsigned short voltage[1024];
  double waveform[4][1024], time[4][1024];
  float bin_width[4][1024];
  vector<int> inpch;
  
}DATAINFO;

/*-----------------------------------------------------------------------------*/

DATAINFO find_header(FILE *f){

  THEADER th;
  EHEADER eh;
  DATAINFO dat;
  int i, ch, chn_index;

  // read time header

  fread(&th, sizeof(th), 1, f);
  printf("Found data for board #%d\n", th.board_serial_number);

  // read time bin widths

  memset(dat.bin_width, sizeof(dat.bin_width), 0);

  for (ch=0; ch<5 ; ch++) {
    fread(dat.hdr, sizeof(dat.hdr), 1, f);

    if (dat.hdr[0] != 'C') {

      // event header found
      fseek(f, -4, SEEK_CUR);
      break;
      
    }

    i = dat.hdr[3] - '0' - 1;
    printf("Found timing calibration for channel #%d\n", i+1);
    fread(&dat.bin_width[i][0], sizeof(float), 1024, f);
    (dat.inpch).push_back(i);
    
  }

    return dat;

}

/************************************************************/


DATAINFO output_dat(FILE *f, DATAINFO dat){

  THEADER th;
  EHEADER eh;
  DATAINFO dat1;
  int i, j, ch, chn_index;
  double t1, t2, dt;

    // read event header

    i = fread(&eh, sizeof(eh), 1, f);

    if (i < 1)
      exit(-1);

    //printf("\nFound event #%d\n", eh.event_serial_number);

    // reach channel data
    
    for (ch=0 ; ch<5 ; ch++) {

      i = fread(dat1.hdr, sizeof(dat1.hdr), 1, f);

      if (i < 1)
        break;

      if (dat1.hdr[0] != 'C') {
        // event header found
        fseek(f, -4, SEEK_CUR);
        break;
      }

      chn_index = dat1.hdr[3] - '0' - 1;
      //cout << "chn_index is " << chn_index << endl;
      
      fread(dat1.voltage, sizeof(short), 1024, f);

      for (i=0 ; i<1024 ; i++) {
        // convert data to volts
        dat1.waveform[chn_index][i] = -1*(dat1.voltage[i] / 65536. - 0.5);
	
        // calculate time for this cell
        for (j=0,dat1.time[chn_index][i]=0 ; j<i ; j++){
	  
	  dat1.time[chn_index][i] += dat.bin_width[chn_index][(j+eh.trigger_cell) % 1024];

	}
      }
    }

    // align cell #0 of all channels
    t1 = dat1.time[0][(1024-eh.trigger_cell) % 1024];

    for (ch=1 ; ch<4 ; ch++) {
      t2 = dat1.time[ch][(1024-eh.trigger_cell) % 1024];
      dt = t1 - t2;
      for (i=0 ; i<1024 ; i++){
        dat1.time[ch][i] += dt;
	//cout << "dat1.time[chn][i] is " << dat1.time[ch][i] << endl;
      }
    }

    return dat1;

}

/*************************************************************************/


void decode(char *filename) {

  THEADER th;
  EHEADER eh;
  DATAINFO datfh;
  DATAINFO dat1;
  int i, n, ev;
  unsigned int chn;
  char rootfile[256];
  string query0, query1,query2;

  // open the binary waveform file

  FILE *f = fopen(Form("%s", filename), "r");
  if (f == NULL) {
    printf("Cannot find file \'%s\'\n", filename);
    return;
  }

  //pass file to read_header function

  datfh = find_header(f);

  //open the root file

  strcpy(rootfile, filename);
  if (strchr(rootfile, '.'))
    *strchr(rootfile, '.') = 0;
  strcat(rootfile, ".root");
  TFile *outfile = new TFile(rootfile, "RECREATE");

  
  // create canvas

  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();

  // create graph for plotting individual waveforms

  while(true)
  {

    cout << "How many events would you like to analyze? ";
    getline(cin,query0);
    stringstream ss(query0);
    if (ss >> ev)
      break;
    cout << "Invalid entry. ";
    
  }

  while(true)
  {

    cout << "Would you like to analyze individual waveforms? Answer with y or n: ";
    getline(cin,query1);
    if(query1 == 'y' || query1 == 'n')
      break;
    cout << "Invalid entry. ";
        
  }

  while(true)
  {

    cout << "Would you like to skip waveforms with positive polarity peaks higher than 25% of pulse minimum? Answer with y or n: ";
    getline(cin,query2);
    if(query2 == 'y' || query2 == 'n')
      break;
    cout << "Invalid entry. ";
        
  }

  //loop over relevant channels

  TH2D** hAllWaveforms = new TH2D*[(datfh.inpch).size()];

  for(chn = 0; chn < (datfh.inpch).size(); chn++){

    //TFile output_file(Form("daq_split_%i.root",iFile),"RECREATE");

    //create TTree
    
    //TTree *T = new TTree(Form("T_chn%i",datfh.inpch[chn]+1),Form("T_chn%i",datfh.inpch[chn]+1));
      
    //create graph for plotting individual waveforms

    TGraph *g = new TGraph(1024, dat1.time[datfh.inpch[chn]], dat1.waveform[datfh.inpch[chn]]);
    
    // create histograms for the Tree

    *hAllWaveforms[chn] = new TH2D(Form("AllWaveforms_ch%i",datfh.inpch[chn]+1),"All Waveforms",1024,-1,260,1000,-0.51,0.05);
    //TH1F *FormArea = new TH1F(Form("WaveformArea_ch%i",datfh.inpch[chn]+1),"Waveform Area",500,-1,10);
    /*TH1F *TailArea = new TH1F(Form("TailArea_ch%i",datfh.inpch[chn]+1),"Tail Area",500,-1,10);
    TH1F *TAoverWA = new TH1F(Form("TAoverWA_ch%i",datfh.inpch[chn]+1),"Tail Area/Waveform Area",500,-1,1);
    TH2F *TAvsWA = new TH2F(Form("TAvsWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Tail Area",1000,-1,8,1000,-1,3);
    TH2F *HvsTAoverWA = new TH2F(Form("HvsTAoverWA_ch%i",datfh.inpch[chn]+1),"Tail Area/Waveform Area vs Pulse Height",1000,-3,3,1000,0,0.55);
    TH2F *FWHMvsWA = new TH2F(Form("FWHMvsWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs FWHM",1000,-1,8,1000,-5,25);
    TH1F *FWHMoverWA1 = new TH1F(Form("FWHMoverWA1_ch%i",datfh.inpch[chn]+1),"FWHM/Waveform Area",500,-150,500);
    TH1F *HoverWA1 = new TH1F(Form("HoverWA1_ch%i",datfh.inpch[chn]+1),"Pulse Height/Waveform Area",500,-0.1,1);
    TH2F *PHvWA = new TH2F(Form("PHvWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Pulse Height",50,-2,10,50,0,0.55);
    TH1F *MaxHeight = new TH1F(Form("MaxHeight_ch%i",datfh.inpch[chn]+1),"Pulse Height",100,0,0.55);
    TH2F *HoverWAvsWA = new TH2F(Form("HoverWAvsWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Height/Waveform Area",1000,-1,9,1000,0,0.25);
    TH2F *PHvFWHM = new TH2F(Form("PHvFWHM_ch%i",datfh.inpch[chn]+1),"Pulse Width vs Pulse Height",1000,0,100,1000,0,0.55);
    TH2F *PHvDT = new TH2F(Form("PHvDT_ch%i",datfh.inpch[chn]+1),"Discharge Time vs Pulse Height",1000,0,50,1000,0,0.55);
    TH1F *WAoverPSD1 = new TH1F(Form("WAoverPSD1_ch%i",datfh.inpch[chn]+1), "Waveform Area/(Pulse Height/Discharge Time)",600,-60,1000);
    TH2F *WAvsPSD1 = new TH2F(Form("WAvsPSD1_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Pulse Height/Discharge Time", 1000,-1,8,1000,0,0.03);*/
    TH1F *WVH = new TH1F(Form("WVH_ch%i",datfh.inpch[chn]+1),"Waveform Height", 10000,-0.01,0.01);

    // T->Branch(Form("WaveformArea_ch%i",datfh.inpch[chn]+1), &FormArea);
    //T->SetBranchAddress(Form("WaveformArea_ch%i",datfh.inpch[chn]+1),&h);
	      
    //loop over all events in datafile

    for (n=0; n < ev; n++){

      //use output_dat function to decode binary data for each event and output waveform and time arrays for ROOT purposes

      dat1 = output_dat(f, datfh);

      //begin ROOT portion of macro

      for (i=5; i<900; i++){
	          
	WVH->Fill(dat1.waveform[datfh.inpch[chn]][i]);

      }

      //Get Baseline

      double BL1 = WVH->GetXaxis()->GetBinCenter(WVH->GetMaximumBin());


      //Find Max value on the Waveform

      int maxheightindex = 0;
      double maxheight = BL1;

      for(i=5; i< 900 ; i++){

	if(abs(maxheight) < abs(dat1.waveform[datfh.inpch[chn]][i]-BL1)){

	  maxheightindex = i;
	  maxheight = dat1.waveform[datfh.inpch[chn]][i] - BL1;

	}

      }


      if(query2 == 'y'){
	//Skip events with Waveforms that have higher than a certain positive voltage relative to V = BL1

	bool skip = true;

	for(i=0 ; i<1024; i++) {

	  if ((dat1.waveform[datfh.inpch[chn]][i]-BL1) < -maxheight/4){

	    skip=false;

	  }

	}

	if (skip == false){

	  WVH->Reset();
	  continue;

	}

      }

      //***************************************** Section for Finding Pulse Widths and Rise/Fall Times **********************************************

      //Find the FWHM Value on the Left Side of Main Pulse

      double delta = 0;
      double old_delta = abs(maxheight/2 - (dat1.waveform[datfh.inpch[chn]][900]-BL1));
      int index = maxheightindex;

      for(i=5; i<maxheightindex ; i++){

	delta = abs(maxheight/2 - (dat1.waveform[datfh.inpch[chn]][i]-BL1));

	if(delta<old_delta){

	  old_delta=delta;
	  index = i;

	}

      }

      //Find the FWHM Value on the Right Side of the Main Pulse

      double delta1 = 0;
      double old_delta1 = abs(maxheight/2 - (dat1.waveform[datfh.inpch[chn]][900]-BL1));
      int index1=maxheightindex;

      for(i=maxheightindex; i<900; i++){

	delta1 =abs(maxheight/2 - (dat1.waveform[datfh.inpch[chn]][i]-BL1));

	if(delta1<old_delta1){

	  old_delta1=delta1;

	  index1 = i;

	}

      }


      //Find the Discharge Time value to left side of Max Height

      double delta2=0;
      double old_delta2 = abs(maxheight/5 - (dat1.waveform[datfh.inpch[chn]][900]-BL1));
      int indexDischargeMin = maxheightindex;

      for(i = 5; i < maxheightindex; i++){

	delta2 = abs(maxheight/5 - (dat1.waveform[datfh.inpch[chn]][i]-BL1));

	if(delta2<old_delta2){

	  old_delta2 = delta2;

	  indexDischargeMin = i;

	}

      }

      //Find the Discharge Time value to right side of Max Height

      double delta3=0;
      double old_delta3 = abs(maxheight/5 - (dat1.waveform[datfh.inpch[chn]][900]-BL1));
      int indexDischargeMax = maxheightindex;

      for(i = maxheightindex; i < 900; i++){

	//cout << "Delta 3 is " << delta3 << " and Old Delta 3 is " << old_delta3 << " at iteration " << i << endl;
	delta3 = abs(maxheight/5 - (dat1.waveform[datfh.inpch[chn]][i]-BL1));

	if(delta3<old_delta3){

	  old_delta3 = delta3;

	  indexDischargeMax = i;

	}

      }

      //***************************************** END OF  Section for Finding Pulse Widths and Rise/Fall Times **********************************************


      //Define index value where the tail starts

      double TailStarts = index1;

      //ghistogram->Reset();

      //Define Integral Parametersize of array

      double gIntegralVBW2 = 0.0;
      double tailintegral = 0.0;

      //Waveform Integral

      for(i = 5; i < 900; i++) {

	gIntegralVBW2 = gIntegralVBW2+(dat1.waveform[datfh.inpch[chn]][i]-BL1)*( (dat1.time[datfh.inpch[chn]][i]-dat1.time[datfh.inpch[chn]][i-1])/2.0 + (dat1.time[datfh.inpch[chn]][i+1]-dat1.time[datfh.inpch[chn]][i])/2.0 );

      }

      //Tail integral

      for(i=TailStarts ; i < 900 ; i++) {

	tailintegral=tailintegral+(dat1.waveform[datfh.inpch[chn]][i]-BL1)*((dat1.time[datfh.inpch[chn]][i]-dat1.time[datfh.inpch[chn]][i-1])/2+(dat1.time[datfh.inpch[chn]][i+1]-dat1.time[datfh.inpch[chn]][i])/2.0);

      }

      double DischargeTime = dat1.time[datfh.inpch[chn]][indexDischargeMax] - dat1.time[datfh.inpch[chn]][indexDischargeMin];
      double WaveformArea  = gIntegralVBW2;
      double TArea  = tailintegral;
      double AreaRatio = TArea/WaveformArea;
      double PulseWidth = dat1.time[datfh.inpch[chn]][index1]-dat1.time[datfh.inpch[chn]][index];
      double FWHMoverWA = PulseWidth/WaveformArea;
      double HoverWA = maxheight/WaveformArea;
      double PSDv1 = maxheight/DischargeTime;
      double PSDv2 = WaveformArea/PSDv1;

      //************************************ Draw Waveforms and Fill Other Histograms **************************************

      if(abs(maxheight) < 0.49){

	// Fill Waveform Histograms

	for(i=0; i < 1024; i++){

	  g->SetPoint(i, dat1.time[datfh.inpch[chn]][i], -1*dat1.waveform[datfh.inpch[chn]][i]);
	  hAllWaveforms->Fill(dat1.time[datfh.inpch[chn]][i],-1*dat1.waveform[datfh.inpch[chn]][i]);

	}

	if(query1 == 'y'){

	  cout << "**********************************" << endl;
	  cout << endl;
	  cout << "Waveform number: " << n << endl;
	  cout << "Pulse Width Max (ns): " << dat1.time[datfh.inpch[chn]][index] << " with index " << index << endl;
	  cout << "Pulse Width Min (ns): " << dat1.time[datfh.inpch[chn]][index1] << " with index " << index1 << endl;
	  cout<<"Pulse Width (ns): "<< PulseWidth <<endl;
	  cout<<"Waveform Area (V*ns): "<< WaveformArea <<endl;
	  cout<<"Tail Area (V*ns): "<< TArea <<endl;
	  cout<<"Max Pulse Height (V): " << maxheight << endl;
	  cout<<"Max Pulse Height Location (ns): " << dat1.time[datfh.inpch[chn]][maxheightindex] << " with index " << maxheightindex << endl;
	  cout<<"Discharge Time (ns): " << DischargeTime << endl;
	  cout<<"Min Discharge Time (ns): " << dat1.time[datfh.inpch[chn]][indexDischargeMin] << " with index " << indexDischargeMin << endl;
	  cout<<"Max Discharge Time (ns): " << dat1.time[datfh.inpch[chn]][indexDischargeMax] << " with index " << indexDischargeMax <<  endl;
	  cout<<"Baseline Value (V): " << -1*BL1 << " with bin " << WVH->GetMaximumBin() << endl;
	  cout<<"Waveform Value at 5 ns (V): " << dat1.waveform[datfh.inpch[chn]][5] - BL1 << endl;
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
	  TLine *line = new TLine(0,-1*BL1,200,-1*BL1);
	  line->SetLineColor(kRed);
	  line->Draw();
	  c1->Update();

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

	}

	//**************** END INDIVIDAUL WAVEFORM SECTION ******************

	//FormArea->Fill(WaveformArea);
        /*TAoverWA->Fill(AreaRatio);
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
	WAvsPSD1->Fill(WaveformArea,PSDv1);*/

        //T->Fill();
      }

      WVH->Reset();

    }

    // print number of events
    printf("\n%d events processed in channel %d\n", ev, datfh.inpch[chn]+1);

    // save and close root file

    hAllWaveforms->Write();

    //FormArea->Write();
    /*TailArea->Write();
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
    WAvsPSD1->Write();*/

    //T->Write();
  }

  printf("\n%s written \n", rootfile);
  outfile->Close();


}
