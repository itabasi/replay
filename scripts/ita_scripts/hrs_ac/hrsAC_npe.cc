void hrsAC_npe(){

  gStyle->SetOptLogy(1);
  TFile* f1= new TFile("/adaqfs/home/a-onl/tritium_work/itabashi/ita_macro/rootfiles/tritium_new93403.root");
  TTree* t1 =(TTree*)f1->Get("tree");

  TH1F *hac1a[100]; //AC1 Ch24 Right &Left ADC
  TH1F *hac2a[100]; // AC2 ch26 Right & Left ADC
  TF1 *fac1a[100][2]; //AC1 Fitting func.
  TF1 *fac2a[100][2]; //AC2 Fitting func.

  double amin,amax;
  amin=-200.0;
  amax=10000.;
  int abin;
  abin=10000;

 //-------- AC1 ---------------//
  for(int i=0;i<24;i++){

    hac1a[i]=new TH1F(Form("hac1a[%d]",i),Form("hac1a[%d]",i),abin,amin,amax);
    t1->Project(Form("hac1a[%d]",i),Form("Ra1a_c[%d]",i)); //AC1 Right PMT

  //------Fit func. ----------//
    // fac1a[i][0]=new TF1(Form("fac1a[%d]",i),gaus,Form("fac1min[&d][0]",i),Form("fac1max[%d][0]",i)); //pedestal
    // fac1a[i][1]=new TF1(Form("fac1a[%d][1]",i),gaus,Form("fac1min[&d][1]",i),Form("fac1max[%d][1]",i));//1PE
}
  //------ AC2 ----------------//
  for(int i=0;i<26;i++){
    hac2a[i]=new TH1F(Form("hac2a[%d]",i),Form("hac2a[%d]",i),abin,amin,amax);
    t1->Project(Form("hac2a[%d]",i),Form("Ra2a_c[%d]",i)); //AC1
   
  //------ Fit func. ----------//    
    // fac2a[i][0]=new TF1(Form("fac2a[%d][0]",i),gaus,Form("fac2min[%d][0]",i),Form("fmax[%d][0]",i)); //pedestal
    //  fac2a[i][1]=new TF1(Form("fac2a[%d][1]",i),gaus,Form("fac2min[%d][1]",i),Form("fac2max[%d][1]",i)); //1PE
 

}


//====== Pedestal and 1PE position ===============//
double ped1[24],gain1[24],ped2[26],gain2[26];

gain1[0]=379.;
gain1[1]=345.;
gain1[2]=341.;
gain1[3]=341.;
gain1[4]=364.;
gain1[5]=459.;
gain1[6]=375.;
gain1[7]=365.;
gain1[8]=370.;
gain1[9]=370.;
gain1[10]=362.;
gain1[11]=357.;
gain1[12]=320.;
gain1[13]=455.;
gain1[14]=341.;
gain1[15]=312.;
gain1[16]=340.;
gain1[17]=0.0;
gain1[18]=356.;
 gain1[19]=330.;
 gain1[20]=347.;
 gain1[21]=400.;
 gain1[22]=372.;
 gain1[23]=300.;

gain2[0]=397.;
gain2[1]=413.;
gain2[2]=378.;
gain2[3]=400.;
gain2[4]=389.;
gain2[5]=410.;
gain2[6]=380.;
gain2[7]=378.;
gain2[8]=410.;
gain2[9]=437.;
gain2[10]=433.;
gain2[11]=480.;
gain2[12]=402;
gain2[13]=390.;
gain2[14]=378.;
gain2[15]=376.;
gain2[16]=375.;
gain2[17]=400.;
gain2[18]=409.;
 gain2[19]=405.;
 gain2[20]=388.;
 gain2[21]=404.;
 gain2[22]=391.;
 gain2[23]=437.;
 gain2[24]=412.;
gain2[25]=368.;


  TCanvas* c0 =new TCanvas("c0","c0");
  c0->Divide(2,2);
  c0->cd(1);
  hac1a[0]->Draw();
  c0->cd(2); 
  hac1a[1]->Draw();
  c0->cd(3);
  hac2a[0]->Draw();
  c0->cd(4); 
  hac2a[1]->Draw();


  //======== Converce NPE ==========//



  TH1F* hac1npe[24];
  TH1F* hac2npe[24];
  for(int i=0;i<24;i++){

    hac1npe[i]=new TH1F(Form("hac1npe[%d]",i),Form("hac1npe[%d]",i),300,-10.,50.);
    t1->Project(Form("hac1npe[%d]",i),
		Form("Ra1a_c[%d]/%lf",i,gain1[i])); //AC1 Right PMT
		//Form("Ra1a_c[%d]/%lf",i,400.)); //AC1 Right PMT
}
  //------ AC2 ----------------//
  for(int i=0;i<26;i++){
    hac2npe[i]=new TH1F(Form("hac2npe[%d]",i),Form("hac2npe[%d]",i),300,-10.,50.);
    t1->Project(Form("hac2npe[%d]",i),Form("Ra2a_c[%d]/%lf",i,gain2[i])); //AC1
}

  TCanvas* c1 =new TCanvas("c1","c1");
  c1->Divide(2,2);
  c1->cd(1);
  hac1npe[0]->Draw();
  c1->cd(2); 
  hac1npe[1]->Draw();
  c1->cd(3);
  hac2npe[0]->Draw();
  c1->cd(4); 
  hac2npe[1]->Draw();


}

