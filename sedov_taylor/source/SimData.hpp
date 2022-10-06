#ifndef SIMDATA_HPP
#define SIMDATA_HPP 1

#include "source/newtonian/one_dimensional/hdsim.hpp"
#include "source/newtonian/one_dimensional/pcm1d.hpp"
#include "source/newtonian/common/ideal_gas.hpp"
#include "source/newtonian/common/hllc.hpp"
#include "source/newtonian/one_dimensional/eulerian1d.hpp"
#include "source/newtonian/one_dimensional/lagrangian1d.hpp"
//#include "source/newtonian/one_dimensional/simple_cfl_1d.hpp"
//#include "source/newtonian/one_dimensional/simple_flux_calculator_1d.hpp"
//#include "source/newtonian/one_dimensional/simple_extensive_updater_1d.hpp"
#include "source/newtonian/one_dimensional/rigid_wall_1d.hpp"
#include "source/newtonian/one_dimensional/zero_force_1d.hpp"
#include "source/newtonian/one_dimensional/cylindrical_complementary_1d.hpp"

class SimData
{
public:
  
  SimData(void);

  hdsim1D& getSim(void);

private:
  //  const SlabSymmetry1D pg_;
  const SphericalSymmetry1D pg_;
  const vector<double> vertices_;
  PCM1D interpm_;
  const Step density_;
  const Step pressure_;
  const Uniform xvelocity_;
  const Uniform yvelocity_;
  const IdealGas eos_;
  const Hllc rs_;
  //  const Lagrangian1D vm_;
  const Eulerian1D vm_;
  //  const SimpleCFL1D tsf_;
  //  const SimpleFluxCalculator1D fc_;
  //const SimpleExtensiveUpdater1D eu_;
  const RigidWall1D bc_;
  //  const ZeroForce1D force_;
  const CylindricalComplementary1D force_;
  //  const SimpleCellUpdater1D cu_;
  hdsim1D sim_;
};

#endif // SIMDATA_HPP
