#ifndef RSFILEMODEL_H
#define RSFILEMODEL_H

#include <QAbstractItemModel>
#include <QVector>
#include "rsfileitem.h"
class RSFileModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit RSFileModel(QObject *parent = 0);
        ~RSFileModel();
    void addRSFileItem(RSFileItem *file);
    RSFileItem* getRSFileItem(const QModelIndex &index);
    void setRSFileItemStatus(const QModelIndex &index,int Status);

    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
private:
    QVector<RSFileItem*> dataList;
};

#endif // RSFILEMODEL_H
