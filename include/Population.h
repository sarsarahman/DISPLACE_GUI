// --------------------------------------------------------------------------
// DISPLACE: DYNAMIC INDIVIDUAL VESSEL-BASED SPATIAL PLANNING
// AND EFFORT DISPLACEMENT
// Copyright (c) 2012, 2013, 2014 Francois Bastardie <fba@aqua.dtu.dk>

//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// --------------------------------------------------------------------------

#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "myutils.h"
#include "Node.h"
#include "Tac.h"

class Population
{
	public:
        Population(int name,
            double _avai0_beta,
            double _avai2_beta,
            double _avai3_beta,
            double _avai5_beta,
            double _avai7_beta,
            const vector<int> &selected_szgroups,
            const vector<double> &init_tot_N_at_szgroup,
            const vector<double> &init_fecundity_at_szgroup,
            vector<double> init_weight_at_szgroup,
            const vector<int> &init_comcat_at_szgroup,
            const vector<double> &init_maturity_at_szgroup,
            vector<double> init_M_at_szgroup,
            const vector<double> &init_proprecru_at_szgroup,
            const vector<double> &param_sr,
            const multimap<int,int> &lst_idx_nodes_per_pop,
            const multimap<int,double> &full_spatial_availability,
            const map<int,double> &oth_land,
            const map<string,double> &relative_stability_key,
            const vector< vector<double> > &percent_szgroup_per_age_matrix,
            const vector< vector<double> > &percent_age_perszgroup_matrix,
            const vector< vector<double> > &growth_transition_matrix,
            const vector<Node* > &nodes,
            const vector<double> &fbar_ages_min_max,
            const vector<double> &init_tac,
            double tac_percent_simulated,
            double landings_so_far,
            double a_calib_cpue_multiplier,
            double a_calib_weight_at_szgroup);

		Population();
		virtual ~Population();
								 // Copy constructor
		int get_name() const;
        const vector<Node *> &get_list_nodes() const;
        const vector<int> &get_selected_szgroups() const;
		double get_avai0_beta() const;
		double get_avai2_beta() const;
		double get_avai3_beta() const;
		double get_avai5_beta() const;
		double get_avai7_beta() const;
		double get_cpue_multiplier() const;
        const vector<double>& get_tot_N_at_szgroup() const;
        const vector<double>& get_tot_N_at_szgroup_just_after_redistribution() const;
        const vector<double>& get_tot_N_at_szgroup_month_minus_1() const;
        const vector<double>& get_tot_N_at_szgroup_year_minus_1() const;
        const vector<double>& get_tot_N_at_age() const;
        const vector<double>& get_tot_N_at_age_last_quarter() const;
        const vector<double>& get_tot_F_at_age() const;
        const vector<double>& get_tot_F_at_age_last_quarter() const;
        const vector<double>& get_tot_M_at_age() const;
        const vector<double>& get_tot_W_at_age() const;
        const vector<double>& get_fbar_ages_min_max() const;
        const vector<double>& get_maturity_at_szgroup() const;
        const vector<double>& get_fecundity_at_szgroup() const;
        const vector<double>& get_weight_at_szgroup() const;
		vector<int> get_comcat_at_szgroup() const;
        const vector<double>& get_M_at_szgroup() const;
        const vector<double>& get_proprecru_at_szgroup() const;
		double get_SSB() const;
        const vector<double>& get_param_sr() const;
		vector< vector <double> >get_growth_transition_matrix() const;
		vector< vector <double> >get_percent_szgroup_per_age_matrix() const;
		vector< vector <double> >get_percent_age_per_szgroup_matrix() const;
		multimap<int,double> get_full_spatial_availability() const;
		map<int,double> get_oth_land() const;
		double get_oth_land_multiplier() const;
		double get_catchability() const;
		Tac* get_tac() const;
		double get_landings_so_far() const;
		void set_selected_szgroups(vector<int> selected_szgroups);
		void set_avai0_beta(double _avai0_beta);
		void set_avai2_beta(double _avai2_beta);
		void set_avai3_beta(double _avai3_beta);
		void set_avai5_beta(double _avai5_beta);
		void set_avai7_beta(double _avai7_beta);
		void set_cpue_multiplier(double _cpue_multiplier);
        void set_tot_N_at_szgroup(const vector<double>& _N_at_szgroup);
        void set_tot_N_at_szgroup_just_after_redistribution(const vector<double>& _N_at_szgroup_just_after_redistribution);
        void set_tot_N_at_szgroup_month_minus_1(const vector<double>& _N_at_szgroup_month_minus_1);
        void set_tot_N_at_szgroup_year_minus_1(const vector<double>& _N_at_szgroup_year_minus_1);
        void set_tot_N_at_age(const vector<double>& _N_at_age);
        void set_tot_N_at_age_last_quarter(const vector<double>& _N_at_age_last_quarter);
        void set_tot_F_at_age(const vector<double>& _F_at_age);
        void set_tot_F_at_age_last_quarter(const vector<double>& _tot_F_at_age_last_quarter);
        void set_tot_M_at_age(const vector<double>& _M_at_age);
        void set_tot_W_at_age(const vector<double>& _W_at_age);
        void set_maturity_at_szgroup(const vector<double>& _maturity_at_szgroup);
        void set_fecundity_at_szgroup(const vector<double>& _fecundity_at_szgroup);
        void set_weight_at_szgroup(const vector<double> &_weight_at_szgroup);
		void set_comcat_at_szgroup(vector <int> _comcat_at_szgroup);
        void set_M_at_szgroup(const vector<double>& _M_at_szgroup);
        void set_proprecru_at_szgroup(const vector<double>& _proprecru_at_szgroup);
		void set_SSB(double _SSB);
								 // in weight...
		void set_landings_so_far(double _landings_so_far);
        void set_param_sr(const vector<double>& _param_sr);
		void set_full_spatial_availability(multimap<int,double> _full_spatial_availability);
		void set_oth_land(map<int,double> _oth_land);
		void set_oth_land_multiplier(double _a_multiplier);
		void set_list_nodes(vector<Node* > _list_nodes);
		void distribute_N();	 // split tot N among nodes
		void aggregate_N();		 // aggregate from nodes
								 // do a cumul for F
		void compute_tot_N_and_F_and_M_and_W_at_age();
		double compute_fbar();
		void compute_TAC();
								 // clear the cumul
		void clear_tot_F_at_age();
		void do_growth();		 // apply the size transition matrix
								 // stock-recruitment relationship via fecundity_at_szgroup
		void add_recruits_from_eggs();
								 // stock-recruitment relationship via fecundity_at_szgroup
		void add_recruits_from_SR();
								 // monthly M on the entire pop (i.e. not spatially disaggregated)
		void apply_natural_mortality();
		void export_popdyn_N(ofstream& popdyn_N, int tstep);
		void export_popdyn_F(ofstream& popdyn_F, int tstep);
		void export_popdyn_annual_indic(ofstream& popdyn_annual_indic, int tstep);

	protected:
	private:
		int name;
		int idx_pop;
		int nb_szgroups;
		vector<int> selected_szgroups;
		double avai0_beta;		 // actually, only one value here...
		double avai2_beta;		 //
		double avai3_beta;		 //
		double avai5_beta;		 //
		double avai7_beta;		 //
		double cpue_multiplier;	 //
								 //
		vector<double> tot_N_at_szgroup;
								 //
		vector<double> tot_N_at_szgroup_just_after_redistribution;
								 //
		vector<double> tot_N_at_szgroup_month_minus_1;
								 //
		vector<double> tot_N_at_szgroup_year_minus_1;
								 //
		vector<double> tot_N_at_age;
								 //
		vector<double> tot_F_at_age;
		vector<double> tot_F_at_age_last_quarter;
								 //
		vector<double> tot_M_at_age;
								 //
		vector<double> tot_W_at_age;
								 //
		vector<double> weight_at_szgroup;
								 //
		vector<int> comcat_at_szgroup;
								 //
		vector<double> maturity_at_szgroup;
								 //
		vector<double> fecundity_at_szgroup;
								 //
		vector<double> M_at_szgroup;
								 //
		vector<double> proprecru_at_szgroup;
		double SSB;				 //
		vector<double> param_sr; //
								 // area distribution
		vector<Node* > list_nodes;
								 // for sharing the total N by node
		multimap<int,double> full_spatial_availability;
		map<int,double> oth_land;// for accounting for the depletion from non-described activities
		vector< vector<double> > percent_szgroup_per_age_matrix;
		vector< vector<double> > percent_age_per_szgroup_matrix;
		vector< vector<double> > growth_transition_matrix;
		double catchability;	 // scaling factor in the catch equation...
		vector<double> fbar_ages_min_max;
		Tac *tac;
		double landings_so_far;	 // global landings, reinit each start of the year...
								 // init at 1. Will change according to the next TAC.
		double oth_land_multiplier;

};
#endif							 // POPULATION_H
