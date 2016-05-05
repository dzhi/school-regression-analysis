#include <QFileDialog>
#include "selectdatasource.h"
#include "tablewindow.h"
#include "ui_selectdatasource.h"

SelectDataSource::SelectDataSource(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDataSource)
{
    ui->setupUi(this);
}

SelectDataSource::~SelectDataSource()
{
    delete ui;
}

void SelectDataSource::on_importFromCsvButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open File"),
            QDir::homePath(),
            tr("CSV Files (*.csv)"));

    if (!fileName.isEmpty()) {
        TableWindow* tableWindow = new TableWindow(); // leak
        tableWindow->importDataFromCsv(fileName);
        tableWindow->show();
        this->hide();
    }
}
