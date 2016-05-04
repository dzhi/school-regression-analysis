#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QFileSystemModel>

namespace Ui {
class MainWindow;
}

class ExplorerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ExplorerWindow(QWidget *parent = 0);
    ~ExplorerWindow();

private slots:
    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QFileSystemModel *dirmodel;
};

#endif // MAINWINDOW_H
