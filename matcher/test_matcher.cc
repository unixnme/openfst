#include "fst/fstlib.h"

int main(int argc, const char **argv) {
    fst::StdVectorFst graph;
    graph.SetStart(graph.AddState());

    graph.AddArc(0, fst::StdArc(0, 1, graph.AddState()));
    graph.AddArc(0, fst::StdArc(1, 2, graph.AddState()));

    fst::SortedMatcher<fst::StdFst> matcher{graph, fst::MATCH_INPUT};
    fst::ExplicitMatcher<fst::SortedMatcher<fst::StdFst>> explicitMatcher{graph, fst::MATCH_INPUT};

    matcher.SetState(0);
    explicitMatcher.SetState(0);

    for (matcher.Find(1); !matcher.Done(); matcher.Next()) {
        const auto &arc = matcher.Value();
        std::cout << "0\t" << arc.nextstate << "\t" << arc.ilabel << "\t" << arc.olabel << "\t" << std::endl;
    }
    std::cout << std::endl;

    for (matcher.Find(0); !matcher.Done(); matcher.Next()) {
        const auto &arc = matcher.Value();
        std::cout << "0\t" << arc.nextstate << "\t" << arc.ilabel << "\t" << arc.olabel << "\t" << std::endl;
    }
    std::cout << std::endl;

    for (explicitMatcher.Find(0); !explicitMatcher.Done(); explicitMatcher.Next()) {
        const auto &arc = explicitMatcher.Value();
        std::cout << "0\t" << arc.nextstate << "\t" << arc.ilabel << "\t" << arc.olabel << "\t" << std::endl;
    }

    std::cout << std::endl;

    graph.SetFinal(1);
    graph.SetFinal(2);
    graph.Write("test.fst");
    return EXIT_SUCCESS;
}