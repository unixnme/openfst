#include <iostream>
#include "fst/fstlib.h"
#include "fst/script/print.h"

int main(int argc, const char** argv) {
    auto ifst = fst::StdVectorFst::Read(argv[1]);
    auto ofst = fst::StdVectorFst::Read(argv[2]);

    fst::FstPrinter<fst::StdArc> printer{*ifst,
                                         ifst->InputSymbols(),
                                         ifst->OutputSymbols(),
                                         nullptr,
                                         false,
                                         false,
                                         "\t"};
    printer.Print(std::cout, "");
    return 0;
}