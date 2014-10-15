﻿#ifndef MAPOBJECTSCONTROLLER_H
#define MAPOBJECTSCONTROLLER_H

#include <QObject>
#include <QList>
#include <QVector>

#include <memory>
#include <mainwindow.h>
#include <palettemanager.h>

#include <QMapControl/Layer.h>
#include <QMapControl/Geometry.h>

namespace qmapcontrol {
class QMapControl;
class MapAdapter;
class LayerMapAdapter;
class LayerGeometry;
class GeometryWidget;
}

class DisplaceModel;
class MapObject;
class HarbourMapObject;
class NodeMapObject;
class VesselMapObject;

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

using qmapcontrol::Geometry;

class MapObjectsController : public QObject
{
    Q_OBJECT

public:
    class LayerList {
    public:
        virtual int getCount() const = 0;
        virtual QString getName(int idx) const = 0;
    };

    class WidgetUserData : public QObjectUserData {
    private:
        std::shared_ptr<qmapcontrol::GeometryWidget> mWidget;
    public:
        WidgetUserData(std::shared_ptr<qmapcontrol::GeometryWidget> w)
            : QObjectUserData(), mWidget(w) {}

        std::shared_ptr<qmapcontrol::GeometryWidget> widget() const { return mWidget; }
    };

    class WidgetAncillaryData : public qmapcontrol::Geometry::AncillaryData {
    private:
        MapObject *mObject;
    public:
        explicit WidgetAncillaryData(MapObject *object)
            : mObject(object) {
        }

        MapObject *object() const { return mObject; }
    };

private:
    static const int MaxLayers = 32;

    class LayerListImpl : public LayerList {
    public:
        LayerListImpl()
            : LayerListImpl(MaxLayers) {}

        explicit LayerListImpl (int sz) {
            layers.fill(std::shared_ptr<qmapcontrol::Layer>(), sz);
            visibility.fill(false, sz);
        }

        QVector<std::shared_ptr<qmapcontrol::Layer> > layers;
        QVector<bool> visibility;

        virtual int getCount() const { return layers.size(); }
        virtual QString getName(int idx) const { return QString::fromStdString(layers[idx]->getName()); }
        virtual bool isVisible(int idx) const { return layers[idx]->isVisible(); }

        virtual void setVisible(int idx, bool v) { layers[idx]->setVisible(v); }
    };

public:
    enum LayerIds {
        LayerMain = 0, LayerSeamarks = 1,
        LayerEntities = 2, LayerGraph = 3,

        LayerMax
    };
    enum OutLayerIds {
        OutLayerPopStats = 0,
        OutLayerBiomass,
        OutLayerPopImpact,
        OutLayerCumFTime,

        OutLayerMax
    };

    MapObjectsController(qmapcontrol::QMapControl *map);

    qmapcontrol::QMapControl *mapWidget() const { return mMap; }

    void createMapObjectsFromModel(int model_n, DisplaceModel *model);
    void updateMapObjectsFromModel(int model_n, DisplaceModel *model);

    void updateVesselPosition (int model, int idx);

    void updateNodes(int model);

    LayerList *getStandardLayerList(int model) {
        return &mLayers[model];
    }
    LayerList *getOutputLayerList(int model) {
        return &mOutputLayers[model];
    }

    enum Visibility { Visible, Invisible };
    /** \brief Sets the visibility of the model objects
     * \param model index of the model
     * \param visibility visibility status
     * */
    void setModelVisibility(int model, Visibility visibility);
    void setLayerVisibility (int model, LayerIds layer, bool visibility);
    bool isLayerVisible (int model, LayerIds layer);
    void setOutLayerVisibility (int model, OutLayerIds layer, bool visibility);
    bool isOutLayerVisible (int model, OutLayerIds layer);

    bool isModelActive (int model) const;

    const Palette &getPalette(int model, PaletteRole n) const {
        return mPaletteManager[model]->palette(n);
    }
    void setPalette (int model, PaletteRole n, const Palette &palette);

    void forceRedraw();

    void showDetailsWidget(const PointWorldCoord &point, QWidget *widget);

protected:
    void addStandardLayer(int model, LayerIds id, std::shared_ptr<Layer> layer);
    void addOutputLayer(int model, OutLayerIds id, std::shared_ptr<Layer> layer);

protected slots:
    void geometryClicked(const Geometry *);
    void widgetClosed(QObject *);

public slots:
    void signalAppIsClosing();
    void removeAllWidgets();

private:
    qmapcontrol::QMapControl *mMap;
    QList<HarbourMapObject *> mHarbourObjects[MAX_MODELS];
    QList<NodeMapObject *> mNodeObjects[MAX_MODELS];
    QList<VesselMapObject *> mVesselObjects[MAX_MODELS];
    std::shared_ptr<PaletteManager> mPaletteManager[MAX_MODELS];

    std::shared_ptr<qmapcontrol::MapAdapter> mMainMapAdapter;
    std::shared_ptr<qmapcontrol::MapAdapter> mSeamarkAdapter;
    std::shared_ptr<qmapcontrol::LayerMapAdapter> mMainLayer;
    std::shared_ptr<qmapcontrol::LayerMapAdapter> mSeamarkLayer;
    std::shared_ptr<qmapcontrol::LayerGeometry> mWidgetLayer;

    QVector<bool> mModelVisibility;
    QVector<LayerListImpl> mLayers;
    QVector<LayerListImpl> mOutputLayers;

    bool mClosing;
};

#endif // MAPOBJECTSCONTROLLER_H
