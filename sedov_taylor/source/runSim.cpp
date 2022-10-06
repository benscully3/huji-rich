#include "runSim.hpp"
#include "source/newtonian/test_1d/main_loop_1d.hpp"
#include "source/newtonian/one_dimensional/hdf5_diagnostics1d.hpp"
#include "MultipleDiagnostics.hpp"
#include "ConsecutiveSnapshots.hpp"

using namespace simulation1d;
using namespace diagnostics1d;

void runSim(hdsim1D& sim)
{
  const int max_iter = 1e7;
  const double tf = 0.1;
  SafeTimeTermination term_cond(tf,max_iter);

  MultipleDiagnostics diag
    ({new ConsecutiveSnapshots(tf/100),
      new WriteTime("time.txt")});

  //  DoNothing manip;
  //  Diffusion manip(1e-1);

  main_loop
    (sim,
     term_cond,
     1,
     &diag);

  write_snapshot_to_hdf5(sim, "output/final.h5");
}
