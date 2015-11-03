#include "lagrangian.hpp"

vector<Vector2D> Lagrangian::operator()
  (const Tessellation& tess,
   const vector<ComputationalCell>& cells,
   const double /*time*/) const 
{
  vector<Vector2D> res(static_cast<size_t>(tess.GetPointNo()));
  for(size_t i=0;i<cells.size();++i)
    res[i] = cells[i].velocity;
  return res;
}
