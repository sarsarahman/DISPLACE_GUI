#include <objects/layerentity.h>

#include <displacemodel.h>
#include <objecttreemodel.h>
#include <mapobjectscontroller.h>

namespace objecttree {


LayerEntity::LayerEntity(ObjectTreeModel::Category type, ObjectTreeModel *_model, int LayerEntity_idx)
    : ObjectTreeEntity(_model),
      mLayerEntityIndex(LayerEntity_idx),
      mLayerEntityType(type)
{
}

LayerEntity::~LayerEntity()
{

}

QModelIndex LayerEntity::parent(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return model->createCategoryEntityFromChild(mLayerEntityType);
}

QModelIndex LayerEntity::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    ObjectTreeEntity * entity = new LayerEntity(mLayerEntityType, model, row);
    return model->createEntity(row, column, entity);
}

int LayerEntity::rowCount() const
{
    if (mLayerEntityIndex == -1 && model->getModelIdx() != -1)
        return model->getMapControl()->getLayerList(model->getModelIdx(), mLayerEntityType)->getCount();

    return 0;
}

int LayerEntity::columnCount() const
{
    return 1;
}

QVariant LayerEntity::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        QString name = model->getMapControl()->getLayerList(model->getModelIdx(), mLayerEntityType)->getName(index.row());
        if (name.startsWith('#')) {
            // Layer name has control index: clean the name.
            name = name.mid(name.lastIndexOf('#')+1);
        }
        return name;
    }
    if (role == Qt::CheckStateRole)
        return QVariant(model->getMapControl()->isLayerVisible(model->getModelIdx(), mLayerEntityType, (MapObjectsController::LayerIds) index.row()) ? Qt::Checked : Qt::Unchecked);
    return QVariant();
}

Qt::ItemFlags LayerEntity::flags(Qt::ItemFlags defFlags, const QModelIndex &index) const
{
    Q_UNUSED(index);
    return defFlags | Qt::ItemIsUserCheckable;
}

bool LayerEntity::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.column() == 0 && role == Qt::CheckStateRole) {
        if (value.toInt() == 0) {
            model->getMapControl()->setLayerVisibility(model->getModelIdx(), mLayerEntityType, (MapObjectsController::LayerIds)index.row(), false);
        } else {
            model->getMapControl()->setLayerVisibility(model->getModelIdx(), mLayerEntityType, (MapObjectsController::LayerIds)index.row(), true);
        }
        return true;
    }
    return false;
}

}
