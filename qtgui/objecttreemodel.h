#ifndef OBJECTTREEMODEL_H
#define OBJECTTREEMODEL_H

#include <QAbstractItemModel>
#include <QList>

class DisplaceModel;
class MapObjectsController;

namespace objecttree {
class ObjectTreeEntity;
}

class ObjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    enum Category {
        Layers = 0,
        OutputLayers,
        Nodes, Harbours, Vessels, Populations, Benthos,
        LastCategory
    };

    explicit ObjectTreeModel(MapObjectsController *map, QObject *parent = 0);

    int columnCount(const QModelIndex &parent) const;
    int rowCount(const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags ( const QModelIndex & index ) const;
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );

    void setCurrentModel (int idx, DisplaceModel *model);
    int getModelIdx() const { return mModelIdx; }
    DisplaceModel *getModel() const { return mModel; }
    MapObjectsController *getMapControl() const { return mMapControl; }

    // For use from ObjectTreeEntities
    QModelIndex createCategoryEntity(int row, int column, Category cat) const;
    QModelIndex createCategoryEntityFromChild (Category cat) const;
    QModelIndex createEntity (int row, int column, objecttree::ObjectTreeEntity *entity) const;

    bool isObject(QModelIndex index) const;
    Category getCategory (QModelIndex index) const;
    objecttree::ObjectTreeEntity *entity (const QModelIndex &index) const;

signals:

public slots:

private:
    MapObjectsController *mMapControl;
    DisplaceModel *mModel;
    int mModelIdx;

protected:
    // helper functions

    bool isRootLevel (const QModelIndex &level) const {
        return !level.isValid();
    }

    bool isCategoryLevel (const QModelIndex &level) const { // level.internalId() > 0 && level.internalId() < LastCategory
        return level.isValid() &&
                (level.row() < entityTemplates.size() &&
                     level.internalPointer() == entityTemplates[level.row()]);
    }

    static QList<objecttree::ObjectTreeEntity *> entityTemplates;
    static QString entityNames[];
};

#endif // OBJECTTREEMODEL_H
