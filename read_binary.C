/*
 
   Name:           read_binary.C
   Created by:     Stefan Ritt <stefan.ritt@psi.ch>
   Date:           July 30th, 2014
 
   Purpose:        Example program under ROOT to read a binary data file written 
                   by the DRSOsc program. Decode time and voltages from waveforms 
                   and display them as a graph. Put values into a ROOT Tree for 
                   further analysis.
 
                   To run it, do:
 
                   - Crate a file test.dat via the "Save" button in DRSOsc
                   - start ROOT
                   root [0] .L read_binary.C+
                   root [1] decode("test.dat");
 
*/
 
#include "TMath.h"
#include "TH1.h"
#include "TF1.h"
#include <string.h>
#include <stdio.h>
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "Getline.h"

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
	TCanvas *c2 = new TCanvas();

   // Divide Canvas

	c1->Divide(3,1);
   
   // create graph
   TGraph *g = new TGraph(1024, (double *)time[0], (double *)waveform[0]);

	// create histograms

	TH1F *h1 = new TH1F("h1","Integral",500,-0.5,0.5);
	TH1F *h2 = new TH1F("h2","Integral V.B.W",500,-0.5,0.5);
	TH1F *h3 = new TH1F("h3","Integral Trapezoidal",500,-0.5,0.5);

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
   for (n=0 ; n<10000 ; n++) {
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
      
      // fill graph
      for (i=0 ; i<1024 ; i++)
         g->SetPoint(i, time[0][i], waveform[0][i]);
      
      // draw graph individual graph
	c2->cd(1);
	g->Draw("ACP");

     //  Create Fit Functions and Fit g
     //	TF1 *f1 = new TF1("f1","[0]+[1]*TMath::Landau(x,[2],[3])",-1000,1000);
	TF1 *f2 = new TF1("f2","pol0",0,1024);
	g->Fit("f2","R");
     // g->Fit("f1");
	
	// Obtain Baseline
	BL = f2->GetParameter(0);

	//Define Integral Parameters 
	double gIntegral=0.0;
	double gIntegralVBW2=0.0;
	double gIntegraltrap=0.0;
	for(int i = 1 ; i < 1023 ; i++)
	{
	  gIntegralVBW2=gIntegralVBW2+(waveform[0][i]-BL)*((time[0][i]-time[0][i-1])/2.0+(time[0][i+1]-time[0][i])/2.0);
	  gIntegral=gIntegral+(waveform[0][i]-BL);
	  gIntegraltrap=gIntegraltrap+(waveform[0][i]-BL)+(waveform[0][i+1]-waveform[0][i])/2.0;
	  
	 
	}
	// Display Integral Results
	cout<<"gIntegralVBW2: "<<gIntegralVBW2 <<endl;
	cout<<"gIntegral: "<<gIntegral <<endl;
	cout<<"gIntegraltrap: "<<gIntegraltrap <<endl;
	c2->Update();
	
	
	// Fill Histograms

	h1->Fill(gIntegral);
	h2->Fill(gIntegralVBW2);
	h3->Fill(gIntegraltrap);
   }
	// Display Histgrams
	c1->cd(1);
	h1->SetLineColor(kRed);
	h1->Draw();
	c1->cd(2);
	h2->SetLineColor(kBlue);
	h2->Draw();
	c1->cd(3);
	h1->SetLineColor(kGreen);
	h3->Draw();
	c1->Modified();
	c1->Update();
	
	// Display all Waveforms 
	/*c1->cd(4);
	rec->Draw("w1:t1");
	TGraph *graph = (TGraph*)gPad->GetPrimitive("Graph");
	c1->Update();
	c1->Modified();*/

	
	
	
	
   // print number of events
   printf("\n%d events processed, \"%s\" written.\n", n, rootfile);
   
   // save and close root file
   //Save histograms 
   h1->Write();
   h2->Write();
   h3->Write();
   rec->Write();
   outfile->Close();
}
