PlotMacro(){

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TFile *_file0 = TFile::Open("VUVham_Jan30_2p9LED_20ns_5mV_10k.root");


  h2->SetLineColor(1);
  h2->GetXaxis()->SetTitle("Pulse Area [V*ns]");
  h2->GetYaxis()->SetTitle("Entries");
  h2->GetXaxis()->CenterTitle();
  h2->GetYaxis()->CenterTitle();
  h2->SetTitleSize(0.05,"XY");
  h2->SetTitleOffset(.9, "XY");
  h2->SetStats(0);
  h2->Draw();

  TF1 *S = new TF1("S","[0]*exp(-((x-[1])^2)/(2*([2]^2)))*(1/(2*[2]*TMath::Pi()))+[3]*exp(-((x-[4])^2)/(2*([5]^2)))*(1/(2*[5]*TMath::Pi()))+[6]*exp(-((x-[7])^2)/(2*([8]^2)))*(1/(2*[8]*TMath::Pi()))+[9]*exp(-((x-[10])^2)/(2*([11]^2)))*(1/(2*[11]*TMath::Pi()))+[12]*exp(-((x-[13])^2)/(2*([14]^2)))*(1/(2*[14]*TMath::Pi()))",-75,50);

  S->SetParameter(0,14);//w
  S->SetParameter(1,-15);//sigma_0
  S->SetParameter(2,5);//Q_0
  S->SetParameter(3,14);//w
  S->SetParameter(4,0);//sigma_0
  S->SetParameter(5,4);//Q_0
  S->SetParameter(6,14);//w
  S->SetParameter(7,15);//sigma_0
  S->SetParameter(8,5);//Q_0
  S->SetParameter(9,14);//w
  S->SetParameter(10,5);//sigma_0
  S->SetParameter(11,5);//Q_0
  S->SetParameter(12,14);//w
  S->SetParameter(13,-5);//sigma_0
  S->SetParameter(14,5);//Q_0

  h2->Fit("S","R");
  
  S->SetLineColor(4);







 
}
