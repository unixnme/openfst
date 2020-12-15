#include "fst/fstlib.h"

using namespace fst;

int Usage(const std::string &program) {
    std::cerr << "Usage: " << program << " phi_idx in1.fst in2.fst out.fst" << std::endl;
    return EXIT_FAILURE;
}

int main(int argc, const char** argv) {
    if (argc != 4 && argc != 5) return Usage(argv[0]);
    const int phi_idx = std::stoi(argv[1]);
    if (phi_idx <= 0) {
        std::cerr << "phi_idx must be a positive integer" << std::endl;
        return EXIT_FAILURE;
    }

    const auto fst1 = StdFst::Read(argv[2]);
    const auto fst2 = StdFst::Read(argv[3]);
    const auto outfile = argc == 4 ? "/dev/stdout" : argv[4];

    using PM = PhiMatcher<SortedMatcher<StdFst>>;

    ComposeFstOptions<StdArc, PM> opts;
    opts.gc_limit = 0;
    opts.matcher1 = new PM(fst1, MATCH_NONE, kNoLabel);
    opts.matcher2 = new PM(fst2, MATCH_INPUT, phi_idx);

    StdComposeFst cfst(*fst1, *fst2, opts);
    StdVectorFst fst3{cfst};
    fst3.Write(outfile);

    delete fst1;
    delete fst2;

    return EXIT_SUCCESS;
}