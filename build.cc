// clang++ build.cc -o maker

#include "maker.hh"
int main() {
    REBUILD_MAKER;
    runCmdE("clang++", {"src/program.cpp", "-lraylib", "-o", "maze"});
}