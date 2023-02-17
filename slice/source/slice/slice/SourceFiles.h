#include <vector>
#include <string>

#include "TFile.h"
#include "TH1.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TError.h"
#include <TLorentzVector.h>

using namespace std;


Double_t lumi_mc16a = 36214.96;
Double_t lumi_mc16d = 44307.4;
Double_t lumi_mc16e = 58450.1;


static vector<string> data_filenames = {
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/data15-18.reproc-30-09-21_full_checked.root"
};


static vector<string> signal_filenames = {
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Znunugamma2jEWK_MC16a_361290.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Znunugamma2jEWK_MC16d_361290.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Znunugamma2jEWK_MC16e_361290.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Znunugamma2jQCD_MC16a_364519.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Znunugamma2jQCD_MC16d_364519.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Znunugamma2jQCD_MC16e_364519.root",
    // "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Znunugamma2jQCD_MC16a_345784.root",
    // "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Znunugamma2jQCD_MC16d_345784.root",
    // "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Znunugamma2jQCD_MC16e_345784.root",
};


static vector<string> background_filenames = {
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361045.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361046.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361047.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361048.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361049.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361050.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361051.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361052.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361053.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361054.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361055.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16a_361056.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361045.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361046.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361047.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361048.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361049.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361050.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361051.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361052.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361053.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361054.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361055.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16d_361056.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361045.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361046.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361047.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361048.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361049.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361050.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361051.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361052.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361053.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361054.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361055.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Gammajet_MC16e_361056.root",

    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_ttgamma_MC16a_410389.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_ttgamma_MC16d_410389.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_ttgamma_MC16e_410389.root",

    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaEWK_MC16a_361273.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaEWK_MC16a_361274.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaEWK_MC16a_361275.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaEWK_MC16d_361273.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaEWK_MC16d_361274.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaEWK_MC16d_361275.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaEWK_MC16e_361273.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaEWK_MC16e_361274.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaEWK_MC16e_361275.root",

    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaQCD_MC16a_364525.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaQCD_MC16a_364530.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaQCD_MC16a_364535.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaQCD_MC16d_364525.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaQCD_MC16d_364530.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaQCD_MC16d_364535.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaQCD_MC16e_364525.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaQCD_MC16e_364530.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_WgammaQCD_MC16e_364535.root",

    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Zllgamma_MC16a_364504.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Zllgamma_MC16a_364509.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Zllgamma_MC16a_364514.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Zllgamma_MC16d_364504.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Zllgamma_MC16d_364509.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Zllgamma_MC16d_364514.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Zllgamma_MC16e_364504.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Zllgamma_MC16e_364509.root",
    "/home/katet/Programs/ZnunugammaInclusive/NewDataSamples/user.akurova.MxAOD_Zllgamma_MC16e_364514.root",
};


static vector<string> etogam_filenames = {
    "/home/katet/Programs/TRExFitter/etogamDD2015-18_reproc-30-09-21.root",
};
