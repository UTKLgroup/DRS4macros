Fit(){

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  //Insert Filename
  TFile *_file0 = TFile::Open("");


  h2->SetLineColor(1);
  h2->GetXaxis()->SetTitle("Pulse Area [V*ns]");
  h2->GetYaxis()->SetTitle("Entries");
  h2->GetXaxis()->CenterTitle();
  h2->GetYaxis()->CenterTitle();
  h2->SetStats(0);
  h2->Draw();
 
 TF1 *S = new TF1("S","(((1.0-[0])/([1]*sqrt(2*TMath::Pi()))*exp(-((x-[2])**2.0)/(2.0*[1]**2.0))+[0]*(x>[2] ? 1.0 : 0.0)*[3]*exp(-[3]*(x-[2])))*exp(-[4])+(([4]**1.0)*exp(-[4])/1.0)*(1.0/([5]*sqrt(2*TMath::Pi()*1.0)))*exp(-((x-[2]-([0]/[3])-1.0*[6])**2.0)/(2*1.0*[5]**2.0))+(([4]**2.0)*exp(-[4])/2.0)*(1.0/([5]*sqrt(2*TMath::Pi()*2.0)))*exp(-((x-[2]-([0]/[3])-2.0*[6])**2.0)/(2*2.0*[5]**2.0))+(([4]**3.0)*exp(-[4])/6.0)*(1.0/([5]*sqrt(2*TMath::Pi()*3.0)))*exp(-((x-[2]-([0]/[3])-3.0*[6])**2.0)/(2*3.0*[5]**2.0))+(([4]**4.0)*exp(-[4])/24.0)*(1.0/([5]*sqrt(2*TMath::Pi()*4.0)))*exp(-((x-[2]-([0]/[3])-4.0*[6])**2.0)/(2*4.0*[5]**2.0))+(([4]**5.0)*exp(-[4])/120.0)*(1.0/([5]*sqrt(2*TMath::Pi()*5.0)))*exp(-((x-[2]-([0]/[3])-5.0*[6])**2.0)/(2*5.0*[5]**2.0))+(([4]**6.0)*exp(-[4])/720.0)*(1.0/([5]*sqrt(2*TMath::Pi()*6.0)))*exp(-((x-[2]-([0]/[3])-6.0*[6])**2.0)/(2*6.0*[5]**2.0)))*[7]", 50,400);
  TF1 *S_ped = new TF1("S_ped","((1.0-[0])/([1]*sqrt(2*TMath::Pi()))*exp(-((x-[2])**2.0)/(2.0*[1]**2.0))+[0]*(x>[2] ? 1.0 : 0.0)*[3]*exp(-[3]*(x-[2])))*exp(-[4])", -50, 300);
  TF1 *S_1pe = new TF1("S_1pe","(([3]**1.0)*exp(-[3])/1.0)*(1.0/([4]*sqrt(2*TMath::Pi()*1.0)))*exp(-((x-[1]-([0]/[2])-1.0*[5])**2.0)/(2*1.0*[4]**2.0))", -50, 300);
  TF1 *S_2pe = new TF1("S_2pe","(([3]**2.0)*exp(-[3])/2.0)*(1.0/([4]*sqrt(2*TMath::Pi()*2.0)))*exp(-((x-[1]-([0]/[2])-2.0*[5])**2.0)/(2*2.0*[4]**2.0))", -50, 300);
  TF1 *S_3pe = new TF1("S_3pe","(([3]**3.0)*exp(-[3])/6.0)*(1.0/([4]*sqrt(2*TMath::Pi()*3.0)))*exp(-((x-[1]-([0]/[2])-3.0*[5])**2.0)/(2*3.0*[4]**2.0))", -50, 300);
  TF1 *S_4pe = new TF1("S_4pe","(([3]**4.0)*exp(-[3])/24.0)*(1.0/([4]*sqrt(2*TMath::Pi()*4.0)))*exp(-((x-[1]-([0]/[2])-4.0*[5])**2.0)/(2*4.0*[4]**2.0))", -50, 300);
  TF1 *S_5pe = new TF1("S_5pe","(([3]**5.0)*exp(-[3])/120.0)*(1.0/([4]*sqrt(2*TMath::Pi()*5.0)))*exp(-((x-[1]-([0]/[2])-5.0*[5])**2.0)/(2*5.0*[4]**2.0))", -50, 300);
  TF1 *S_6pe = new TF1("S_6pe","(([3]**6.0)*exp(-[3])/720.0)*(1.0/([4]*sqrt(2*TMath::Pi()*6.0)))*exp(-((x-[1]-([0]/[2])-6.0*[5])**2.0)/(2*6.0*[4]**2.0))", -50, 300);

  S->SetParameter(0,0.383);//w
  S->SetParameter(1,0.192);//sigma_0
  S->SetParameter(2,75.26);//Q_0
  S->SetParameter(3,0.034);//alpha
  S->SetParameter(4,1.68);//mu
  S->SetParameter(5,11.73);//sigma_1
  S->SetParameter(6,20.04);//Q_1
  S->SetParameter(7,1);//Norm
    h2->Fit("S","RMVW");
  

  S_ped->SetParameter(0,S->GetParameter(0));//w
  S_ped->SetParameter(1,S->GetParameter(1));//sigma_0
  S_ped->SetParameter(2,S->GetParameter(2));//Q_0
  S_ped->SetParameter(3,S->GetParameter(3));//alpha
  S_ped->SetParameter(4,S->GetParameter(4));//mu
  S_1pe->SetParameter(0,S->GetParameter(0));//w
  S_1pe->SetParameter(1,S->GetParameter(1));//Q_0
  S_1pe->SetParameter(2,S->GetParameter(3));//alpha
  S_1pe->SetParameter(3,S->GetParameter(4));//mu
  S_1pe->SetParameter(4,S->GetParameter(5));//sigma_1
  S_1pe->SetParameter(5,S->GetParameter(6));//Q_1
  S_2pe->SetParameter(0,S->GetParameter(0));//w
  S_2pe->SetParameter(1,S->GetParameter(1));//Q_0
  S_2pe->SetParameter(2,S->GetParameter(3));//alpha
  S_2pe->SetParameter(3,S->GetParameter(4));//mu
  S_2pe->SetParameter(4,S->GetParameter(5));//sigma_1
  S_2pe->SetParameter(5,S->GetParameter(6));//Q_1
  S_3pe->SetParameter(0,S->GetParameter(0));//w
  S_3pe->SetParameter(1,S->GetParameter(1));//Q_0
  S_3pe->SetParameter(2,S->GetParameter(3));//alpha
  S_3pe->SetParameter(3,S->GetParameter(4));//mu
  S_3pe->SetParameter(4,S->GetParameter(5));//sigma_1
  S_3pe->SetParameter(5,S->GetParameter(6));//Q_1
  S_4pe->SetParameter(0,S->GetParameter(0));//w
  S_4pe->SetParameter(1,S->GetParameter(1));//Q_0
  S_4pe->SetParameter(2,S->GetParameter(3));//alpha
  S_4pe->SetParameter(3,S->GetParameter(4));//mu
  S_4pe->SetParameter(4,S->GetParameter(5));//sigma_1
  S_4pe->SetParameter(5,S->GetParameter(6));//Q_1
  S_5pe->SetParameter(0,S->GetParameter(0));//w
  S_5pe->SetParameter(1,S->GetParameter(1));//Q_0
  S_5pe->SetParameter(2,S->GetParameter(3));//alpha
  S_5pe->SetParameter(3,S->GetParameter(4));//mu
  S_5pe->SetParameter(4,S->GetParameter(5));//sigma_1
  S_5pe->SetParameter(5,S->GetParameter(6));//Q_1
  S_6pe->SetParameter(0,S->GetParameter(0));//w
  S_6pe->SetParameter(1,S->GetParameter(1));//Q_0
  S_6pe->SetParameter(2,S->GetParameter(3));//alpha
  S_6pe->SetParameter(3,S->GetParameter(4));//mu
  S_6pe->SetParameter(4,S->GetParameter(5));//sigma_1
  S_6pe->SetParameter(5,S->GetParameter(6));//Q_1
  S->SetLineColor(4);
 
  
S_ped->Draw("same");
  S_1pe->Draw("same");
  S_2pe->Draw("same");
  S_3pe->Draw("same");
  S_4pe->Draw("same");
  S_5pe->Draw("same");
  S_6pe->Draw("same");
  

}
