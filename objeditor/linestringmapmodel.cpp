#include "linestringmapmodel.h"

#include <QMapControl.h>
#include <LayerGeometry.h>
#include <GeometryLineString.h>

#include <graphics/shiplanesgraphics.h>

LineStringMapModel::LineStringMapModel(qmapcontrol::QMapControl *control)
    : MapControlGraphicsModel(control)
{
    mGraphicsLayer = std::make_shared<qmapcontrol::LayerGeometry>("Geometry");
    control->addLayer(mGraphicsLayer);
    mGraphicsLayer->setVisible(true);
}

void LineStringMapModel::addGraphicsData(int row, int id, float lat, float lon)
{
    while (mGraphics.size() <= (size_t)id)
        mGraphics.push_back(nullptr);
    while (mLaneData.size() <= (size_t)row)
        mLaneData.push_back(LaneData());

    if (mGraphics[id] == nullptr) {
        mGraphics[id] = mGeometryBuilder(lat, lon);
        mGraphicsLayer->addGeometry(mGraphics[id]);
        qDebug() << "Lane " << id << " for row" << row << lat << lon;
    }

    // add the current point

    LaneData ld;
    ld.lane = id;
    ld.coordid = mGraphics[id]->points().size();
    mGraphics[id]->addPoint(qmapcontrol::PointWorldCoord(lon,lat));
    mLaneData[row] = ld;
}

void LineStringMapModel::updateGraphicsData(int row, float lat, float lon)
{
    LaneData ld = mLaneData[row];

    auto points = mGraphics[ld.lane]->points();
    points[ld.coordid] = qmapcontrol::PointWorldCoord(lon, lat);
    mGraphics[ld.lane]->setPoints(points);
}

void LineStringMapModel::setGeometryBuilder(LineStringMapModel::GeometryBuilder builder)
{
    mGeometryBuilder = builder;
}
