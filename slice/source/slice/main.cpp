#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <TLorentzVector.h>

#include "ConfigReader.h"
#include "SourceFiles.h"


using namespace std;


int main() {
    merge_files("data", data_filenames);
    merge_files("signal", signal_filenames);
    merge_files("bkg", background_filenames);
}


int merge_files(string type, vector<string> &filenames) {
    ConfigReader config = ConfigReader("/workenv/slice/slice/config.cfg");

    double Nbins = 150, MinValue = 0, MaxValue = 3.15;

    TH1D *hist_SL1 = new TH1D("hist_SL1", "hist_SL1", Nbins, MinValue, MaxValue);
    TH1D *hist_SL2 = new TH1D("hist_SL2", "hist_SL2", Nbins, MinValue, MaxValue);
    TH1D *hist_SL3 = new TH1D("hist_SL3", "hist_SL3", Nbins, MinValue, MaxValue);
    TH1D *hist_SL4 = new TH1D("hist_SL4", "hist_SL4", Nbins, MinValue, MaxValue);
    TH1D *hist_SL5 = new TH1D("hist_SL5", "hist_SL5", Nbins, MinValue, MaxValue);


    double weight;
    double ph_pt, ph_phi, ph_eta;
    double jet_lead_pt, jet_lead_eta, jet_lead_phi, jet_lead_E;
    double metTST_pt, metTST_phi, metTSTsignif;
    double ph_iso_et40, ph_iso_et20, ph_iso_pt, ph_isem, ph_z_point;
    UInt_t n_ph, n_jet, n_mu, n_e_looseBL;

    TLorentzVector met, ph, jet;

    for(int i = 0; i < filenames.length(); i++) {
        TFile *file = new TFile(filenames[i], "READ");

        cout << "[" << type << "] " << filenames[i] << endl;

        TTree *tree = (TTree*)file->Get("output_tree");

        if (type != "data")
            tree->SetBranchAddress("weight",&weight);

        tree->SetBranchAddress("ph_pt",&ph_pt);
        tree->SetBranchAddress("ph_phi",&ph_phi);
        tree->SetBranchAddress("ph_eta",&ph_eta);

        tree->SetBranchAddress("jet_lead_pt", &jet_lead_pt);
        tree->SetBranchAddress("jet_lead_eta", &jet_lead_eta);
        tree->SetBranchAddress("jet_lead_phi", &jet_lead_phi);
        tree->SetBranchAddress("jet_lead_E", &jet_lead_E);

        tree->SetBranchAddress("metTST_pt", &metTST_pt);
        tree->SetBranchAddress("metTST_phi", &metTST_phi);
        tree->SetBranchAddress("metTSTsignif", &metTSTsignif);

        tree->SetBranchAddress("ph_iso_et40", &ph_iso_et40);
        tree->SetBranchAddress("ph_iso_et20", &ph_iso_et20);
        tree->SetBranchAddress("ph_iso_pt", &ph_iso_pt);
        tree->SetBranchAddress("ph_isem", &ph_isem);
        tree->SetBranchAddress("ph_z_point", &ph_z_point);

        tree->SetBranchAddress("n_ph", &n_ph);
        tree->SetBranchAddress("n_jet", &n_jet);
        tree->SetBranchAddress("n_mu", &n_mu);
        tree->SetBranchAddress("n_e_looseBL", &n_e_looseBL);

        for(long i = 0; i < tree->GetEntries(); i++){
            tree->GetEntry(i);

            double IsoVar = ph_iso_et20/ph_pt;

            met.SetPtEtaPhiM(metTST_pt,0,metTST_phi,0);
            ph.SetPtEtaPhiE(ph_pt,ph_eta,ph_phi,ph_iso_et40);
            jet.SetPtEtaPhiE(jet_lead_pt,jet_lead_eta,jet_lead_phi,jet_lead_E);

        }
    }
}