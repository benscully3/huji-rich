#ifndef GETTIMESTEP_HPP
#define GETTIMESTEP_HPP 1

#include "source/misc/simple_io.hpp"
#include "source/newtonian/test_2d/main_loop_2d.hpp"


class getTimeStep: public DiagnosticFunction
 {
 public:
 
   explicit getTimeStep(const string& fname);
 
   void operator()(const hdsim& sim);
 
 private:
   const string fname_;
   double prev_time_;
 };

#endif // GETTIMSTEP_HPP
