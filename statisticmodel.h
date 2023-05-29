
#ifndef STATISTICMODEL_H
#define STATISTICMODEL_H

#include <QAbstractTableModel>

class ExtensionNode;
typedef QMap<QString, ExtensionNode *> * ExtensionMap;

/*************************************************************************
 *   Пример класса который строит модель собранной статистики
 *   Визуальная часть
*************************************************************************/

class StatisticModel : public QAbstractTableModel {
    Q_OBJECT
public:
    StatisticModel( QObject* parent = 0 );

    int rowCount( const QModelIndex& parent ) const;
    int columnCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    bool setData( const QModelIndex& index, const QVariant& value, int role );
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;
    Qt::ItemFlags flags( const QModelIndex& index ) const;

    void clearModel();
    // Распределяем полученные данные или обновляем статистику в строке
    // или добавляем новую строку с расширением
    void updateExtension(const QString &extension, const int &countFiles, const long long &size);
    // Перезаполняем модель
    void refillExtension();
    // Связываем внешний словарь статистики с текущим объектом
    void setExternalMap(QMap<QString, ExtensionNode *> *map);

private:
    enum Column {
        EXTENSION = 0,
        COUNT_FILES,
        COMMON_SIZE,
        AVG_SIZE,
        LAST
    };

    typedef QHash< Column, QVariant > StatisticData;
    typedef QList< StatisticData > Extensions;
    QMap<QString, ExtensionNode *> *externalExtMap = nullptr;

    QMap<QString, int> extMap;
    Extensions m_ExtData;
};

#endif // STATISTICMODEL_H
