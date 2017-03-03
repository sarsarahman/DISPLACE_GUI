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
#ifndef NODEDATA_H
#define NODEDATA_H

#include <Node.h>

#include <QList>
#include <QVector>

#include <memory>

class DisplaceModel;

/** \brief an extension class for Node
 *
 * */
class NodeData : public std::enable_shared_from_this<NodeData>
{
public:
    struct Edge {
        Edge()
            : source(), target(), weight(0.0) {
        }

        Edge(std::shared_ptr<NodeData> src, std::shared_ptr<NodeData> nd, double wt)
            : source(src), target(nd), weight(wt) {
        }

        std::weak_ptr<NodeData> source;
        std::weak_ptr<NodeData> target;
        double weight;
    };

    typedef QVector<std::shared_ptr<Edge>> AdiacencyList;

    std::shared_ptr<Node> mNode;    /* This is crap. But we have no other choice */

    explicit NodeData(std::shared_ptr<Node> nd, DisplaceModel *model);
    virtual ~NodeData();

    /* Proxy functions to avoid changing too much code */
    int get_idx_node() const { return mNode->get_idx_node(); }
    int get_marine_landscape() const { return mNode->get_marine_landscape(); }
    double get_wind() const { return mNode->get_wind(); }
    double get_sst() const { return mNode->get_sst(); }
    double get_salinity() const { return mNode->get_salinity(); }
    double get_init_benthos_biomass() const { return mNode->get_init_benthos_biomass(); }
    vector<double> get_benthos_biomass_per_funcgr() const { return mNode->get_benthos_biomass_per_funcgr(); }
    double get_init_benthos_number() const { return mNode->get_init_benthos_number(); }
    vector<double> get_benthos_number_per_funcgr() const { return mNode->get_benthos_number_per_funcgr(); }
    double get_init_benthos_meanweight() const { return mNode->get_init_benthos_meanweight(); }
    vector<double> get_benthos_meanweight_per_funcgr() const { return mNode->get_benthos_meanweight_per_funcgr(); }
    void setMarineLandscape(int id) { mNode->setMarineLandscape(id); }
    void setWind(double w) { mNode->setWind(w); }
    void setSST(double t) { mNode->setSST(t); }
    void setSalinity(double s) { mNode->setSalinity(s); }
    void setBenthosBiomass(double val) { mNode->setBenthosBiomass(val); }
    void setBenthosNumber(double val) { mNode->setBenthosNumber(val); }
    void setBenthosMeanweight(double val) { mNode->setBenthosMeanweight(val); }

    int get_code_area() const { return mNode->get_code_area(); }
    void setCodeArea(int id) { mNode->setCodeArea(id); }

    virtual string get_name() const { return mNode->get_name(); }
    double get_x() const { return mNode->get_x(); }
    double get_y() const { return mNode->get_y(); }
    int get_harbour() const { return mNode->get_harbour(); }
    bool get_is_harbour() const { return mNode->get_is_harbour(); }
    int get_cumftime() const { return mNode->get_cumftime(); }
    void set_cumftime(int i) { mNode->set_cumftime(i); }
    double get_cumsweptarea() const { return mNode->get_cumsweptarea(); }
    void set_cumsweptarea(int i) { mNode->set_cumsweptarea(i); }
    double get_cumcatches() const { return mNode->get_cumcatches(); }
    void set_cumcatches(int i) { mNode->set_cumcatches(i); }
    vector<double> get_tariffs() const { return mNode->get_tariffs(); }
    void set_tariffs(vector<double> tariffs) { mNode->set_tariffs(tariffs); }
    const vector<int> &get_usual_fgrounds() { return mNode->get_usual_fgrounds(); }
    void set_usual_fgrounds(vector<int> usual_fgrounds) { mNode->set_usual_fgrounds(usual_fgrounds); }
    const vector<double> &get_freq_usual_fgrounds() { return mNode->get_freq_usual_fgrounds(); }
    void set_freq_usual_fgrounds(vector<double> freq_usual_fgrounds) { mNode->set_freq_usual_fgrounds(freq_usual_fgrounds); }
    int evaluateAreaType() { return mNode->evaluateAreaType(); }

    /* Extending functions */

    DisplaceModel *getModel() const { return mModel; }

    int getPopCount() const;
    int getBenthosPopCount() const;
    void setPop(int pop, double v);
    void setPopTot(double tot);
    void setPop(QList<double> v, double tot);
    double getPop(int pop) const ;
    double getPopTot () const {
        return mPopTot;
    }

    void setPopW(int pop, double val);
    void setPopWTot(double tot);
    void setPopW(QList<double> v, double tot);
    double getPopW(int pop) const ;
    double getPopWTot () const {
        return mPopWTot;
    }

    void setImpact(int pop, double impact);
    double getImpact(int pop) const {
        return mImpact[pop];
    }

    void setCumcatchesPerPop(int pop, double cumcatchesperpop);
    double getCumcatchesPerPop(int pop) const {
        return mCumcatchesPerPop[pop];
    }

    void setBenthosBiomass(int func, double benthosbiomass);
    double getBenthosBiomass(int func) const {
        return mBenthosBiomass[func];
    }
    void setBenthosNumber(int func, double benthosnumber);
    double getBenthosNumber(int func) const {
        return mBenthosNumber[func];
    }
    void setBenthosMeanweight(int func, double benthosmeanweight);
    double getBenthosMeanweight(int func) const {
        return mBenthosMeanweight[func];
    }

    int getHarbourId() const;
    void setHarbourId(int value);

    /* Adiacency functions */
    int appendAdiancency(std::shared_ptr<NodeData> target, double weight);
    void removeAdiacencyByIdx(int idx);
    void removeAdiacencyByTarget(std::shared_ptr<NodeData> target);
    void removeAllAdiacencies();
    int getAdiacencyCount() const;
    std::shared_ptr<Edge> getAdiacencyByIdx(int idx) const;
    double getAdiacencyWeight(int idx) const;
    void setAdiacencyWeight (int idx, double w);
    void setAreaType (int value);

    bool isDeleted() const;
    void setDeleted(bool value = true);

private:
    DisplaceModel *mModel;
    bool mDeleted;
    int mHarbourId;
    double *mPop;
    double mPopTot;
    double *mPopW;
    double mPopWTot;

    double *mImpact;
    double *mCumcatchesPerPop;
    double *mBenthosBiomass;
    double *mBenthosNumber;
    double *mBenthosMeanweight;
    int areaType;

    AdiacencyList mAdiacency;
};

#endif // NODEDATA_H
