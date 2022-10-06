#include "ConsecutiveSnapshots.hpp"
#include "source/misc/int2str.hpp"
#include "source/newtonian/one_dimensional/hdf5_diagnostics1d.hpp"

using namespace diagnostics1d;

ConsecutiveSnapshots::ConsecutiveSnapshots(double dt):
  dt_(dt), t_next_(0), index_(0) {}
		
void ConsecutiveSnapshots::operator()(const hdsim1D& sim)
{
  if(sim.GetTime()>t_next_){
    write_snapshot_to_hdf5(sim,"output/snapshot_"+int2str(index_)+".h5");
    ++index_;
    t_next_ += dt_;
  }
}

void ConsecutiveSnapshots::diagnose(const hdsim1D& sim)
{
  if(sim.GetTime()>t_next_){
    write_snapshot_to_hdf5(sim,"output/snapshot_"+int2str(index_)+".h5");
    ++index_;
    t_next_ += dt_;
  }
}
