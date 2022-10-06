#include "pressure_floor.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;
using std::function;


// PressureFloor::PressureFloor(void){}


vector<ComputationalCell> PressureFloor::operator()
  (const Tessellation& tess,
   const PhysicalGeometry& /*pg*/,
   const EquationOfState& eos,
   vector<Extensive>& extensives,
   const vector<ComputationalCell>& old,
   const CacheData& cd) const
{
  size_t N = static_cast<size_t>(tess.GetPointNo());
  vector<ComputationalCell> res(N, old[0]);
  for(size_t i=0;i<N;++i){
    Extensive& extensive = extensives[i];
    const double volume = cd.volumes[i];
    const Vector2D r = tess.GetMeshPoint(static_cast<int>(i));
    res[i].density = extensive.mass / volume;
    if(res[i].density<0)
      {
	res[i].density = 1e-15;
	ofstream m_out;
	m_out.open("added_mass.txt", ios_base::app);
	const string sep = ", ";
	m_out << 1e-15*volume << sep
	      << r.x << sep
	      << r.y << "\n";
	m_out.close();
      }
      //throw UniversalError("Negative density");
    res[i].velocity = extensive.momentum / extensive.mass;
    const double energy = extensive.energy / extensive.mass - 
      0.5*ScalarProd(res[i].velocity, res[i].velocity);
    try{
      if(energy>0){
	
	res[i].pressure = eos.de2p(res[i].density,
				   energy,
				   res[i].tracers);
      }
      
      else
	{
	  res[i].pressure = 1e-16;
	  ofstream m_out;
	  m_out.open("added_pressure.txt", ios_base::app);
	  const string sep = ", ";
	  m_out << 1e-10*volume << sep
		<< r.x << sep
		<< r.y << "\n";
	  m_out.close();
	}
    }
    catch(UniversalError& eo){
      eo.addEntry("cell density", res[i].density);
      eo.addEntry("cell energy", energy);
      throw;
    }
    for(size_t j=0;j<extensive.tracers.size();++j)
      res.at(i).tracers.at(j) = extensive.tracers.at(j)/extensive.mass;
  }	
  return res;
}

				   
