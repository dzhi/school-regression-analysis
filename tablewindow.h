#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QMainWindow>

namespace Ui {
class TableWindow;
}

class TableWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TableWindow(QString path, QWidget *parent = 0);
    ~TableWindow();

private:
    Ui::TableWindow *ui;
};

#endif // TABLEWINDOW_H
