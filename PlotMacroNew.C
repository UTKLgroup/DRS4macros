PlotMacroNew(){

  TFile *_file0 = TFile::Open("Cosmic_SiPM_0p7Gss.root");

  TCanvas *c1 = new TCanvas("c1","c1",400,100,520,400);
  TCanvas *c2 = new TCanvas("c2","c2",400,100,520,400);
  TCanvas *c3 = new TCanvas("c3","c3",400,100,520,400);
  TCanvas *c4 = new TCanvas("c4", "c4",400,100,520,400);
  TCanvas *c5 = new TCanvas("c5", "c5",400,100,520,400);
  TCanvas *c6 = new TCanvas("c6", "c6",400,100,520,400);
  TCanvas *c7 = new TCanvas("c7", "c7",400,100,520,400);
  TCanvas *c8 = new TCanvas("c8", "c8",400,100,520,400);
  TCanvas *c9 = new TCanvas("c9", "c9",400,100,520,400);
  TCanvas *c10 = new TCanvas("c10", "c10",400,100,520,400);
  TCanvas *c11 = new TCanvas("c11", "c11",400,100,520,400);
  TCanvas *c12 = new TCanvas("c12", "c12",400,100,520,400);
  TCanvas *c13 = new TCanvas("c13", "c13",400,100,520,400);
  TCanvas *c14 = new TCanvas("c14", "c14",400,100,520,400);
  TCanvas *c15 = new TCanvas("c15", "c15",400,100,520,400);
  TCanvas *c16 = new TCanvas("c16", "c16",400,100,520,400);
  TCanvas *c17 = new TCanvas("c17", "c17",400,100,520,400);
  TCanvas *c18 = new TCanvas("c18", "c18",400,100,520,400);
  TCanvas *c19 = new TCanvas("c19", "c19",400,100,520,400);
  TCanvas *c20 = new TCanvas("c20", "c20",400,100,520,400);
  TCanvas *c21 = new TCanvas("c21", "c21",400,100,520,400);
  TCanvas *c22 = new TCanvas("c22", "c22",400,100,520,400);
  TCanvas *c23 = new TCanvas("c23", "c23",400,100,520,400);
  TCanvas *c24 = new TCanvas("c24", "c24",400,100,520,400);
  TCanvas *c25 = new TCanvas("c25", "c25",400,100,520,400);
  TCanvas *c26 = new TCanvas("c26", "c26",400,100,520,400);
  TCanvas *c27 = new TCanvas("c27", "c27",400,100,520,400);
  TCanvas *c28 = new TCanvas("c28", "c28",400,100,520,400);
  TCanvas *c29 = new TCanvas("c29", "c29",400,100,520,400);
  TCanvas *c30 = new TCanvas("c30", "c30",400,100,520,400);
  TCanvas *c31 = new TCanvas("c31", "c31",400,100,520,400);
  TCanvas *c32 = new TCanvas("c32", "c32",400,100,520,400);
  TCanvas *c33 = new TCanvas("c33","c33",400,100,520,400);
  TCanvas *c34 = new TCanvas("c34","c34",400,100,520,400);
  TCanvas *c35 = new TCanvas("c35","c35",400,100,520,400);
  TCanvas *c36 = new TCanvas("c36","c36",400,100,520,400);
  
  //create 1st set of clones

  TH2D *PHvWA_clone = (TH2D*)PHvWA->Clone();
  TH2D *TAvsWA_clone = (TH2D*)TAvsWA->Clone();  
  TH2D *FWHMvsWA_clone = (TH2D*)FWHMvsWA->Clone();
  TH2D *HoverWAvsWA_clone = (TH2D*)HoverWAvsWA->Clone();
  TH2D *WAvsPSD1_clone =  (TH2D*)WAvsPSD1->Clone();
  TH1D *WAoverPSD1_clone = (TH1D*)WAoverPSD1->Clone();
  TH2D *HvsTAoverWA_clone = (TH2D*)HvsTAoverWA->Clone();
  TH2D *PHvDT_clone = (TH2D*)PHvDT->Clone();
  TH2D *PHvFWHM_clone = (TH2D*)PHvFWHM->Clone();
  
  //create 2nd set of clones
  
  TH2D *PHvWA_clone1 = (TH2D*)PHvWA->Clone();
  TH2D *TAvsWA_clone1 = (TH2D*)TAvsWA->Clone();  
  TH2D *FWHMvsWA_clone1 = (TH2D*)FWHMvsWA->Clone();
  TH2D *HoverWAvsWA_clone1 = (TH2D*)HoverWAvsWA->Clone();			      
  TH2D *WAvsPSD1_clone1 =  (TH2D*)WAvsPSD1->Clone();
  TH1D *WAoverPSD1_clone1 = (TH1D*)WAoverPSD1->Clone();
  TH2D *HvsTAoverWA_clone1 = (TH2D*)HvsTAoverWA->Clone();
  TH2D *PHvDT_clone1 = (TH2D*)PHvDT->Clone();
  TH2D *PHvFWHM_clone1 = (TH2D*)PHvFWHM->Clone();
  
  //create 3rd set of clones for dividing histograms

  TH2D *PHvWA_clone2 = (TH2D*)PHvWA->Clone();
  TH2D *TAvsWA_clone2 = (TH2D*)TAvsWA->Clone();  
  TH2D *FWHMvsWA_clone2 = (TH2D*)FWHMvsWA->Clone();
  TH2D *HoverWAvsWA_clone2 = (TH2D*)HoverWAvsWA->Clone();			      
  TH2D *WAvsPSD1_clone2 =  (TH2D*)WAvsPSD1->Clone();
  TH1D *WAoverPSD1_clone2 = (TH1D*)WAoverPSD1->Clone();
  TH2D *HvsTAoverWA_clone2 = (TH2D*)HvsTAoverWA->Clone();
  TH2D *PHvDT_clone2 = (TH2D*)PHvDT->Clone();
  TH2D *PHvFWHM_clone2 = (TH2D*)PHvFWHM->Clone();
  
  //set labels for 1st set of clones
  
  const char* title_clone1 = "Cosmic Data for SiPM";
  PHvWA_clone->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  PHvWA_clone->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvWA_clone->GetXaxis()->CenterTitle();
  PHvWA_clone->GetYaxis()->CenterTitle();
  PHvWA_clone->SetTitleOffset(1.3,"XY");
  PHvWA_clone->SetTitle(title_clone1);
  PHvWA_clone->SetMarkerColor(kRed);

  TAvsWA_clone->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  TAvsWA_clone->GetYaxis()->SetTitle("Tail Area (V*ns)");
  TAvsWA_clone->GetXaxis()->CenterTitle();
  TAvsWA_clone->GetYaxis()->CenterTitle();
  TAvsWA_clone->SetTitleOffset(1.3,"XY");
  TAvsWA_clone->SetTitle(title_clone1);
  TAvsWA_clone->SetMarkerColor(kRed);

  FWHMvsWA_clone->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  FWHMvsWA_clone->GetYaxis()->SetTitle("Pulse Width (ns)");
  FWHMvsWA_clone->GetXaxis()->CenterTitle();
  FWHMvsWA_clone->GetYaxis()->CenterTitle();
  FWHMvsWA_clone->SetTitleOffset(1.3,"XY");
  FWHMvsWA_clone->SetTitle(title_clone1);
  FWHMvsWA_clone->SetMarkerColor(kRed);

  HoverWAvsWA_clone->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  HoverWAvsWA_clone->GetYaxis()->SetTitle("Pulse Height/Waveform Area (1/ns)");
  HoverWAvsWA_clone->GetXaxis()->CenterTitle();
  HoverWAvsWA_clone->GetYaxis()->CenterTitle();
  HoverWAvsWA_clone->SetTitleOffset(1.3,"XY");
  HoverWAvsWA_clone->SetTitle(title_clone1);
  HoverWAvsWA_clone->SetMarkerColor(kRed);

  WAvsPSD1_clone->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  WAvsPSD1_clone->GetYaxis()->SetTitle("Pulse Height/Discharge Time (V/ns)");
  WAvsPSD1_clone->GetXaxis()->CenterTitle();
  WAvsPSD1_clone->GetYaxis()->CenterTitle();
  WAvsPSD1_clone->SetTitleOffset(1.5,"XY");
  WAvsPSD1_clone->SetTitle(title_clone1);
  WAvsPSD1_clone->SetMarkerColor(kRed);

  WAoverPSD1_clone->GetXaxis()->SetTitle("Waveform Area/(Pulse Height/Discharge Time) (ns^2)");
  WAoverPSD1_clone->GetYaxis()->SetTitle("Events");
  WAoverPSD1_clone->GetXaxis()->CenterTitle();
  WAoverPSD1_clone->GetYaxis()->CenterTitle();
  WAoverPSD1_clone->SetTitleOffset(1.3,"XY");
  WAoverPSD1_clone->SetTitle(title_clone1);
  WAoverPSD1_clone->SetLineColor(kRed);

  HvsTAoverWA_clone->GetXaxis()->SetTitle("Tail Area/Waveform Area");
  HvsTAoverWA_clone->GetYaxis()->SetTitle("Pulse Height (V)");
  HvsTAoverWA_clone->GetXaxis()->CenterTitle();
  HvsTAoverWA_clone->GetYaxis()->CenterTitle();
  HvsTAoverWA_clone->SetTitleOffset(1.3,"XY");
  HvsTAoverWA_clone->SetTitle(title_clone1);
  HvsTAoverWA_clone->SetMarkerColor(kRed);

  PHvDT_clone->GetXaxis()->SetTitle("Discharge Time (ns)");
  PHvDT_clone->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvDT_clone->GetXaxis()->CenterTitle();
  PHvDT_clone->GetYaxis()->CenterTitle();
  PHvDT_clone->SetTitleOffset(1.3,"XY");
  PHvDT_clone->SetTitle(title_clone1);
  PHvDT_clone->SetMarkerColor(kRed);

  PHvFWHM_clone->GetXaxis()->SetTitle("Pulse Width (ns)");
  PHvFWHM_clone->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvFWHM_clone->GetXaxis()->CenterTitle();
  PHvFWHM_clone->GetYaxis()->CenterTitle();
  PHvFWHM_clone->SetTitleOffset(1.3,"XY");
  PHvFWHM_clone->SetTitle(title_clone1);
  PHvFWHM_clone->SetMarkerColor(kRed);
  
  //set labels for 2nd set of clones; labels are for superimposed plots

  const char* title_all = "Cf 252 and Cosmics";
  
  PHvWA_clone1->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  PHvWA_clone1->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvWA_clone1->GetXaxis()->CenterTitle();
  PHvWA_clone1->GetYaxis()->CenterTitle();
  PHvWA_clone1->SetTitleOffset(1.3,"XY");
  PHvWA_clone1->SetTitle(title_all);
  PHvWA_clone1->SetMarkerColor(kRed);
  
  TAvsWA_clone1->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  TAvsWA_clone1->GetYaxis()->SetTitle("Tail Area (V*ns)");
  TAvsWA_clone1->GetXaxis()->CenterTitle();
  TAvsWA_clone1->GetYaxis()->CenterTitle();
  TAvsWA_clone1->SetTitleOffset(1.3,"XY");
  TAvsWA_clone1->SetTitle(title_all);
  TAvsWA_clone1->SetMarkerColor(kRed);
  
  FWHMvsWA_clone1->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  FWHMvsWA_clone1->GetYaxis()->SetTitle("Pulse Width (ns)");
  FWHMvsWA_clone1->GetXaxis()->CenterTitle();
  FWHMvsWA_clone1->GetYaxis()->CenterTitle();
  FWHMvsWA_clone1->SetTitleOffset(1.3,"XY");
  FWHMvsWA_clone1->SetTitle(title_all);
  FWHMvsWA_clone1->SetMarkerColor(kRed);
  
  HoverWAvsWA_clone1->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  HoverWAvsWA_clone1->GetYaxis()->SetTitle("Pulse Height/Waveform Area (1/ns)");
  HoverWAvsWA_clone1->GetXaxis()->CenterTitle();
  HoverWAvsWA_clone1->GetYaxis()->CenterTitle();
  HoverWAvsWA_clone1->SetTitleOffset(1.3,"XY");
  HoverWAvsWA_clone1->SetTitle(title_all);
  HoverWAvsWA_clone1->SetMarkerColor(kRed);

  WAvsPSD1_clone1->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  WAvsPSD1_clone1->GetYaxis()->SetTitle("Pulse Height/Discharge Time (ns^2)");
  WAvsPSD1_clone1->GetXaxis()->CenterTitle();
  WAvsPSD1_clone1->GetYaxis()->CenterTitle();
  WAvsPSD1_clone1->SetTitleOffset(1.5,"XY");
  WAvsPSD1_clone1->SetTitle(title_all);
  WAvsPSD1_clone1->SetMarkerColor(kRed);

  WAoverPSD1_clone1->GetXaxis()->SetTitle("Waveform Area/(Pulse Height/Discharge Time) (ns^2)");
  WAoverPSD1_clone1->GetYaxis()->SetTitle("Events");
  WAoverPSD1_clone1->GetXaxis()->CenterTitle();
  WAoverPSD1_clone1->GetYaxis()->CenterTitle();
  WAoverPSD1_clone1->SetTitleOffset(1.3,"XY");
  WAoverPSD1_clone1->SetTitle(title_all);
  WAoverPSD1_clone1->SetLineColor(kRed);

  HvsTAoverWA_clone1->GetXaxis()->SetTitle("Tail Area/Waveform Area");
  HvsTAoverWA_clone1->GetYaxis()->SetTitle("Pulse Height (V)");
  HvsTAoverWA_clone1->GetXaxis()->CenterTitle();
  HvsTAoverWA_clone1->GetYaxis()->CenterTitle();
  HvsTAoverWA_clone1->SetTitleOffset(1.3,"XY");
  HvsTAoverWA_clone1->SetTitle(title_all);
  HvsTAoverWA_clone1->SetMarkerColor(kRed);

  PHvDT_clone1->GetXaxis()->SetTitle("Discharge Time (ns)");
  PHvDT_clone1->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvDT_clone1->GetXaxis()->CenterTitle();
  PHvDT_clone1->GetYaxis()->CenterTitle();
  PHvDT_clone1->SetTitleOffset(1.3,"XY");
  PHvDT_clone1->SetTitle(title_all);
  PHvDT_clone1->SetMarkerColor(kRed);

  PHvFWHM_clone1->GetXaxis()->SetTitle("Pulse Width (ns)");
  PHvFWHM_clone1->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvFWHM_clone1->GetXaxis()->CenterTitle();
  PHvFWHM_clone1->GetYaxis()->CenterTitle();
  PHvFWHM_clone1->SetTitleOffset(1.3,"XY");
  PHvFWHM_clone1->SetTitle(title_all);
  PHvFWHM_clone1->SetMarkerColor(kRed);
  
  //labels for divided histograms

  const char* title_div = "Cosmics over Cf 252";
  
  PHvWA_clone2->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  PHvWA_clone2->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvWA_clone2->GetXaxis()->CenterTitle();
  PHvWA_clone2->GetYaxis()->CenterTitle();
  PHvWA_clone2->SetTitleOffset(1.3,"XY");
  PHvWA_clone2->SetTitle(title_div);
  
  TAvsWA_clone2->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  TAvsWA_clone2->GetYaxis()->SetTitle("Tail Area (V*ns)");
  TAvsWA_clone2->GetXaxis()->CenterTitle();
  TAvsWA_clone2->GetYaxis()->CenterTitle();
  TAvsWA_clone2->SetTitleOffset(1.3,"XY");
  TAvsWA_clone2->SetTitle(title_div);
   
  FWHMvsWA_clone2->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  FWHMvsWA_clone2->GetYaxis()->SetTitle("Pulse Width (ns)");
  FWHMvsWA_clone2->GetXaxis()->CenterTitle();
  FWHMvsWA_clone2->GetYaxis()->CenterTitle();
  FWHMvsWA_clone2->SetTitleOffset(1.3,"XY");
  FWHMvsWA_clone2->SetTitle(title_div);
  
  HoverWAvsWA_clone2->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  HoverWAvsWA_clone2->GetYaxis()->SetTitle("Pulse Height/Waveform Area (1/ns)");
  HoverWAvsWA_clone2->GetXaxis()->CenterTitle();
  HoverWAvsWA_clone2->GetYaxis()->CenterTitle();
  HoverWAvsWA_clone2->SetTitleOffset(1.3,"XY");
  HoverWAvsWA_clone2->SetTitle(title_div);

  WAvsPSD1_clone2->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  WAvsPSD1_clone2->GetYaxis()->SetTitle("Pulse Height/Discharge Time (ns^2)");
  WAvsPSD1_clone2->GetXaxis()->CenterTitle();
  WAvsPSD1_clone2->GetYaxis()->CenterTitle();
  WAvsPSD1_clone2->SetTitleOffset(1.5,"XY");
  WAvsPSD1_clone2->SetTitle(title_div);

  WAoverPSD1_clone2->GetXaxis()->SetTitle("Waveform Area/(Pulse Height/Discharge Time) (ns^2)");
  WAoverPSD1_clone2->GetYaxis()->SetTitle("Efficiency");
  WAoverPSD1_clone2->GetXaxis()->CenterTitle();
  WAoverPSD1_clone2->GetYaxis()->CenterTitle();
  WAoverPSD1_clone2->SetTitleOffset(1.3,"XY");
  WAoverPSD1_clone2->SetTitle(title_div);

  HvsTAoverWA_clone2->GetXaxis()->SetTitle("Tail Area/Waveform Area");
  HvsTAoverWA_clone2->GetYaxis()->SetTitle("Pulse Height (V)");
  HvsTAoverWA_clone2->GetXaxis()->CenterTitle();
  HvsTAoverWA_clone2->GetYaxis()->CenterTitle();
  HvsTAoverWA_clone2->SetTitleOffset(1.3,"XY");
  HvsTAoverWA_clone2->SetTitle(title_div);
  HvsTAoverWA_clone2->SetMarkerColor(kRed);

  PHvDT_clone2->GetXaxis()->SetTitle("Discharge Time (ns)");
  PHvDT_clone2->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvDT_clone2->GetXaxis()->CenterTitle();
  PHvDT_clone2->GetYaxis()->CenterTitle();
  PHvDT_clone2->SetTitleOffset(1.3,"XY");
  PHvDT_clone2->SetTitle(title_div);
  PHvDT_clone2->SetMarkerColor(kRed);

  PHvFWHM_clone2->GetXaxis()->SetTitle("Pulse Width (ns)");
  PHvFWHM_clone2->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvFWHM_clone2->GetXaxis()->CenterTitle();
  PHvFWHM_clone2->GetYaxis()->CenterTitle();
  PHvFWHM_clone2->SetTitleOffset(1.3,"XY");
  PHvFWHM_clone2->SetTitle(title_div);
  PHvFWHM_clone2->SetMarkerColor(kRed);
  
  //load in 2nd root file

  TFile *_file1 = TFile::Open("Cf252_SiPM_0p7Gss.root");

  //set labels for plots in 2nd root file

  const char* title_2 = "Cf 252 Data for SiPM";
  
  PHvWA->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  PHvWA->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvWA->GetXaxis()->CenterTitle();
  PHvWA->GetYaxis()->CenterTitle();
  PHvWA->SetTitleOffset(1.3,"XY");
  PHvWA->SetTitle(title_2);
  
  TAvsWA->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  TAvsWA->GetYaxis()->SetTitle("Tail Area (V*ns)");
  TAvsWA->GetXaxis()->CenterTitle();
  TAvsWA->GetYaxis()->CenterTitle();
  TAvsWA->SetTitleOffset(1.3,"XY");
  TAvsWA->SetTitle(title_2);
  
  FWHMvsWA->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  FWHMvsWA->GetYaxis()->SetTitle("Pulse Width (ns)");
  FWHMvsWA->GetXaxis()->CenterTitle();
  FWHMvsWA->GetYaxis()->CenterTitle();
  FWHMvsWA->SetTitleOffset(1.3,"XY");
  FWHMvsWA->SetTitle(title_2);
  
  HoverWAvsWA->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  HoverWAvsWA->GetYaxis()->SetTitle("Pulse Height/Waveform Area (1/ns)");
  HoverWAvsWA->GetXaxis()->CenterTitle();
  HoverWAvsWA->GetYaxis()->CenterTitle();
  HoverWAvsWA->SetTitleOffset(1.3,"XY");
  HoverWAvsWA->SetTitle(title_2);
  
  WAvsPSD1->GetXaxis()->SetTitle("Waveform Area (V*ns)");
  WAvsPSD1->GetYaxis()->SetTitle("Pulse Height/Discharge Time (V/ns)");
  WAvsPSD1->GetXaxis()->CenterTitle();
  WAvsPSD1->GetYaxis()->CenterTitle();
  WAvsPSD1->SetTitleOffset(1.5,"XY");
  WAvsPSD1->SetTitle(title_2);

  WAoverPSD1->GetXaxis()->SetTitle("Waveform Area/(Pulse Height/Discharge Time) (ns^2)");
  WAoverPSD1->GetYaxis()->SetTitle("Events");
  WAoverPSD1->GetXaxis()->CenterTitle();
  WAoverPSD1->GetYaxis()->CenterTitle();
  WAoverPSD1->SetTitleOffset(1.3,"XY");
  WAoverPSD1->SetTitle(title_2);

  HvsTAoverWA->GetXaxis()->SetTitle("Tail Area/Waveform Area");
  HvsTAoverWA->GetYaxis()->SetTitle("Pulse Height (V)");
  HvsTAoverWA->GetXaxis()->CenterTitle();
  HvsTAoverWA->GetYaxis()->CenterTitle();
  HvsTAoverWA->SetTitleOffset(1.3,"XY");
  HvsTAoverWA->SetTitle(title_2);

  PHvDT->GetXaxis()->SetTitle("Discharge Time (ns)");
  PHvDT->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvDT->GetXaxis()->CenterTitle();
  PHvDT->GetYaxis()->CenterTitle();
  PHvDT->SetTitleOffset(1.3,"XY");
  PHvDT->SetTitle(title_2);

  PHvFWHM->GetXaxis()->SetTitle("Pulse Width (ns)");
  PHvFWHM->GetYaxis()->SetTitle("Pulse Height (V)");
  PHvFWHM->GetXaxis()->CenterTitle();
  PHvFWHM->GetYaxis()->CenterTitle();
  PHvFWHM->SetTitleOffset(1.3,"XY");
  PHvFWHM->SetTitle(title_2);
  
  //draw plots on canvases

  //gStyle->SetOptStat(0);
  //gStyle->SetLabelSize(1,"X");
  //gStyle->SetTitleOffset(.9, "XY”);
  
   c1->cd();
   PHvWA_clone->Draw();

   c2->cd();
   PHvWA->Draw();
  
   c3->cd();
   PHvWA_clone1->Draw();
   PHvWA->Draw("same");
   //leg = new TLegend(0.1,0.7,0.48,0.9);
   //leg->AddEntry(PHvWA_clone,"-20 mV Trigger Level","p");
   //leg->AddEntry(PHvWA,"-80 mV Trigger Level","p");
   //leg->Draw();

   c4->cd();
   //PHvWA_clone2->RebinY(1/5);
   //PHvWA->RebinY(1/5);
   PHvWA_clone2->Divide(PHvWA);
   PHvWA_clone2->Draw("colz");
   
   c5->cd();
   TAvsWA_clone->Draw();

   c6->cd();
   TAvsWA->Draw();
   /*TLine *line = new TLine(-8,-2.8,0,0);
   line->SetLineColor(kRed);
   line->Draw();*/
   
   c7->cd();
   TAvsWA_clone1->Draw();
   TAvsWA->Draw("same");
   //leg = new TLegend(0.1,0.7,0.48,0.9);
   //leg->AddEntry(TAvsWA_clone,"-20 mV Trigger Level","p");
   //leg->AddEntry(TAvsWA,"-80 mV Trigger Level","p");
   //leg->Draw();

   c8->cd();
   TAvsWA_clone2->RebinX(10);
   TAvsWA_clone2->RebinY(10);
   TAvsWA->RebinX(10);
   TAvsWA->RebinY(10);
   TAvsWA_clone2->Divide(TAvsWA);
   TAvsWA_clone2->Draw("colz");
   
   c9->cd();
   FWHMvsWA_clone->Draw();

   c10->cd();
   FWHMvsWA->Draw();

   c11->cd();
   FWHMvsWA_clone1->Draw();
   FWHMvsWA->Draw("same");
   //leg = new TLegend(0.1,0.7,0.48,0.9);
   //leg->AddEntry(FWHMvsWA_clone,"-20 mV Trigger Level","p");
   //leg->AddEntry(FWHMvsWA,"-80 mV Trigger Level","p");
   //leg->Draw();

   c12->cd();
   FWHMvsWA_clone2->RebinX(16);
   FWHMvsWA_clone2->RebinY(16);
   FWHMvsWA->RebinX(16);
   FWHMvsWA->RebinY(16);
   FWHMvsWA_clone2->Divide(FWHMvsWA);
   FWHMvsWA_clone2->Draw("colz");
   
   c13->cd();
   HoverWAvsWA_clone->Draw();

   c14->cd();
   HoverWAvsWA->Draw();

   c15->cd();
   HoverWAvsWA_clone1->Draw();
   HoverWAvsWA->Draw("same");
   //leg = new TLegend(0.1,0.7,0.48,0.9);
   //leg->AddEntry(HoverWAvsWA_clone,"-20 mV Trigger Level","p");
   //leg->AddEntry(HoverWAvsWA,"-80 mV Trigger Level","p");
   //leg->Draw();

   c16->cd();
   //HoverWAvsWA_clone2->RebinX(4);
   //HoverWAvsWA_clone2->RebinY(4);
   //HoverWAvsWA->RebinX(4);
   //HoverWAvsWA->RebinY(4);
   HoverWAvsWA_clone2->Divide(HoverWAvsWA);
   HoverWAvsWA_clone2->Draw("colz");
   
   c17->cd();
   WAvsPSD1_clone->Draw();

   c18->cd();   
   WAvsPSD1->Draw();

   c19->cd();
   leg = new TLegend(0.1,0.7,0.48,0.9);
   leg->AddEntry(WAoverPSD1_clone,"Cosmics","p");
   leg->AddEntry(WAoverPSD1,"PuBe","p");
   WAvsPSD1_clone1->Draw();
   WAvsPSD1->Draw("same");
   //leg->Draw();

   c20->cd();
   WAvsPSD1_clone2->RebinX(10);
   WAvsPSD1_clone2->RebinY(10);
   WAvsPSD1->RebinX(10);
   WAvsPSD1->RebinY(10);
   WAvsPSD1_clone2->Divide(WAvsPSD1);
   WAvsPSD1_clone2->Draw("colz");

   c21->cd();
   WAoverPSD1_clone->Draw();

   c22->cd();
   WAoverPSD1->Draw();

   c23->cd();
   leg = new TLegend(0.1,0.7,0.48,0.9);
   leg->AddEntry(WAoverPSD1,"PuBe","l");
   leg->AddEntry(WAoverPSD1_clone1,"Cosmics","l");
   WAoverPSD1->Draw();
   WAoverPSD1_clone1->Draw("same");
   leg->Draw();

   c24->cd();    
   WAoverPSD1_clone2->Divide(WAoverPSD1);
   WAoverPSD1_clone2->SetMaximum(3);
   WAoverPSD1_clone2->Draw();

   c25->cd();
   HvsTAoverWA_clone->Draw();

   c26->cd();
   HvsTAoverWA->Draw();

   c27->cd();
   HvsTAoverWA_clone1->Draw();
   HvsTAoverWA->Draw("same");

   c28->cd();
   HvsTAoverWA_clone2->RebinX(4);
   HvsTAoverWA_clone2->RebinY(4);
   HvsTAoverWA->RebinX(4);
   HvsTAoverWA->RebinY(4);
   HvsTAoverWA_clone2->Divide(HvsTAoverWA);
   HvsTAoverWA_clone2->Draw("colz");

   c29->cd();
   PHvDT_clone->Draw();

   c30->cd();
   PHvDT->Draw();

   c31->cd();
   PHvDT_clone1->Draw();
   PHvDT->Draw("same");

   c32->cd();
   PHvDT_clone2->RebinX(4);
   PHvDT_clone2->RebinY(4);
   PHvDT->RebinX(4);
   PHvDT->RebinY(4);
   PHvDT_clone2->Divide(PHvDT);
   PHvDT_clone2->Draw("colz");
   
   c33->cd();
   PHvFWHM_clone->Draw();

   c34->cd();
   PHvFWHM->Draw();

   c35->cd();
   PHvFWHM_clone1->Draw();
   PHvFWHM->Draw("same");
   
   c36->cd();
   PHvFWHM_clone2->RebinX(16);
   PHvFWHM_clone2->RebinY(16);
   PHvFWHM->RebinX(16);
   PHvFWHM->RebinY(16);
   PHvFWHM_clone2->Divide(PHvFWHM);
   PHvFWHM_clone2->Draw("colz");

   
   
   
}

