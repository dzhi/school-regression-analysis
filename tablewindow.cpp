#include "tablewindow.h"
#include "ui_tablewindow.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <QString>
#include <QStandardItemModel>
#include <vector>

using namespace std;

TableWindow::TableWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TableWindow)
{
    ui->setupUi(this);
}

void TableWindow::importDataFromCsv(QString path)
{
    ifstream file(path.toStdString());
    string value;
    vector<vector<QString>> v;
    QStandardItemModel *model = new QStandardItemModel(5,5,this);
        string line;
        while(getline(file,line))
        {
            vector<QString> row;
            stringstream lineStream(line);
            string cell;
            while(getline(lineStream,cell,',')) {
                row.push_back(QString::fromStdString(cell));
            }
            v.push_back(row);
        }
        for(int i = 0; i < 6; i++)
        {
            for(int j = 0; j < 5; j++)
            {
                if(i == 0)
                    model->setHorizontalHeaderItem(j, new QStandardItem(v[i][j]));
                else
                    model->setItem(i-1,j, new QStandardItem(v[i][j]));
            }
        }
        ui->tableView->setModel(model);
        ui->tableView->resizeColumnsToContents();

    for(int i = 0; i < 5; i++) {
        ui->comboBox->addItem(v[0][i]);
        ui->comboBox_2->addItem(v[0][i]);
    }
}

TableWindow::~TableWindow()
{
    delete ui;
}

//choose variable 1
void TableWindow::on_pushButton_clicked()
{
    int index1 = ui->comboBox->currentIndex();
}

//choose variable 2
void TableWindow::on_pushButton_2_clicked()
{
    int index2 = ui->comboBox->currentIndex();
}
