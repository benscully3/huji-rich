#include "make_aspherical.hpp"
#include <iostream>
#include <cmath>
using namespace std;
using std::function;

//#include <vector>
//#include "source/newtonian/two_dimensional/computational_cell_2d.hpp"
//#include "source/tessellation/tessellation.hpp"

/*
If shock front is at a certain fraction(frac) of the radius
x-velocity is removed to make the explosion aspherical
 */

MakeAspherical::MakeAspherical(const double radius,
			       const double frac,
			       bool check,
			       const Tessellation& tess,
			       const PhysicalGeometry& pg,
			       const string fname):
  radius_(radius), frac_(frac), check_(check),
  tess_(tess), pg_(pg),fname_(fname){}


namespace
// Locates shock front based on maximum velocity
{
  int getShockfront(const vector<ComputationalCell>& cells){
    double mx_v = 0;
    int mx_ind = 0;
    for(size_t i=0;i<cells.size();++i){				     
      const Vector2D& v_vec = cells[i].velocity;
      const double v_r = abs(v_vec);
      if(v_r>mx_v){
        mx_v = v_r;
        mx_ind = static_cast<int>(i);	
      }
    }
    return mx_ind;
  }

  double getKineticEnergy(const vector<ComputationalCell>& cells,
			  const CachedLazyList<double>& vols)
  {
    double e_k = 0.;
    for (size_t i=0;i<cells.size();i++)
      {
	double v = abs(cells[i].velocity);
	e_k = e_k + 0.5*cells[i].density*vols[i]*pow(v, 2);
      }
    return e_k;
  }

  double getThermalEnergy(const vector<ComputationalCell>& cells,
			  const CachedLazyList<double>& vols,
			  const double ad_idx){
    double e_t = 0.;
    for (size_t i=0;i<cells.size();i++)
      {
	e_t = e_t + (cells[i].pressure*vols[i])/(ad_idx - 1);
      }
    return e_t;
  }

  void write_energy(const vector<ComputationalCell>& cells,
		    const Tessellation& tess,
		    const PhysicalGeometry& pg,
		    const double ad_idx,
		    const string fname){
  
    const CacheData cd = CacheData(tess, pg);
    const CachedLazyList<double> vols = cd.volumes;
    const double e_k = getKineticEnergy(cells, vols);
    const double e_t = getThermalEnergy(cells, vols, ad_idx);
    const double e_tot = e_k + e_t;

    write_number(e_tot, fname);  
  }
}

void MakeAspherical::operator()(hdsim& sim)
{
  if (check_){
    const double poi = radius_*frac_;  // point of interest (where we make aspherical)
  
    vector<ComputationalCell>& cells = sim.getAllCells();
    const Tessellation& tess = sim.getTessellation();
    int ind = getShockfront(cells); // finds shock front
  
    const Vector2D& r = tess.GetMeshPoint(static_cast<int>(ind));
    const double R = abs(r);
    if (R>poi){
      // if shock front is past poi remove x-velocity
      const double ad_idx = 4./3.;
      // energy before removing v_x
      write_energy(cells, tess_, pg_, ad_idx, "Pre_energy.txt");
      for(size_t i=0;i<cells.size();++i){
        cells[i].velocity.x = 0;		    
      }
      check_ = false; // ensures v_x won't be removed again in later timesteps
      write_energy(cells, tess_, pg_, ad_idx, fname_); // energy after removing v_x
    }
    sim.recalculateExtensives();
    
  }
}
