#include "combined_manip.hpp"
#include <iostream>


// Manipulation object to combine the two
// manipulations needed

using namespace std;

CombinedManip::CombinedManip(MakeAspherical& make_aspherical,
		ConservativeAMR& amr):
    make_aspherical_(make_aspherical), amr_(amr){}

void CombinedManip::operator()(hdsim& sim)
{
  make_aspherical_(sim);
  amr_(sim);
}
