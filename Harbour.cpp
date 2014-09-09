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



#include "Harbour.h"

//#define VERBOSE
#ifdef VERBOSE
#define dout cout
#else
#define dout 0 && cout
#endif


Harbour::Harbour(int idx,
                 double xval,
                 double yval,
                 int _harbour,
                 int _code_area,
                 int _code_landscape,
                 int nbpops,
                 int nbszgroups,
                 string _name,
                 //multimap<string, double> _mean_fish_price_per_met_per_pop,
                 multimap<int, double> _mean_fish_price_per_pop_per_cat,
                 map<string, double> _fuelprice)
    : Node(idx, xval, yval,  _harbour, _code_area, _code_landscape, nbpops, nbszgroups)
{
    name=_name;
    //mean_fish_price_per_met_per_pop= _mean_fish_price_per_met_per_pop;
    mean_fish_price_per_pop_per_cat=_mean_fish_price_per_pop_per_cat;
    fuelprice= _fuelprice;


}

Harbour::Harbour(string name)
{
}

Harbour::Harbour()
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
        dout << "hi there..." << endl;
        a_price=spe_prices.at(pop);
    }
    else
    {
        dout << "no price info for this met on this harbour..." << endl;
        // then, replace by taking another met eg the first met as it comes...
        multimap<string, double>::iterator it = mean_fish_price_per_met_per_pop.begin(); it++; // why it.begin() is void?
        dout << "instead, get met " << it->first << endl;
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
        dout << "harbour "<< this->get_name() << endl;
        a_price=spe_prices.at(cat); // caution to relative vector index here....

    }
    else
    {
        dout << "no price info for this pop on this harbour..." << endl;
        // then, replace by taking another met eg the first met as it comes...
        multimap<int, double>::iterator it = mean_fish_price_per_pop_per_cat.begin(); //it++; // why it.begin() is void?
        dout << "instead, get pop " << it->first << endl;
        vector <double> spe_prices = find_entries_i_d(mean_fish_price_per_pop_per_cat,
                                     it->first);
        a_price=spe_prices.at(cat);
    }

    return(a_price);
}



double Harbour::get_fuelprices(string vsize)
{
// search into the map 'prices per vsize'
    double a_fuelprice= fuelprice.find(vsize)->second;

    return(a_fuelprice);
}




