#include "tablewindow.h"
#include "ui_tablewindow.h"
#include "qcustomplot.h"
#include "csvparser.h"

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
    connect(p, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onPlotMouseMove(QMouseEvent*)));
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

void TableWindow::onPlotMouseMove(QMouseEvent *event) {
    QCustomPlot* p = ui->plot;
    double eventPixelX = event->pos().x();
    double eventPixelY = event->pos().y();

    double distanceThreshold = 15.0;

    for (int i=0; i<this->selectedXValues.size() && i<this->selectedYValues.size(); i++) {
        double coordX = this->selectedXValues[i];
        double coordY = this->selectedYValues[i];
        double pixelX = p->xAxis->coordToPixel(coordX);
        double pixelY = p->yAxis->coordToPixel(coordY);
        if (abs(pixelX-eventPixelX) + abs(pixelY-eventPixelY) < distanceThreshold) {
            QToolTip::showText(event->globalPos(),
                    QString::number(coordX) + QString(", ") + QString::number(coordY));
        }
    }
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
    size_t numRows = v.size();
    size_t numCols = v[0].size();
    for(int i = 0; i < numRows; i++)
    {
        for(int j = 0; j < numCols; j++)
        {
            if(i == 0)
                model->setHorizontalHeaderItem(j, new QStandardItem(v[i][j]));
            else
                model->setItem(i-1,j, new QStandardItem(v[i][j]));
        }
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();

    for(int i = 0; i < numCols; i++) {
        ui->column1_comboBox->addItem(v[0][i]);
        ui->column2_comboBox->addItem(v[0][i]);
    }
}

void TableWindow::on_selectColumnsButton_clicked()
{
    int c1 = ui->column1_comboBox->currentIndex();
    int c2 = ui->column2_comboBox->currentIndex();
    QString label1 = ui->column1_comboBox->currentText();
    QString label2 = ui->column2_comboBox->currentText();
    QAbstractItemModel* model = ui->tableView->model();
    int numDataPoints = model->rowCount();
    selectedXValues = QVector<double>(numDataPoints);
    selectedYValues = QVector<double>(numDataPoints);

    for (int i = 0; i < numDataPoints; i++)
    {
        selectedXValues[i] = model->data(model->index(i, c1)).toDouble();
        selectedYValues[i] = model->data(model->index(i, c2)).toDouble();
    }

    plotData(selectedXValues, selectedYValues, label1, label2);

    double v;
    real_1d_array xs = QVectorToALGLIBArray(selectedXValues);
    real_1d_array ys = QVectorToALGLIBArray(selectedYValues);

    v = cov2(xs, ys); // Covariance
    ui->covTextEdit->setText(QString::number(v));
    v = pearsoncorr2(xs, ys); // Correlation
    ui->corrTextEdit->setText(QString::number(v));
    v = v*v; // R-squared
    ui->RsquaredTextEdit->setText(QString::number(v));
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
