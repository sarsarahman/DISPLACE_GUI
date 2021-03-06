/* --------------------------------------------------------------------------
 * DISPLACE: DYNAMIC INDIVIDUAL VESSEL-BASED SPATIAL PLANNING
 * AND EFFORT DISPLACEMENT
 * Copyright (c) 2012, 2013, 2014 Francois Bastardie <fba@aqua.dtu.dk>
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2 of the License, or
 *    (at your option) any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License along
 *    with this program; if not, write to the Free Software Foundation, Inc.,
 *    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * --------------------------------------------------------------------------
 */
#ifndef FISHFARMDATA_H
#define FISHFARMDATA_H

#include <Fishfarm.h>
#include <QList>

#include <memory>

class FishfarmStats;

class FishfarmData
{

    int mIdx;
    int mFishfarmId;

public:
    std::shared_ptr<Fishfarm> mFishfarm; /* This is crap too. */

    FishfarmData(std::shared_ptr<Fishfarm> fishfarm);

    int getId() const { return mFishfarmId; }


    double getX() const;
    double getY() const;

    int getIdx() const;
    void setIdx(int idx);


private:
    double mX;
    double mY;
  };

class FishfarmStats {
public:
    double x;
    double y;

};

#endif // FISHFARMDATA_H
