#include <iostream>
#include "fst/fstlib.h"
#include "fst/script/print.h"

int main(int argc, const char** argv) {
    auto ifst = fst::StdVectorFst::Read(argv[1]);
    auto ofst = fst::StdVectorFst::Read(argv[2]);

    typedef fst::PhiMatcher<fst::SortedMatcher<fst::StdFst> > PM;
    fst::ComposeFstOptions<fst::StdArc, PM> opts;
    opts.gc_limit = 0;
    opts.matcher1 = new PM{ifst, fst::MATCH_NONE, fst::kNoLabel};
    opts.matcher2 = new PM{ofst, fst::MATCH_INPUT, 1};

    fst::StdComposeFst cfst{*ifst, *ofst, opts};

    fst::FstPrinter<fst::StdArc> printer{cfst,
                                         cfst.InputSymbols(),
                                         cfst.OutputSymbols(),
                                         nullptr,
                                         false,
                                         false,
                                         "\t"};
    printer.Print(std::cout, "");
    return 0;
}