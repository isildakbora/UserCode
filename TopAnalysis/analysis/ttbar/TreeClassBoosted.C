#define TreeClassBoosted_cxx
#include "TreeClassBoosted.h"
#include <iostream>
#include <TString.h>
#include <TH1F.h>
#include <TDirectory.h>
using std::cin;
using std::cout;
using std::endl;

void TreeClassBoosted::Loop(TDirectory *DIR)
{
//---- define histograms ------------------
  char name[1000];
  const int NVAR = 10;
  TString var[NVAR] = {"ht","nvtx","nJets","nBJets","met","mJJ","ptJJ","yJJ","dPhiJJ","mva"}; 
  double dX[NVAR]   = {10,1,1,1,1,1,1,0.01,0.01,0.01};
  double XMIN[NVAR] = {0,0,0,0,0,0,0,-3,0,-1};
  double XMAX[NVAR] = {3000,50,15,10,150,13000,4500,3,3.142,1};
  
  TH1F *hVar[NVAR][2],*hVarCut[NVAR][2];
  cout<<"Booking histograms.........."<<endl;
  for(int ivar=0;ivar<NVAR;ivar++) {
    int NBINS = (XMAX[ivar]-XMIN[ivar])/dX[ivar];
    sprintf(name,"h_%s_sig",var[ivar].Data()); 
    hVar[ivar][0] = new TH1F(name,name,NBINS,XMIN[ivar],XMAX[ivar]);
    sprintf(name,"h_%s_Cut_sig",var[ivar].Data()); 
    hVarCut[ivar][0] = new TH1F(name,name,NBINS,XMIN[ivar],XMAX[ivar]); 
    sprintf(name,"h_%s_ctl",var[ivar].Data()); 
    hVar[ivar][1] = new TH1F(name,name,NBINS,XMIN[ivar],XMAX[ivar]);
    sprintf(name,"h_%s_Cut_ctl",var[ivar].Data()); 
    hVarCut[ivar][1] = new TH1F(name,name,NBINS,XMIN[ivar],XMAX[ivar]);
  }
  cout<<"Booking jet histograms.........."<<endl;
  const int NJETVAR = 8;
  TString varJet[NJETVAR] = {"jetPt","jetEta","jetBtag","jetMassSoftDrop","jetTau32","jetTau31","jetMassSub0","jetMassSub1"}; 
  double dXJET[NJETVAR]   = {1,0.1,0.01,0.5,0.01,0.01,1,1};
  double XMINJET[NJETVAR] = {0,-3,-10,0,0,0,0,0};
  double XMAXJET[NJETVAR] = {2000,3,1,1000,1,1,200,200};
  TH1F *hJetVar[NJETVAR][2][2],*hJetVarCut[NJETVAR][2][2];
  
  for(int ivar=0;ivar<NJETVAR;ivar++) {
    int NBINS = (XMAXJET[ivar]-XMINJET[ivar])/dXJET[ivar];
    for(int j=0;j<2;j++) {
      sprintf(name,"h_%s%d_sig",varJet[ivar].Data(),j);
      hJetVar[ivar][j][0] = new TH1F(name,name,NBINS,XMINJET[ivar],XMAXJET[ivar]);
      sprintf(name,"h_%s%d_Cut_sig",varJet[ivar].Data(),j);
      hJetVarCut[ivar][j][0] = new TH1F(name,name,NBINS,XMINJET[ivar],XMAXJET[ivar]);
      sprintf(name,"h_%s%d_ctl",varJet[ivar].Data(),j);
      hJetVar[ivar][j][1] = new TH1F(name,name,NBINS,XMINJET[ivar],XMAXJET[ivar]);
      sprintf(name,"h_%s%d_Cut_ctl",varJet[ivar].Data(),j);
      hJetVarCut[ivar][j][1] = new TH1F(name,name,NBINS,XMINJET[ivar],XMAXJET[ivar]);
    }
  }
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  cout<<"Reading "<<nentries<<" events"<<endl;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    if (jentry % (nentries/10) == 0) cout<<100*jentry/nentries<<"%"<<endl;
    fChain->GetEntry(jentry);
    bool cut_trigger = ((*triggerBit)[0]);
    bool cut_nJets   = (nJets > 1);
    bool cut_nBJets  = (nBJets > 0);
    bool cut_leptons = (nLeptons == 0); 
    bool cut_jetPt   = ((*jetPt)[0] > 550);
    bool cut_mva     = (mva > 0.77);
    bool cut_nBSub   = ((*jetNBSub)[0] > 0);
    bool cut_tau32   = ((*jetTau3)[0]/(*jetTau2)[0]<0.5);
    bool cut_tau31   = ((*jetTau3)[0]/(*jetTau1)[0]<1.0);

    if (!cut_trigger)  continue;
    if (!cut_nJets)    continue;
    //if (!cut_nBJets)   continue;
    if (!cut_leptons)  continue;
    if (!cut_jetPt)    continue;
    //if (!cut_mva)      continue;
    //if (!cut_nBSub)    continue; 

    float x[NVAR] = {ht,float(nvtx),float(nJets),float(nBJets),met,mJJ,ptJJ,yJJ,dPhiJJ,mva};
     
    for(int ivar=0;ivar<NVAR;ivar++) {
      if (cut_nBSub && cut_nBJets) { 
        hVar[ivar][0]->Fill(x[ivar]);
      }
      else {
        hVar[ivar][1]->Fill(x[ivar]);
      } 
      if (cut_tau32 && cut_tau31) {
      //if (cut_mva) {
        if (cut_nBSub && cut_nBJets) { 
          hVarCut[ivar][0]->Fill(x[ivar]);
        }
        else {
          hVarCut[ivar][1]->Fill(x[ivar]); 
        }
      }
    }
    for(int j=0;j<2;j++) {
      float xJet[NJETVAR] = {(*jetPt)[j],(*jetEta)[j],(*jetBtag)[j],(*jetMassSoftDrop)[j],(*jetTau3)[j]/(*jetTau2)[j],(*jetTau3)[j]/(*jetTau1)[j],(*jetMassSub0)[j],(*jetMassSub1)[j]}; 
      for(int ivar=0;ivar<NJETVAR;ivar++) {
        if (cut_nBSub && cut_nBJets) {
          hJetVar[ivar][j][0]->Fill(xJet[ivar]);
        }
        else {
          hJetVar[ivar][j][1]->Fill(xJet[ivar]);
        } 
        if (cut_tau32 && cut_tau31) {
        //if (cut_mva) { 
          if (cut_nBSub && cut_nBJets) {
            hJetVarCut[ivar][j][0]->Fill(xJet[ivar]);
          }
          else {
            hJetVarCut[ivar][j][1]->Fill(xJet[ivar]);
          } 
        }
      }
    }    
  }
  DIR->Write();
}
