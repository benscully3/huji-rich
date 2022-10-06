#ifndef REMOVEEDGECELLS_HPP
#define REMOVEEDGECELLS_HPP 1

#include "source/newtonian/two_dimensional/amr.hpp"
#include "source/newtonian/test_2d/main_loop_2d.hpp"

/*
Returns cells that are withing a distance 'close' to the edge
of the computational domain for ConservativeAMR to remove. This
helps prevent cells from leaving the domain and crashing the 
simulation
 */

using namespace simulation2d;

class RemoveEdgeCells: public CellsToRemove
{
public:
  RemoveEdgeCells(const double close,
		  //const Tessellation& tess,
		  const double left,
		  const double right,
		  const double top,
		  const double bot);

  pair<vector<size_t>, vector<double> > ToRemove(Tessellation const& tess,
						 vector<ComputationalCell> const& /*cells*/,
						 double /*time*/) const;

private:
  const double close_;
  //const Tessellation& tess_;
  const double left_;
  const double right_;
  const double top_;
  const double bot_;
  
};

#endif //REMOVEEDGECELLS_HPP
