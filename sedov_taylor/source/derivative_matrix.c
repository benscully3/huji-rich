/******************************************************************************
 *                      Code generated with sympy 1.6.2                       *
 *                                                                            *
 *              See http://www.sympy.org/ for more information.               *
 *                                                                            *
 *                        This file is part of 'fick'                         *
 ******************************************************************************/
#include "derivative_matrix.h"
#include <math.h>

double calc_previous(double d1, double d2, double d3, double x1, double x2, double x3) {

   double calc_previous_result;
   calc_previous_result = (d1*x2*pow(x2 - x3, 2) + 3*d2*x2*pow(x1 - x2, 2) + 2*d2*x2*(x1 - x2)*(x2 - x3) - d2*x2*pow(x2 - x3, 2) + 2*d2*pow(x1 - x2, 2)*(x2 - x3) + 2*d2*(x1 - x2)*pow(x2 - x3, 2) - d3*x2*pow(x1 - x2, 2))/(x2*pow(x1 - x2, 2)*(pow(x1 - x2, 2) + 2*(x1 - x2)*(x2 - x3) + pow(x2 - x3, 2)));
   return calc_previous_result;

}

double calc_current(double d1, double d2, double d3, double x1, double x2, double x3) {

   double calc_current_result;
   calc_current_result = (d1*x2*(x1 - x2)*pow(x2 - x3, 2) - d1*x2*pow(x2 - x3, 3) + d2*x2*pow(x1 - x2, 3) - 3*d2*x2*pow(x1 - x2, 2)*(x2 - x3) - 3*d2*x2*(x1 - x2)*pow(x2 - x3, 2) + d2*x2*pow(x2 - x3, 3) + 2*d2*pow(x1 - x2, 3)*(x2 - x3) - 2*d2*(x1 - x2)*pow(x2 - x3, 3) - d3*x2*pow(x1 - x2, 3) + d3*x2*pow(x1 - x2, 2)*(x2 - x3))/(x2*pow(x1 - x2, 2)*(x1 - x3)*pow(x2 - x3, 2));
   return calc_current_result;

}

double calc_next(double d1, double d2, double d3, double x1, double x2, double x3) {

   double calc_next_result;
   calc_next_result = (-d1*x2*pow(x2 - x3, 2) - d2*x2*pow(x1 - x2, 2) + 2*d2*x2*(x1 - x2)*(x2 - x3) + 3*d2*x2*pow(x2 - x3, 2) - 2*d2*pow(x1 - x2, 2)*(x2 - x3) - 2*d2*(x1 - x2)*pow(x2 - x3, 2) + d3*x2*pow(x1 - x2, 2))/(x2*pow(x2 - x3, 2)*(pow(x1 - x2, 2) + 2*(x1 - x2)*(x2 - x3) + pow(x2 - x3, 2)));
   return calc_next_result;

}
