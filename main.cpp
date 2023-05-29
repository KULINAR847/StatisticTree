
#include "widgettree.h"

#include <QApplication>
#include <QFile>
#include <QTreeView>
#include <QTableView>
#include <QGridLayout>
#include <QStandardItem>
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WidgetTree tree;
    tree.show();

    return a.exec();
}
