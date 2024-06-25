/*
    THE BUILD BINARY MUST BE STORED IN A FILE NAMED maker OR THIS WILL NOT WORK!
    This software is distrubuted under https://fsc.pogging.fish/license/txt/ppplv1.txt
    Luna Driver 2024
*/

#ifndef __MAKER_HH
#define __MAKER_HH

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <filesystem>

bool quiet = false;
bool oq = quiet;

#define REBUILD_MAKER rebuildMaker(slasher(__FILE__))
#define TEMP_QUIET(val) bool oq = quiet; quiet = val
#define QUIET_SWITCHBACK() quiet = oq
#define LOG(CMD, ARGS) if (!quiet) std::cout << "* " << CMD << disectArgs(ARGS) << std::endl

enum COMPARISONS {
    SAME,
    DIFFERENT,
    FILE_NOT_FOUND
};

std::string slasher (const char * filename) {
    return std::filesystem::path(filename).filename().string();
}

std::string disectArgs(std::vector<std::string> s) {
    std::stringstream st;
    st << " ";
    std::for_each(s.begin(), s.end(), [&st](std::string e){st << e << " ";});
    return st.str();
}

int runCmd(std::string command, std::vector<std::string> arguments) {
    LOG(command, arguments);
    int ex = std::system(command.append(disectArgs(arguments)).c_str());
    return ex >> 8;
}

int runCmdE(std::string command, std::vector<std::string> arguments) {
    int ex = runCmd(command, arguments);
    if (ex != 0) {
        std::cout << "Command '" << command << disectArgs(arguments) << "\b' exited with code " << (ex) << std::endl;
        exit(ex);
    }
    return 0;
}

COMPARISONS compare(std::string file1, std::string file2) {
    TEMP_QUIET(true);
    COMPARISONS r = (COMPARISONS)runCmd("cmp", {file1, file2, "--quiet"});
    QUIET_SWITCHBACK();
    return r;
}

void rebuildMaker(std::string file) {
    TEMP_QUIET(true);
    runCmd("g++", {file, "-onewmaker"});
    switch(compare("maker", "newmaker")) {
        case DIFFERENT:
            std::cout << "-- change detected --" << std::endl;
            runCmd("mv", {"newmaker", "maker"});
            exit(runCmd("./maker", {}));
            break;
        case FILE_NOT_FOUND:
            std::cout << "-- failed to rebuild! --" << std::endl;
            exit(1);
            break;
        case SAME:
            runCmd("rm", {"./newmaker"});
            break;
    }
    QUIET_SWITCHBACK();
}

#endif