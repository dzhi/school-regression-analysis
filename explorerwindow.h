#ifndef EXPLORERWINDOW_H
#define EXPLORERWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QFileSystemModel>

namespace Ui {
class ExplorerWindow;
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
    Ui::ExplorerWindow *ui;
    QFileSystemModel *dirmodel;
};

#endif // EXPLORERWINDOW_H
