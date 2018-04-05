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

#include <commons_global.h>

#include <stdio.h>
#include <stdlib.h>
#include <helpers.h>

#include <string>
#include <vector>

#include <Node.h>


bool COMMONSSHARED_EXPORT diffuse_Nitrogen_in_every_directions(vector<Node*>&list_of_nodes, adjacency_map_t& adjacency_map, double coeff);
bool COMMONSSHARED_EXPORT diffuse_Nitrogen_with_gradients(vector<Node*>&list_of_nodes, adjacency_map_t& adjacency_map);

