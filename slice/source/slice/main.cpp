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

using namespace std;


int main() {
    ConfigReader config = ConfigReader("/home/katet/Programs/ZGamSliceMethod/slice/slice/config.cfg");

    TH1D *data_hists[5];
    TH1D *signal_hists[5];
    TH1D *bkg_hists[5];
    TH1D *etogam_hists[5];
    
    merge_files("data", data_filenames, data_hists);
    merge_files("signal", signal_filenames, signal_hists);
    merge_files("bkg", background_filenames, bkg_hists);
    merge_files("etogam", etogam_filenames, etogam_hists);

    for (int i = 0; i < 4; i++) {
        bkg_hists[i]->Add(etogam_hists[i]);
    }

    if (config.getString("Region") == "CR_iso") {
        TFile *fOut = new TFile("/home/katet/Programs/ZGamSliceMethod/background.root", "RECREATE");

        for (int i = 1; i < 5; i++) {
            stringstream histname;
            histname << "hist_SL" << i;
            bkg_hists[0]->SetName(histname.str().data());
            bkg_hists[0]->Write();
        }
        fOut->Close();
    }

    else {
        TFile *fOut = new TFile("/home/katet/Programs/ZGamSliceMethod/zj.root", "RECREATE");
        for (int i = 1; i < 5; i++) {
            stringstream histname;
            histname << "hist_SL" << i;

            data_hists[i] -> Add(signal_hists[i], -1);
            data_hists[i] -> Add(bkg_hists[i], -1);
            data_hists[i] -> Add(etogam_hists[i], -1);
            data_hists[i] -> SetName(histname.str().data());
            data_hists[i] -> Write();
        }
        fOut->Close();
    }
}
