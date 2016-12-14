// --------------------------------------------------------------------------
// DISPLACE: DYNAMIC INDIVIDUAL VESSEL-BASED SPATIAL PLANNING
// AND EFFORT DISPLACEMENT
// Copyright (c) 2012, 2013, 2014, 2015, 2016 Francois Bastardie <fba@aqua.dtu.dk>

//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// --------------------------------------------------------------------------

#include "windmillentity.h"

#include <objecttreemodel.h>
#include <displacemodel.h>

namespace objecttree {

WindmillEntity::WindmillEntity(ObjectTreeModel *_model, int id)
    : ObjectTreeEntity(_model),
      mWindmillId(id)
{
}

QModelIndex WindmillEntity::parent(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return model->createCategoryEntityFromChild(ObjectTreeModel::Windmills);
}

QModelIndex WindmillEntity::index(int row, int column, const QModelIndex &parent) const
{
    Q_UNUSED(parent);

    ObjectTreeEntity * entity = new WindmillEntity(model, row);
  //   std::cout << "id (row) is assigned here and is " << row << endl;
    return model->createEntity(row, column, entity);
}

int WindmillEntity::rowCount() const
{
    if (mWindmillId == -1 && model->getModel() != 0)
        return model->getModel()->getWindmillCount();

    return 0;
}

int WindmillEntity::columnCount() const
{
    return 1;
}

QVariant WindmillEntity::data(const QModelIndex &index, int role) const
{
    if (mWindmillId != -1 && model->getModel() != 0 && index.column() == 0) {
        if (role == Qt::DisplayRole)
            return model->getModel()->getWindmillId(mWindmillId);
        if (role == Qt::ToolTipRole) {
            std::shared_ptr<WindmillData> ff = model->getModel()->getWindmillList()[mWindmillId];
            return QString("%1 %2").arg(ff->mWindmill->get_y()).arg(ff->mWindmill->get_x());
        }
    }

    return QVariant();
}

}