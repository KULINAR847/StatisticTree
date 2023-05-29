
#ifndef STATISTICDIR_H
#define STATISTICDIR_H


#include <QObject>
#include <QString>
#include <QMap>
#include <QDir>

class ExtensionNode;
typedef QMap<QString, ExtensionNode *> * ExtensionMap;

/*************************************************************************
 *   Пример класса который считает необходимую информацию
 *   для выбранной директории
 *   В задании не говорилось про скрытые папки, системные, симлинки и т.д.
 *   поэтому расчёт идёт только по "видимым" файлам и папкам
 *   Сама механика расчета
*************************************************************************/

class StatisticDir : public QObject
{
    Q_OBJECT
public:
    explicit StatisticDir(QObject *parent = nullptr);
    ~StatisticDir();

    // Строковое представление объекта для удобства отладки
    operator QString () const;

    // Прямой проход по директории(без рекурсии)
    void recalcDirStatisticFor(QString dirPath);
    // Печать полученных данных по файлам(для удобства отладки)
    void printMap();
    // Корректная очистка данных по файлам
    void clearMap();

    // Получаем доступ к словарю аккумулированной статистики
    QMap<QString, ExtensionNode *> *getMap();
    // Получаем доступ к счётчику подкаталогов
    int getCountDir();

private:
    // Рекурсивный проход по директории
    void recalcRecursiveInfo(QString dirPath);

    // Подсчёт количества папок в директории(!!!Не рекурсивно)
    int dirCount;
    // Собираем статистику по всем расширениям файлов внутри папки
    // !!! Рекурсивно
    QMap<QString, ExtensionNode *> extInfo;
    // Общие счётчики статистики по файлам
    int resCountFiles;
    long long int resSize;

    // Чтобы не создавать в методах
    // для проверки существования файла или директории
    QDir dir;

signals:

};

#endif // STATISTICDIR_H
