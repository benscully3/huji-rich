#ifndef PRESSUREFLOOR_HPP
#define PRESSUREFLOOR_HPP 1

#include "source/newtonian/two_dimensional/computational_cell_2d.hpp"
#include "source/newtonian/two_dimensional/cell_updater_2d.hpp"
#include "source/newtonian/test_2d/main_loop_2d.hpp" 

using namespace simulation2d;

class PressureFloor: public CellUpdater
{
public:

  PressureFloor(void) {}

  vector<ComputationalCell> operator()
  (const Tessellation& tess,
   const PhysicalGeometry& /*pg*/,
   const EquationOfState& eos,
   vector<Extensive>& extensives,
   const vector<ComputationalCell>& old,
   const CacheData& cd) const;
  
};

#endif // PRESSUREFLOOR_HPP
