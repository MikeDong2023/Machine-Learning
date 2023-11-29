#include <iostream>
#include "csvstream.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    //Error checking
    if(argc != 3 && argc != 4) {
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return 1;
    }
    try {csvstream trainFile(argv[1]);}
    catch(csvstream_exception) {
        cout << "Error opening file: " << argv[1] << endl;
        return 1;
    }
    try {csvstream testFile(argv[2]);}
    catch(csvstream_exception) {
        cout << "Error opening file: " << argv[2] << endl;
        return 1;
    }
}