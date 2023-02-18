#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

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

Double_t lumi_mc16a = 36214.96;
Double_t lumi_mc16d = 44307.4;
Double_t lumi_mc16e = 58450.1;


int merge_files(string type, vector<string> &filenames) {
    ConfigReader config = ConfigReader("/home/katet/Programs/ZGamSliceMethod/slice/slice/config.cfg");

    double NBins = config.getInt("NBins");
    double MinValue = config.getDouble("MinValue"), MaxValue = config.getDouble("MaxValue");

    TH1D *hist_SL = new TH1D("hist_SL", "hist_SL", NBins, MinValue, MaxValue);
    TH1D *hist_SL1 = new TH1D("hist_SL1", "hist_SL1", NBins, MinValue, MaxValue);
    TH1D *hist_SL2 = new TH1D("hist_SL2", "hist_SL2", NBins, MinValue, MaxValue);
    TH1D *hist_SL3 = new TH1D("hist_SL3", "hist_SL3", NBins, MinValue, MaxValue);
    TH1D *hist_SL4 = new TH1D("hist_SL4", "hist_SL4", NBins, MinValue, MaxValue);

    double weight;
    double ph_pt, ph_phi, ph_eta;
    double jet_lead_pt, jet_lead_eta, jet_lead_phi, jet_lead_E;
    double metTST_pt, metTST_phi, metTSTsignif;
    double ph_iso_et40, ph_iso_et20, ph_iso_pt, ph_z_point;
    UInt_t n_ph, n_jet, n_mu, n_e_looseBL, ph_isem;

    double sum_of_weights_bk_xAOD, koef, sumw_MC16 = 0;

    TLorentzVector met, ph, jet;

    double SL1 = config.getDouble("SL1");
    double SL2 = config.getDouble("SL2");
    double SL3 = config.getDouble("SL3");
    double SL4 = config.getDouble("SL4");
    double SL5 = config.getDouble("SL5");

    for(int i = 0; i < filenames.size(); i++) {
        char *ftempname = filenames[i].data();
        TString new_ftempname = TString(ftempname);

        cout << "[" << type << "] " << filenames[i] << endl;

        TFile *file = new TFile(ftempname, "READ");
        TTree *tree = (TTree*)file->Get((type == "etogam") ? "etogam" : "output_tree");

        if (type == "etogam") tree->SetBranchAddress("weight",&weight);

        else if (type != "data") {
            tree->SetBranchAddress("weight",&weight);
            TTree *tree_MC_sw = (TTree*)file->Get("output_tree_sw");
            TTree *tree_norm = (TTree*)file->Get("norm_tree");

            tree_norm->SetBranchAddress("koef",&koef);
            tree_MC_sw->SetBranchAddress("sum_of_weights_bk_xAOD",&sum_of_weights_bk_xAOD);

            sumw_MC16 = 0;
            for (long i = 0; i < tree_MC_sw->GetEntries(); i++) {
                tree_MC_sw->GetEntry(i);
                sumw_MC16 += sum_of_weights_bk_xAOD;
            }

            tree_norm->GetEntry(0);
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

        if (type != "etogam") {
            tree->SetBranchAddress("ph_iso_et40", &ph_iso_et40);
            tree->SetBranchAddress("ph_iso_et20", &ph_iso_et20);
            tree->SetBranchAddress("ph_iso_pt", &ph_iso_pt);
            tree->SetBranchAddress("ph_isem", &ph_isem);
            tree->SetBranchAddress("ph_z_point", &ph_z_point);
        }

        tree->SetBranchAddress("n_ph", &n_ph);
        tree->SetBranchAddress("n_jet", &n_jet);
        tree->SetBranchAddress("n_mu", &n_mu);
        tree->SetBranchAddress("n_e_looseBL", &n_e_looseBL);

        for(long i = 0; i < tree->GetEntries(); i++){
            tree->GetEntry(i);
            
            double TrackIsoVar, IsoVar;

            if (type != "etogam") {
                IsoVar = ph_iso_et20/ph_pt;
                TrackIsoVar = ph_iso_pt/ph_pt;
            }
            
            met.SetPtEtaPhiM(metTST_pt,0,metTST_phi,0);
            ph.SetPtEtaPhiE(ph_pt,ph_eta,ph_phi,ph_iso_et40);
            jet.SetPtEtaPhiE(jet_lead_pt,jet_lead_eta,jet_lead_phi,jet_lead_E);

            if(fabs(weight)>=100) continue;
            if(ph_pt <= 150) continue;
            if(n_ph !=1 || n_mu !=0 || n_e_looseBL != 0) continue;

            if (type != "etogam") {
                if(fabs(ph_z_point)>=250) continue;
                if(ph_isem != 0) continue;
            }

            if (config.getString("Variable") == "DeltaPhiMetJet") {
                if(n_jet < 1) continue;
            }

            if (config.getString("Region") == "CR_iso" || config.getString("Region") == "CR_noniso") {
                if(
                    metTST_pt >= config.getDouble("MetTSTPt") && 
                    metTSTsignif >= config.getDouble("MetTSTSignif") && 
                    !(n_jet >= 1 && fabs(met.DeltaPhi(jet)) <= config.getDouble("DeltaPhiMetJet")) && 
                    fabs(met.DeltaPhi(ph)) >= config.getDouble("DeltaPhiMetPh")
                ) continue;
            }
            else if (config.getString("Region") == "SR_noniso") {
                if(fabs(met.DeltaPhi(ph)) <= config.getDouble("DeltaPhiMetPh")) continue;
                if(metTST_pt <= config.getDouble("MetTSTPt")) continue;
                if(metTSTsignif <= config.getDouble("MetTSTSignif")) continue;
                if(n_jet >= 1 && fabs(met.DeltaPhi(jet)) <= config.getDouble("DeltaPhiMetJet")) continue;
            }

            double var;
            if(config.getString("Variable") == "DeltaPhiMetJet") var = fabs(met.DeltaPhi(jet));
            if(config.getString("Variable") == "DeltaPhiMetPh") var = fabs(met.DeltaPhi(ph));
            if(config.getString("Variable") == "MetTSTPt") var = metTST_pt;
            if(config.getString("Variable") == "MetTSTSignif") var = metTSTsignif;


            double event_weight = 1.0;
            if (type == "etogam") event_weight = weight;
            else if (type != "data") {
                if(new_ftempname.Contains("MC16a")) event_weight = lumi_mc16a*koef*weight/(sumw_MC16);
                if(new_ftempname.Contains("MC16d")) event_weight = lumi_mc16d*koef*weight/(sumw_MC16);
                if(new_ftempname.Contains("MC16e")) event_weight = lumi_mc16e*koef*weight/(sumw_MC16);
            }

            if (type == "etogam") hist_SL->Fill(var, event_weight);

            else {
                if (config.getString("Region") == "CR_iso") {
                    if(IsoVar < 0.065 && TrackIsoVar < 0.05) {
                        hist_SL->Fill(var, event_weight);
                    }
                }
                else if (config.getString("Region") == "CR_noniso" || config.getString("Region") == "SR_noniso") {
                    if (TrackIsoVar <= 0.05) continue;

                    if (IsoVar > SL1 && IsoVar < SL2) hist_SL1->Fill(var, event_weight);
                    else if (IsoVar > SL2 && IsoVar < SL3) hist_SL2->Fill(var, event_weight);
                    else if (IsoVar > SL3 && IsoVar < SL4) hist_SL3->Fill(var, event_weight);
                    else if (IsoVar > SL4 && IsoVar < SL5) hist_SL4->Fill(var, event_weight);
                }
            }        
        }
        file->Close();
    }

    Double_t err_SL, err_SL1, err_SL2, err_SL3, err_SL4;

    if (config.getString("Region") == "CR_iso") {
        if (type == "etogam") hist_SL -> Scale(config.getDouble("EtogamSL")/hist_SL->Integral());
        double N_SL = hist_SL->IntegralAndError(1, NBins, err_SL, "");
        cout<<"N_SL = "<<N_SL<<" +- "<<err_SL<<endl;
    }
    else if (config.getString("Region") == "CR_noniso" || config.getString("Region") == "SR_noniso") {
        if (type == "etogam") {
            hist_SL1 -> Scale(config.getDouble("EtogamSL1")/hist_SL1->Integral());
            hist_SL2 -> Scale(config.getDouble("EtogamSL2")/hist_SL2->Integral());
            hist_SL3 -> Scale(config.getDouble("EtogamSL3")/hist_SL3->Integral());
            hist_SL4 -> Scale(config.getDouble("EtogamSL4")/hist_SL4->Integral());
        }

        double N_SL1 = hist_SL1->IntegralAndError(1, NBins, err_SL1, "");
        double N_SL2 = hist_SL2->IntegralAndError(1, NBins, err_SL2, "");
        double N_SL3 = hist_SL3->IntegralAndError(1, NBins, err_SL3, "");
        double N_SL4 = hist_SL4->IntegralAndError(1, NBins, err_SL4, "");

        cout<<"N_SL1 = "<<N_SL1<<" +- "<<err_SL1<<endl;
        cout<<"N_SL2 = "<<N_SL2<<" +- "<<err_SL2<<endl;
        cout<<"N_SL3 = "<<N_SL3<<" +- "<<err_SL3<<endl;
        cout<<"N_SL4 = "<<N_SL4<<" +- "<<err_SL4<<endl;
    }

    TH1D histos[5] = {hist_SL, hist_SL1, hist_SL2, hist_SL3, hist_SL4};

    for (int i = 0; i < 5; i++) {
        double lastBin = histos[i]->GetBinContent(NBins) + histos[i]->GetBinContent(NBins+1);
        double lastBinErr = sqrt(pow(histos[i]->GetBinError(NBins),2) + pow(histos[i]->GetBinError(NBins+1),2));
        histos[i]->SetBinContent(NBins, lastBin);
        histos[i]->SetBinError(NBins, lastBinErr);

        histos[i]->SetBinContent(NBins+1, 0);
        histos[i]->SetBinError(NBins+1, 0);
    }

    stringstream fOutName;
    fOutName << "/home/katet/Programs/ZGamSliceMethod/" << type << ".root";
    TFile *fOut = new TFile(fOutName.str().data(), "RECREATE");

    for (int i = 1; i < 5; i++) {
        stringstream histname;
        histname << "hist_SL" << i;
        hist_SL->SetName(histname.str().data());
        hist_SL->Write();
    }
    fOut->Close();

    return 0;
}
