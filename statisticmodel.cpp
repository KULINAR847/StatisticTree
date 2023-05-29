
#include "statisticmodel.h"
#include "statisticdir.h"
#include "extensionnode.h"
#include <QString>
#include <QAbstractTableModel>

#include <QDebug>

StatisticModel::StatisticModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int StatisticModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED( parent )
    return m_ExtData.count();
}

int StatisticModel::columnCount( const QModelIndex& parent ) const {
    Q_UNUSED( parent )
    return LAST;
}

QVariant StatisticModel::headerData( int section, Qt::Orientation orientation, int role ) const {
    if( role != Qt::DisplayRole ) {
        return QVariant();
    }

    if( orientation == Qt::Vertical ) {
        return section;
    }

    switch( section ) {
    case EXTENSION:
        return  "Расширение" ;
    case COUNT_FILES:
        return  "Кол-во файлов" ;
    case COMMON_SIZE:
        return  "Общий размер" ;
    case AVG_SIZE:
        return  "Средний размер" ;
    }

    return QVariant();
}

QVariant StatisticModel::data( const QModelIndex& index, int role ) const {
    if(
        !index.isValid() ||
        m_ExtData.count() <= index.row() || m_ExtData.size() <= index.row() ||
        ( role != Qt::DisplayRole )//&& role != Qt::EditRole )
        ) {
        return QVariant();
    }

    return m_ExtData[ index.row() ][ Column( index.column() ) ];
}

bool StatisticModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if( !index.isValid() || m_ExtData.count() <= index.row() ) { // || role != Qt::EditRole
        return false;
    }

    m_ExtData[ index.row() ][ Column( index.column() ) ] = value;
    emit dataChanged( index, index );

    return true;
}

Qt::ItemFlags StatisticModel::flags( const QModelIndex& index ) const {
    Qt::ItemFlags flags = QAbstractTableModel::flags( index );
    return flags;
}

void StatisticModel::clearModel()
{
    extMap.clear();
    m_ExtData.clear();
}

void StatisticModel::updateExtension( const QString& extension, const int& countFiles, const long long int& size ) {
    StatisticData data;
    data[ EXTENSION ] = extension;
    data[ COUNT_FILES ] = countFiles;
    data[ COMMON_SIZE ] = size;
    data[ AVG_SIZE ] = countFiles > 0 ? (long long int)(size / countFiles) : 0;    

    if( extMap.keys().contains(extension) )
    {
        int row = extMap[extension];
        m_ExtData[row] = data;
        QModelIndex topLeft = createIndex(row,row);
        emit dataChanged(topLeft, topLeft);
    }
    else
    {
        int row = m_ExtData.count();
        extMap[extension] = row;
        beginInsertRows( QModelIndex(), row, row );
        data[ LAST ] = row;
        m_ExtData.append( data );
        endInsertRows();
    }
}

void StatisticModel::refillExtension()
{
    int count = 0;

    for(const auto &e : externalExtMap->keys())
    {
        updateExtension(e, (externalExtMap->value(e))->getFileCount(), (externalExtMap->value(e))->getFilesSize());
    }
}

void StatisticModel::setExternalMap(QMap<QString, ExtensionNode *> *map)
{
    externalExtMap = map;
}

