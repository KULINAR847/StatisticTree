
#include "widgettree.h"
#include "./ui_widgettree.h"

#include "filesystemmodel.h"
#include "statisticmodel.h"

#include <QDebug>
#include <QThread>
#include <QTimer>
#include <QSortFilterProxyModel>
#include <QMessageBox>

WidgetTree::WidgetTree(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WidgetTree)
{
    ui->setupUi(this);

    ui->leRootDir->setText(QDir::currentPath());

    m_fileSystemModel =
        QSharedPointer<FilesystemModel>(new FilesystemModel(ui->leRootDir->text(), this), &QObject::deleteLater);

    ui->treeView->setModel(m_fileSystemModel.get());
    ui->treeView->setColumnWidth(0, 500);

    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSortingEnabled(true);
    m_statViewModel =
        QSharedPointer<StatisticModel>(new StatisticModel(this), &QObject::deleteLater);

    m_statViewEmptyModel =
        QSharedPointer<StatisticModel>(new StatisticModel(this), &QObject::deleteLater);
    ui->tableView->setModel(m_statViewModel.get());

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&WidgetTree::updateViewInfo));
}

WidgetTree::~WidgetTree()
{
    delete ui;
    timer->stop();
    if(timer)
    {
        timer->stop();
        delete timer;
    }
}

void WidgetTree::updateViewInfo()
{
    qDebug() << "Timer worked << " << debugCount++;
    timer->stop();

    ui->statusbar->showMessage(QString("Количество папок = %1").arg(m_statisticsDir->getCountDir()));
    m_statViewModel.get()->setExternalMap(m_statisticsDir->getMap());

    QThread* thread = QThread::create(std::bind(&StatisticModel::refillExtension, m_statViewModel));
    connect(thread, &QThread::finished, this, &WidgetTree::startTimerAfter);
    thread->start();
}

void WidgetTree::calcEnd()
{    
    timer->stop();
    updateViewInfo();    
}

void WidgetTree::startTimerAfter()
{
    m_statViewSortModel =
        QSharedPointer<QSortFilterProxyModel>(new QSortFilterProxyModel(this), &QObject::deleteLater);


    m_statViewSortModel.get()->setSourceModel(m_statViewModel.get());
    ui->tableView->setModel(m_statViewSortModel.get());

    m_statViewSortModel.get()->sort(0);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->resizeRowsToContents();

    ui->statusbar->showMessage(QString("Количество папок = %1").arg(m_statisticsDir->getCountDir()));

    timer->start();
}



void WidgetTree::on_treeView_clicked(const QModelIndex &index)
{
    qDebug() << index.data();

    ui->tableView->setModel(m_statViewEmptyModel.get());

    m_statViewModel =
        QSharedPointer<StatisticModel>(new StatisticModel(this), &QObject::deleteLater);
    ui->tableView->setModel(m_statViewModel.get());

    m_statisticsDir =
        QSharedPointer<StatisticDir>(new StatisticDir(this), &QObject::deleteLater);

    // Поправим индекс если случайно будет выбран не тот столбец
    QModelIndex correctIndex = index.siblingAtColumn(0);
    qDebug() << index << "  =  " << correctIndex;

    QString rootDir = m_fileSystemModel.get()->getData(correctIndex, Qt::DisplayRole);
    qDebug() << "Собираем статистику для " << rootDir;
    QThread* thread = QThread::create(std::bind(&StatisticDir::recalcDirStatisticFor, m_statisticsDir.get(), rootDir));
    connect(thread, &QThread::finished, this, &WidgetTree::calcEnd);
    connect(thread, &QThread::finished, thread, &QObject::deleteLater);
    thread->start();
    timer->start(10);
}

void WidgetTree::on_pbDrawTree_clicked()
{
    QDir checkPath(ui->leRootDir->text());
    if(checkPath.exists() && QFileInfo(ui->leRootDir->text()).isDir())
    {
        m_fileSystemModel =
            QSharedPointer<FilesystemModel>(new FilesystemModel(ui->leRootDir->text(), this), &QObject::deleteLater);

        ui->treeView->setModel(m_fileSystemModel.get());
    }
    //    m_fileSystemModel.get()->fetchRootDirectory(ui->leRootDir->text());
    else if (!checkPath.exists())
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Внимание! %1 такого пути не существует!")
                           .arg(ui->leRootDir->text()));
        msgBox.exec();
        qDebug() << ui->leRootDir->text() + " такого пути не существует!";
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Внимание! %1 - это не директория!")
                           .arg(ui->leRootDir->text()));
        msgBox.exec();
        qDebug() << ui->leRootDir->text() +  " - это не директория!";
    }
}




