#include "remove_edge_cells.hpp"
#include <iostream>
#include <cmath>

using namespace std;
using std::function;

/*
Returns cells that are withing a distance 'close' to the edge
of the computational domain for ConservativeAMR (test.cpp:499)
to remove. This helps prevent cells from leaving the domain
and crashing the simulation
 */

RemoveEdgeCells::RemoveEdgeCells(const double close,
				 //const Tessellation& tess,
				 const double left,
				 const double right,
				 const double top,
				 const double bot):
  close_(close), //tess_(tess),
  left_(left), right_(right), top_(top), bot_(bot){}

pair<vector<size_t>, vector<double> > RemoveEdgeCells::ToRemove(Tessellation const& tess,
								vector<ComputationalCell> const& /*cells*/,
								double /*time*/) const
{
  pair<vector<size_t>, vector<double> >res;
  vector<double> merits;
  vector<size_t> indices;
  size_t N = static_cast<size_t>(tess.GetPointNo());
  for (size_t i = 0; i < N; ++i)
    {
      Vector2D const& point = tess.GetMeshPoint(static_cast<int>(i));
      // condition to add cells to removal list
      if ((point.x-left_ < close_)||(point.y-bot_ < close_)||
	  (top_-point.y < close_)||(right_-point.x < close_))
	{
	  indices.push_back(i);
	  vector<double> dists = {point.x-left_,point.y-bot_,
				  top_-point.y,right_-point.x};
	  double closest = close_;
	  for (size_t j = 0; j <dists.size();++j)
	    {
	      if (dists[j]<closest)
		dists[j]=closest;
	    }
	  merits.push_back(1/closest); // prioritizes removing cells closest to edge
	}
    } 
  return pair<vector<size_t>, vector<double> >(indices, merits);
  
}

