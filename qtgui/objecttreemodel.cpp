#include "objecttreemodel.h"

#include <objects/objecttreeentity.h>
#include <objects/layerentity.h>

#include <displacemodel.h>
#include <mapcontrol.h>

QList<objecttree::ObjectTreeEntity *> ObjectTreeModel::entityTemplates;
QString ObjectTreeModel::entityNames[] = {
    QT_TR_NOOP_UTF8("Layers"),
    QT_TR_NOOP_UTF8("Output Layers"),
    QT_TR_NOOP_UTF8("Nodes"),
    QT_TR_NOOP_UTF8("Harbours"),
    QT_TR_NOOP_UTF8("Vessels"),
    QT_TR_NOOP_UTF8("Populations"),
    QT_TR_NOOP_UTF8("Benthos"),
};

ObjectTreeModel::ObjectTreeModel(qmapcontrol::MapControl *map, DisplaceModel *model, QObject *parent) :
    QAbstractItemModel(parent),
    mMapControl(map),
    mModel (model)
{
    if (entityTemplates.size() == 0) {
        // Registers all entity templaes
        entityTemplates.reserve(LastCategory);

        for (int i = 0; i < LastCategory; ++i)
            entityTemplates.push_back(0);

        entityTemplates[Layers] = new objecttree::LayerEntity(this);

        qDebug() << "initialized EntityTemplates: " << entityTemplates.size();
    }
}

int ObjectTreeModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    if (isCategoryLevel(parent)) {
    }

    return 2;
}

int ObjectTreeModel::rowCount(const QModelIndex &parent) const
{
    if (isRootLevel(parent)) { // Categories level
        return LastCategory;
    } else if (isCategoryLevel(parent)) {
        if (entityTemplates[parent.row()])
            return entityTemplates[parent.row()]->rowCount();
        return 0;
    } else {
        return entity(parent)->rowCount();
    }
}

QModelIndex ObjectTreeModel::parent(const QModelIndex &child) const
{
    if (isRootLevel(child) || isCategoryLevel(child))
        return QModelIndex();

    return entity(child)->parent(child);
}

QModelIndex ObjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (isRootLevel(parent)) {
        return createCategoryEntity(row, column, (Category)row);
    } else if (isCategoryLevel(parent)) {
        return entityTemplates[parent.row()]->index(row, column, parent);
    } else {
        return entity(parent)->index(row, column, parent);
    }
}

QVariant ObjectTreeModel::data(const QModelIndex &index, int role) const
{
    if (isCategoryLevel(index)) {
        if (role != Qt::DisplayRole)
            return QVariant();

        if (index.column() != 0)
          return QVariant();

        return QVariant(entityNames[index.row()]);
    } else if(!isRootLevel(index)) {
        /*
        quint64 type = parCatFromId(index.internalId());
        switch (type) {
        case Layers:
        case Vessels:
            if (index.column() == 0 && role == Qt::DisplayRole)
                return QString(mModel->getShipId(index.row()));
            return QVariant();
        case Nodes:
            if (index.column() != 0 && role == Qt::DisplayRole)
                return QString(tr("Node %1")).arg(index.row());
            return QVariant();
        }
        */

        return entity(index)->data(index, role);
    }

    return QVariant();
}

Qt::ItemFlags ObjectTreeModel::flags(const QModelIndex &index) const
{
    if (!isCategoryLevel(index) && !isRootLevel(index))
        return entity(index)->flags(QAbstractItemModel::flags(index), index);

    return QAbstractItemModel::flags(index);

}

bool ObjectTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    qDebug() << "ObjData " << index << value << role;

    if (!isCategoryLevel(index) && !isRootLevel(index)) {
        if (entity(index)->setData(index, value, role)) {
            emit dataChanged(index, index);
            return true;
        }
    }
    return false;
}

void ObjectTreeModel::setCurrentModel(DisplaceModel *model)
{
    beginResetModel();
    mModel = model;
    endResetModel();
}

QModelIndex ObjectTreeModel::createCategoryEntity(int row, int column, Category cat) const
{
    return createIndex(row, column, entityTemplates[row]);
}

QModelIndex ObjectTreeModel::createEntity(int row, int column, objecttree::ObjectTreeEntity *entity) const
{
    return createIndex(row, column, entity);
}

objecttree::ObjectTreeEntity *ObjectTreeModel::entity(const QModelIndex &index) const
{
    return (objecttree::ObjectTreeEntity*)index.internalPointer();
}
