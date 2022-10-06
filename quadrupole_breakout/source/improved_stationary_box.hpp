#ifndef IMPROVED_STATIONARY_BOX_HPP
#define IMPROVED_STATIONARY_BOX_HPP 1

#include "source/newtonian/two_dimensional/edge_velocity_calculator.hpp"

//! \brief Edge velocity calculator for a stationary box
class ImprovedStationaryBox: public EdgeVelocityCalculator
{
public:

  ImprovedStationaryBox(void);

  vector<Vector2D> operator()
  (const Tessellation& tess,
   const vector<Vector2D>& point_velocities) const override;
};

#endif // IMPROVED_STATIONARY_BOX_HPP
