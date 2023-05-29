
#ifndef WIDGETTREE_H
#define WIDGETTREE_H

#include <QMainWindow>
#include "statisticdir.h"


QT_BEGIN_NAMESPACE
namespace Ui { class WidgetTree; }
QT_END_NAMESPACE

#include <QThread>
#include <QSortFilterProxyModel>

class StatisticModel;
class FilesystemModel;
class WidgetTree : public QMainWindow
{
    Q_OBJECT

public:
    WidgetTree(QWidget *parent = nullptr);
    ~WidgetTree();

private:
    QTimer *timer = nullptr;
    int debugCount = 0;

private slots:
    // Обновляем информацию в визуальных частях статистики(промежуточное)
    void updateViewInfo();
    // Обновляем информацию в визуальных частях статистики(итоговое)
    void calcEnd();
    // Выполняем сортировку с помощью прокси модели
    void startTimerAfter();
    // Начинаем расчёт для заданной директории
    void on_treeView_clicked(const QModelIndex &index);
    // Устанавливаем новую модель дерева
    void on_pbDrawTree_clicked();

private:
    QSharedPointer<StatisticDir> m_statisticsDir;
    QSharedPointer<StatisticModel> m_statViewModel;
    QSharedPointer<StatisticModel> m_statViewEmptyModel;
    QSharedPointer<QSortFilterProxyModel> m_statViewSortModel;
    QSharedPointer<FilesystemModel> m_fileSystemModel;
    Ui::WidgetTree *ui;
};

#endif // WIDGETTREE_H
