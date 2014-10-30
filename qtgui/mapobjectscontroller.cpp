#include "mapobjectscontroller.h"

#include <displacemodel.h>
#include <mapobjects/harbourmapobject.h>
#include <mapobjects/nodemapobject.h>
#include <mapobjects/vesselmapobject.h>
#include <mapobjects/edgemapobject.h>
#include <mapobjects/edgelayer.h>

#include <QMapControl/QMapControl.h>
#include <QMapControl/MapAdapterOSM.h>
#include <QMapControl/Layer.h>
#include <QMapControl/MapAdapterOpenSeaMap.h>
#include <QMapControl/LayerGeometry.h>
#include <QMapControl/GeometryPointCircle.h>
#include <QMapControl/LayerMapAdapter.h>
#include <QMapControl/ImageManager.h>
#include <QMapControl/GeometryWidget.h>
#include <QMapControl/LayerESRIShapefile.h>

#include <QTextEdit>

/* If defined, CONFIG_ALLOW_MULTIPLE_DETAILS_WINDOWS allows opening more than one Details window.
 */
#undef CONFIG_ALLOW_MULTIPLE_DETAILS_WINDOWS

MapObjectsController::MapObjectsController(qmapcontrol::QMapControl *map)
    : mMap(map),
      mPaletteManager(),
      mModelVisibility(MAX_MODELS, false),
      mLayers(MAX_MODELS, LayerListImpl(LayerMax)),
      mOutputLayers(MAX_MODELS, LayerListImpl(OutLayerMax)),
      mShapefileLayers(MAX_MODELS, LayerVarListImpl()),
      mEditorMode(NoEditorMode),
      mClosing(false)
{
    // create mapadapter, for mainlayer and overlay
    mMainMapAdapter = std::shared_ptr<qmapcontrol::MapAdapter> (new qmapcontrol::MapAdapterOSM());
    mSeamarkAdapter = std::shared_ptr<qmapcontrol::MapAdapter> (new qmapcontrol::MapAdapterOpenSeaMap());

    // create a layer with the mapadapter and type MapLayer
    mMainLayer = std::shared_ptr<qmapcontrol::LayerMapAdapter>(new qmapcontrol::LayerMapAdapter("OpenStreetMap", mMainMapAdapter));
    mSeamarkLayer = std::shared_ptr<qmapcontrol::LayerMapAdapter>(new qmapcontrol::LayerMapAdapter("Seamark", mSeamarkAdapter));
    mWidgetLayer = std::shared_ptr<qmapcontrol::LayerGeometry>(new qmapcontrol::LayerGeometry("Details"));

    mMap->addLayer(mMainLayer);
    mMap->addLayer(mSeamarkLayer);
    mMap->addLayer(mWidgetLayer);

    mMap->setMapFocusPoint(qmapcontrol::PointWorldCoord(11.54105,54.49299));
    mMap->setZoom(10);

    connect (mMap, SIGNAL(geometryClicked(const Geometry*)), this, SLOT(geometryClicked(const Geometry*)));
}

void MapObjectsController::setModel(int model_n, std::shared_ptr<DisplaceModel> model)
{
    mModels[model_n] = model;
}

void MapObjectsController::createMapObjectsFromModel(int model_n, DisplaceModel *model)
{
    mPaletteManager[model_n] = std::shared_ptr<PaletteManager>(new PaletteManager());
    QFile pf(":/palettes/iso1996_2.p2c");
    Palette p;
    p.loadFromFile(&pf);

    for (int i = 0; i < (int)LastRole; ++i)
        mPaletteManager[model_n]->setPalette((PaletteRole)i, p);

    addStandardLayer(model_n, LayerMain, mMainLayer);
    addStandardLayer(model_n, LayerSeamarks, mSeamarkLayer);

    std::shared_ptr<qmapcontrol::LayerGeometry> mEntityLayer = std::shared_ptr<qmapcontrol::LayerGeometry>(new qmapcontrol::LayerGeometry("Entities"));
    std::shared_ptr<qmapcontrol::LayerGeometry> mGraphLayer = std::shared_ptr<qmapcontrol::LayerGeometry>(new qmapcontrol::LayerGeometry("Graph"));
    mEdgesLayer[model_n] = std::shared_ptr<EdgeLayer>(new EdgeLayer(this, QString(QObject::tr("Graph Edges"))));
    mEdgesLayer[model_n]->setVisible(false);

//    mShapefileLayer[model_n] = std::shared_ptr<qmapcontrol::LayerESRIShapefile> (new qmapcontrol::LayerESRIShapefile("Shapefile"));
//    mShapefileLayer[model_n]->setVisible(true);

    addStandardLayer(model_n, LayerEntities, mEntityLayer);
    addStandardLayer(model_n, LayerGraph, mGraphLayer);
//    addStandardLayer(model_n, LayerShapefile, mShapefileLayer[model_n]);
    addStandardLayer(model_n, LayerEdges, mEdgesLayer[model_n]->layer());

    std::shared_ptr<qmapcontrol::LayerGeometry> popstatslayer = std::shared_ptr<qmapcontrol::LayerGeometry>(new qmapcontrol::LayerGeometry("Abundance"));
    addOutputLayer(model_n, OutLayerPopStats, popstatslayer);

    std::shared_ptr<qmapcontrol::LayerGeometry> biomasslayer = std::shared_ptr<qmapcontrol::LayerGeometry>(new qmapcontrol::LayerGeometry("Biomass"));
    addOutputLayer(model_n, OutLayerBiomass, biomasslayer);

    std::shared_ptr<qmapcontrol::LayerGeometry> impactlayer = std::shared_ptr<qmapcontrol::LayerGeometry>(new qmapcontrol::LayerGeometry("Impact"));
    addOutputLayer(model_n, OutLayerPopImpact, impactlayer);

    std::shared_ptr<qmapcontrol::LayerGeometry> cumftimelayer = std::shared_ptr<qmapcontrol::LayerGeometry>(new qmapcontrol::LayerGeometry("Fishing Effort"));
    addOutputLayer(model_n, OutLayerCumFTime, cumftimelayer);

    const QList<HarbourData *> &harbours = model->getHarboursList();
    foreach (HarbourData *h, harbours) {
        HarbourMapObject *obj = new HarbourMapObject(this, model, h);
        mHarbourObjects[model_n].append(obj);

        mEntityLayer->addGeometry(obj->getGeometryEntity());
    }

    const QList<NodeData *> &nodes = model->getNodesList();
    foreach (NodeData *nd, nodes) {
        if (nd->get_harbour())
            continue;

        NodeMapObject *obj = new NodeMapObject(this, model_n, NodeMapObject::GraphNodeRole, nd);
        connect(obj, SIGNAL(nodeSelectionHasChanged(NodeMapObject*)), this, SLOT(nodeSelectionHasChanged(NodeMapObject*)));
        mNodeObjects[model_n].append(obj);

        mGraphLayer->addGeometry(obj->getGeometryEntity());

        /* add here other roles */
        obj = new NodeMapObject(this, model_n,NodeMapObject::GraphNodeWithPopStatsRole, nd);
        mNodeObjects[model_n].append(obj);
        popstatslayer->addGeometry(obj->getGeometryEntity());

        obj = new NodeMapObject(this, model_n,NodeMapObject::GraphNodeWithCumFTimeRole, nd);
        mNodeObjects[model_n].append(obj);
        cumftimelayer->addGeometry(obj->getGeometryEntity());

        obj = new NodeMapObject(this, model_n,NodeMapObject::GraphNodeWithPopImpact, nd);
        mNodeObjects[model_n].append(obj);
        impactlayer->addGeometry(obj->getGeometryEntity());

        obj = new NodeMapObject(this, model_n,NodeMapObject::GraphNodeWithBiomass, nd);
        mNodeObjects[model_n].append(obj);
        biomasslayer->addGeometry(obj->getGeometryEntity());

        for (int i = 0; i < nd->getAdiacencyCount(); ++i) {
            EdgeMapObject *edge = new EdgeMapObject(this, i, nd);

            connect (edge, SIGNAL(edgeSelectionHasChanged(EdgeMapObject*)), this, SLOT(edgeSelectionHasChanged(EdgeMapObject*)));

            mEdgesLayer[model_n]->addEdge(edge);
        }
    }

    const QList<VesselData *> &vessels = model->getVesselList();
    foreach (VesselData *vsl, vessels) {
        VesselMapObject *obj = new VesselMapObject(this,vsl);
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
        obj->updateProperties();
        obj->getGeometryEntity()->requestRedraw();
    }
}

void MapObjectsController::setModelVisibility(int model, MapObjectsController::Visibility visibility)
{
    bool visible = (visibility == Visible);
    mModelVisibility[model] = visible;

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

void MapObjectsController::setLayerVisibility(int model, ObjectTreeModel::Category type, int layer, bool visibility)
{
    switch (type) {
    case ObjectTreeModel::Layers:
        mLayers[model].setVisible(layer, visibility);
        if (isModelActive(model))
            mLayers[model].layers[layer]->setVisible(visibility);
        break;
    case ObjectTreeModel::OutputLayers:
        mOutputLayers[model].setVisible(layer, visibility);
        if (isModelActive(model))
            mOutputLayers[model].layers[layer]->setVisible(visibility);
        break;
    case ObjectTreeModel::ShapefileLayers:
        mShapefileLayers[model].setVisible(layer, visibility);
        if (isModelActive(model))
            mShapefileLayers[model].layers[layer]->setVisible(visibility);
        break;
    }
}

bool MapObjectsController::isLayerVisible(int model, ObjectTreeModel::Category type, int layer)
{
    if (!mModelVisibility[model])
        return false;

    switch (type) {
    case ObjectTreeModel::Layers:
        return mLayers[model].isVisible(layer);
    case ObjectTreeModel::OutputLayers:
        return mOutputLayers[model].isVisible(layer);
    case ObjectTreeModel::ShapefileLayers:
        return mShapefileLayers[model].isVisible(layer);
    }
}

/*
void MapObjectsController::setOutLayerVisibility(int model, MapObjectsController::OutLayerIds layer, bool visibility)
{
    mOutputLayers[model].setVisible(layer, visibility);
    if (isModelActive(model))
        mOutputLayers[model].layers[layer]->setVisible(visibility);
}

bool MapObjectsController::isOutLayerVisible(int model, OutLayerIds layer)
{
    return mModelVisibility[model] && mOutputLayers[model].isVisible(layer);
}*/

bool MapObjectsController::isModelActive(int model) const
{
    return mModelVisibility[model];
}

void MapObjectsController::setPalette(int model, PaletteRole n, const Palette &palette)
{
    mPaletteManager[model]->setPalette(n, palette);
}

void MapObjectsController::forceRedraw()
{
    mMap->requestRedraw();
}

void MapObjectsController::showDetailsWidget(const PointWorldCoord &point, QWidget *widget)
{
#ifndef CONFIG_ALLOW_MULTIPLE_DETAILS_WINDOWS
    removeAllWidgets();
//    foreach (const auto &g, mWidgetLayer->getGeometryWidgets()) {
//        mWidgetLayer->removeGeometry(g);
//    }
#endif

    std::shared_ptr<qmapcontrol::GeometryWidget>  mDetailsWidgetContainer = std::shared_ptr<qmapcontrol::GeometryWidget>(new qmapcontrol::GeometryWidget(point, widget));
    mDetailsWidgetContainer->setAlignmentType(GeometryPoint::AlignmentType::BottomLeft);
    mDetailsWidgetContainer->setVisible(true);

    widget->setUserData(0, new WidgetUserData(mDetailsWidgetContainer));
    widget->setAttribute(Qt::WA_DeleteOnClose);
    connect(widget, SIGNAL(destroyed(QObject*)), this, SLOT(widgetClosed(QObject*)));

    mWidgetLayer->addGeometry(mDetailsWidgetContainer);
}

bool MapObjectsController::importShapefile(int model_idx, QString path, QString layername)
{
    std::shared_ptr<ESRIShapefile> file (new ESRIShapefile(path.toStdString(), layername.toStdString()));

    file->setPenPolygon(QPen(Qt::red));
    file->setBrushPolygon(QBrush(Qt::yellow));

    QFileInfo info(path);
    QString label = QString("Shapefile %1").arg(info.fileName());

    if (!layername.isEmpty())
        label += QString (" (%1)").arg(layername);

    std::shared_ptr<qmapcontrol::LayerESRIShapefile> newlayer(new qmapcontrol::LayerESRIShapefile(label.toStdString()));
    newlayer->addESRIShapefile(file);
    addShapefileLayer(model_idx, newlayer);

    return true;
}

void MapObjectsController::setEditorMode(MapObjectsController::EditorModes mode)
{
    mEditorMode = mode;
}

void MapObjectsController::delSelected(int model)
{
    if (mEditorMode == NoEditorMode)
        return;

    switch (mEditorMode) {
    case EdgeEditorMode:
        delSelectedEdges(model);
        break;
    }
}

void MapObjectsController::addStandardLayer(int model, LayerIds id, std::shared_ptr<Layer> layer)
{
    if (layer != mMainLayer && layer != mSeamarkLayer)
        mMap->addLayer(layer);
    mLayers[model].layers[id] = layer;
}

void MapObjectsController::addOutputLayer(int model, OutLayerIds id, std::shared_ptr<Layer> layer)
{
    mMap->addLayer(layer);
    mOutputLayers[model].layers[id] = layer;
}

void MapObjectsController::addShapefileLayer(int model, std::shared_ptr<Layer> layer, bool show)
{
    mMap->addLayer(layer);
    mShapefileLayers[model].add(layer, show);
}

void MapObjectsController::delSelectedEdges(int model)
{
    foreach (EdgeMapObject *edge, mEdgeSelection[model]) {
        NodeData *nd = edge->node();
        NodeData *tg = edge->target();

//        int nodeid1 = nd->get_idx_node();

        // TODO: automatically remove other side edges
//        int nodeid2 = nd->getAdiacencyByIdx(eid1);

//        NodeData *nd2 = mModels[model]->getNodesList()[nodeid2];
//        nd2->removeAdiacencyByTarget(nodeid1);
//        nd->removeAdiacencyByIdx(eid1);
        nd->removeAdiacencyByTarget(tg->get_idx_node());

        mEdgesLayer[model]->removeEdge(edge);
    }

    mEdgeSelection[model].clear();
    emit edgeSelectionChanged(0);
}

void MapObjectsController::geometryClicked(const Geometry *geometry)
{
    WidgetAncillaryData *objPtr = reinterpret_cast<WidgetAncillaryData *>(geometry->ancillaryData());

    if (objPtr && objPtr->object()) {
        switch(mEditorMode) {
        case NoEditorMode:
            objPtr->object()->showProperties();
            break;
        case EdgeEditorMode:
            objPtr->object()->toggleSelection();
            break;
        case NodeEditorMode:
            objPtr->object()->toggleSelection();
            break;
        }
    }
}

void MapObjectsController::widgetClosed(QObject *widget)
{
    if (mClosing)
        return;
    WidgetUserData *obj = reinterpret_cast<WidgetUserData*>(widget->userData(0));
    if (obj) {
        mWidgetLayer->removeGeometry(obj->widget(), mClosing);
    }
}

void MapObjectsController::signalAppIsClosing()
{
    mClosing = true;
}

void MapObjectsController::removeAllWidgets()
{
    foreach (const auto &wid, mWidgetLayer->getGeometryWidgets()) {
        wid->getWidget()->close();
        mWidgetLayer->removeGeometry(wid, true);
    }
}

void MapObjectsController::edgeSelectionHasChanged(EdgeMapObject *object)
{
    int modelIndex = object->node()->getModel()->index();

    if (object->selected())
        mEdgeSelection[modelIndex].insert(object);
    else
        mEdgeSelection[modelIndex].remove(object);

    emit edgeSelectionChanged(mEdgeSelection[modelIndex].size());
}

void MapObjectsController::nodeSelectionHasChanged(NodeMapObject *node)
{
    int modelIndex = node->node()->getModel()->index();

    if (node->selected())
        mNodeSelection[modelIndex].insert(node);
    else
        mNodeSelection[modelIndex].remove(node);

    emit nodeSelectionChanged(mNodeSelection[modelIndex].size());
}
