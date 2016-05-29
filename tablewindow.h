#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QString>

#include "alglib/statistics.h"
#include "alglib/stdafx.h"
#include "qcustomplot.h"
#include "selectdatasource.h"

namespace Ui {
    class TableWindow;
}

class TableWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TableWindow(QWidget *parent = 0);
    ~TableWindow();

private slots:
    void showThisWindow();
    void on_selectColumnsButton_clicked();
    void importCSVFile();
    void onPlotMouseMove(QMouseEvent* event);
    void onPlotMouseClick(QCPAbstractPlottable* plottable, QMouseEvent* event);
    void on_actionImport_from_CSV_triggered();
    void on_actionExport_triggered();

private:
    Ui::TableWindow *ui;
    SelectDataSource* sds;
    QVector<double> selectedXValues;
    QVector<double> selectedYValues;

    int findIndexOfNearbyDataPoint(double pixelXGiven, double pixelYGiven, double pixelDistanceThreshold);
    void importDataFromCsv(QString path);
    void plotData(QVector<double> &x, QVector<double> &y, QString xAxisLabel, QString yAxisLabel);
    alglib::real_1d_array QVectorToALGLIBArray(QVector<double> &v) const;
    int getMinAndMaxVals(QVector<double> &v, double &min, double &max) const;
};

#endif // TABLEWINDOW_H
