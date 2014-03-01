
#include "rsfilemodel.h"
#include <QDebug>
RSFileModel::RSFileModel(QObject *parent) :
    QAbstractTableModel(parent)
{

}
QVariant RSFileModel::headerData(int section, Qt::Orientation orientation,
                                 int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Name");
        case 1:
            return tr("Size");
        case 2:
            return tr("Time");
        case 3:
            return tr("Path");
        default:
            return QVariant();
        }
    }

    return QVariant();
}
RSFileModel::~RSFileModel()
{
        qDeleteAll(dataList);
}

void RSFileModel::addRSFileItem(RSFileItem *file)
{
    int count = dataList.count();
    beginInsertRows(QModelIndex(),count,count);
    dataList.push_back(file);
    endInsertRows();
}
RSFileItem * RSFileModel::getRSFileItem(const QModelIndex &index)
{
        RSFileItem * item = NULL;
        if(index.isValid())
        {
                item = dataList[index.row()];
        }
        return item;
}

QVariant RSFileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

//    RSFileItem *item = static_cast<RSFileItem*>(index.internalPointer());

    const RSFileItem *item = dataList[index.row()];
    switch (index.column()) {
    case 0:
        return item->filename();
    case 1:
        return item->filesize();
    case 2:
        return item->modifyTime();
    case 3:
        return item->fullname();
    default:
        return QVariant();
    }
}

Qt::ItemFlags RSFileModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}
int RSFileModel::rowCount(const QModelIndex &parent ) const
{
    return dataList.count();
}
int RSFileModel::columnCount(const QModelIndex &parent ) const
{
    return 4;
}

