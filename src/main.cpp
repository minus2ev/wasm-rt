#include <iostream>

#include "module.h"

using namespace wasm_rt;

int main(int argc, char** argv)
{
    Module module;
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <wasm_file>" << std::endl;
        return 1;
    }
    module.load(argv[1]);
    return 0;
}