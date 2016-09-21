#include "maplistadapter.h"

#include <LayerESRIShapefile.h>
#include <ESRIShapefile.h>
#include <QMapControl.h>

#include <QFile>
#include <QMessageBox>
#include <QDebug>

#include <sstream>

MapListAdapter::MapListAdapter(qmapcontrol::QMapControl *control)
    : mMapController(control)
{

}

void MapListAdapter::clearPaths()
{
    mPaths.clear();
}

void MapListAdapter::addPath(const QString &path)
{
    mPaths << path;
}

void MapListAdapter::refresh()
{
    bool errorShown = false;

    QStringList filter;
    filter << "*.shp";

    /// @TODO Optimize this. We can cache paths that aren't changed.
    mEntries.clear();

    for (auto path : mPaths) {
        QDir f(path);
        auto files = f.entryInfoList(filter, QDir::Files);

        for (auto file: files) {
            QString name = file.filePath().mid(path.length());
            try {
                mEntries.emplace_back(std::move(MapEntry(file.filePath(), mMapController, name)));
            } catch (std::runtime_error &x) {
                qWarning() << "Cannot load : " << file.filePath();
                if (!errorShown) {
                    QMessageBox::warning(nullptr, tr("Failed loading shapefiles."),
                                         tr("There was a problem loading some shapefile. Please check the data files."));
                    errorShown = true;
                }
            }
        }
    }
}


int MapListAdapter::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return mEntries.size();
}

QVariant MapListAdapter::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return mEntries[index.row()].getName();
    case Qt::CheckStateRole:
        return mEntries[index.row()].isVisible() ? Qt::Checked : Qt::Unchecked;
    }

    return QVariant();
}

Qt::ItemFlags MapListAdapter::flags(const QModelIndex &index) const
{
    return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable;
}

bool MapListAdapter::setData(const QModelIndex &index, const QVariant &value, int role)
{
    switch (role) {
    case Qt::CheckStateRole:
        mEntries[index.row()].setVisible(value.toBool());
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

MapListAdapter::MapEntry::MapEntry(const QString &path, qmapcontrol::QMapControl *controller, QString name)
    : mPath (path), mName(name), mController(controller)
{
    mDataSource = OGRSFDriverRegistrar::Open(path.toStdString().c_str(), FALSE);
    if (!mDataSource) {
        // throw
        std::ostringstream ss;
        ss << "Cannot open shapefile " << mPath.toStdString();
        throw std::runtime_error(ss.str());
    }

    auto file = std::make_shared<qmapcontrol::ESRIShapefile>(mDataSource, "");

    file->setPenPolygon(QPen(Qt::red));
    QColor col (Qt::yellow);
    col.setAlpha(64);
    file->setBrushPolygon(QBrush(col));

    mLayer = std::make_shared<qmapcontrol::LayerESRIShapefile>(mPath.toStdString());
    mLayer->addESRIShapefile(file);
    mLayer->setVisible(false);

    mController->addLayer(mLayer);
}

MapListAdapter::MapEntry::~MapEntry()
{
    qDebug() << "Destroy: " << mPath;
    mController->removeLayer(mPath.toStdString());

    //OGRSFDriverRegistrar::GetRegistrar()->ReleaseDataSource(mDataSource);
}

QString MapListAdapter::MapEntry::getName() const
{
    return mName;
}

bool MapListAdapter::MapEntry::isVisible() const
{
    return mLayer->isVisible();
}

void MapListAdapter::MapEntry::setVisible(bool visible)
{
    mLayer->setVisible(visible);
}
