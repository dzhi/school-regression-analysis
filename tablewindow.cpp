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

TableWindow::TableWindow(QString path, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TableWindow)
{
    ifstream file(path.toStdString());
    string value;
    vector<vector<QString>> v;
    ui->setupUi(this);
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
            ui->tableView->resize(1000,1000);
}

TableWindow::~TableWindow()
{
    delete ui;
}
