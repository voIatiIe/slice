#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "MergeFiles.h"
#include "SourceFiles.h"
#include "ConfigReader.h"

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "TGraph.h"

#include "AtlasUtils.C"
#include "AtlasLabels.C"
#include "AtlasStyle.C"

using namespace std;

void scale(double *values, double *errors);
void fit(double *values, double *errors);

int main() {
    ConfigReader config = ConfigReader("/home/katet/Programs/ZGamSliceMethod/slice/slice/config.cfg");

    TH1D data_hists[5];
    TH1D signal_hists[5];
    TH1D bkg_hists[5];
    TH1D etogam_hists[5];

    merge_files("data", data_filenames, data_hists);
    merge_files("signal", signal_filenames, signal_hists);
    merge_files("bkg", background_filenames, bkg_hists);
    merge_files("etogam", etogam_filenames, etogam_hists);

    for (int i = 0; i < 4; i++) {
        bkg_hists[i].Add(&etogam_hists[i]);
    }

    if (config.getString("Region") == "CR_iso") {
        TFile *fOut = new TFile("/home/katet/Programs/ZGamSliceMethod/background.root", "RECREATE");

        for (int i = 1; i < 5; i++) {
            stringstream histname;
            histname << "hist_SL" << i;
            bkg_hists[0].SetName(histname.str().data());
            bkg_hists[0].Write();
        }
        fOut->Close();
    }

    else {
        double values[4], errors[4];
        TFile *fOut = new TFile("/home/katet/Programs/ZGamSliceMethod/zj.root", "RECREATE");
        for (int i = 1; i < 5; i++) {
            stringstream histname;
            histname << "hist_SL" << i;
            data_hists[i].Add(&signal_hists[i], -1);
            data_hists[i].Add(&bkg_hists[i], -1);
            data_hists[i].SetName(histname.str().data());
            data_hists[i].Write();

            values[i-1] = data_hists[i].IntegralAndError(1, 150, errors[i-1], "");
        }
        fOut->Close();

        scale(values, errors);
        fit((*int)values, (*int)errors);

    }
}


void scale(double *values, double *errors) {
    ConfigReader config = ConfigReader("/home/katet/Programs/ZGamSliceMethod/slice/slice/config.cfg");

    double T[4] = {
        config.getDouble("T1"),
        config.getDouble("T2"),
        config.getDouble("T3"),
        config.getDouble("T4"),
    };
    double Terr[4] = {
        config.getDouble("T1err"),
        config.getDouble("T2err"),
        config.getDouble("T3err"),
        config.getDouble("T4err"),
    };

    for (int i = 0; i < 4; i++) {
        errors[i] = sqrt(pow(errors[i]*T[i],2) + pow(Terr[i]*values[i],2));
        values[i] *= T[i];
    }
}


void fit(int *values, int *errors) {
    ConfigReader config = ConfigReader("/home/katet/Programs/ZGamSliceMethod/slice/slice/config.cfg");

    SetAtlasStyle();
    TCanvas *c = new TCanvas("", "", 1000, 700);

    double ext = config.getDouble("Extrapolation");
    double border = config.getDouble("SL5")+0.015;
    double X[4] = {
        (config.getDouble("SL1")+config.getDouble("SL2"))/2,
        (config.getDouble("SL2")+config.getDouble("SL3"))/2,
        (config.getDouble("SL3")+config.getDouble("SL4"))/2,
        (config.getDouble("SL4")+config.getDouble("SL5"))/2,
    };

    TGraphErrors* graph = new TGraphErrors(4, X, values, {0,0,0,0}, errors);
    graph -> GetXaxis()->SetLimits(0, border);
    graph -> GetXaxis() -> SetRangeUser(0, border);

    TF1 *func = new TF1("func", "[0]*x+[1]", 0, border);
    graph->Fit(func, "", "", 0, border);

    double k = func->GetParameter(0);
    double kerr = func->GetParError(0);
    double b = func->GetParameter(1);
    double berr = func->GetParError(1);

    double ext_val = ext*k + b;
    double ext_val_err = sqrt(pow(ext*kerr,2)+pow(berr,2));

    cout << "Values: {"
    for (int i = 0; i < 4; i++) {
        cout << values[i] << ", "
    }
    cout << "}" << endl;

    cout << "Value errors: {"
    for (int i = 0; i < 4; i++) {
        cout << errors[i] << ", "
    }
    cout << "}" << endl;

    cout << "Extimate: " << ext_val << " +- " << ext_val_err << endl;
}