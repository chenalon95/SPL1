#include <iostream>
#include "../include/Restaurant.h"


using namespace std;

Restaurant* backup = nullptr;

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "usage: rest <config_path>" << std::endl;
        return 0;
    }
    string configurationFile = argv[1];
    Restaurant rest(configurationFile);
    rest.start();
    if (backup != nullptr) {
        delete backup;
        backup = nullptr;
    }
    string line;
    while (!line.compare("CloseAll")) {
        getline(cin, line);
    }
    return 0;


}