#ifndef MAINWINDOW2_H
#define MAINWINDOW2_H

#include <QMainWindow>

namespace Ui {
class MainWindow2;
}

class TableWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TableWindow(QString path, QWidget *parent = 0);
    ~TableWindow();

private:
    Ui::MainWindow2 *ui;
};

#endif // MAINWINDOW2_H
