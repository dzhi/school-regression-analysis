#include "selectdatasource.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SelectDataSource w;
    w.show();

    return a.exec();
}
