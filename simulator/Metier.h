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



#ifndef METIER_H
#define METIER_H

#include <string>
#include <iostream>
#include <list>
#include <vector>
#include <map>

using namespace std;

class Metier
{
  	private:
	    int name;
        int type;
        double target_factor; //NOT IN USE...
	    vector<double> selectivity;
	    vector<double> discards;
	    vector<double> betas_per_pop;
        double gear_width_a;
        double gear_width_b;
        string gear_width_model;
        multimap<int, double> loss_after_1_passage;

    public:
        Metier();
        Metier(int name,
               int type,
               vector<double> selectivity,
               vector<double> discards,
               vector<double> metier_betas_per_pop,
               double gear_width_a,
               double gear_width_b,
               string gear_width_model,
               multimap<int, double> loss_after_1_passage
               );
        ~Metier();
        int get_name();
        int get_metier_type();
        double get_target_factor();
        vector<double> get_selectivity_ogive();
        vector<double> get_discards_ogive();
        double  get_gear_width_a();
        double  get_gear_width_b();
        string  get_gear_width_model();
        multimap<int, double>  get_loss_after_1_passage();
        vector<double> get_betas_per_pop();
        void set_betas_per_pop(vector<double> metier_betas_per_pop);


};

#endif // METIER_H
