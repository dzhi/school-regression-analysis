#ifndef SELECTDATASOURCE_H
#define SELECTDATASOURCE_H

#include <QDialog>

namespace Ui {
class SelectDataSource;
}

class SelectDataSource : public QDialog
{
    Q_OBJECT

public:
    explicit SelectDataSource(QWidget *parent = 0);
    ~SelectDataSource();

private slots:
    void on_importFromCsvButton_clicked();

private:
    Ui::SelectDataSource *ui;
};

#endif // SELECTDATASOURCE_H
