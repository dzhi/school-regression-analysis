#include "matrix.h"
#include "ui_matrix.h"
#include "tablewindow.h"
#include "csvparser.h"
#include "qcpdocumentobject.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <QString>
#include <QStandardItemModel>
#include <vector>
#include <QFileDialog>
#include <QAbstractItemModel>
#include <QtMath>
#include <QPrinter>
#include <String>

using namespace std;

Matrix::Matrix(QVector<QVector<double>> data, QVector<QStandardItem*> headers, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Matrix)
{
    ui->setupUi(this);
    QStandardItemModel* model = new QStandardItemModel(data.size(),data.size(),this);
    size_t s = data.size();
    for (size_t i = 0; i < s; i++){
        for (size_t j = 0; j < i; j++){
            model->setItem(i,j,new QStandardItem(QString::number(data[i][j])));
        }
    }
    for (size_t i = 0; i < s; i++){
        model->setHorizontalHeaderItem(i,new QStandardItem(headers[i]->text()));
        model->setVerticalHeaderItem(i,new QStandardItem(headers[i]->text()));
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();
}

Matrix::~Matrix()
{
    delete ui;
}
