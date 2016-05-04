#include "explorerwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ExplorerWindow w;
    w.show();

    return a.exec();
}
