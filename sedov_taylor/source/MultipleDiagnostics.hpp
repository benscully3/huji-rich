#ifndef MULTIPLE_DIAGNOSTICS_HPP
#define MULTIPLE_DIAGNOSTICS_HPP 1

#include "source/newtonian/test_1d/main_loop_1d.hpp"

using namespace simulation1d;

class MultipleDiagnostics: public DiagnosticsFunction
{
public:
  explicit MultipleDiagnostics
  (const vector<DiagnosticsFunction*>& diag_list);
	   
  void operator()(const hdsim1D& sim);

  void diagnose(const hdsim1D& sim);
	   
  ~MultipleDiagnostics(void);
	
private:
  const vector<DiagnosticsFunction*> diag_list_;
};

#endif // MULTIPLE_DIAGNOSTICS_HPP
