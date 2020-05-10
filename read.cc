#include "fst/fst.h"
#include "fst/fstlib.h"
#include "fst/script/fst-class.h"
#include <string>

namespace fst {
REGISTER_FST(ConstFst, StdArc);
REGISTER_FST(ConstFst, LogArc);
REGISTER_FST(VectorFst, StdArc);
}

int main(int argc, const char** argv) {
    const std::string filename{argv[1]};

    auto myfst = fst::ExpandedFst<fst::LogArc>::Read(filename);
    std::cout << myfst->NumStates() << std::endl;
    fst::script::FstClass *ifst = fst::script::FstClass::Read(filename);
    std::cout << ifst->ArcType() << std::endl;


    return 0;
}