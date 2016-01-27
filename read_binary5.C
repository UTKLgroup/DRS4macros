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
		   - The Histogram disappears once the file is save. Type in the following to make the histogram again.
		   root [2] TFile MyFile("test.root")
		   root [3] h1->Draw()
 
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
   double t1, t2, dt, BL, ChargeCountVBW;

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
	c1->Divide(2,1);
	  
   // create graph
   TGraph *g = new TGraph(1024, (double *)time[0], (double *)waveform[0]);

	// create histograms

	TH1F *h1 = new TH1F("h1","Charge Count",1000,-12000,12000);
	
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
	c1->cd(1);
	g->Draw("ACP");
	g->SetTitle("; Time [nsec]; Voltage [mV]");

		
     //  Create Fit Functions and Fit g
     	TF1 *f1 = new TF1("f1","pol0",0,1024);
	g->Fit("f1","R");
	
	// Obtain Baseline
	BL = f1->GetParameter(0);

	//Define Integral Parameters 
	double gIntegralVBW=0.0;

	for(int i = 1 ; i < 1023 ; i++)
	{
	  gIntegralVBW=gIntegralVBW+(waveform[0][i]-BL)*((time[0][i]-time[0][i-1])/2.0+(time[0][i+1]-time[0][i])/2.0);
		   
	}
	// Display Integral Results in mV*ns and # of electrons
	double electron=1.60217662e-07;
	ChargeCountVBW = (gIntegralVBW/50)/electron;
	cout<<"gIntegralVBW: "<<gIntegralVBW <<endl;
	cout<<"ChargeCountVBW: "<<ChargeCountVBW <<endl;
	
	// Fill Histograms

	h1->Fill(ChargeCountVBW);
	
	// Display Histgrams
	c1->cd(2);
	h1->SetLineColor(kMagenta);
	h1->SetTitle("Electron Count;Charge [e]; Count");
	h1->Draw();
	c1->Update();
	}
		
   // print number of events
   printf("\n%d events processed, \"%s\" written.\n", n, rootfile);
   
   // save and close root file
   //Save histograms 
   h1->Write();
   rec->Write();
   outfile->Close();
}
