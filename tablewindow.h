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
    explicit TableWindow(QWidget *parent = 0);
    void importDataFromCsv(QString path);
    ~TableWindow();

private slots:
    void on_pushButton_clicked();

private:
    Ui::TableWindow *ui;
};

#endif // TABLEWINDOW_H
