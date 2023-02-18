#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TError.h"
#include <TLorentzVector.h>

#include "ConfigReader.h"

using namespace std;


int merge_files(string type, vector<string> &filenames) {
    ConfigReader config = ConfigReader("/workenv/slice/slice/config.cfg");

    double Nbins = 150, MinValue = 0, MaxValue = 3.15;

    TH1D *hist_SL = new TH1D("hist_SL1", "hist_SL1", Nbins, MinValue, MaxValue);

    double weight;
    double ph_pt, ph_phi, ph_eta;
    double jet_lead_pt, jet_lead_eta, jet_lead_phi, jet_lead_E;
    double metTST_pt, metTST_phi, metTSTsignif;
    double ph_iso_et40, ph_iso_et20, ph_iso_pt, ph_z_point;
    UInt_t n_ph, n_jet, n_mu, n_e_looseBL, ph_isem;

    double sum_of_weights_bk_xAOD, koef, sumw_MC16 = 0;

    TLorentzVector met, ph, jet;

    for(int i = 0; i < filenames.size(); i++) {
        char *ftempname = filenames[i].data();
        TString new_ftempname = TString(ftempname);

        cout << "[" << type << "] " << filenames[i] << endl;

        TFile *file = new TFile(ftempname, "READ");
        TTree *tree = (TTree*)file->Get("output_tree");

        if (type != "data") {
            tree->SetBranchAddress("weight",&weight);
            TTree *tree_MC_sw = (TTree*)file->Get("output_tree_sw");
            TTree *tree_norm = (TTree*)file->Get("norm_tree");

            tree_norm->SetBranchAddress("koef",&koef);
            tree_MC_sw->SetBranchAddress("sum_of_weights_bk_xAOD",&sum_of_weights_bk_xAOD);

            for (long i = 0; i < tree_MC_sw->GetEntries(); i++) {
                tree_MC_sw->GetEntry(i);
                sumw_MC16 += sum_of_weights_bk_xAOD;
            }

            tree_norm->GetEntry(i);
        }

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
            double TrackIsoVar = ph_iso_pt/ph_pt;

            met.SetPtEtaPhiM(metTST_pt,0,metTST_phi,0);
            ph.SetPtEtaPhiE(ph_pt,ph_eta,ph_phi,ph_iso_et40);
            jet.SetPtEtaPhiE(jet_lead_pt,jet_lead_eta,jet_lead_phi,jet_lead_E);

            if(fabs(weight)>=100) continue;
            if(fabs(ph_z_point)>=250) continue;
            if(ph_pt <= 150) continue;
            if(n_ph !=1 || n_mu !=0 || n_e_medium != 0) continue;
            if(ph_isem != 0) continue;


            if (config.getString("Variable") == "DeltaPhiMetJet") {
                if(n_jet < 1) continue;
            }
            
            if (config.getString("Region") == "CR") {
                if(
                    metTST_pt >= 130 && 
                    metTSTsignif >= 8 && 
                    !(n_jet >= 1 && fabs(met.DeltaPhi(jet)) <= 0.4) && 
                    fabs(met.DeltaPhi(ph)) >= 0.7
                ) continue;
            }
            else if (config.getString("Region") == "SR") {
                if(fabs(met.DeltaPhi(ph)) <= 0.7) continue;
                if(metTST_pt <= 130) continue;
                if(metTSTsignif <= 11) continue;
                if(n_jet >= 1 && fabs(met.DeltaPhi(jet)) <= 0.4 ) continue;
            }

            map<string, int> variables{
                {"DeltaPhiMetJet", fabs(met.DeltaPhi(jet))}, 
                {"DeltaPhiMetPh", fabs(met.DeltaPhi(ph))}, 
                {"MetTSTPt", metTST_pt},
                {"MetTSTSignif", metTSTsignif},
            };

            double var = variables[config.getString("Variable")];
            if(IsoVar < 0.065 && TrackIsoVar < 0.05) {
                hist_SL->Fill(var, 1.0);
            }
        }

        Double_t err_SL;

        double N_SL = hist_SL->IntegralAndError(1, Nbins, err_SL, "");
        cout<<"N_SL = "<<N_SL<<" +- "<<err_SL<<endl;
    }

    return 0;
}