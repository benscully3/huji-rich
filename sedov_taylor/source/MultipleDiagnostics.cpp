#include "MultipleDiagnostics.hpp"

MultipleDiagnostics::MultipleDiagnostics
(const vector<DiagnosticsFunction*>& diag_list):
  diag_list_(diag_list) {}
	   
void MultipleDiagnostics::operator()(const hdsim1D& sim)
{
  for(size_t i=0;i<diag_list_.size();++i)
    (*diag_list_.at(i)).diagnose(sim);
}

void MultipleDiagnostics::diagnose(const hdsim1D& sim)
{
  for(size_t i=0;i<diag_list_.size();++i)
    (*diag_list_.at(i)).diagnose(sim);
}
	   
MultipleDiagnostics::~MultipleDiagnostics(void)
{
  for(size_t i=0;i<diag_list_.size();++i)
    delete diag_list_.at(i);
}
