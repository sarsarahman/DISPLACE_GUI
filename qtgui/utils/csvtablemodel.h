#ifndef CSVTABLEMODEL_H
#define CSVTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QStringList>

#include <memory>

class CsvTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CsvTableModel(std::shared_ptr<QList<QStringList> > data, QObject *parent = 0);

    int rowCount(const QModelIndex &) const { return mRowCount - (mHeaders ? 1 : 0); }
    int columnCount(const QModelIndex &) const { return mColCount; }

    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    void setSource (std::shared_ptr<QList<QStringList> > data);
    void setFirstLineHeaders(bool x);
signals:

public slots:

private:
    bool mHeaders;
    std::shared_ptr<QList<QStringList> > mData;
    int mRowCount, mColCount;

    void scan();
};

#endif // CSVTABLEMODEL_H
