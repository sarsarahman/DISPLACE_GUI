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
#ifndef GRAPHBUILDER_H
#define GRAPHBUILDER_H

#include <QList>
#include <QPointF>

#include <memory>
#include <gdal/ogrsf_frmts.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <utility>

class GraphBuilder
{
public:
    class Feedback {
    public:
        virtual void setMax(int m) = 0;
        virtual void setStep(int step) = 0;
    };

    enum Type { Hex, Quad };

    class Node {
    public:
        QPointF point;
        int harbour;
        QList<int> adiacencies;
        QList<double> weight;
        bool good;

        Node()
            : point(), harbour(0), adiacencies(), weight(), good(false) {
        }
    };

    GraphBuilder();

    void setType (Type type) {
        mType = type;
    }

    void setDefaultDistance (double distance) {
        mStep = distance;
    }

    void setDistance1 (double distance) {
        mStep1 = distance;
    }

    void setDistance2 (double distance) {
        mStep2 = distance;
    }

    void setFeedback (Feedback *feedback) {
        mFeedback = feedback;
    }

    void setLimits (double lonMin, double lonMax, double latMin, double latMax) ;
    void setIncludingShapefile1 (std::shared_ptr<OGRDataSource> src);
    void setIncludingShapefile2 (std::shared_ptr<OGRDataSource> src);
    void setExcludingShapefile (std::shared_ptr<OGRDataSource> src);
    void setExcludeZoneEdgeRemovalEnabled (bool en) {
        mRemoveEdgesInExcludeZone = en;
    }

    QList<Node> buildGraph();

    static void pointSumWithBearing (const QPointF &p1, double dist, double bearing, QPointF &p2);
    static const double earthRadius;

    bool outsideEnabled() const;
    void setOutsideEnabled(bool outsideEnabled);
    void setLinkLimits(double limit_km);

private:
    typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
    typedef CGAL::Triangulation_vertex_base_with_info_2<unsigned, K>    Vb;

    /*
    typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
    typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Delaunay;*/

    typedef CGAL::Constrained_triangulation_face_base_2<K>           Fb;
    typedef CGAL::Triangulation_data_structure_2<Vb,Fb>              TDS;
    typedef CGAL::Exact_predicates_tag                               Itag;
    typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> CDT;

    typedef CDT::Point                                             Point;

    void fillWithNodes(QList<Node> &res, CDT &tri,
                       double stepx, double fal, std::vector<std::shared_ptr<OGRDataSource> > including, std::vector<std::shared_ptr<OGRDataSource> > excluding, bool outside);
    void pushAd(QList<Node> &node, int source, int target);

    Type mType;
    bool mOutsideEnabled, mRemoveEdgesInExcludeZone;
    double mStep, mStep1, mStep2;
    double mLatMin, mLatMax, mLonMin, mLonMax;
    double mLinkLimits;

    std::shared_ptr<OGRDataSource> mShapefileInc1;
    std::shared_ptr<OGRDataSource> mShapefileInc2;
    std::shared_ptr<OGRDataSource> mShapefileExc;

    Feedback *mFeedback;
};

#endif // GRAPHBUILDER_H
