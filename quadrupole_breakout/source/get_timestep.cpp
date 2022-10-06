#include "get_timestep.hpp"

/*
DiagnosticFunction object to write the current
timestep to a file
 */

// DiagnosticFunction::~DiagnosticFunction(void) {}
 
getTimeStep::getTimeStep(string const& fname):
  fname_(fname), prev_time_(0) {}
 
void getTimeStep::operator()(hdsim const& sim)
{
  const double time = sim.getTime();
  const double timestep = time - prev_time_;
  write_number(timestep, fname_);
  prev_time_ =  time;
}
