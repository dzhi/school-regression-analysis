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
    void on_openBlankButton_clicked();

private:
    Ui::SelectDataSource *ui;

signals:
    void importButtonClicked();
    void openBlankButtonClicked();
};

#endif // SELECTDATASOURCE_H
