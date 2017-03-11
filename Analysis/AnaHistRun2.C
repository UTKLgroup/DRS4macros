#include "TF2.h"
#include "TH2.h"
#include "TMath.h"
#include "TColor.h"

Double_t g2(Double_t * x, Double_t * par) {
  Double_t rho = par[5] / (par[2] * par[4]);
  Double_t z = TMath::Power((x[0] - par[1]) / par[2], 2) + TMath::Power((x[1] - par[3]) / par[4], 2) - (2 * rho * (x[0] - par[1]) * (x[1] - par[3])) / (par[2] * par[4]);
  Double_t g = par[0] * TMath::Exp(-0.5 * z / (1 - TMath::Sqrt(TMath::Power(rho, 2))));

  return g;
}

Double_t bigaus(Double_t * x, Double_t * par) {
  Double_t * p1 = &par[0];
  Double_t result = g2(x, p1);

  return result;
}

Double_t doublebigaus(Double_t * x, Double_t * par) {
  Double_t * p1 = &par[0];
  Double_t * p2 = &par[6];
  Double_t result = g2(x, p1) + g2(x, p2);

  return result;
}


void AnaHistRun2() {
  TFile * f = new TFile("./07-14-16-drs4run2.Analyzed.root");


  TH1D * hAreaRatio = (TH1D *) f->Get("hAreaRatio");
  TCanvas * cAreaRatio = new TCanvas();
  hAreaRatio->Sumw2();
  TF1 * fitAreaRatio = new TF1("fitAreaRatio", "gaus(0) + gaus(3)", 0.18, 0.48);
  fitAreaRatio->SetParameters(10000, 0.24, 0.0245, 6000, 0.41, 0.045);
  fitAreaRatio->SetLineColor(2);
  hAreaRatio->Fit("fitAreaRatio", "r");
  hAreaRatio->GetXaxis()->SetTitle("Charge Ratio [A.U.]");
  hAreaRatio->Draw();
  double FOMAreaRatio = TMath::Abs(fitAreaRatio->GetParameter(1) - fitAreaRatio->GetParameter(4)) / (fitAreaRatio->GetParameter(2) + fitAreaRatio->GetParameter(5));
  std::cout << "FOM: " << FOMAreaRatio << std::endl;
  TF1 * fit1AreaRatio = new TF1("fit1AreaRatio", "gaus(0)", 0., 0.7);
  fit1AreaRatio->SetParameters(fitAreaRatio->GetParameter(0), fitAreaRatio->GetParameter(1), fitAreaRatio->GetParameter(2));
  fit1AreaRatio->SetLineStyle(2);
  fit1AreaRatio->SetLineColor(2);
  fit1AreaRatio->Draw("same");
  TF1 * fit2AreaRatio = new TF1("fit2AreaRatio", "gaus(0)", 0., 0.7);
  fit2AreaRatio->SetParameters(fitAreaRatio->GetParameter(3), fitAreaRatio->GetParameter(4), fitAreaRatio->GetParameter(5));
  fit2AreaRatio->SetLineStyle(2);
  fit2AreaRatio->SetLineColor(3);
  fit2AreaRatio->Draw("same");
  TF1 * fitAllAreaRatio = new TF1("fitAllAreaRatio", "gaus(0) + gaus(3)", 0., 0.7);
  fitAllAreaRatio->SetParameters(fitAreaRatio->GetParameter(0), fitAreaRatio->GetParameter(1), fitAreaRatio->GetParameter(2), fitAreaRatio->GetParameter(3), fitAreaRatio->GetParameter(4), fitAreaRatio->GetParameter(5));
  fitAllAreaRatio->SetLineStyle(1);
  fitAllAreaRatio->SetLineColor(kBlack);
  fitAllAreaRatio->Draw("same");
  TLegend * legAreaRatio = new TLegend(0.65, 0.65, 0.85, 0.85);
  legAreaRatio->SetHeader(Form("FOM = %.2f", FOMAreaRatio));
  legAreaRatio->AddEntry(hAreaRatio, "Data", "lep");
  legAreaRatio->AddEntry(fit1AreaRatio, "Gamma Fit", "l");
  legAreaRatio->AddEntry(fit2AreaRatio, "Neutron Fit", "l");
  legAreaRatio->AddEntry(fitAllAreaRatio, "Combined Fit", "l");
  legAreaRatio->Draw();
  cAreaRatio->SaveAs("TailRatio.eps");


  /*  const Int_t npar = 6;

  TH1D * hPSD = (TH1D *) f->Get("hPSD");
  TCanvas * cPSD = new TCanvas();
  Double_t f2params1[npar] = {400, 0.40, 0.05, 0.04, 0.02, -0.0006};
  TF2 * f1 = new TF2("f1", bigaus, 0.36, 0.45, 0.042, 0.06, npar);
  f1->SetParameters(f2params1);

  Double_t f2params2[npar] = {728, 0.23, 0.05, 0.07, 0.009, -2.79e-04};
  TF2 * f2 = new TF2("f2", bigaus, 0.18, 0.28, 0.062, 0.078, npar);
  f2->SetParameters(f2params2);

  hPSD->Fit(f1, "r0");
  hPSD->Fit(f2, "r0");

  hPSD->GetXaxis()->SetTitle("Charge Ratio [A.U.]");
  hPSD->GetYaxis()->SetTitle("Amplitude Ratio [A.U.]");
  hPSD->GetZaxis()->SetTitle();
  hPSD->Draw("colz");

  TF2 * f1ReDraw = new TF2("f1ReDraw", bigaus, 0., 0.6, 0., 0.15, npar);
  TF2 * f2ReDraw = new TF2("f2ReDraw", bigaus, 0., 0.6, 0., 0.15, npar);
  Double_t f1RedrawParams[6];
  Double_t f2RedrawParams[6];
  for (int i = 0; i < 6; i++) {
    f1RedrawParams[i] = f1->GetParameter(i);
    f2RedrawParams[i] = f2->GetParameter(i);
  }
  f1ReDraw->SetParameters(f1RedrawParams);
  f2ReDraw->SetParameters(f2RedrawParams);
  f1ReDraw->SetContour(4);
  f2ReDraw->SetContour(4);
  f1ReDraw->SetNpx(500);
  f1ReDraw->SetNpy(500);
  f2ReDraw->SetNpx(500);
  f2ReDraw->SetNpy(500);
  f1ReDraw->SetLineColor(kGreen);
  f2ReDraw->SetLineColor(kRed);
  f1ReDraw->Draw("cont3 same");
  f2ReDraw->Draw("cont3 same");

  TLegend * legPSD = new TLegend(0.65, 0.65, 0.85, 0.85);
  legPSD->AddEntry(hPSD, "Data", "lep");
  legPSD->AddEntry(f1ReDraw, "Gamma Fit", "l");
  legPSD->AddEntry(f2ReDraw, "Neutron Fit", "l");
  legPSD->Draw();

/*
  TF2 * fitAllPSD = new TF2("fitAllPSD", doublebigaus, 0., 0.6, 0., 0.15, 12);
  Double_t finalFit[12];
  for (int i = 0; i < 6; i++) {
    finalFit[i]     = f1->GetParameter(i);
    finalFit[i + 6] = f2->GetParameter(i);
  }
  fitAllPSD->SetParameters(finalFit);
  hPSD->GetXaxis()->SetTitle("Charge Ratio [A.U.]");
  hPSD->GetYaxis()->SetTitle("Amplitude Ratio [A.U.]");
  hPSD->GetZaxis()->SetTitle();
  gStyle->SetPalette(kBird);
  hPSD->Draw("colz");
  fitAllPSD->SetNpx(500);
  fitAllPSD->SetNpy(500);
  fitAllPSD->SetContour(3);
  fitAllPSD->Draw("cont3 same");


  cPSD->SaveAs("PSDFit.eps"); */

}
