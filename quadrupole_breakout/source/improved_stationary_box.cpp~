#include "improved_stationary_box.hpp"

ImprovedStationaryBox::ImprovedStationaryBox(void) {}

namespace {
  bool is_outer_edge
  (const Edge& edge,
   const vector<Vector2D>& point_velocities)
  {
    size_t frst = edge.neighbors.first;
    size_t scnd = edge.neighbors.second;
    size_t len = point_velocities.size();
    return (frst>len) || (scnd>len);
  }
}

vector<Vector2D> ImprovedStationaryBox::operator()
(const Tessellation& tess,
 const vector<Vector2D>& point_velocities) const
{
  const vector<Edge>& edge_list = tess.getAllEdges();
  vector<Vector2D> res(edge_list.size());
  const size_t nloop = res.size();
  for(size_t i=0;i<nloop;++i)
  {
    const Edge& edge = edge_list[i];
    res[i] = is_outer_edge(edge,point_velocities) ?
      Vector2D() :
      tess.CalcFaceVelocity
      (point_velocities.at(static_cast<size_t>(edge.neighbors.first)),
       point_velocities.at(static_cast<size_t>(edge.neighbors.second)),
       tess.GetMeshPoint(edge.neighbors.first),
       tess.GetMeshPoint(edge.neighbors.second),
       calc_centroid(edge));
  }
  return res;
}
