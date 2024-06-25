// g++ build.cc -o maker

#include "maker.hh"
int main() {
    REBUILD_MAKER;
    runCmdE("g++", {"src/program.cpp", "-lraylib", "-o", "maze"});
}