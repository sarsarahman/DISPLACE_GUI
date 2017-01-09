#ifndef GRAPHBUILDER_SHP_H
#define GRAPHBUILDER_SHP_H

#include <gdal/ogrsf_frmts.h>
#include <QPoint>
#include <QList>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"
#pragma GCC diagnostic ignored "-Wshift-negative-value"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#pragma GCC diagnostic pop

namespace displace {
namespace graphbuilders {
class GeographicGridBuilder;
}
}

class GraphBuilder
{
public:
    class Feedback {
    public:
        virtual void setMax(int m) = 0;
        virtual void setStep(int step) = 0;
    };

    enum Type { Hex, Quad, HexTrivial, QuadTrivial };

    class Node {
    public:
        QPointF point;
        int harbour = 0;
        QList<int> adiacencies;
        QList<double> weight;
        bool good = false;

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
    void setMaxLinks(int num = -1) {
        mMaxLinks = num;
    }
    void setMinLinks(int num = -1) {
        mMinLinks = num;
    }

    QList<Node> buildGraph();

    static void pointSumWithBearing (const QPointF &p1, double dist, double bearing, QPointF &p2);
    static const double earthRadius;

    bool outsideEnabled() const;
    void setOutsideEnabled(bool outsideEnabled);
    void setLinkLimits(double limit_km);

private:
    std::shared_ptr<displace::graphbuilders::GeographicGridBuilder> createBuilder (Type type, double step);
    void createGrid (OGRDataSource *tempDatasource,
                     std::shared_ptr<displace::graphbuilders::GeographicGridBuilder> builder,
                     OGRLayer *lyOut,
                     OGRLayer *lyGrid,
                     OGRLayer *lyIncluded, OGRLayer *lyExclusion1, OGRLayer *lyExclusion2);

    void clip (OGRLayer *in, OGRLayer *feature, OGRLayer *out);
    void diff (OGRLayer *in1, OGRLayer *in2, OGRLayer *out, OGRDataSource *tempds);
    void copyLayerContent(OGRLayer *src, OGRLayer *dst);
    void makePartProgress(double x);

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

//    void fillWithNodes(displace::graphbuilders::GeographicGridBuilder *builder, QList<Node> &res, CDT &tri, std::vector<std::shared_ptr<OGRDataSource> > including, std::vector<std::shared_ptr<OGRDataSource> > excluding, bool outside, int &progress);
//    void pushAd(QList<Node> &node, int source, int target);

    Type mType;
    bool mOutsideEnabled, mRemoveEdgesInExcludeZone;
    double mStep, mStep1, mStep2;
    double mLatMin, mLatMax, mLonMin, mLonMax;
    double mLinkLimits;
    int mMaxLinks, mMinLinks;
    int progress = 0;

    std::shared_ptr<OGRDataSource> mShapefileInc1;
    std::shared_ptr<OGRDataSource> mShapefileInc2;
    std::shared_ptr<OGRDataSource> mShapefileExc;

    Feedback *mFeedback = 0;

    static int waitfunc(double progress, const char *msg, void *thiz);
};

#endif // GRAPHBUILDER_SHP_H
