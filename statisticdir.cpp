
#include "statisticdir.h"
#include "extensionnode.h"

#include <QDir>
#include <QDirIterator>
#include <QMessageBox>
#include <QDebug>
#include <QChar>
#include <QString>

#define RES_KEY_NAME QString("ИТОГО")

StatisticDir::StatisticDir(QObject *parent)
    : QObject{parent}
{
    dirCount = 0;
    resCountFiles = 0;
    resSize = 0;
}

StatisticDir::~StatisticDir()
{
    clearMap();
}

StatisticDir::operator QString() const
{
    return "StatisticDir DIRS = " + QString::number(dirCount);
}

void StatisticDir::recalcDirStatisticFor(QString dirPath)
{
    qDebug() << "start = " << dirPath;
    // Обновим переменные
    dirCount = 0;
    resCountFiles = 0;
    resSize = 0;
    clearMap();

    extInfo[RES_KEY_NAME] = new ExtensionNode(RES_KEY_NAME);
    extInfo[RES_KEY_NAME]->setStatistic(resCountFiles, resSize);
    //qDebug() << "exists = " << dir.exists(dirPath);
    // Проверим, что такая директория существует
    if(dir.exists(dirPath))
    {
        // Путь до файла или директории в операционной системе
        QString nodePath;
        // Данные о файле или директории
        QFileInfo checkNode;
        // Расширение файла
        QString extension;

        // Получим сразу количество подпапок
        QDirIterator itDir(dirPath, QDir::NoDotAndDotDot | QDir::AllEntries); //, QDirIterator::Subdirectories);
        while (itDir.hasNext()) {
            checkNode.setFile(itDir.next());
            if(checkNode.isDir())
            {
                ++dirCount;
            }
        }

        // Проделаем динамический расчёт для файлов
        QDirIterator it(dirPath, QDir::NoDotAndDotDot | QDir::AllEntries); //, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            nodePath = it.next();
            checkNode.setFile(nodePath);

            // Если директория пройдём её рекурсивно
            if(checkNode.isDir())
            {
                recalcRecursiveInfo(nodePath);
            }
            // Если файл выполним расчёт
            else if(checkNode.isFile())
            {
                resCountFiles++;
                extension = checkNode.completeSuffix();

                if(!extInfo.keys().contains(extension))
                    extInfo[extension] = new ExtensionNode(extension);

                resSize += extInfo[extension]->addNode(nodePath);
            }

            // Обновим итоговую статистику каждые 100 файлов
            if(resCountFiles % 100 == 0)
                extInfo[RES_KEY_NAME]->setStatistic(resCountFiles, resSize);
        }

        extInfo[RES_KEY_NAME]->setStatistic(resCountFiles, resSize);
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Внимание! Такой директории не существует %1")
                           .arg(dirPath));
        msgBox.exec();
    }

    qDebug() << "end = " << dirPath;
}

void StatisticDir::printMap()
{
    for(const auto &e : extInfo.toStdMap())
    {
        qDebug() << e.first << "," << QString(*e.second) << '\n';
    }
}

void StatisticDir::recalcRecursiveInfo(QString dirPath)
{
    // Путь до файла или директории в операционной системе
    QString nodePath;
    // Данные о файле или директории
    QFileInfo checkNode;
    // Расширение файла
    QString extension;

    QDirIterator it(dirPath, QDir::NoDotAndDotDot | QDir::AllEntries); //, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        nodePath = it.next();
        checkNode.setFile(nodePath);

        if(checkNode.isDir())
        {
            //qDebug() << nodePath;
            recalcRecursiveInfo(nodePath);
        }
        else if(checkNode.isFile())
        {
            resCountFiles++;
            extension = checkNode.completeSuffix();

//            if(extension == "")
//                extension = "no.extension";

            if(!extInfo.keys().contains(extension))
                extInfo[extension] = new ExtensionNode(extension);

            resSize += extInfo[extension]->addNode(nodePath);
        }

        if(resCountFiles % 100 == 0)
            extInfo[RES_KEY_NAME]->setStatistic(resCountFiles, resSize);
    }
}

void StatisticDir::clearMap()
{
    qDeleteAll( extInfo );  //  Не забываем очистить память словаря
    extInfo.clear();        //  Очищаем словарь
}

QMap<QString, ExtensionNode *> *StatisticDir::getMap()
{
    return &extInfo;
}

int StatisticDir::getCountDir()
{
    return dirCount;
}

