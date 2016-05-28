#include "tablewindow.h"
#include "ui_tablewindow.h"
#include "qcustomplot.h"
#include "csvparser.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <QString>
#include <QStandardItemModel>
#include <vector>
#include <QFileDialog>
#include <QAbstractItemModel>

using namespace std;
using namespace alglib;

TableWindow::TableWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TableWindow),
    sds(new SelectDataSource)
{
    ui->setupUi(this);

    QCustomPlot* p = ui->plot;
    p->addGraph();

    QCPScatterStyle myScatter;
    myScatter.setShape(QCPScatterStyle::ssDisc);
    myScatter.setPen(QPen(Qt::blue));
    myScatter.setBrush(Qt::white);
    myScatter.setSize(5);
    p->graph(0)->setScatterStyle(myScatter);

    p->graph(0)->setLineStyle(QCPGraph::lsNone);

    p->setInteraction(QCP::iRangeDrag, true);
    p->setInteraction(QCP::iRangeZoom, true);

    connect(sds, SIGNAL(importButtonClicked()), this, SLOT(importCSVFile()));
    sds->show();
}

TableWindow::~TableWindow()
{
    delete ui;
    delete sds;
}

void TableWindow::showThisWindow()
{
    raise();
    activateWindow();
    show();
}

void TableWindow::importCSVFile()
{
    QString fileName = QFileDialog::getOpenFileName(
            this,
            tr("Open File"),
            QDir::homePath(),
            tr("CSV Files (*.csv)"));

    if (!fileName.isEmpty())
        importDataFromCsv(fileName);

    showThisWindow();
}

void TableWindow::importDataFromCsv(QString path)
{
    CsvParser *csvparser = CsvParser_new(path.toStdString().c_str(), ",", 0);
    CsvRow *row;
    ifstream file(path.toStdString());
    string value;
    vector<vector<QString>> v;
    string line;
    while(row = CsvParser_getRow(csvparser))
    {
        vector<QString> vrow;
        const char **rowFields = CsvParser_getFields(row);
        for (int  i = 0 ; i < CsvParser_getNumFields(row) ; i++) {
            vrow.push_back(rowFields[i]);
        }
        v.push_back(vrow);
    }
    QStandardItemModel *model = new QStandardItemModel(v.size()-1,v[0].size(),this);
    cout << v[v.size()-1].size() << ", " <<v.size() <<endl;
    for(int i = 0; i < v.size(); i++)
    {
        for(int j = 0; j < v[i].size(); j++)
        {
            if(i == 0)
                model->setHorizontalHeaderItem(j, new QStandardItem(v[i][j]));
            else
                model->setItem(i-1,j, new QStandardItem(v[i][j]));
        }
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();

    for(int i = 0; i < v[0].size(); i++) {
        ui->column1_comboBox->addItem(v[0][i]);
        ui->column2_comboBox->addItem(v[0][i]);
    }
}

void TableWindow::on_selectColumnsButton_clicked()
{
    int c1 = ui->column1_comboBox->currentIndex();
    int c2 = ui->column2_comboBox->currentIndex();
    QAbstractItemModel* model = ui->tableView->model();
    int numDataPoints = model->rowCount();
    QVector<double> x(numDataPoints);
    QVector<double> y(numDataPoints);

    for (int i = 0; i < numDataPoints; i++)
    {
        x[i] = model->data(model->index(i, c1)).toDouble();
        y[i] = model->data(model->index(i, c2)).toDouble();
    }

    plotData(x, y, "x", "y");

    double v;
    real_1d_array xs = QVectorToALGLIBArray(x);
    real_1d_array ys = QVectorToALGLIBArray(y);

    v = cov2(xs, ys);
    v = pearsoncorr2(xs, ys);
    ui->corrTextEdit->setText(QString::number(v));
    v = spearmancorr2(xs, ys);
    //ui->corrTextEdit->setText(QString::number(v));
}

// Graphing

void TableWindow::plotData(QVector<double> &x, QVector<double> &y, QString xAxisLabel, QString yAxisLabel)
{
    QCustomPlot* p = ui->plot;
    p->graph(0)->setData(x, y);
    p->xAxis->setLabel(xAxisLabel);
    p->yAxis->setLabel(yAxisLabel);

    double xMin, xMax, yMin, yMax;
    getMinAndMaxVals(x, xMin, xMax);
    getMinAndMaxVals(y, yMin, yMax);
    double xRangeDiff = 0.2 * (xMax - xMin);
    double yRangeDiff = 0.2 * (yMax - yMin);

    p->xAxis->setRange(xMin - xRangeDiff, xMax + xRangeDiff);
    p->yAxis->setRange(yMin - yRangeDiff, yMax + yRangeDiff);
    p->replot();
}

real_1d_array TableWindow::QVectorToALGLIBArray(QVector<double> &v) const
{
    QString s = "[";
    if (v.size() > 0)
        s.append(QString::number(v[0]));
    for (int i = 1; i < v.size(); i++)
    {
        s.append(",");
        s.append(QString::number(v[i]));
    }
    s.append("]");

    real_1d_array r1da = s.toStdString().c_str();
    return r1da;
}

int TableWindow::getMinAndMaxVals(QVector<double> &v, double &min, double &max) const
{
    if (v.size() <= 0)
        return -1;

    min = max = v[0];
    for (int i = 1; i < v.size(); i++)
    {
        if (v[i] < min)
            min = v[i];
        if (v[i] > max)
            max = v[i];
    }

    return 0;
}
