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
#ifndef NATIONALITYDATA_H
#define NATIONALITYDATA_H

#include <QString>
#include <QVector>

class NationData
{
    QString mName;
public:
    NationData();
    explicit NationData(QString n) : mName(n) {}

    QString getName() const;
    void setName(const QString &value);
};

class NationStats {
public:
    NationStats()
        : szGroups()
    {
    }

    int numTrips = 0;
    double mTotCatches = 0.0;
    double mTotDiscards = 0.0;
    double mRevenues = 0.0;
    double mExRevenues = 0.0;
    double mTimeAtSea = 0.0;
    double mGav = 0.0;
    double mVpuf = 0.0;
    double mSweptArea = 0.0;
    double mRevenuePerSweptArea = 0.0;
    double cumVpuf=0.0;
    double cumRevenuePerSweptArea=0.0;

    double GVA=0.0;
    double cumGVAPerRevenue = 0.0;
    double GVAPerRevenue = 0.0;
    double LabourSurplus = 0.0;
    double GrossProfit = 0.0;
    double NetProfit  = 0.0;
    double NetProfitMargin = 0.0;
    double RoFTA = 0.0;
    double GVAPerFTE = 0.0;
    double BER = 0.0;
    double CRBER = 0.0;
    double NetPresentValue = 0.0;

    double cumNetProfitMargin= 0.0;
    double cumGVAPerFTE= 0.0;
    double cumRoFTA= 0.0;
    double cumBER= 0.0;
    double cumCRBER= 0.0;


    QVector<double> szGroups;
};

#endif // NATIONALITYDATA_H
