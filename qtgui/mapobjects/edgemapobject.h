#ifndef EDGEMAPOBJECT_H
#define EDGEMAPOBJECT_H

#include <QObject>

#include <mapobjects/mapobject.h>
#include <modelobjects/nodedata.h>
#include <mapobjects/nodegraphics.h>
#include <QMapControl/GeometryLineString.h>

namespace qmapcontrol {
    class RectWorldCoord;
}

class MapObjectsController;
class NodeDetailsWidget;

class EdgeMapObject : public QObject, public MapObject
{
    Q_OBJECT

public:
    EdgeMapObject(MapObjectsController *controller, int indx, NodeData *node);

    std::shared_ptr<qmapcontrol::Geometry> getGeometryEntity() const {
        return mGeometry;
    }

    virtual bool showProperties();
//    virtual void update();

protected:
//    QString updateStatText(QString prefix);

private slots:
//    void widgetClosed();

signals:
    void selected(/*EdgeMapObject *, */ bool);

private:
    static QPen mNormalPen, mSelectedPen;

    MapObjectsController *mController;
    NodeData *mNode;
    int mEdgeIndex;
    bool mSelected;

    std::shared_ptr<qmapcontrol::GeometryLineString> mGeometry;
    NodeDetailsWidget *mWidget;
};

#endif // EDGEMAPOBJECT_H
