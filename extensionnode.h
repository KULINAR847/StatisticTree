
#ifndef EXTENSIONNODE_H
#define EXTENSIONNODE_H


#include <QObject>
#include <QString>
#include <QFileInfo>

/*************************************************************************
 *   Пример класса который считает необходимую информацию
 *   для определённого расширения файлов в директории
*************************************************************************/

class ExtensionNode : public QObject
{
    Q_OBJECT
public:
    explicit ExtensionNode(QString extension, QObject *parent = nullptr);

    // Добавление нового файла с текущим расширением и пересчёт счётчиков
    long long int addNode(QString fullPath);
    // Строковое представление объекта для удобства отладки
    operator QString () const;

    // Методы для получения данных о собранной информации по расширению
    int getFileCount();    
    long long int getFilesSize();

    // Нужен для удобства обновления информации в секции ИТОГО
    void setStatistic(int fileCount, long long int size);

private:
    // Создадим один раз с объектом, чтобы не создавать в методе каждый раз
    QFileInfo fileInfo;
    // Общий размер файлов
    long long int size;
    // Количество файлов
    int fileCount;
    // Расширение текущей ноды(для проверки не сделали ли ошибку)
    QString extension;

signals:

};

#endif // EXTENSIONNODE_H
