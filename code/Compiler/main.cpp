
#include <iostream>
#include <iosfwd>

#include "Compiler/Compiler.h"

int main()
{
    std::cout << "hello compiler!" << std::endl;

    const std::string root_dir = "E:\\develop\\Compiler\\";
    compiler::Compiler::ptr comp(new compiler::Compiler);

    comp->Compile(root_dir + "input.txt", root_dir + "output.txt");

    system("pause");
    return 0;
}

