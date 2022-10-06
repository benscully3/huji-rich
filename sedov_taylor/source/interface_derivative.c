/******************************************************************************
 *                      Code generated with sympy 1.6.2                       *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                        This file is part of 'fick'                         *
 ******************************************************************************/
#include "interface_derivative.h"
#include <math.h>

double calc_previous(double r_l, double r_m, double r_r) {

   double calc_previous_result;
   calc_previous_result = 4*(pow(r_l, 3) - pow(r_m, 3))*(pow(r_m, 3) - pow(r_r, 3))/((pow(r_l, 3) - pow(r_m, 3))*(pow(r_m, 4) - 4*r_m*pow(r_r, 3) + 3*pow(r_r, 4)) + (pow(r_m, 3) - pow(r_r, 3))*(3*pow(r_l, 4) - 4*pow(r_l, 3)*r_m + pow(r_m, 4)));
   return calc_previous_result;

}

double calc_next(double r_l, double r_m, double r_r) {

   double calc_next_result;
   calc_next_result = -4*(pow(r_l, 3) - pow(r_m, 3))*(pow(r_m, 3) - pow(r_r, 3))/((pow(r_l, 3) - pow(r_m, 3))*(pow(r_m, 4) - 4*r_m*pow(r_r, 3) + 3*pow(r_r, 4)) + (pow(r_m, 3) - pow(r_r, 3))*(3*pow(r_l, 4) - 4*pow(r_l, 3)*r_m + pow(r_m, 4)));
   return calc_next_result;

}
