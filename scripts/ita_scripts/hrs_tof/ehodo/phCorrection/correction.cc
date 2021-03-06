/*
  "correction.cc"
  
  Pulse height correction for TOF detector @Kyoto-U.
  Toshi Gogami , 15Oct2014
*/

#include "correction.hh"
#include "ParamMan.hh"
#include <TFile.h>
#include <TTree.h>
#include <TH2F.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TStyle.h>
#include <TVirtualFitter.h>
#include <TMinuit.h>
#include <TGraph.h>
#include <TObjArray.h>
#include <TMath.h>
#include <TApplication.h>
#include <TROOT.h>

#include <fstream>
#include <iostream>
using namespace std;

//void correction(){
int main(int argc, char** argv){
  // ====== General conditions =========
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);
  
  // ===== TApplication ================
  TApplication app("app",&argc,argv); 
  
  // ==== Input data ===================
  Int_t nite = 0; // The number of iteration
  char inputfilename[500];
  char outputfilename[500];
  char chi2filename[500];
  char resultname[500];
  char parfilename[500];
  char tempc[500];
  char printname[500];
  bool printflag=false;
  ifstream* input_dragon = new ifstream("input.dat");
  *input_dragon >> tempc >> nite;
  *input_dragon >> tempc >> inputfilename;
  *input_dragon >> tempc >> outputfilename;
  *input_dragon >> tempc >> chi2filename;
  *input_dragon >> tempc >> resultname;
  *input_dragon >> tempc >> parfilename;
  *input_dragon >> tempc >> w1 >> w2 >> w3 >> w4 >> wtof;
  *input_dragon >> tempc >> printname >> printflag;
  input_dragon->close();
  
  // ~~~~ Graph of weights ~~~~
  const int nw = 5;
  double xw[nw];
  double yw[nw];
  xw[0] = 1.0; yw[0] = w1; // ch1
  xw[1] = 2.0; yw[1] = w2; // ch2
  xw[2] = 3.0; yw[2] = w3; // ch3
  xw[3] = 4.0; yw[3] = w4; // ch4
  xw[4] = 5.0; yw[4] = wtof; // TOF
  TGraph* gr_weight = new TGraph(nw,xw,yw);
  gr_weight->SetName("gr_weight");
  gr_weight->SetTitle("Weights");
  gr_weight->GetYaxis()->SetTitle("weight");
  
  
  // ============ Open ROOT file ===================
  //TFile* f1 = new TFile("data.root");
  TFile* f1 = new TFile(inputfilename);
  TTree* tree1 = (TTree*)f1->Get("tree");
  double adc1, adc2, adc3, adc4;
  //double tdc1, tdc2, tdc3, tdc4;
  double time1, time2, time3, time4;
  //Double_t tof1_meant, tof2_meant;
  double tof;
  double ent;
  ent = tree1->GetEntries();
  
  tree1->SetBranchAddress("adc1",&adc1);
  tree1->SetBranchAddress("adc2",&adc2);
  tree1->SetBranchAddress("adc3",&adc3);
  tree1->SetBranchAddress("adc4",&adc4);
  //tree1->SetBranchAddress("tdc1",&tdc1);
  //tree1->SetBranchAddress("tdc2",&tdc2);
  //tree1->SetBranchAddress("tdc3",&tdc3);
  //tree1->SetBranchAddress("tdc4",&tdc4);
  tree1->SetBranchAddress("time1",&time1);
  tree1->SetBranchAddress("time2",&time2);
  tree1->SetBranchAddress("time3",&time3);
  tree1->SetBranchAddress("time4",&time4);
  //tree1->SetBranchAddress("tof1_meant",&tof1_meant);
  //tree1->SetBranchAddress("tof2_meant",&tof2_meant);
  tree1->SetBranchAddress("tof",&tof);
  
  // ========= Event selection =======================
  bool gencut=false;
  int j=0;
  for(int i=0 ; i<ent ; i++){
    tree1->GetEntry(i);
    // ~~~~ Cut conditions ~~~~
    if(adc1>500 && adc1<2000.0 && fabs(time1-31.6)<3.0 && 
       adc2>500 && adc2<2000.0 && fabs(time2-21.3)<3.0 && 
       adc3>580 && adc3<2000.0 && fabs(time3-24.6)<3.0 && 
       adc4>580 && adc4<2000.0 && fabs(time4-22.8)<3.0){
      gencut = true;
    }
    else gencut = false;
    
    if(gencut && j<max_event){
      // ---- ADC -----
      //a1[j] = (double)adc1;
      //a2[j] = (double)adc2;
      //a3[j] = (double)adc3;
      //a4[j] = (double)adc4;
      a1[j] = adc1;
      a2[j] = adc2;
      a3[j] = adc3;
      a4[j] = adc4;
      // ---- TDC -----
      t1[j] = time1;
      t2[j] = time2;
      t3[j] = time3;
      t4[j] = time4;
      //filled_event++;
      j++;
    }
  }
  filled_event = j;
  //cout << filled_event << endl;
  
  // ========= Correction starts !!!!!!! =========
  ParamMan* pman = new ParamMan(); // Parameter Manager
  ifstream* ifs = new ifstream("./initial/ini_20171119.dat");
  char temps[500]; // temporary strings
  //double par[100]; // parameters
  // double* par; // parameters
  int tempn;       // temporary integer
  //Int_t allparam = 0;
  *ifs >> temps >> temps >> tempn >> par[0] >> par[1] >> par[2];
  pman->SetParam1(par[0],par[1],par[2]);
  allparam = allparam + tempn;
  *ifs >> temps >> temps >> tempn >> par[3] >> par[4] >> par[5];
  pman->SetParam2(par[3],par[4],par[5]);
  allparam = allparam + tempn;
  *ifs >> temps >> temps >> tempn >> par[6] >> par[7] >> par[8];
  pman->SetParam3(par[6],par[7],par[8]);
  allparam = allparam + tempn;
  *ifs >> temps >> temps >> tempn >> par[9] >> par[10] >> par[11];
  pman->SetParam4(par[9],par[10],par[11]);
  allparam = allparam + tempn;
  //pman->CheckParam(); // Check Parameters

  ofstream* ofs = new ofstream(chi2filename);
  double chi2;
  const int ngr = nite;
  double chi2_gr[ngr];
  double x_gr[ngr];
  //for (int i=0 ; i<10 ; i++){
  //for (int i=0 ; i<100 ; i++){
  for (int i=0 ; i<nite ; i++){
    // ===== Parameter optimization =======
    chi2 = tune(par,i); // ================
    // ====================================
    
    //cout << " " << i+1 << ": chi2=" << chi2 << endl;
    *ofs << i+1 << " " << chi2 << endl;
    x_gr[i] = i+1;
    chi2_gr[i] = chi2;
  }
  ofs->close();
  
  // ~~~~ Graph of chi-square ~~~~~~
  TGraph* gr = new TGraph(ngr,x_gr,chi2_gr);
  gr->SetName("gr");
  gr->SetTitle("Chi-square");
  gr->GetXaxis()->SetTitle("Number of iteration");
  gr->GetYaxis()->SetTitle("#chi^{2}");
  
  // ~~~~ Histograms ~~~~~~
  //TH1F* h_tof = new TH1F("h_tof","TOF w/o correction",200,-5.0,5.0);
  TH1F* h_tof = new TH1F("h_tof","TOF w/o correction",100,-2.5,2.5);
  h_tof->GetXaxis()->SetTitle("[ns]");
  //h_tof->GetYaxis()->SetTitle("[Counts / 0.1 ns]");
  h_tof->GetYaxis()->SetTitle("[Counts / 0.05 ns]");
  h_tof->SetLineColor(9);
  h_tof->SetLineStyle(7);
  h_tof->SetLineWidth(2);
  TH1F* h_ctof = (TH1F*)h_tof->Clone("h_ctof");
  h_ctof->SetTitle("TOF w/ correction");
  h_ctof->SetLineColor(1);
  h_ctof->SetLineStyle(1);
  h_ctof->SetLineWidth(2);
  
  // ===== Cretate NEW ROOT file ==============
  Double_t ctime1, ctime2, ctime3, ctime4;
  Double_t ctof;
  TFile* fnew = new TFile(outputfilename,"recreate");
  TTree* tree2 = new TTree("tree","Data of TOF test (after correction)");
  tree2->Branch("adc1",&adc1,"adc1/D");
  tree2->Branch("adc2",&adc2,"adc2/D");
  tree2->Branch("adc3",&adc3,"adc3/D");
  tree2->Branch("adc4",&adc4,"adc4/D");
  //tree1->Branch("tdc1",&tdc1,"tdc1/I");
  //tree1->Branch("tdc2",&tdc2,"tdc2/I");
  //tree1->Branch("tdc3",&tdc3,"tdc3/I");
  //tree1->Branch("tdc4",&tdc4,"tdc4/I");
  tree2->Branch("time1",&time1,"time1/D");
  tree2->Branch("time2",&time2,"time2/D");
  tree2->Branch("time3",&time3,"time3/D");
  tree2->Branch("time4",&time4,"time4/D");
  tree2->Branch("ctime1",&ctime1,"ctime1/D");
  tree2->Branch("ctime2",&ctime2,"ctime2/D");
  tree2->Branch("ctime3",&ctime3,"ctime3/D");
  tree2->Branch("ctime4",&ctime4,"ctime4/D");
  //tree1->Branch("tof1_meant",&tof1_meant,"tof1_meant/D");
  //tree1->Branch("tof2_meant",&tof2_meant,"tof2_meant/D");
  tree2->Branch("tof",&tof,"tof/D");
  tree2->Branch("ctof",&ctof,"ctof/D");
  
  for(int i=0 ; i<filled_event ; i++){
    adc1 = a1[i];
    adc2 = a2[i];
    adc3 = a3[i];
    adc4 = a4[i];
    time1 = t1[i];
    time2 = t2[i];
    time3 = t3[i];
    time4 = t4[i];
    ctime1 = calcTime(par[0], par[1],  par[2], a1[i], t1[i]);
    ctime2 = calcTime(par[3], par[4],  par[5], a2[i], t2[i]);
    ctime3 = calcTime(par[6], par[7],  par[8], a3[i], t3[i]);
    ctime4 = calcTime(par[9], par[10], par[11], a4[i], t4[i]);
    tof  = (time1+time2)/2.0 - (time3+time4)/2.0;
    ctof = (ctime1+ctime2)/2.0 - (ctime3+ctime4)/2.0;
    h_tof  -> Fill(tof-2.5);
    h_ctof -> Fill(ctof);
    tree2->Fill();
  }
  tree2->Write();
  
  TObjArray hlist(0);
  hlist.Add(gr);
  hlist.Add(gr_weight);
  hlist.Add(h_tof);
  hlist.Add(h_ctof);
  hlist.Write();
  
  fnew -> Close();

  // =========== Draw histograms for check =============
  //TFile* ff = new TFile(outputfilename);
  new TFile(outputfilename);
  TCanvas* c1 = new TCanvas("c1","c1",850,700);
  c1->Divide(2,1);
  c1->cd(1);
  TPad* pad = new TPad("pad","",0.0,0.0,1.0,1.0);
  pad->Divide(1,2);
  pad->Draw();
  pad->cd(1);gr_weight->Draw("*al"); // weight for chi-square
  pad->cd(2);gr->Draw("*ac");       // chi-square
  gr_weight->SetMarkerStyle(25);
  gr->SetMarkerStyle(27);
  c1->cd(2);
  h_ctof->GetXaxis()->SetRangeUser(-1.2,1.2);
  h_ctof->Draw();      // TOF with correction
  h_tof->Draw("same"); // TOF without correction
  
  // ======= Fitting =======
  double sigma_wc;   // TOF sigma with correction
  double sigma_wcer; // TOF sigma with correction
  TF1* ffunc1 = new TF1("ffunc1","[0]*1./sqrt(2.0*3.14159)/[2]*exp(-0.5*pow((x-[1])/[2],2.0))",-2.0,2.0);
  ffunc1->SetParName(0,"Coeff");
  ffunc1->SetParName(1,"Mean");
  ffunc1->SetParName(2,"Sigma");
  ffunc1->SetParameters(200.0,0.0,0.12);
  h_ctof->Fit("ffunc1","NB","",-0.3,0.3);
  ffunc1->Draw("same");
  ffunc1->SetLineColor(2);
  ffunc1->SetNpx(1000);
  sigma_wc   = ffunc1->GetParameter(2);
  sigma_wcer = ffunc1->GetParError(2);
  
  fnew -> Close();
  
  // ========== Comments ==================
  cout << endl;
  cout << " Number of iteration: " << nite << endl;
  cout << " Input  ROOT file:    " << inputfilename  
       << " (" << ent << ")" << endl;
  cout << " Output ROOT file:    " << outputfilename 
       << " (" << filled_event << ")" << endl;
  cout << " Chi-square file:     " << chi2filename   << endl;
  cout << " Weights:             " 
       << w1 << " " <<  w2 << " " <<  w3 << " " 
       << w4 << " " <<  wtof << endl;
  cout << " TOF sigma after correction = " 
       << sigma_wc*1.0e+3 << " +/- " 
       << sigma_wcer*1.0e+3 << " ps" << endl;
  cout << " --> One segment time resolution (sigma) =  " 
       << sigma_wc*1.0e+3/sqrt(2.0) << " +/- " 
       << sigma_wcer*1.0e+3/sqrt(2.0) << " ps" << endl;

  ofstream* dragon_fire = new ofstream(resultname);
  *dragon_fire << " Number of iteration: " << nite << endl;
  *dragon_fire << " Input  ROOT file:    " << inputfilename  
	       << " (" << ent << ")" << endl;
  *dragon_fire << " Output ROOT file:    " << outputfilename 
	       << " (" << filled_event << ")" << endl;
  *dragon_fire << " Chi-square file:     " << chi2filename   << endl;
  *dragon_fire << " Weights:             " 
	       << w1 << " " <<  w2 << " " <<  w3 << " " 
	       << w4 << " " <<  wtof << endl;
  *dragon_fire << " TOF sigma after correction = " 
	       << sigma_wc*1.0e+3 << " +/- " 
	       << sigma_wcer*1.0e+3 << " ps" << endl;
  *dragon_fire << " --> One segment time resolution (sigma) =  " 
	       << sigma_wc*1.0e+3/sqrt(2.0) << " +/- " 
	       << sigma_wcer*1.0e+3/sqrt(2.0) << " ps" << endl;
  *dragon_fire << endl;
  *dragon_fire << " dragonvaio:~/ehodo/phCorrection/" << endl;
  *dragon_fire << " Toshiyuki Gogami , 19Nov2017" << endl;
  dragon_fire->close();

  // ======= Print =========
  char pname1[600], pname2[600];
  sprintf(pname1,"%s.png",printname);
  sprintf(pname2,"%s.eps",printname);
  if(printflag==true){
    c1->Print(pname1,"png");
    c1->Print(pname2,"eps");
  }

  ofstream* dragon_water = new ofstream(parfilename);
  *dragon_water << par[0] << " " << par[1] << " " << par[2] << endl;
  *dragon_water << par[3] << " " << par[4] << " " << par[5] << endl;
  *dragon_water << par[6] << " " << par[7] << " " << par[8] << endl;
  *dragon_water << par[9] << " " << par[10]<< " " << par[11]<< endl;
  *dragon_water << endl;
  *dragon_water << " Parameter file was generated at: " << endl;
  *dragon_water << " dragonvaio:~/ehodo/phCorrection/" << endl;
  *dragon_water << "    Toshiyuki Gogami , 19Nov2017" << endl;
  dragon_water->close();
  
    
  app.Run();// TApplication for display
  
  return 0;
}


// #############################################################
void fcn(Int_t &nPar, Double_t* /*grad*/, Double_t &fval, Double_t* param, Int_t /*iflag*/)
// #############################################################
{
  double chi2 = 0.0;
  double chi2_tof = 0.0;
  double chi2_1 = 0.0, chi2_2 = 0.0, chi2_3 = 0.0, chi2_4 = 0.0;
  double newtime1, newtime2, newtime3, newtime4;
  double newtof;
  double res_tof; //residual
  double res1, res2, res3, res4; //residual
  const double reftof  = 0.0; // reference [ns]
  const double reftime = 0.0; // reference [ns]
  double n=0;
  const double sigma_tof   = 0.120; // Sigma of TOF [ns]
  const double sigma_time  = 0.070; // Sigma for normal counter [ns]
  const double sigma_time0 = 0.040; // Sigma for timing counter [ns]
  
  for(int i=0 ; i<filled_event ; i++){
    newtime1 = calcTime(param[0], param[1],  param[2], a1[i], t1[i]);
    newtime2 = calcTime(param[3], param[4],  param[5], a2[i], t2[i]);
    newtime3 = calcTime(param[6], param[7],  param[8], a3[i], t3[i]);
    newtime4 = calcTime(param[9], param[10], param[11], a4[i], t4[i]);
    newtof = (newtime1+newtime2)/2.0 - (newtime3+newtime4)/2.0;
    // ~~~ Corrected time ~~~
    res1 = newtime1 - reftime;
    chi2_1 = chi2_1 + res1*res1;
    res2 = newtime2 - reftime;
    chi2_2 = chi2_2 + res2*res2;
    res3 = newtime3 - reftime;
    chi2_3 = chi2_3 + res3*res3;
    res4 = newtime4 - reftime;
    chi2_4 = chi2_4 + res4*res4;
    // ~~~ TOF ~~~
    res_tof = newtof - reftof;
    chi2_tof = chi2_tof + res_tof*res_tof;
    n++;
  }
  
  chi2_1 = chi2_1/(double)n;
  chi2_2 = chi2_2/(double)n;
  chi2_3 = chi2_3/(double)n;
  chi2_4 = chi2_4/(double)n;
  chi2_tof = chi2_tof/(double)n;
  
  /* double w1, w2, w3, w4, wtof; // weight
  // double factor = 1./6.86; //
  w1 = 0.0;
  w2 = 1.0;
  w3 = 1.0;
  w4 = 1.0;
  wtof = 3.0;
  */
  chi2_1 = chi2_1*w1/pow(sigma_time0 ,2.0);
  chi2_2 = chi2_2*w2/pow(sigma_time ,2.0);
  chi2_3 = chi2_3*w3/pow(sigma_time ,2.0);
  chi2_4 = chi2_4*w4/pow(sigma_time ,2.0);
  chi2_tof = chi2_tof*wtof/pow(sigma_tof ,2.0);
  
  chi2 = chi2_1 + chi2_2 + chi2_3 + chi2_4 + chi2_tof;
  chi2 = chi2 / (w1+w2+w3+w4+wtof);

  fval = chi2;
}

// #############################################################
double calcTime(double pa1, double pa2, double pa3,
		int adc, double tdc)
// #############################################################
{
  double corr;
  double newtdc;

  // ------------------------------- //
  // ----Correction function ------- //
  // ------------------------------- //
  //corr = pa1 + pa2*adc + pa3*pow(adc,2.0);
  corr = pa1 + pa2/sqrt(adc) + pa3/(adc);
  
  newtdc = tdc - corr;
  
  return newtdc;
}

// #############################################################
double tune(double* pa, int j)
// #############################################################
{
  double chi2 = 0.0;
  double arglist[10];
  int ierflg = 0;
  
  //cout << allparam << endl;
  
  TMinuit* minuit = new TMinuit(allparam);
  minuit->SetFCN(fcn);
  
  // ~~~ Chi-square ~~~~
  arglist[0] = 1;
  minuit -> mnexcm("SET ERR",arglist,1,ierflg);
  
  minuit -> SetPrintLevel(-1);
  double start[allparam];
  double step[allparam];
  double LLim[allparam];
  double ULim[allparam];
  char pname[500];
  for(int i=0 ; i<allparam ; i++){
    sprintf(pname,"param_%d",i+1);
    start[i] = pa[i];
    step[i] = 1.0e-3;
    //if(i==4 || i==5) {
    //  step[i] = 0.0;
    //  start[i] = 0.0;
    //  pa[i]=0.0;
    //}
    //if(i==0 || i==1 || i==2) {
    //  step[i] = 0.0;
    //  start[i] = 0.0;
    //  pa[i]=0.0;
    //}
    LLim[i] = pa[i] - pa[i]*0.05;
    ULim[i] = pa[i] + pa[i]*0.05;
    minuit -> mnparm(i,pname,start[i],step[i],LLim[i],ULim[i],ierflg);
  }

  // ~~~~ Strategy ~~~~
  arglist[0] = 2.0;
  minuit->mnexcm("SET STR",arglist,1,ierflg);
  
  // ~~~~ Migrad + Simplex  ~~~~
  arglist[0] = 20000;
  arglist[1] = 0.01;
  minuit -> mnexcm("MINImize",arglist,2,ierflg);
  
  double amin,edm,errdef;
  int nvpar,nparx,icstat;
  double e;
  minuit -> mnstat(amin,edm,errdef,nvpar,nparx,icstat);
  minuit -> mnprin(0,amin);
  if(amin>0) chi2=amin;
  
  for(int i=0 ; i<allparam ; i++){
    minuit -> GetParameter(i,par[i],e);
  }
  
  return chi2;
}

