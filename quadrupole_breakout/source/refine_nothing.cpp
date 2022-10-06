#include "refine_nothing.hpp"
#include <iostream>

/*
Returns no cells to refine - necessary because ConservativeAMR
(test.cpp:499) takes both cells to remove and cells to refine.
Used ConservativeAMR to only remove cells
 */

RefineNothing::RefineNothing(void){}

vector<size_t> RefineNothing::ToRefine(Tessellation const& /*tess*/,
				       vector<ComputationalCell> const& /*cells*/,
				       double /*time*/) const
{
  vector<size_t> res = {};
  return res;
}

RefineNothing::~RefineNothing(void){}
