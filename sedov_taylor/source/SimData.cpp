#include "SimData.hpp"
#include <cmath>

namespace {
  vector<double> logspace
  (const double low,
   const double high,
   const size_t n)
  {
    vector<double> res(n);
    for(size_t i=0;i<n;++i)
      res.at(i) = pow
	(10,
	 low+(high-low)*static_cast<double>(i)/static_cast<double>(n-1));
    return res;
  }
}
  
SimData::SimData(void):
  pg_(),
  vertices_(logspace(-2,1,100)),
  interpm_(),
  density_(1., 0.01, 0.3),
  pressure_(1e3, 1e-9,1e-1),
  xvelocity_(0),
  yvelocity_(0),
  eos_(5./3.),
  rs_(),
  //vm_(false),
  vm_(),
  //  tsf_(0.3),
  //  fc_(rs_,
  //      interpm_,
  //      bc_),
  //  eu_(),
  bc_(),
  force_(),
  //  cu_(),
  sim_
  (pg_,
   vertices_,
   interpm_,
   density_,
   pressure_,
   xvelocity_,
   yvelocity_,
   eos_,
   rs_,
   vm_,
   bc_,
   force_
   //   tsf_,
   //   fc_,
   //   eu_,
   //   cu_
   ) {}

hdsim1D& SimData::getSim(void)
{
  return sim_;
}
