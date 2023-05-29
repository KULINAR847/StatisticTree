
#include "extensionnode.h"
#include <QFileInfo>
#include <QMessageBox>

ExtensionNode::ExtensionNode(QString extension, QObject *parent)
    : QObject{parent}
{
    size = 0;
    fileCount = 0;

    this->extension = extension;
}

long long int ExtensionNode::addNode(QString fullPath)
{
    fileInfo.setFile(fullPath);

    if(fileInfo.completeSuffix() != this->extension)
    {
        QMessageBox msgBox;
        msgBox.setText(QString("Внимание! Несовпадение расширений текущего %1 и добавляемого %2")
                           .arg(this->extension).arg(fileInfo.completeSuffix()));
        msgBox.exec();
    }
    else
    {
        // Аккумулируем размер файла и количество файлов
        size += fileInfo.size();
        ++fileCount;
    }

    return fileInfo.size();
}

ExtensionNode::operator QString() const
{
    return "ExtensionNode[" + extension + "] = " + QString::number(fileCount) + "/" +  QString::number(size);
}

int ExtensionNode::getFileCount()
{
    return fileCount;
}

long long ExtensionNode::getFilesSize()
{
    return size;
}

void ExtensionNode::setStatistic(int fileCount, long long size)
{
    this->fileCount = fileCount;
    this->size = size;
}

