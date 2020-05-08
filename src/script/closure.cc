// See www.openfst.org for extensive documentation on this weighted
// finite-state transducer library.

#include <fst/script/closure.h>

#include <fst/script/script-impl.h>

namespace fst {
namespace script {

void Closure(MutableFstClass *fst, ClosureType closure_type) {
  ClosureArgs args(fst, closure_type);
  Apply<Operation<ClosureArgs>>("Closure", fst->ArcType(), &args);
}

REGISTER_FST_OPERATION_3ARCS(Closure, ClosureArgs);

}  // namespace script
}  // namespace fst
