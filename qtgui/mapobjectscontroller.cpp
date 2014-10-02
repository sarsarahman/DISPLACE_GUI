#include "mapobjectscontroller.h"

#include <displacemodel.h>
#include <mapobjects/harbourmapobject.h>
#include <mapobjects/nodemapobject.h>
#include <mapobjects/vesselmapobject.h>

#include <QMapControl/QMapControl.h>
#include <QMapControl/MapAdapterOSM.h>
#include <QMapControl/Layer.h>
#include <QMapControl/MapAdapterOpenSeaMap.h>
#include <QMapControl/LayerGeometry.h>
#include <QMapControl/GeometryPointCircle.h>
#include <QMapControl/LayerMapAdapter.h>
#include <QMapControl/ImageManager.h>

MapObjectsController::MapObjectsController(qmapcontrol::QMapControl *map)
    : mMap(map),
      mLayers(MAX_MODELS, LayerList(LayerMax)),
      mOutputLayers(MAX_MODELS, LayerList(OutLayerMax))
{
    // create mapadapter, for mainlayer and overlay
    mMainMapAdapter = std::shared_ptr<qmapcontrol::MapAdapter> (new qmapcontrol::MapAdapterOSM());
    mSeamarkAdapter = std::shared_ptr<qmapcontrol::MapAdapter> (new qmapcontrol::MapAdapterOpenSeaMap());

    // create a layer with the mapadapter and type MapLayer
    mMainLayer = std::shared_ptr<qmapcontrol::LayerMapAdapter>(new qmapcontrol::LayerMapAdapter("OpenStreetMap", mMainMapAdapter));
    mSeamarkLayer = std::shared_ptr<qmapcontrol::LayerMapAdapter>(new qmapcontrol::LayerMapAdapter("Seamark", mSeamarkAdapter));

    mMap->addLayer(mMainLayer);
    mMap->addLayer(mSeamarkLayer);

    mMap->setMapFocusPoint(qmapcontrol::PointWorldCoord(11.54105,54.49299));
    mMap->setZoom(10);
}

void MapObjectsController::createMapObjectsFromModel(int model_n, DisplaceModel *model)
{
    addStandardLayer(model_n, LayerMain, mMainLayer);
    addStandardLayer(model_n, LayerSeamarks, mSeamarkLayer);

    std::shared_ptr<qmapcontrol::LayerGeometry> mEntityLayer = std::shared_ptr<qmapcontrol::LayerGeometry>(new qmapcontrol::LayerGeometry("Entities"));
    std::shared_ptr<qmapcontrol::LayerGeometry> mGraphLayer = std::shared_ptr<qmapcontrol::LayerGeometry>(new qmapcontrol::LayerGeometry("Graph"));

    addStandardLayer(model_n, LayerEntities, mEntityLayer);
    addStandardLayer(model_n, LayerGraph, mGraphLayer);

    const QList<Harbour *> &harbours = model->getHarboursList();
    foreach (Harbour *h, harbours) {
        HarbourMapObject *obj = new HarbourMapObject(h);
        mHarbourObjects[model_n].append(obj);

        mEntityLayer->addGeometry(obj->getGeometryEntity());
    }

    const QList<NodeData *> &nodes = model->getNodesList();
    foreach (NodeData *nd, nodes) {
        NodeMapObject *obj = new NodeMapObject(nd);
        mNodeObjects[model_n].append(obj);

        mGraphLayer->addGeometry(obj->getGeometryEntity());
    }

    const QList<VesselData *> &vessels = model->getVesselList();
    foreach (VesselData *vsl, vessels) {
        VesselMapObject *obj = new VesselMapObject(vsl);
        mVesselObjects[model_n].append(obj);

        mEntityLayer->addGeometry(obj->getGeometryEntity());
    }
}

void MapObjectsController::updateMapObjectsFromModel(int model_n, DisplaceModel *model)
{
    const QList<VesselData *> &vessels = model->getVesselList();
    foreach (VesselData *vsl, vessels) {
        updateVesselPosition(model_n, vsl->mVessel->get_idx());
    }

}

void MapObjectsController::updateVesselPosition(int model, int idx)
{
    mVesselObjects[model].at(idx)->vesselUpdated();
}

void MapObjectsController::updateNodes(int model)
{
    foreach (NodeMapObject *obj, mNodeObjects[model]) {
        obj->getGeometryEntity()->requestRedraw();
    }
}

void MapObjectsController::setModelVisibility(int model, MapObjectsController::Visibility visibility)
{
    bool visible = (visibility == Visible);
    foreach (HarbourMapObject *h, mHarbourObjects[model]) {
        h->getGeometryEntity()->setVisible(visible);
    }
    foreach (NodeMapObject *n, mNodeObjects[model]) {
        n->getGeometryEntity()->setVisible(visible);
    }
    foreach (VesselMapObject *v, mVesselObjects[model]) {
        v->getGeometryEntity()->setVisible(visible);
    }
}

void MapObjectsController::addStandardLayer(int model, LayerIds id, std::shared_ptr<Layer> layer)
{
    if (layer != mMainLayer && layer != mSeamarkLayer)
        mMap->addLayer(layer);
    mLayers[model].layers[id] = layer;
}

void MapObjectsController::addOutputLayer(int model, LayerIds id, std::shared_ptr<Layer> layer)
{
    mMap->addLayer(layer);
    mOutputLayers[model].layers[id] = layer;
}
