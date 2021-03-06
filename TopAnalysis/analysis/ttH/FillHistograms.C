#include "TreeClass.C"
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TString.h> 
#include <TCollection.h>
#include <TKey.h>
void FillHistograms(TString SAMPLE)
{
  cout<<"Processing sample "<<SAMPLE<<endl;
  //TString PATH("root://eoscms//eos/cms/store/cmst3/user/kkousour/ttH/flat/");
  TString PATH("/afs/cern.ch/work/k/kkousour/private/CMSSW_7_4_12/src/KKousour/TopAnalysis/prod/ttH/");
  TFile *inf  = TFile::Open(PATH+"flatTree_"+SAMPLE+".root");
  TFile *outf = TFile::Open(TString::Format("Histo_%s.root",SAMPLE.Data()),"RECREATE");

  TIter nextKey(inf->GetListOfKeys());
  TKey *key;
  while ((key = (TKey*)nextKey())) {
    TString dirName(key->GetName());
    cout<<"Found directory "<<dirName<<endl;
    
    TH1F *hPass = (TH1F*)inf->Get(dirName+"/TriggerPass");
    outf->mkdir(dirName);  
    TDirectory *dir = (TDirectory*)outf->Get(dirName); 
    TTree *tr   = (TTree*)inf->Get(dirName+"/events");
    
    TreeClass myTree(tr);
    dir->cd();
    hPass->Write("TriggerPass");
    myTree.Loop(dir);
    cout<<"Loop finished"<<endl;
    dir->Close();
    delete tr;
  }
  outf->Close();
  inf->Close();
}
