#ifndef REFINENOTHING_HPP
#define REFINENOTHING_HPP 1

#include "source/newtonian/two_dimensional/amr.hpp"

/*
Returns no cells to refine - necessary because ConservativeAMR
(test.cpp:499) takes both cells to remove and cells to refine.
Used ConservativeAMR to only remove cells
 */

class RefineNothing: public CellsToRefine
{
public:
  RefineNothing(void);

  ~RefineNothing(void);

  vector<size_t> ToRefine(Tessellation const& tess,
			  vector<ComputationalCell> const& cells,
			  double /*time*/) const;
private:
};

#endif //REFINENOTHING_HPP


  
  
