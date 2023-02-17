#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "MergeFiles.h"
#include "SourceFiles.h"

using namespace std;


int main() {
    merge_files("data", data_filenames);
    merge_files("signal", signal_filenames);
    merge_files("bkg", background_filenames);
}
