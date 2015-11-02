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
#ifndef NODEGRAPHICS_H
#define NODEGRAPHICS_H

#include <QMapControl/GeometryPointShapeScaled.h>
#include <modelobjects/nodedata.h>

#include <QBrush>

class MapObjectsController;

class NodeGraphics : public qmapcontrol::GeometryPointShapeScaled {
protected:
    QColor c;

    NodeData *mNode;
    MapObjectsController *mController;
    int mModelIndex;
    QSizeF mGrid;
public:
    NodeGraphics (NodeData *node, MapObjectsController *controller, int indx);

protected:
    virtual void drawShape(QPainter &painter, const qmapcontrol::RectWorldPx &rect);
};

class NodeWithPopStatsGraphics : public NodeGraphics {
public:
    enum Type { Population, Biomass, Impact, BenthosBiomass,
              LastType };

    NodeWithPopStatsGraphics(Type type, NodeData *node, MapObjectsController *controller, int indx)
        : NodeGraphics(node, controller, indx),
          mType(type)
    {}

protected:
    virtual void drawShape(QPainter &painter, const qmapcontrol::RectWorldPx &rect);

    Type getType() const { return mType; }
    double getValueForPop(int pop) const;

private:
    Type mType;
};

class NodeWithCumFTimeGraphics : public NodeGraphics {
public:
    NodeWithCumFTimeGraphics(NodeData *node, MapObjectsController *controller, int indx)
        : NodeGraphics(node, controller, indx) {}

protected:
    virtual void drawShape(QPainter &painter, const qmapcontrol::RectWorldPx &rect);
};

class NodeWithCumSweptAreaGraphics : public NodeGraphics {
public:
    NodeWithCumSweptAreaGraphics(NodeData *node, MapObjectsController *controller, int indx)
        : NodeGraphics(node, controller, indx) {}

protected:
    virtual void drawShape(QPainter &painter, const qmapcontrol::RectWorldPx &rect);
};


class NodeWithCumCatchesGraphics : public NodeGraphics {
public:
    NodeWithCumCatchesGraphics(NodeData *node, MapObjectsController *controller, int indx)
        : NodeGraphics(node, controller, indx) {}

protected:
    virtual void drawShape(QPainter &painter, const qmapcontrol::RectWorldPx &rect);
};


#endif // NODEGRAPHICS_H
