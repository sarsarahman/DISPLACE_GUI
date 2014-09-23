#include "harbourentity.h"

#include <objecttreemodel.h>
#include <displacemodel.h>
#include <mapcontrol.h>

namespace objecttree {

HarbourEntity::HarbourEntity(ObjectTreeModel *_model, int id)
    : ObjectTreeEntity(_model),
      mHarbourId(id)
{
}

QModelIndex HarbourEntity::parent(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return model->createCategoryEntity(0, 0, ObjectTreeModel::Harbours);
}

QModelIndex HarbourEntity::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    ObjectTreeEntity * entity = new HarbourEntity(model, row);
    return model->createEntity(row, column, entity);
}

int HarbourEntity::rowCount() const
{
    if (mHarbourId == -1 && model->getModel() != 0)
        return model->getModel()->getHarboursCount();

    return 0;
}

int HarbourEntity::columnCount() const
{
    return 1;
}

QVariant HarbourEntity::data(const QModelIndex &index, int role) const
{
    if (mHarbourId == -1 && model->getModel() != 0 && role == Qt::DisplayRole)
        return model->getModel()->getHarbourId(index.row());

    return QVariant();
}

Qt::ItemFlags HarbourEntity::flags(Qt::ItemFlags defflags, const QModelIndex &index) const
{
    return defflags;
}

bool HarbourEntity::setData(const QModelIndex &index, const QVariant &value, int role)
{
    return false;   // item is not editable
}

}
