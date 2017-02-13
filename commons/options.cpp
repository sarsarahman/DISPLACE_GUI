// --------------------------------------------------------------------------
// DISPLACE: DYNAMIC INDIVIDUAL VESSEL-BASED SPATIAL PLANNING
// AND EFFORT DISPLACEMENT
// Copyright (c) 2012, 2013, 2014, 2015, 2016 Francois Bastardie <fba@aqua.dtu.dk>

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

#include "options.h"
using namespace Options;

#define O(MAP,NAME) MAP.insert(std::make_pair(#NAME,NAME))
#define M(MAP,NAME,ENUM) MAP.insert(std::make_pair(NAME,ENUM))

DynAllocOptions::DynAllocOptions()
{
    M(mMap,"baseline", alloc_baseline);
    O(mMap,focus_on_high_profit_grounds);
    O(mMap,TACs);
    O(mMap,discard_ban);
    O(mMap,LTMP);
    O(mMap,FMSY);
    M(mMap,"area_closure", area_closure);
    M(mMap,"area_monthly_closure", area_monthly_closure);
    O(mMap,fuelprice_plus20percent);
    O(mMap,reduced_speed_10percent);
    O(mMap,reduced_speed_20percent);
    O(mMap,reduced_speed_30percent);
    O(mMap,closer_grounds);
    O(mMap,closer_port);
    O(mMap,focus_on_high_previous_cpue);
    O(mMap,shared_harbour_knowledge);
    O(mMap,twoFoldTACconstr);
    O(mMap,fishing_credits);

    for (int i = 0; i < n(); ++i)
        mOptions[i] = false;
}


PopSceOptions::PopSceOptions()
{
    M(mMap,"baseline", pop_baseline);
    O(mMap,use_SMS);
    O(mMap,use_SMS_single);
    O(mMap,diffuseN);
    O(mMap,white_noise_on_avai);
    O(mMap,with_stochast_oth_land);
    O(mMap,stop_mig_35065);

    for (int i = 0; i < n(); ++i)
        mOptions[i] = false;
}

void ClosureOptions::update()
{
    auto &l = option(banned_metiers);
    for (auto &n : l) {
        while (mBannedMetiers.size() <= (size_t)n)
            mBannedMetiers.push_back(false);
        mBannedMetiers[n] = true;
    }
}
