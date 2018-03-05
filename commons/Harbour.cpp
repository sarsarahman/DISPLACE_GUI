// --------------------------------------------------------------------------
// DISPLACE: DYNAMIC INDIVIDUAL VESSEL-BASED SPATIAL PLANNING
// AND EFFORT DISPLACEMENT
// Copyright (c) 2012, 2013, 2014, 2015, 2016, 2017 Francois Bastardie <fba@aqua.dtu.dk>

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

#include "Harbour.h"
#include <helpers.h>

Harbour::Harbour(types::NodeId idx,
double xval,
double yval,
int _harbour,
int _code_area,
int _code_landscape,
double _wind,
double _sst,
double _salinity,
double _Nitrogen,
double _Phosphorus,
double _Oxygen,
double _DissolvedCarbon,
double _benthos_biomass,
double _benthos_number,
double _benthos_meanweight,
double _benthos_biomass_K,
double _benthos_number_K,
int nbpops,
int nbbenthospops,
int nbszgroups,
string _name,
//multimap<string, double> _mean_fish_price_per_met_per_pop,
multimap<int, double> _mean_fish_price_per_pop_per_cat,
map<int, double> _fuelprice,
vector<types::NodeId> _usual_fgrounds,
vector<double> _freq_usual_fgrounds)
: Node(idx, xval, yval,  _harbour, _code_area, _code_landscape, _wind,_sst,_salinity,
       _Nitrogen, _Phosphorus, _Oxygen, _DissolvedCarbon,
       _benthos_biomass, _benthos_number,_benthos_meanweight, _benthos_biomass_K, _benthos_number_K, nbpops, nbbenthospops, nbszgroups)
{
	name=_name;
	//mean_fish_price_per_met_per_pop= _mean_fish_price_per_met_per_pop;
	mean_fish_price_per_pop_per_cat=_mean_fish_price_per_pop_per_cat;
	fuelprice= _fuelprice;
    usual_fgrounds=_usual_fgrounds;
    freq_usual_fgrounds=_freq_usual_fgrounds;

}


Harbour::Harbour(string _name)
    : Node(), name(_name),
      fuelprice(), mean_fish_price_per_pop_per_cat()
{
}


Harbour::Harbour()
{
}

Harbour::Harbour(types::NodeId idx, double xval, double yval, int _harbour)
    : Node(idx, xval, yval,  _harbour, 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0)
{

}


Harbour::~Harbour()
{
	//dtor
}


string Harbour::get_name() const
{
	return(this->name);
}


/*
double Harbour::get_prices(string met, int pop)
{
	double a_price;
// search into the multimap 'prices in met/pop'
	vector <double> spe_prices = find_entries_s_d(mean_fish_price_per_met_per_pop, met);
	//print_d(spe_prices);
	if(!spe_prices.empty())
	{
        dout(cout  << "hi there..." << endl);
		a_price=spe_prices.at(pop);
	}
	else
	{
        dout(cout  << "no price info for this met on this harbour..." << endl);
		// then, replace by taking another met eg the first met as it comes...
		multimap<string, double>::iterator it = mean_fish_price_per_met_per_pop.begin(); it++; // why it.begin() is void?
        dout(cout  << "instead, get met " << it->first << endl);
		vector <double> spe_prices = find_entries_s_d(mean_fish_price_per_met_per_pop,
									 it->first);
		a_price=spe_prices.at(pop);
	}

	return(a_price);
}
*/

double Harbour::get_prices_per_cat(int pop, int cat)
{
	double a_price;
	// search into the multimap 'prices in pop/cat'
	vector <double> spe_prices = find_entries_i_d(mean_fish_price_per_pop_per_cat, pop);
	//print_d(spe_prices);
	if(!spe_prices.empty())
	{
       // dout(cout  << "fish price on harbour "<< this->get_name() << " and pop " << pop << endl);
								 // caution to relative vector index here....
       // dout(cout  << "...for this cat "<< cat << "is:" << endl);
        a_price=spe_prices.at(cat);
       // dout(cout  << a_price << endl);

	}
	else
	{
        dout(cout  << "no price info for this pop on this harbour..." << endl);
		// then, replace by taking another met eg the first met as it comes...
								 //it++; // why it.begin() is void?
		multimap<int, double>::iterator it = mean_fish_price_per_pop_per_cat.begin();
        dout(cout  << "instead, get pop " << it->first << endl);
		vector <double> spe_prices = find_entries_i_d(mean_fish_price_per_pop_per_cat,
			it->first);
		a_price=spe_prices.at(cat);
	}

	return(a_price);
}


double Harbour::get_fuelprices(int vsize)
{
	// search into the map 'prices per vsize'
	double a_fuelprice= fuelprice.find(vsize)->second;

    return(a_fuelprice);
}


const vector<types::NodeId> &Harbour::get_usual_fgrounds() const
{
    return(usual_fgrounds);
}


const vector<double> &Harbour::get_freq_usual_fgrounds() const
{
    return(freq_usual_fgrounds);
}

void Harbour::set_usual_fgrounds(const vector<types::NodeId> &_usual_fgrounds)
{
    usual_fgrounds=_usual_fgrounds;
}


void Harbour::set_freq_usual_fgrounds(const vector <double> &_freq_usual_fgrounds)
{
    freq_usual_fgrounds=_freq_usual_fgrounds;
}

vector<types::NodeId> Harbour::get_usual_fgrounds_this_met(int met)
{
    auto usual_fgrounds_this_met  = find_entries( usual_fgrounds_per_met, met );

    return(usual_fgrounds_this_met);
}


vector<double> Harbour::get_freq_usual_fgrounds_this_met(int met)
{
     vector<double> freq_usual_fgrounds_this_met = find_entries_i_d( freq_usual_fgrounds_per_met, met );

    return(freq_usual_fgrounds_this_met);
}

void Harbour::set_usual_fgrounds_per_met(multimap<int, types::NodeId> _usual_fgrounds_per_met)
{
    usual_fgrounds_per_met=_usual_fgrounds_per_met;
}


void Harbour::set_freq_usual_fgrounds_per_met(multimap<int, double> _freq_usual_fgrounds_per_met)
{
    freq_usual_fgrounds_per_met=_freq_usual_fgrounds_per_met;
}


const multimap<types::NodeId, int> &Harbour::get_usual_metiers() const
{
    return(usual_metiers);
}


const multimap<int, double> &Harbour::get_freq_usual_metiers() const
{
    return(freq_usual_metiers);
}

void Harbour::set_usual_metiers(multimap<types::NodeId, int> _usual_metiers)
{
 usual_metiers=_usual_metiers;
}


void Harbour::set_freq_usual_metiers(multimap<int, double> _freq_usual_metiers)
{
 freq_usual_metiers=_freq_usual_metiers;
}


