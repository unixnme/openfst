#include <string>
#include "fst/fstlib.h"

// Compare class for comparing input labels of arcs.
template <class Arc>
class IlabelWeightCompare {
public:
    constexpr IlabelWeightCompare() {}

    constexpr bool operator()(const Arc &lhs, const Arc &rhs) const {
        return std::forward_as_tuple(lhs.ilabel, lhs.weight.Value()) <
               std::forward_as_tuple(rhs.ilabel, rhs.weight.Value());
    }

    constexpr uint64 Properties(uint64 props) const {
        return (props & fst::kArcSortProperties) | fst::kILabelSorted |
               (props & fst::kAcceptor ? fst::kOLabelSorted : 0);
    }
};

int Usage(const std::string &program) {
    std::cerr << "Usage: " << program << " in.fst [out.fst]" << std::endl;
    std::cerr << "\tin.fst: input fst to sort their arcs by index and weight" << std::endl;
    std::cerr << "\tout.fst: output fst after sort" << std::endl;
    return EXIT_FAILURE;
}

int main(int argc, const char** argv) {
//    using fst::script::MutableFstClass;

    if (argc != 2 && argc != 3) return Usage(argv[0]);
    const std::string in_name{ "-" == std::string{argv[1]} ? "/dev/stdin" : argv[1]};
    const std::string out_name{argc == 2 || "-" == std::string{argv[2]} ? "/dev/stdout" : argv[2]};

    std::unique_ptr<fst::StdMutableFst> in_fst(fst::StdMutableFst::Read(in_name, true));
    if (!in_fst) {
        std::cerr << "Failed reading " << in_name << std::endl;
        return EXIT_FAILURE;
    }

    const IlabelWeightCompare<fst::StdArc> icomp;
    ArcSort(in_fst.get(), icomp);

    in_fst->Write(out_name);
    return EXIT_SUCCESS;
}