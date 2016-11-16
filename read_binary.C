/*

  This macro is meant to decode data taken on any channel or set of channels on the DRS4 digitizer. 
  It was adapted from a macro written by Stefan Ritt of PSI which can be found here:
  https://midas.psi.ch/elogs/DRS4+Forum/361

  Primary Author: Abhishek Rajput abhi_rajput5@utexas.edu
  Other Author(s): Will Flanagan will.flanagan@utexas.edu
     
  You can always pull the latest version of this code with:
  git clone https://github.com/UTKLgroup/DRS4macros

  root [0] .L read_binary.C+
  root [1] decode("filename.dat");

  filename.root will be written to the same directory as the binary file upon completion of decoding

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
  int ext;
  
}DATAINFO;


typedef struct {

  int ev;
  string query0, query1, query2;

}QUERYUSER;

/**********************************************************************************************************************************/

DATAINFO find_header(FILE *f);
DATAINFO output_dat(FILE *f, DATAINFO dat);
QUERYUSER queries();

void decode(char *filename){

  THEADER th;
  EHEADER eh;
  DATAINFO datfh;
  DATAINFO dat1;
  QUERYUSER quer;
  int i, n;
  unsigned int chn, v;
  char rootfile[256];
  double sumform[4][1024], sumtime[4][1024], normform[4][1024];
  
  // open the binary waveform file

  FILE *f = fopen(Form("%s", filename), "r");
  if (f == NULL) {
    printf("Cannot find file \'%s\'\n", filename);
    return;
  }

  //pass file to read_header function

  datfh = find_header(f);

  //query user for information

  quer = queries();

  // create canvas

  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();

  //define dynamic array of pointers for histograms to be plotted
  
  TH2D** hAllWaveforms = new TH2D*[(datfh.inpch).size()];
  TH2D** NormWaveform = new TH2D*[(datfh.inpch).size()];
  TH2D** AvgWaveform = new TH2D*[(datfh.inpch).size()];
  
  TH1F** FormArea = new TH1F*[(datfh.inpch).size()];
  TH1F** TailArea = new TH1F*[(datfh.inpch).size()];
  TH1F** TAoverWA = new TH1F*[(datfh.inpch).size()];   
  TH1F** FWHMoverWA = new TH1F*[(datfh.inpch).size()];
  TH1F** PHoverWA = new TH1F*[(datfh.inpch).size()];
  TH1F** MaxHeight = new TH1F*[(datfh.inpch).size()];
  TH1F** WAoverPSD1 = new TH1F*[(datfh.inpch).size()];
  TH1F** WVH = new TH1F*[(datfh.inpch).size()];
  
  TH2F** TAvsWA = new TH2F*[(datfh.inpch).size()];
  TH2F** PHvsTAoverWA = new TH2F*[(datfh.inpch).size()];
  TH2F** FWHMvsWA = new TH2F*[(datfh.inpch).size()];
  TH2F** PHvWA = new TH2F*[(datfh.inpch).size()];
  TH2F** PHoverWAvsWA = new TH2F*[(datfh.inpch).size()];
  TH2F** PHvFWHM = new TH2F*[(datfh.inpch).size()];
  TH2F** PHvDT = new TH2F*[(datfh.inpch).size()];
  TH2F** WAvsPSD1 = new TH2F*[(datfh.inpch).size()];
  TGraph** g = new TGraph*[(datfh.inpch).size()];

  //loop to pre-define relevant histograms for all input channels

  for(chn = 0; chn < (datfh.inpch).size(); chn++){

    // create histograms for the Tree

    //for Cryostat testing
      
    hAllWaveforms[chn] = new TH2D(Form("AllWaveforms_ch%i",datfh.inpch[chn]+1),"All Waveforms",1024,-1,1650,1000,-0.51,0.05);
    NormWaveform[chn] = new TH2D(Form("NormWaveform_ch%i",datfh.inpch[chn]+1),"Normalized Avg Waveform",1024,-1,1650,1000,-0.51,0.05);
    AvgWaveform[chn] = new TH2D(Form("AvgWaveform_ch%i",datfh.inpch[chn]+1),"Average Waveform",1024,-1,1650,1000,-0.51,0.05);
    
    FormArea[chn] = new TH1F(Form("WaveformArea_ch%i",datfh.inpch[chn]+1),"Waveform Area",500,0,50);
    TailArea[chn] = new TH1F(Form("TailArea_ch%i",datfh.inpch[chn]+1),"Tail Area",250,0,50);
    TAoverWA[chn] = new TH1F(Form("TAoverWA_ch%i",datfh.inpch[chn]+1),"Tail Area/Waveform Area",250,0,1);
    TAvsWA[chn] = new TH2F(Form("TAvsWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Tail Area",1000,0,50,1000,0,50);
    PHvsTAoverWA[chn] = new TH2F(Form("PHvsTAoverWA_ch%i",datfh.inpch[chn]+1),"Tail Area/Waveform Area vs Pulse Height",1000,-3,3,1000,0,0.55);
    FWHMvsWA[chn] = new TH2F(Form("FWHMvsWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs FWHM",1000,0,20,1000,0,100);
    FWHMoverWA[chn] = new TH1F(Form("FWHMoverWA_ch%i",datfh.inpch[chn]+1),"FWHM/Waveform Area",250,-10,100);
    PHoverWA[chn] = new TH1F(Form("PHoverWA_ch%i",datfh.inpch[chn]+1),"Pulse Height/Waveform Area",250,0,0.1);
    PHvWA[chn] = new TH2F(Form("PHvWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Pulse Height",500,0,50,500,0,0.55);
    MaxHeight[chn] = new TH1F(Form("MaxHeight_ch%i",datfh.inpch[chn]+1),"Pulse Height",300,0,0.55);
    PHoverWAvsWA[chn] = new TH2F(Form("PHoverWAvsWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Height/Waveform Area",1000,0,20,1000,0,0.25);
    PHvFWHM[chn] = new TH2F(Form("PHvFWHM_ch%i",datfh.inpch[chn]+1),"Pulse Width vs Pulse Height",1000,0,25,1000,0,0.55);
    PHvDT[chn] = new TH2F(Form("PHvDT_ch%i",datfh.inpch[chn]+1),"Discharge Time vs Pulse Height",1000,0,50,1000,0,0.55);
    WAoverPSD1[chn] = new TH1F(Form("WAoverPSD1_ch%i",datfh.inpch[chn]+1), "Waveform Area/(Pulse Height/Discharge Time)",250,0,1000);
    WAvsPSD1[chn] = new TH2F(Form("WAvsPSD1_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Pulse Height/Discharge Time", 1000,0,50,1000,0,0.03);
    

    /* //for liquid scintillator 
       hAllWaveforms[chn] = new TH2D(Form("AllWaveforms_ch%i",datfh.inpch[chn]+1),"All Waveforms",1024,-1,260,1000,-0.51,0.05);
       FormArea[chn] = new TH1F(Form("WaveformArea_ch%i",datfh.inpch[chn]+1),"Waveform Area",500,-1,10);
       TailArea[chn] = new TH1F(Form("TailArea_ch%i",datfh.inpch[chn]+1),"Tail Area",500,-1,10);
       TAoverWA[chn] = new TH1F(Form("TAoverWA_ch%i",datfh.inpch[chn]+1),"Tail Area/Waveform Area",500,-1,1);
       TAvsWA[chn] = new TH2F(Form("TAvsWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Tail Area",1000,-1,8,1000,-1,3);
       PHvsTAoverWA[chn] = new TH2F(Form("PHvsTAoverWA_ch%i",datfh.inpch[chn]+1),"Tail Area/Waveform Area vs Pulse Height",1000,-3,3,1000,0,0.55);
       FWHMvsWA[chn] = new TH2F(Form("FWHMvsWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs FWHM",1000,-1,8,1000,-5,25);
       FWHMoverWA[chn] = new TH1F(Form("FWHMoverWA_ch%i",datfh.inpch[chn]+1),"FWHM/Waveform Area",500,-150,500);
       PHoverWA[chn] = new TH1F(Form("PHoverWA_ch%i",datfh.inpch[chn]+1),"Pulse Height/Waveform Area",500,-0.1,1);
       PHvWA[chn] = new TH2F(Form("PHvWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Pulse Height",50,-2,10,50,0,0.55);
       MaxHeight[chn] = new TH1F(Form("MaxHeight_ch%i",datfh.inpch[chn]+1),"Pulse Height",100,0,0.55);
       PHoverWAvsWA[chn] = new TH2F(Form("PHoverWAvsWA_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Height/Waveform Area",1000,-1,9,1000,0,0.25);
       PHvFWHM[chn] = new TH2F(Form("PHvFWHM_ch%i",datfh.inpch[chn]+1),"Pulse Width vs Pulse Height",1000,0,100,1000,0,0.55);
       PHvDT[chn] = new TH2F(Form("PHvDT_ch%i",datfh.inpch[chn]+1),"Discharge Time vs Pulse Height",1000,0,50,1000,0,0.55);
       WAoverPSD1[chn] = new TH1F(Form("WAoverPSD1_ch%i",datfh.inpch[chn]+1), "Waveform Area/(Pulse Height/Discharge Time)",600,-60,1000);
       WAvsPSD1[chn] = new TH2F(Form("WAvsPSD1_ch%i",datfh.inpch[chn]+1),"Waveform Area vs Pulse Height/Discharge Time", 1000,-1,8,1000,0,0.03);*/

    WVH[chn] = new TH1F(Form("WVH_ch%i",datfh.inpch[chn]+1),"Waveform Height", 10000,-0.01,0.01);
    g[chn] = new TGraph(1024, dat1.time[datfh.inpch[chn]], dat1.waveform[datfh.inpch[chn]]);
    
  }

  //loop over events

  printf("\nCommencing binary data decoding...\n");

  for(n=0; n < quer.ev; n++){

    //use output_dat function to decode binary data for each event and output waveform and time arrays for ROOT purposes
   
    dat1 = output_dat(f, datfh);

    if(dat1.ext < 1){

      break;

    }

    //loop over all the channels for each event
    
    for(chn = 0; chn < (datfh.inpch).size(); chn++){
      
      //fill waveform into histogram to determine 
      
      for (i=5; i<900; i++){
	          
	WVH[chn]->Fill(dat1.waveform[datfh.inpch[chn]][i]);

      }


      //Get Baseline

      double BL1 = WVH[chn]->GetXaxis()->GetBinCenter(WVH[chn]->GetMaximumBin());


      //Find Max value on the Waveform

      int maxheightindex = 0;
      double maxheight = BL1;

      for(i=5; i< 900 ; i++){

	if(abs(maxheight) < abs(dat1.waveform[datfh.inpch[chn]][i]-BL1)){

	  maxheightindex = i;
	  maxheight = dat1.waveform[datfh.inpch[chn]][i] - BL1;

	}

      }


      //Skip events with Waveforms that have higher than a certain positive voltage relative to V = BL1
      
      if(quer.query2 == 'y'){


	bool skip = true;

	for(i=0 ; i<1024; i++) {

	  if ((dat1.waveform[datfh.inpch[chn]][i]-BL1) < -maxheight/4){

	    skip=false;

	  }

	}

	if (skip == false){

	  WVH[chn]->Reset();
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
      ///Find Tail and Waveform Areas and define all quantities from these and above parameters

      //Define index value where the tail starts

      double TailStarts = index1;

      //ghistogram->Reset();

      //Define Integral Parametersize of array

      double WaveformArea = 0.0;
      double TArea = 0.0;

      //Waveform Integral

      for(i = 5; i < 900; i++) {

	WaveformArea += (dat1.waveform[datfh.inpch[chn]][i]-BL1)*( (dat1.time[datfh.inpch[chn]][i]-dat1.time[datfh.inpch[chn]][i-1])/2.0 + (dat1.time[datfh.inpch[chn]][i+1]-dat1.time[datfh.inpch[chn]][i])/2.0 );

      }

      //Tail integral

      for(i=TailStarts ; i < 900 ; i++) {

	TArea += (dat1.waveform[datfh.inpch[chn]][i]-BL1)*((dat1.time[datfh.inpch[chn]][i]-dat1.time[datfh.inpch[chn]][i-1])/2+(dat1.time[datfh.inpch[chn]][i+1]-dat1.time[datfh.inpch[chn]][i])/2.0);

      }

      double DischargeTime = dat1.time[datfh.inpch[chn]][indexDischargeMax] - dat1.time[datfh.inpch[chn]][indexDischargeMin];
      double AreaRatio = TArea/WaveformArea;
      double PulseWidth = dat1.time[datfh.inpch[chn]][index1]-dat1.time[datfh.inpch[chn]][index];
      double FWHMoverWA1 = PulseWidth/WaveformArea;
      double HoverWA = maxheight/WaveformArea;
      double PSDv1 = maxheight/DischargeTime;
      double PSDv2 = WaveformArea/PSDv1;

      //************************************ Draw Individaul Waveforms and/or Fill Other Histogram  **************************************

      if(abs(maxheight) < 0.49){

	// Fill Waveform Histograms

	for(i=0; i < 1024; i++){

	  //cout << "dat1 time is " << dat1.time[datfh.inpch[chn]][i] << endl;
	  sumtime[chn][i] +=  dat1.time[datfh.inpch[chn]][i];
	  normform[chn][i] += dat1.waveform[datfh.inpch[chn]][i]*(-1/abs(WaveformArea));
	  sumform[chn][i] += -1*dat1.waveform[datfh.inpch[chn]][i];
	  g[chn]->SetPoint(i, dat1.time[datfh.inpch[chn]][i], -1*dat1.waveform[datfh.inpch[chn]][i]);
	  hAllWaveforms[chn]->Fill(dat1.time[datfh.inpch[chn]][i],-1*dat1.waveform[datfh.inpch[chn]][i]);
	  
	}

	if(quer.query1 == 'y'){

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
	  cout<<"Baseline Value (V): " << -1*BL1 << " with bin " << WVH[chn]->GetMaximumBin() << endl;
	  cout<<"Waveform Value at 5 ns (V): " << dat1.waveform[datfh.inpch[chn]][5] - BL1 << endl;
	  cout << endl;

	  c1->cd();
	  g[chn]->Draw("acp");
	  g[chn]->GetXaxis()->SetTitle("Time (ns)");
	  g[chn]->GetXaxis()->CenterTitle();
	  g[chn]->GetYaxis()->SetTitle("Voltage (V)");
	  g[chn]->GetYaxis()->CenterTitle();
	  g[chn]->GetXaxis()->SetTitleOffset(1);
	  g[chn]->GetYaxis()->SetTitleOffset(1);
	  g[chn]->GetXaxis()->SetTitleSize(0.05);
	  g[chn]->GetYaxis()->SetTitleSize(0.05);
	  TLine *line = new TLine(0,-1*BL1,200,-1*BL1);
	  line->SetLineColor(kRed);
	  line->Draw();
	  c1->Update();

	  c2->cd();
	  WVH[chn]->GetXaxis()->SetTitle("Voltage (V)");
	  WVH[chn]->GetYaxis()->SetTitle("Counts");
	  WVH[chn]->GetXaxis()->CenterTitle();
	  WVH[chn]->GetYaxis()->CenterTitle();
	  WVH[chn]->GetXaxis()->SetTitleOffset(1);
	  WVH[chn]->GetYaxis()->SetTitleOffset(1);
	  WVH[chn]->GetXaxis()->SetTitleSize(0.05);
	  WVH[chn]->GetYaxis()->SetTitleSize(0.05);
	  WVH[chn]->Draw();
	  c2->Update();

	  gPad->WaitPrimitive();

	}

	//**************** END INDIVIDAUL WAVEFORM SECTION ******************
	
 	FormArea[chn]->Fill(WaveformArea);
	TAoverWA[chn]->Fill(AreaRatio);
	TailArea[chn]->Fill(TArea);
	PHvsTAoverWA[chn]->Fill(AreaRatio,maxheight);
	FWHMvsWA[chn]->Fill(WaveformArea,PulseWidth);
	TAvsWA[chn]->Fill(WaveformArea,TArea);
	FWHMoverWA[chn]->Fill(FWHMoverWA1);


	PHvWA[chn]->Fill(WaveformArea,maxheight);

	PHoverWA[chn]->Fill(HoverWA);
	MaxHeight[chn]->Fill(maxheight);

	PHoverWAvsWA[chn]->Fill(WaveformArea,HoverWA);

	PHvFWHM[chn]->Fill(PulseWidth,maxheight);
	PHvDT[chn]->Fill(DischargeTime,maxheight);
	WAoverPSD1[chn]->Fill(PSDv2);
	WAvsPSD1[chn]->Fill(WaveformArea,PSDv1);

      }

      WVH[chn]->Reset();
   
    }
      
	
  }  

  if(n == quer.ev  || dat1.ext < 1){

    double factor = (double)1/(double)(n+1);
	
    for(i=0; i < 1024; i++){

      for(chn = 0; chn < ((datfh.inpch).size()); chn++){	      
	//if(chn == 1){
	//cout << "sumtime is " << sumtime[chn][i]*factor << " and sumform is " << sumform[chn][i]*factor << " and numform is " << normform[chn][i]*factor << endl; //}
	AvgWaveform[chn]->Fill(sumtime[chn][i]*factor,sumform[chn][i]*factor);
	NormWaveform[chn]->Fill(sumtime[chn][i]*factor,normform[chn][i]*factor);

      }
	  
    }
	
  }

  printf("\n%d events processed in all channels \n", n);
  
  //open the root file
  strcpy(rootfile, filename);
  if (strchr(rootfile, '.'))
    *strchr(rootfile, '.') = 0;
  strcat(rootfile, ".root");
  TFile *outfile = new TFile(rootfile, "RECREATE");
  
  //create directory structure plotting individual waveforms

  TDirectory *drs4 = outfile->mkdir("DRS4 Data");
  drs4->cd();
  TDirectory *drs4chn[(datfh.inpch).size()];
  
  for(chn = 0; chn < (datfh.inpch).size(); chn++){

    //create directories for the channels
    drs4chn[chn] = drs4->mkdir(Form("channel%i",datfh.inpch[chn]+1));
    drs4chn[chn]->cd();

    hAllWaveforms[chn]->Write();
    NormWaveform[chn]->Write();
    AvgWaveform[chn]->Write();
    FormArea[chn]->Write();
    TailArea[chn]->Write();
    TAvsWA[chn]->Write();
    TAoverWA[chn]->Write();
    PHvsTAoverWA[chn]->Write();
    FWHMvsWA[chn]->Write();
    FWHMoverWA[chn]->Write();

    MaxHeight[chn]->Write();
    PHvWA[chn]->Write();
    PHvFWHM[chn]->Write();
    PHoverWA[chn]->Write();
    PHoverWAvsWA[chn]->Write();

    PHvDT[chn]->Write();
    WAoverPSD1[chn]->Write();
    WAvsPSD1[chn]->Write();

  }

  printf("\n%s written \n \n", rootfile);
  outfile->Close();

}

/*****************************************************************************************************************************************************/

DATAINFO find_header(FILE *f){

  THEADER th;
  EHEADER eh;
  DATAINFO dat;
  int i, ch, chn_index, t, d, b;

  // read time header
  /*sizeof(th) is 8
    sizeof(hdr) is 4
    sizeof(float) is 4
    sizeof(eh) is 32
    sizeof(short) is 2 */
  
  t = fread(&th, sizeof(th), 1, f);
  
  //printf("Found data for board #%d\n", th.board_serial_number);

  // read time bin widths

  memset(dat.bin_width, sizeof(dat.bin_width), 0);

  for (ch=0; ch<5 ; ch++) {
    d = fread(dat.hdr, sizeof(dat.hdr), 1, f);

    if (dat.hdr[0] != 'C') {

      // event header found
      fseek(f, -4, SEEK_CUR);
      break;
      
    }

    i = dat.hdr[3] - '0' - 1;
    printf("\n Found timing calibration for channel #%d\n\n", i+1);
    b = fread(&dat.bin_width[i][0], sizeof(float), 1024, f);

    (dat.inpch).push_back(i);
    
  }
  
    return dat;
    
}

/*****************************************************************************************************************************************************/

DATAINFO output_dat(FILE *f, DATAINFO dat){

  THEADER th;
  EHEADER eh;
  DATAINFO dat1;
  int i, j, chn_index,k;
  unsigned int ch;
  double t1, t2, dt;

    // read event header

    dat1.ext = fread(&eh, sizeof(eh), 1, f);

    //printf("\nFound event #%d, i value is %i\n", eh.event_serial) 
             

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

      
      k = fread(dat1.voltage, sizeof(short), 1024, f);

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
    //t1 = dat1.time[0][(1024-eh.trigger_cell) % 1024];

    t1 = dat1.time[dat.inpch[0]][(1024-eh.trigger_cell) % 1024];

    for (ch=1; ch<(dat.inpch).size() ; ch++) {

      t2 = dat1.time[dat.inpch[ch]][(1024-eh.trigger_cell) % 1024];
      dt = t1 - t2;

      for (i=0 ; i<1024 ; i++){
        dat1.time[dat.inpch[ch]][i] += dt;

      }
    }

    return dat1;

}

/*****************************************************************************************************************************************************/

QUERYUSER queries(){

  QUERYUSER quer;
  
  while(true){

    cout << "How many events would you like to analyze? ";
    getline(cin,quer.query0);
    stringstream ss(quer.query0);
    if (ss >> quer.ev)
      break;
    cout << "Invalid entry. ";
    
  }

  while(true){

    cout << "Would you like to analyze individual waveforms? Answer with y or n: ";
    getline(cin,quer.query1);
    if(quer.query1 == 'y' || quer.query1 == 'n')
      break;
    cout << "Invalid entry. ";
        
  }

  while(true){

    cout << "Would you like to skip waveforms with positive overshoots higher than 25% of pulse minimum? Answer with y or n: ";
    getline(cin,quer.query2);
    if(quer.query2 == 'y' || quer.query2 == 'n')
      break;
    cout << "Invalid entry. ";
        
  }

  return quer;
  
}

/*****************************************************************************************************************************************************/
 
/*int main() {                                                                                                                                                                                                      
  //char dummy[22];                                                                                                                                                                                                
  std::string dummy = "Cryo100mV1kall.bin";                                                                                                                                                                    
  char *cstr = &dummy[0];                                                                                                                                                                                          
  decode(cstr); //ADD FILE NAME                                                                                                                                                                                    
  return 0;
  
  }
*/
