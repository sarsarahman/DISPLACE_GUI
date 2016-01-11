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

#include "config.h"

#include <exceptions.h>

#include <readdata.h>
#include <vector>

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QtAlgorithms>

#define NBSZGROUP 14

Config::Config()
    : nbpops(0),
      nbbenthospops(0),
      szGroups(0),
      m_implicit_pops(),
      m_calib_oth_landings(),
      m_calib_weight_at_szgroup(),
      m_calib_cpue_multiplier()
{
}

const QList<int> &Config::implicit_pops() const
{
    return m_implicit_pops;
}

void Config::setImplicit_pops(const QList<int> &implicit_pops)
{
    m_implicit_pops = implicit_pops;
}

const QList<double> &Config::calib_oth_landings() const
{
    return m_calib_oth_landings;
}

void Config::setCalib_oth_landings(const QList<double> &calib_oth_landings)
{
    m_calib_oth_landings = calib_oth_landings;
}

const QList<double> &Config::calib_weight_at_szgroup() const
{
    return m_calib_weight_at_szgroup;
}

void Config::setCalib_weight_at_szgroup(const QList<double> &calib_weight_at_szgroup)
{
    m_calib_weight_at_szgroup = calib_weight_at_szgroup;
}

const QList<double> &Config::calib_cpue_multiplier() const
{
    return m_calib_cpue_multiplier;
}

void Config::setCalib_cpue_multiplier(const QList<double> &calib_cpue_multiplier)
{
    m_calib_cpue_multiplier = calib_cpue_multiplier;
}

int Config::getNbpops() const
{
    return nbpops;
}

void Config::setNbpops(int value)
{
    nbpops = value;
}

int Config::getNbbenthospops() const
{
    return nbbenthospops;
}

void Config::setNbbenthospops(int value)
{
    nbbenthospops = value;
}


int Config::getSzGroups() const
{
    return szGroups;
}

void Config::setSzGroups(int value)
{
    szGroups = value;
}

bool Config::save(QString path, QString modelname, QString outputname, QString *error)
{
    Q_UNUSED(outputname);

    QString realpath = path + "/simusspe_" + modelname +"/config.dat";
    QFile file (realpath);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        if (error) *error = file.errorString();
        return false;
    }

    QTextStream stream (&file);

    stream << "# nbpops"<< endl << nbpops << endl;

    stream << "# nbbenthospops"<< endl << nbbenthospops << endl;

    stream <<"# implicit stocks"<< endl;
    foreach (int a, m_implicit_pops)
        stream << a << " ";
    stream << endl;

    stream <<"# calib the other landings per stock"<< endl;
    foreach (double a, m_calib_oth_landings)
        stream << a << " ";
    stream << endl;

    stream <<"# calib weight-at-szgroup per stock"<< endl;
    foreach (double a, m_calib_weight_at_szgroup)
        stream << a << " ";
    stream << endl;

    stream <<"# calib the cpue multiplier per stock"<< endl;
    foreach (double a, m_calib_cpue_multiplier)
        stream << a << " ";
    stream << endl;

    stream <<"# Interesting harbours"<< endl;
    foreach (int a, m_interesting_harbours)
        stream << a << " ";
    stream << endl;

    file.close();
    return true;
}

Config Config::readFromFile(QString path, QString modelname, QString outputname)
{
    Q_UNUSED(outputname);

    Config config;

    config.szGroups = NBSZGROUP;

    std::vector <int> implicit_pops;
    std::vector <int> implicit_harbours;
    std::vector <double> calib_oth_landings;
    std::vector <double> calib_weight_at_szgroup;
    std::vector <double> calib_cpue_multiplier;

    if (read_config_file (
        path.toStdString(),
        modelname.toStdString(),
        config.nbpops,
        config.nbbenthospops,
        implicit_pops,
        calib_oth_landings,
        calib_weight_at_szgroup,
        calib_cpue_multiplier,
                implicit_harbours
        ) <0 )

        throw DisplaceException(QString(QObject::tr("Cannot load configuration file: %1"))
                                .arg(::getLastErrorMessage().c_str()));


    for (std::vector<int>::iterator it = implicit_pops.begin(); it != implicit_pops.end(); ++it)
        config.m_implicit_pops.push_back(*it);
    for (std::vector<double>::iterator it = calib_oth_landings.begin(); it != calib_oth_landings.end(); ++it)
        config.m_calib_oth_landings.push_back(*it);
    for (std::vector<double>::iterator it = calib_weight_at_szgroup.begin(); it != calib_weight_at_szgroup.end(); ++it)
        config.m_calib_weight_at_szgroup.push_back(*it);
    for (std::vector<double>::iterator it = calib_cpue_multiplier.begin(); it != calib_cpue_multiplier.end(); ++it)
        config.m_calib_cpue_multiplier.push_back(*it);
    for (std::vector<int>::iterator it = implicit_harbours.begin(); it != implicit_harbours.end(); ++it)
        config.m_interesting_harbours.push_back(*it);

    qSort(config.m_interesting_harbours);

    return config;
}
