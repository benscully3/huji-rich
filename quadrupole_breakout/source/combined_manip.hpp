#ifndef COMBINEDMANIP_HPP
#define COMBINEDMANIP_HPP 1

#include "remove_edge_cells.hpp"
#include "refine_nothing.hpp"
#include "make_aspherical.hpp"
#include "source/newtonian/two_dimensional/amr.hpp"

// Manipulation object to combine the two
// manipulations needed

class CombinedManip: public Manipulate
{
public:
  CombinedManip(MakeAspherical& make_aspherical,
		ConservativeAMR& amr);

  void operator()(hdsim& sim);

private:
  MakeAspherical& make_aspherical_;
  ConservativeAMR& amr_;
};
#endif // COMBINEDMANIP_HPP
