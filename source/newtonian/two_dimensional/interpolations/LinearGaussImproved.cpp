#include "LinearGaussImproved.hpp"
#include "../../../misc/utils.hpp"
#ifdef RICH_MPI
#include "../../../mpi/mpi_commands.hpp"
#endif //RICH_MPI



namespace 
{
	void GetNeighborMesh(Tessellation const& tess, vector<const Edge *> const& edges,size_t cell_index,
		vector<Vector2D> &res)
	{
		res.resize(edges.size());
		const size_t nloop = edges.size();
		for (size_t i = 0; i < nloop; ++i)
		{
			const int neigh0 = edges[i]->neighbors.first;
			const int neigh1 = edges[i]->neighbors.second;
			if (neigh0 == static_cast<int>(cell_index))
				res[i]=tess.GetMeshPoint(neigh1);
			else
				res[i]=tess.GetMeshPoint(neigh0);
		}
	}

	void GetNeighborCM(Tessellation const& tess, vector<const Edge*> const& edges, size_t cell_index,
		vector<Vector2D> &res)
	{
		res.resize(edges.size());
		const size_t nloop = edges.size();
		for (size_t i = 0; i<nloop; ++i)
		{
			const int neigh0 = edges[i]->neighbors.first;
			const int neigh1 = edges[i]->neighbors.second;
			if (neigh0 == static_cast<int>(cell_index))
					res[i]=tess.GetCellCM(neigh1);
			else
					res[i]=tess.GetCellCM(neigh0);
		}
	}

	vector<ComputationalCell const*> GetNeighborCells(vector<const Edge *> const& edges, size_t cell_index,
						   vector<ComputationalCell> const& cells, boost::container::flat_map<size_t, ComputationalCell> const& ghost_cells,size_t /*npoints*/)
	{
		vector<ComputationalCell const*> res(edges.size());
		const size_t nloop = edges.size();
		for (size_t i = 0; i<nloop; ++i)
		{
			size_t other_cell = (edges[i]->neighbors.first == static_cast<int>(cell_index)) ? static_cast<size_t>
				(edges[i]->neighbors.second) : static_cast<size_t> (edges[i]->neighbors.first);
			const boost::container::flat_map<size_t,ComputationalCell>::const_iterator it = 
			  ghost_cells.find(other_cell);
			if(it==ghost_cells.end())
			  res[i]=&cells.at(other_cell);
			else
				res[i]=&it->second;
		}
		return res;
	}

	void GetEdgeList(Tessellation const& tess,
		vector<int> const& edge_indices,vector<const Edge*> &res)
	{
		res.reserve(edge_indices.size());
		const size_t nloop = edge_indices.size();
		for (size_t i = 0; i<nloop; ++i)
			res.push_back(&tess.GetEdge(edge_indices[i]));
	}

	void calc_naive_slope(ComputationalCell const& cell,
		Vector2D const& center, Vector2D const& cell_cm, double cell_volume, vector<ComputationalCell const*> const& neighbors,
		vector<Vector2D> const& neighbor_centers, vector<Vector2D> const& neigh_cm, vector<const Edge*> const& edge_list,
		Slope &res,
		Slope &vec_compare)
	{
		size_t n = edge_list.size();
		if (n>20)
		{
			UniversalError eo("Cell has too many neighbors");
			eo.AddEntry("Cell x cor", center.x);
			eo.AddEntry("Cell y cor", center.y);
			throw eo;
		}
		// Create the matrix to invert and the vector to compare
		vector<double> m(4, 0);
		for (size_t i = 0; i < edge_list.size(); ++i)
		{
			const Vector2D c_ij = CalcCentroid(*edge_list[i]) -0.5*(neigh_cm[i] + cell_cm);
			const double e_length = edge_list[i]->GetLength();
			const Vector2D r_ij = normalize(neighbor_centers[i] - center)*e_length;
			m[0] -= c_ij.x*r_ij.x;
			m[1] -= c_ij.y*r_ij.x;
			m[2] -= c_ij.x*r_ij.y;
			m[3] -= c_ij.y*r_ij.y;
			if (i == 0)
			{
				ReplaceComputationalCell(vec_compare.xderivative, cell);
				ReplaceComputationalCell(vec_compare.yderivative, cell);
				vec_compare.xderivative *= r_ij.x*0.5;
				vec_compare.yderivative *= r_ij.y*0.5;
			}
			else
			{
				ComputationalCellAddMult(vec_compare.xderivative, cell, r_ij.x*0.5);
				ComputationalCellAddMult(vec_compare.yderivative, cell, r_ij.y*0.5);
			}
			ComputationalCellAddMult(vec_compare.yderivative, *neighbors[i], r_ij.y*0.5);
			ComputationalCellAddMult(vec_compare.xderivative, *neighbors[i], r_ij.x*0.5);
		}
		m[0] += cell_volume;
		m[3] += cell_volume;
		// Find the det
		const double det = m[0] * m[3] - m[1] * m[2];
		// Check none singular
		if (std::abs(det) < 1e-10*cell_volume*cell_volume)
		{
			UniversalError eo("Singular matrix");
			eo.AddEntry("Cell x cor", center.x);
			eo.AddEntry("Cell y cor", center.y);
			eo.AddEntry("Cell volume", cell_volume);
			eo.AddEntry("Det was", det);
			throw eo;
		}
		// Invert the matrix
		vector<double> m_inv(4);
		const double det_inv = 1.0 / det;
		m_inv[0] = m[3] * det_inv;
		m_inv[1] = -m[1] * det_inv;
		m_inv[2] = -m[2] * det_inv;
		m_inv[3] = m[0] * det_inv;
		// Calculate the gradient
		ReplaceComputationalCell(res.xderivative, vec_compare.xderivative);
		ReplaceComputationalCell(res.yderivative, vec_compare.yderivative);
		res.xderivative *= m_inv[0];
		res.yderivative *= m_inv[3];
		ComputationalCellAddMult(res.xderivative, vec_compare.yderivative, m_inv[1]);
		ComputationalCellAddMult(res.yderivative, vec_compare.xderivative, m_inv[2]);
	}

	double PressureRatio(ComputationalCell cell, vector<ComputationalCell const*> const& neigh)
	{
		double res = 1;
		double p = cell.pressure;
		for (size_t i = 0; i<neigh.size(); ++i)
		{
			if (p>neigh[i]->pressure)
				res = std::min(res, neigh[i]->pressure / p);
			else
				res = std::min(res, p / neigh[i]->pressure);
		}
		return res;
	}

	bool is_shock(Slope const& naive_slope, double cell_width, double shock_ratio,
		ComputationalCell const& cell, vector<ComputationalCell const*> const& neighbor_list, double pressure_ratio,double cs)
	{
		const bool cond1 =(naive_slope.xderivative.velocity.x + naive_slope.yderivative.velocity.y)*
			cell_width<(-shock_ratio)*cs;
		const bool cond2 = PressureRatio(cell, neighbor_list)<pressure_ratio;
		return cond1 || cond2;
	}
	
	ComputationalCell interp(ComputationalCell const& cell,Slope const& slope,
		Vector2D const& target,Vector2D const& cm)
	{
		ComputationalCell res(cell);
		ComputationalCellAddMult(res, slope.xderivative, target.x - cm.x);
		ComputationalCellAddMult(res, slope.yderivative, target.y - cm.y);
		return res;
	}

	void interp2(ComputationalCell &res, Slope const& slope,
		Vector2D const& target, Vector2D const& cm)
	{
		ComputationalCellAddMult(res, slope.xderivative, target.x - cm.x);
		ComputationalCellAddMult(res, slope.yderivative, target.y - cm.y);
	}

	void slope_limit(ComputationalCell const& cell,Vector2D const& cm,
		vector<ComputationalCell const*> const& neighbors, vector<const Edge*> const& edge_list,
		Slope &slope,
		ComputationalCell &cmax,
		ComputationalCell &cmin,
		ComputationalCell &maxdiff,
		ComputationalCell &mindiff)
	{
		ReplaceComputationalCell(cmax, cell);
		ReplaceComputationalCell(cmin, cell);
		// Find maximum.minimum neighbor values
		size_t nloop = neighbors.size();
		for (size_t i = 0; i < nloop; ++i)
		{
			ComputationalCell const& cell_temp = *neighbors[i];
			cmax.density = std::max(cmax.density, cell_temp.density);
			cmax.pressure = std::max(cmax.pressure, cell_temp.pressure);
			cmax.velocity.x = std::max(cmax.velocity.x, cell_temp.velocity.x);
			cmax.velocity.y = std::max(cmax.velocity.y, cell_temp.velocity.y);
			cmin.density = std::min(cmin.density, cell_temp.density);
			cmin.pressure = std::min(cmin.pressure, cell_temp.pressure);
			cmin.velocity.x = std::min(cmin.velocity.x, cell_temp.velocity.x);
			cmin.velocity.y = std::min(cmin.velocity.y, cell_temp.velocity.y);
			for (size_t j = 0; j < cell_temp.tracers.size(); ++j)
			{
			  (cmax.tracers.begin()+static_cast<int>(j))->second = std::max((cmax.tracers.begin() + static_cast<int>(j))->second,
											(cell_temp.tracers.begin() + static_cast<int>(j))->second);
			  (cmin.tracers.begin() + static_cast<int>(j))->second = std::min((cmin.tracers.begin() + static_cast<int>(j))->second,
											  (cell_temp.tracers.begin() + static_cast<int>(j))->second);
			}
		}
		ReplaceComputationalCell(maxdiff, cmax);
		maxdiff -= cell;
		ReplaceComputationalCell(mindiff, cmin);
		mindiff -= cell;
		// limit the slope
		ComputationalCell centroid_val = interp(cell, slope, CalcCentroid(*edge_list[0]), cm);
		ComputationalCell dphi = centroid_val - cell;
		vector<double> psi(4 + cell.tracers.size(), 1);
		const size_t nedges = edge_list.size();
		for (size_t i = 0; i<nedges; ++i)
		{
			if (i > 0)
			{
				ReplaceComputationalCell(centroid_val, cell);
				interp2(centroid_val, slope, CalcCentroid(*edge_list[i]), cm);
				ReplaceComputationalCell(dphi, centroid_val);
				dphi -= cell;
			}
			// density
			if (std::abs(dphi.density) > 0.1*std::max(std::abs(maxdiff.density),std::abs(mindiff.density))||centroid_val.density*cell.density < 0)
			{
				if (dphi.density > 1e-9*cell.density)
					psi[0] = std::min(psi[0], maxdiff.density / dphi.density);
				else 
					if (dphi.density<-1e-9*cell.density)
						psi[0] = std::min(psi[0], mindiff.density / dphi.density);
			}
			// pressure
			if (std::abs(dphi.pressure) > 0.1*std::max(std::abs(maxdiff.pressure), std::abs(mindiff.pressure)) || centroid_val.pressure*cell.pressure < 0)
			{
				if (dphi.pressure > 1e-9*cell.pressure)
					psi[1] = std::min(psi[1], maxdiff.pressure / dphi.pressure);
				else
					if (dphi.pressure<-1e-9*cell.pressure)
						psi[1] = std::min(psi[1], mindiff.pressure / dphi.pressure);
			}
			// xvelocity
			if (std::abs(dphi.velocity.x) > 0.1*std::max(std::abs(maxdiff.velocity.x), std::abs(mindiff.velocity.x)) || centroid_val.velocity.x*cell.velocity.x < 0)
			{
				if (dphi.velocity.x > std::abs(1e-9*cell.velocity.x))
					psi[2] = std::min(psi[2], maxdiff.velocity.x / dphi.velocity.x);
				else
					if (dphi.velocity.x<-std::abs(1e-9*cell.velocity.x))
						psi[2] = std::min(psi[2], mindiff.velocity.x / dphi.velocity.x);
			}
			// yvelocity
			if (std::abs(dphi.velocity.y) > 0.1*std::max(std::abs(maxdiff.velocity.y),std::abs(mindiff.velocity.y)) || centroid_val.velocity.y*cell.velocity.y < 0)
			{
				if (dphi.velocity.y > std::abs(1e-9*cell.velocity.y))
					psi[3] = std::min(psi[3], maxdiff.velocity.y / dphi.velocity.y);
				else
					if (dphi.velocity.y<-std::abs(1e-9*cell.velocity.y))
						psi[3] = std::min(psi[3], mindiff.velocity.y / dphi.velocity.y);
			}
			// tracers
			for (size_t j = 0; j < dphi.tracers.size(); ++j)
			{
			  double cell_tracer = (cell.tracers.begin() + static_cast<int>(j))->second;
			  double diff_tracer = (maxdiff.tracers.begin() + static_cast<int>(j))->second;
			  if (std::abs((dphi.tracers.begin() + static_cast<int>(j))->second) > 0.1*std::max(std::abs(diff_tracer), std::abs((mindiff.tracers.begin() + static_cast<int>(j))->second)) || (centroid_val.tracers.begin() + static_cast<int>(j))->second *cell_tracer < 0)
				{
				  if ((dphi.tracers.begin() + static_cast<int>(j))->second > std::abs(1e-9*cell_tracer))
				    psi[4 + j] = std::min(psi[4 + j], diff_tracer / (dphi.tracers.begin() + static_cast<int>(j))->second);
					else
					  if ((dphi.tracers.begin() + static_cast<int>(j))->second < -std::abs(1e-9 * cell_tracer))
					    psi[4 + j] = std::min(psi[4 + j], (mindiff.tracers.begin() + static_cast<int>(j))->second
								  / (dphi.tracers.begin() + static_cast<int>(j))->second);
				}
			}
		}
		slope.xderivative.density *= psi[0];
		slope.yderivative.density *= psi[0];
		slope.xderivative.pressure *= psi[1];
		slope.yderivative.pressure *= psi[1];
		slope.xderivative.velocity.x *= psi[2];
		slope.yderivative.velocity.x *= psi[2];
		slope.xderivative.velocity.y *= psi[3];
		slope.yderivative.velocity.y *= psi[3];
		size_t counter = 0;
		for (boost::container::flat_map<std::string, double>::iterator it = slope.xderivative.tracers.begin();
			it != slope.xderivative.tracers.end(); ++it)
		{
			it->second *= psi[4 + counter];
			(slope.yderivative.tracers.begin()+static_cast<int>(counter))->second*= psi[4 + counter];
			++counter;
		}
	}

	void shocked_slope_limit(ComputationalCell const& cell, Vector2D const& cm,
		vector<ComputationalCell const*> const& neighbors, vector<const Edge*> const& edge_list,
		Slope  &slope,double diffusecoeff)
	{
		ComputationalCell cmax(cell), cmin(cell);
		// Find maximum values
		for (size_t i = 0; i < neighbors.size(); ++i)
		{
			ComputationalCell const& cell_temp = *neighbors[i];
			cmax.density = std::max(cmax.density, cell_temp.density);
			cmax.pressure = std::max(cmax.pressure, cell_temp.pressure);
			cmax.velocity.x = std::max(cmax.velocity.x, cell_temp.velocity.x);
			cmax.velocity.y = std::max(cmax.velocity.y, cell_temp.velocity.y);
			cmin.density = std::min(cmin.density, cell_temp.density);
			cmin.pressure = std::min(cmin.pressure, cell_temp.pressure);
			cmin.velocity.x = std::min(cmin.velocity.x, cell_temp.velocity.x);
			cmin.velocity.y = std::min(cmin.velocity.y, cell_temp.velocity.y);
			for (boost::container::flat_map<std::string, double>::iterator it = cmax.tracers.begin(); it != cmax.tracers.end(); ++it)
			  it->second = std::max(it->second, safe_retrieve(cell_temp.tracers,it->first));
			for (boost::container::flat_map<std::string, double>::iterator it = cmin.tracers.begin(); it != cmin.tracers.end(); ++it)
			  it->second = std::min(it->second, safe_retrieve(cell_temp.tracers,it->first));
		}
		ComputationalCell maxdiff = cmax - cell, mindiff = cmin - cell;
		// limit the slope
		vector<double> psi(4 + cell.tracers.size(), 1);
		for (size_t i = 0; i<edge_list.size(); ++i)
		{
			ComputationalCell centroid_val = interp(cell, slope, CalcCentroid(*edge_list[i]), cm);
			ComputationalCell dphi = centroid_val - cell;
			// density
			if (std::abs(dphi.density) > 0.1*std::max(std::abs(maxdiff.density), std::abs(mindiff.density)) || centroid_val.density*cell.density < 0)
			{
				if (std::abs(dphi.density) > 1e-9*cell.density)
					psi[0] = std::min(psi[0], std::max(diffusecoeff*(neighbors[i]->density-cell.density)/dphi.density,0.0));
			}
			// pressure
			if (std::abs(dphi.pressure) > 0.1*std::max(std::abs(maxdiff.pressure), std::abs(mindiff.pressure)) || centroid_val.pressure*cell.pressure < 0)
			{
				if (std::abs(dphi.pressure) > 1e-9*cell.pressure)
					psi[1] = std::min(psi[1], std::max(diffusecoeff*(neighbors[i]->pressure - cell.pressure) / dphi.pressure, 0.0));
			}
			// xvelocity
			if (std::abs(dphi.velocity.x) > 0.1*std::max(std::abs(maxdiff.velocity.x), std::abs(mindiff.velocity.x)) || centroid_val.velocity.x*cell.velocity.x < 0)
			{
				if (std::abs(dphi.velocity.x) > 1e-9*cell.velocity.x)
					psi[2] = std::min(psi[2], std::max(diffusecoeff*(neighbors[i]->velocity.x - cell.velocity.x) / dphi.velocity.x, 0.0));
			}
			// yvelocity
			if (std::abs(dphi.velocity.y) > 0.1*std::max(std::abs(maxdiff.velocity.y), std::abs(mindiff.velocity.y)) || centroid_val.velocity.y*cell.velocity.y < 0)
			{
				if (std::abs(dphi.velocity.y) > 1e-9*cell.velocity.y)
					psi[3] = std::min(psi[3], std::max(diffusecoeff*(neighbors[i]->velocity.y - cell.velocity.y) / dphi.velocity.y, 0.0));
			}
			// tracers
			size_t counter = 0;
			for (boost::container::flat_map<std::string, double>::iterator it = dphi.tracers.begin(); it != dphi.tracers.end(); ++it)
			{
			  double cell_tracer = safe_retrieve(cell.tracers,it->first);
			  double diff_tracer = safe_retrieve(maxdiff.tracers,it->first);
			  double centroid_tracer = safe_retrieve(centroid_val.tracers,it->first);
				if (std::abs(it->second) > 0.1*std::max(std::abs(diff_tracer), std::abs(safe_retrieve(mindiff.tracers,it->first))) || centroid_tracer*cell_tracer < 0)
				{
					if (std::abs(it->second) > std::abs(1e-9*cell_tracer))
					  psi[4 + counter] = std::min(psi[4 + counter], std::max(diffusecoeff*(safe_retrieve(neighbors[i]->tracers,it->first)- cell_tracer) / it->second, 0.0));
				}
				++counter;
			}
		}
		slope.xderivative.density *= psi[0];
		slope.yderivative.density *= psi[0];
		slope.xderivative.pressure *= psi[1];
		slope.yderivative.pressure *= psi[1];
		slope.xderivative.velocity.x *= psi[2];
		slope.yderivative.velocity.x *= psi[2];
		slope.xderivative.velocity.y *= psi[3];
		slope.yderivative.velocity.y *= psi[3];
		size_t counter = 0;
		for (boost::container::flat_map<std::string, double>::iterator it = slope.xderivative.tracers.begin();
			it != slope.xderivative.tracers.end(); ++it)
		{
			it->second *= psi[4 + counter];
			safe_retrieve(slope.yderivative.tracers,it->first) *= psi[4 + counter];
			++counter;
		}
	}

	void calc_slope
	(Tessellation const& tess,
	 vector<ComputationalCell> const& cells,
	 size_t cell_index,
	 bool slf,
	 double shockratio,
	 double diffusecoeff,
	 double pressure_ratio,
	 EquationOfState const& eos,
	 boost::container::flat_map<size_t, ComputationalCell> const& ghost_cells,
	 const vector<string>& flat_tracers,
	 Slope &naive_slope_,
	Slope & res,
		Slope & temp1,
		ComputationalCell &temp2,
		ComputationalCell &temp3, 
		ComputationalCell &temp4, 
		ComputationalCell &temp5,
		vector<const Edge *> const& edge_list)
{
	vector<Vector2D> neighbor_mesh_list;
	GetNeighborMesh(tess, edge_list, cell_index,neighbor_mesh_list);
	vector<Vector2D> neighbor_cm_list;
	GetNeighborCM(tess, edge_list, cell_index,neighbor_cm_list);
	vector<ComputationalCell const* > neighbor_list = GetNeighborCells(edge_list, cell_index,cells,ghost_cells,static_cast<size_t>(
		tess.GetPointNo()));

	ComputationalCell const& cell = cells[cell_index];
	calc_naive_slope(cell, tess.GetMeshPoint(static_cast<int>(cell_index)), tess.GetCellCM(static_cast<int>(cell_index)),
		tess.GetVolume(static_cast<int>(cell_index)), neighbor_list, neighbor_mesh_list, neighbor_cm_list, edge_list,
		res,temp1);

	naive_slope_ = res;

	for(size_t i=0;i<flat_tracers.size();++i)
	{
	  res.xderivative.tracers[flat_tracers[i]] = 0;
	  res.yderivative.tracers[flat_tracers[i]] = 0;
	}

	if (slf)
	{
		if (!is_shock(res, tess.GetWidth(static_cast<int>(cell_index)), shockratio, cell, neighbor_list, pressure_ratio,
			      eos.dp2c(cell.density,cell.pressure,cell.tracers)))
		{
			slope_limit(cell, tess.GetCellCM(static_cast<int>(cell_index)), neighbor_list, edge_list, res,temp2,temp3,
				temp4,temp5);
		}
		else
		{
			shocked_slope_limit(cell, tess.GetCellCM(static_cast<int>(cell_index)), neighbor_list, edge_list, res, diffusecoeff);
		}
	}
}


}

ComputationalCell LinearGaussImproved::Interp(ComputationalCell const& cell, size_t cell_index, Vector2D const& cm,
	Vector2D const& target)const
{
	return interp(cell, rslopes_[cell_index], target, cm);
}

LinearGaussImproved::LinearGaussImproved
(EquationOfState const& eos,
 GhostPointGenerator const& ghost,
 bool slf,
 double delta_v,
 double theta,
 double delta_P,
 const vector<string>& flat_tracers): 
  eos_(eos), 
  ghost_(ghost),
  rslopes_(),
  naive_rslopes_(),
  slf_(slf),
  shockratio_(delta_v),
  diffusecoeff_(theta),
  pressure_ratio_(delta_P),
  flat_tracers_(flat_tracers) {}

#ifdef RICH_MPI
namespace
{
	void exchange_ghost_slopes(Tessellation const& tess,vector<Slope> & slopes)
	{
		MPI_exchange_data(tess, slopes, true);
	}
}
#endif//RICH_MPI

void LinearGaussImproved::operator() (const Tessellation& tess,
	const vector<ComputationalCell>& cells,double time, vector<pair<ComputationalCell, ComputationalCell> > &res) const
{
	const size_t CellNumber = static_cast<size_t>(tess.GetPointNo());
	vector<int> boundaryedges;
	// Get ghost points
	boost::container::flat_map<size_t,ComputationalCell> ghost_cells = ghost_.operator()(tess,cells,time);
	// Prepare slopes
	rslopes_.resize(CellNumber,Slope(cells[0],cells[0]));
	naive_rslopes_.resize(CellNumber);
	Slope temp1(cells[0],cells[0]);
	ComputationalCell temp2(cells[0]);
	ComputationalCell temp3(cells[0]);
	ComputationalCell temp4(cells[0]);
	ComputationalCell temp5(cells[0]);
	for (size_t i = 0; i < CellNumber; ++i)
	{
		vector<int> const& edge_index = tess.GetCellEdges(static_cast<int>(i));
		vector<const Edge *> edge_list;
		GetEdgeList(tess, edge_index, edge_list);
		calc_slope(tess, cells, i, slf_, shockratio_, diffusecoeff_, pressure_ratio_, eos_, ghost_cells,
			flat_tracers_, naive_rslopes_[i], rslopes_[i], temp1, temp2, temp3, temp4, temp5,edge_list);
		const size_t nloop = edge_index.size();
		for (size_t j = 0; j < nloop; ++j)
		{
			Edge const& edge = *edge_list[j];
			if (edge.neighbors.first == static_cast<int>(i))
			{
				ReplaceComputationalCell(res[static_cast<size_t>(edge_index[j])].first, 
					cells[i]);
				interp2(res[static_cast<size_t>(edge_index[j])].first,
					rslopes_[i],CalcCentroid(edge), tess.GetCellCM(static_cast<int>(i)));
				if (edge.neighbors.second > static_cast<int>(CellNumber))
					boundaryedges.push_back(edge_index[j]);
			}
			else
			{
				ReplaceComputationalCell(res[static_cast<size_t>(edge_index[j])].second,
					cells[i]);
				interp2(res[static_cast<size_t>(edge_index[j])].second,
					rslopes_[i], CalcCentroid(edge), tess.GetCellCM(static_cast<int>(i)));
				if (edge.neighbors.first > static_cast<int>(CellNumber))
					boundaryedges.push_back(edge_index[j]);
			}
		}
	}
#ifdef RICH_MPI
	// communicate ghost slopes
	exchange_ghost_slopes(tess, rslopes_);
#endif //RICH_MPI
	// Interpolate the boundary edges
	for (size_t i = 0; i < boundaryedges.size(); ++i)
	{
		Edge const& edge = tess.GetEdge(boundaryedges[i]);
		if (edge.neighbors.first > static_cast<int>(CellNumber))
		{
#ifdef RICH_MPI
			if (tess.GetOriginalIndex(edge.neighbors.second) != tess.GetOriginalIndex(edge.neighbors.first))
			{
				res[static_cast<size_t>(boundaryedges[i])].first = cells[static_cast<size_t>(edge.neighbors.first)];
				interp2(res[static_cast<size_t>(boundaryedges[i])].first,
					rslopes_[static_cast<size_t>(edge.neighbors.first)], CalcCentroid(edge), tess.GetCellCM(edge.neighbors.first));
			}
			else
			{
				res[static_cast<size_t>(boundaryedges[i])].first = safe_retrieve(ghost_cells, static_cast<size_t>(edge.neighbors.first));
				res[static_cast<size_t>(boundaryedges[i])].first = interp(res[static_cast<size_t>(boundaryedges[i])].first,
					ghost_.GetGhostGradient(tess, cells, rslopes_, static_cast<size_t>(
					edge.neighbors.first), time, edge), CalcCentroid(edge), tess.GetCellCM(edge.neighbors.first));
			}
#else
			res[static_cast<size_t>(boundaryedges[i])].first = safe_retrieve(ghost_cells, static_cast<size_t>(edge.neighbors.first));
			res[static_cast<size_t>(boundaryedges[i])].first = interp(res[static_cast<size_t>(boundaryedges[i])].first,
				ghost_.GetGhostGradient(tess, cells, rslopes_, static_cast<size_t>(
				edge.neighbors.first), time, edge), CalcCentroid(edge), tess.GetCellCM(edge.neighbors.first));
#endif //RICH_MPI
		}
		else
		{
#ifdef RICH_MPI
			if (tess.GetOriginalIndex(edge.neighbors.second) != tess.GetOriginalIndex(edge.neighbors.first))
			{
				res[static_cast<size_t>(boundaryedges[i])].second = cells[static_cast<size_t>(edge.neighbors.second)];
				interp2(res[static_cast<size_t>(boundaryedges[i])].second,
					rslopes_[static_cast<size_t>(edge.neighbors.second)], CalcCentroid(edge), tess.GetCellCM(edge.neighbors.second));
			}
			else
			{
				res[static_cast<size_t>(boundaryedges[i])].second = safe_retrieve(ghost_cells, static_cast<size_t>(edge.neighbors.second));
				res[static_cast<size_t>(boundaryedges[i])].second = interp(res[static_cast<size_t>(boundaryedges[i])].second,
					ghost_.GetGhostGradient(tess, cells, rslopes_, static_cast<size_t>(
						edge.neighbors.second), time, edge), CalcCentroid(edge), tess.GetCellCM(edge.neighbors.second));
			}
#else
			res[static_cast<size_t>(boundaryedges[i])].second = safe_retrieve(ghost_cells, static_cast<size_t>(edge.neighbors.second));
			res[static_cast<size_t>(boundaryedges[i])].second = interp(res[static_cast<size_t>(boundaryedges[i])].second,
				ghost_.GetGhostGradient(tess, cells, rslopes_, static_cast<size_t>(
					edge.neighbors.second), time, edge), CalcCentroid(edge), tess.GetCellCM(edge.neighbors.second));
#endif //RICH_MPI
		}
	}
}


vector<Slope>& LinearGaussImproved::GetSlopes(void)const
{
	return rslopes_;
}

vector<Slope>& LinearGaussImproved::GetSlopesUnlimited(void)const
{
	return naive_rslopes_;
}

