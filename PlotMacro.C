  /*
    Author: Will Flanagan
    root [0] .x PlotMacro.C

    Insert root file name manually.
*/
   

PlotMacro(){

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TFile *_file0 = TFile::Open("");

  hAllWaveforms->GetXaxis()->SetTitle("Time [ns]");
  hAllWaveforms->GetYaxis()->SetTitle("Voltage [V]");
  hAllWaveforms->GetXaxis()->CenterTitle();
  hAllWaveforms->GetYaxis()->CenterTitle();
  hAllWaveforms->SetTitleSize(0.05,"XY");
  hAllWaveforms->SetTitleOffset(.9, "XY");

  hAllWaveforms->Draw("colz");

  TLegend* legend = new TLegend(0.11,0.91,0.51,0.97,"LN_{2} V_{B}=46.5V","NDC");
  legend->SetTextSize(.05);
  legend->SetTextColor(4);
  legend->SetFillColor(0);
  legend->SetBorderSize(0);
  legend->Draw("same");

  c1->SaveAs("Cold_Waveforms.jpg");

  h1->SetLineColor(1);
  h1->GetXaxis()->SetTitle("");
  h1->GetYaxis()->SetTitle("Entries");
  h1->GetXaxis()->CenterTitle();
  h1->GetYaxis()->CenterTitle();
  h1->SetTitleSize(0.05,"XY");
  h1->SetTitleOffset(.9, "XY");
  h1->SetStats(0);
  h1->Draw();

  legend->Draw("same");

  c1->SaveAs("Cold_PEpeaks.jpg");

  TFile *_file0 = TFile::Open("VUV_SiPM_1PE_57V_LED2920_4d3ns_500k.root");

  hAllWaveforms->GetXaxis()->SetTitle("Time [ns]");
  hAllWaveforms->GetYaxis()->SetTitle("Voltage [V]");
  hAllWaveforms->GetXaxis()->CenterTitle();
  hAllWaveforms->GetYaxis()->CenterTitle();
  hAllWaveforms->SetTitleSize(0.05,"XY");
  hAllWaveforms->SetTitleOffset(.9, "XY");

  hAllWaveforms->Draw("colz");

  TLegend* legend2 = new TLegend(0.11,0.91,0.51,0.97,"Room Temperature V_{B}=57V","NDC");
  legend2->SetTextSize(.05);
  legend2->SetTextColor(2);
  legend2->SetFillColor(0);
  legend2->SetBorderSize(0);
  legend2->Draw("same");

  c1->SaveAs("Hot_Waveforms.jpg");

  h1->SetLineColor(1);
  h1->GetXaxis()->SetTitle("Pulse Area [V*ns]");
  h1->GetYaxis()->SetTitle("Entries");
  h1->GetXaxis()->CenterTitle();
  h1->GetYaxis()->CenterTitle();
  h1->SetTitleSize(0.05,"XY");
  h1->SetTitleOffset(.9, "XY");
  h1->SetStats(0);
  h1->Draw();

  legend2->Draw("same");

  c1->SaveAs("Hot_PEpeaks.jpg");

}
