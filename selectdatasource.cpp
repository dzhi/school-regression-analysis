#include "selectdatasource.h"
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
    hide();
    emit importButtonClicked();
}

void SelectDataSource::on_openBlankButton_clicked()
{
    hide();
    emit openBlankButtonClicked();
}
