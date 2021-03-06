#include "tablewindow.h"
#include "ui_tablewindow.h"
#include "csvparser.h"
#include "qcpdocumentobject.h"
#include "matrix.h"

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

using namespace std;
using namespace alglib;

TableWindow::TableWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TableWindow),
    sds(new SelectDataSource),
    regressionValid(false)
{
    ui->setupUi(this);

    QCustomPlot* p = ui->plot;

    // graph(0) is the main QCPGraph which will contain all the data points
    p->addGraph();
    QCPScatterStyle mainScatterStyle;
    mainScatterStyle.setShape(QCPScatterStyle::ssDisc);
    mainScatterStyle.setPen(QPen(QColor(0x0d, 0x47, 0xa1))); // dark blue
    mainScatterStyle.setBrush(Qt::white);
    mainScatterStyle.setSize(5);
    p->graph(0)->setScatterStyle(mainScatterStyle);
    p->graph(0)->setLineStyle(QCPGraph::lsNone);

    // graph(1) will contain the point highlighted when you hover over it
    p->addGraph();
    QCPScatterStyle highlightStyle;
    highlightStyle.setShape(QCPScatterStyle::ssDisc);
    highlightStyle.setPen(QPen(QColor(0x1e, 0x88, 0xe5))); // light blue
    highlightStyle.setBrush(Qt::white);
    highlightStyle.setSize(9);
    p->graph(1)->setScatterStyle(highlightStyle);
    p->graph(1)->setLineStyle(QCPGraph::lsNone);

    // graph(2) will contain the linear regression line
    p->addGraph();
    p->graph(2)->setPen(QPen(QColor(255, 120, 0)));

    p->setInteraction(QCP::iRangeDrag, true);
    p->setInteraction(QCP::iRangeZoom, true);

    connect(sds, SIGNAL(importButtonClicked()), this, SLOT(importCSVFile()));
    connect(sds, SIGNAL(openBlankButtonClicked()), this, SLOT(showThisWindow()));
    connect(p, SIGNAL(mouseMove(QMouseEvent*)), this, SLOT(onPlotMouseMove(QMouseEvent*)));
    connect(p, SIGNAL(plottableClick(QCPAbstractPlottable*, QMouseEvent*)),
            this, SLOT(onPlotMouseClick(QCPAbstractPlottable*, QMouseEvent*)));
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

// Finds a data point whose distance from the given pixel is less than a given threshold, and
// returns the index of that point in the selectedXValues/selectedYValues vectors.
//
// If there are multiple points within the threshold, this function just returns the first one
// found.
// Returns -1 if no points are within the threshold.
int TableWindow::findIndexOfNearbyDataPoint(double pixelXGiven, double pixelYGiven, double pixelDistanceThreshold)
{
    QCustomPlot* p = ui->plot;
    for (int i=0; i<this->selectedXValues.size() && i<this->selectedYValues.size(); i++) {
        double coordX = this->selectedXValues[i];
        double coordY = this->selectedYValues[i];
        double pixelX = p->xAxis->coordToPixel(coordX);
        double pixelY = p->yAxis->coordToPixel(coordY);
        // using taxicab distance instead of Euclidean distance for convenience (it doesn't really matter)
        if (abs(pixelX-pixelXGiven) + abs(pixelY-pixelYGiven) < pixelDistanceThreshold) {
            return i;
        }
    }
    return -1;
}

// Moving the mouse near a data point highlights it, shows a tooltip, and changes the cursor style
// to pointing hand.
void TableWindow::onPlotMouseMove(QMouseEvent *event)
{
    QCustomPlot* p = ui->plot;
    double eventPixelX = event->pos().x();
    double eventPixelY = event->pos().y();

    double distanceThreshold = 4.0;

    int index = findIndexOfNearbyDataPoint(eventPixelX, eventPixelY, distanceThreshold);

    if (index >= 0) {
        double coordX = this->selectedXValues[index];
        double coordY = this->selectedYValues[index];
        double pixelX = p->xAxis->coordToPixel(coordX);
        double pixelY = p->yAxis->coordToPixel(coordY);
        QToolTip::showText(p->mapToGlobal(QPoint(pixelX, pixelY)),
                QString("%1, %2").arg(coordX).arg(coordY));

        QVector<double> xs;
        xs.append(coordX);
        QVector<double> ys;
        ys.append(coordY);
        p->graph(1)->setData(xs, ys);
        p->replot();
        setCursor(Qt::PointingHandCursor);
    } else {
        QToolTip::hideText();
        p->graph(1)->clearData();
        p->replot();
        setCursor(Qt::ArrowCursor);
    }
}

// Clicking a data point selects the corresponding row in the table.
void TableWindow::onPlotMouseClick(QCPAbstractPlottable* plottable, QMouseEvent* event)
{
    double eventPixelX = event->pos().x();
    double eventPixelY = event->pos().y();
    double distanceThreshold = 4.0;
    int index = findIndexOfNearbyDataPoint(eventPixelX, eventPixelY, distanceThreshold);
    if (index >= 0) {
        ui->tableView->selectRow(index);
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
    for(size_t i = 0; i < numRows; i++)
    {
        for(size_t j = 0; j < numCols; j++)
        {
            if(i == 0)
                model->setHorizontalHeaderItem(j, new QStandardItem(v[i][j]));
            else
                model->setItem(i-1,j, new QStandardItem(v[i][j]));
        }
    }
    ui->tableView->setModel(model);
    ui->tableView->resizeColumnsToContents();

    ui->column1_comboBox->clear();
    ui->column2_comboBox->clear();
    for (size_t i = 0; i < numCols; i++) {
        ui->column1_comboBox->addItem(v[0][i]);
        ui->column2_comboBox->addItem(v[0][i]);
    }


    regressionValid = false;
}

void TableWindow::on_selectColumnsButton_clicked()
{
    // Get the selected columns and variable names
    int c1 = ui->column1_comboBox->currentIndex();
    int c2 = ui->column2_comboBox->currentIndex();
    QString label1 = ui->column1_comboBox->currentText();
    QString label2 = ui->column2_comboBox->currentText();
    QAbstractItemModel* model = ui->tableView->model();
    int numDataPoints = model->rowCount();
    selectedXValues = QVector<double>(numDataPoints);
    selectedYValues = QVector<double>(numDataPoints);

    // Get the data points
    for (int i = 0; i < numDataPoints; i++)
    {
        selectedXValues[i] = model->data(model->index(i, c1)).toDouble();
        selectedYValues[i] = model->data(model->index(i, c2)).toDouble();
    }

    // Plot data and update statistical parameters
    plotData(selectedXValues, selectedYValues, label1, label2);
}

void TableWindow::interleaveVectors(QVector<double> &x, QVector<double>& y, QVector<double>& xy)
{
    for (int i = 0; i < x.length() && i < y.length(); i++)
    {
        xy.append(x[i]);
        xy.append(y[i]);
    }
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

    // Store data points in ALGLIB arrays
    real_1d_array xALGLIB, yALGLIB;
    xALGLIB.setcontent(x.length(), x.constData());
    yALGLIB.setcontent(y.length(), y.constData());
    QVector<double> xy;
    interleaveVectors(x, y, xy);
    real_2d_array xyALGLIB;
    ae_int_t numPoints = xy.length()/2;
    xyALGLIB.setcontent(numPoints, 2, xy.constData());

    // Perform linear regression on data
    ae_int_t info;
    ae_int_t numVars;
    linearmodel linModel;
    lrreport rep;
    real_1d_array linReg;
    lrbuild(xyALGLIB, numPoints, 1, info, linModel, rep);
    lrunpack(linModel, linReg, numVars);
    slope = linReg.getcontent()[0];
    yIntercept = linReg.getcontent()[1];
    QVector<double> linRegX(2), linRegY(2);
    linRegX[0] = 0;
    linRegY[0] = yIntercept;
    linRegX[1] = 1.5*xMax;
    linRegY[1] = slope*linRegX[1] + yIntercept;
    // Plot regression line
    p->graph(2)->setData(linRegX, linRegY);

    p->replot();

    ui->trendLineTextEdit->setText(
            QString("y = %1x + %2").arg(slope).arg(yIntercept));

    // Get covariance, correlation, and R-squared values
    double v;
    v = cov2(xALGLIB, yALGLIB); // Covariance
    ui->covTextEdit->setText(QString::number(v));
    v = pearsoncorr2(xALGLIB, yALGLIB); // Correlation
    ui->corrTextEdit->setText(QString::number(v));
    v = v*v; // R-squared
    ui->RsquaredTextEdit->setText(QString::number(v));

    regressionValid = true;
    updatePredictedYValue(ui->newXLineEdit->text());
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

void TableWindow::on_actionImport_from_CSV_triggered()
{
    this->importCSVFile();
}

void TableWindow::on_actionExport_triggered()
{
        QTextEdit *editor = new QTextEdit();
        QTextCursor cursor = editor->textCursor();
        QCPDocumentObject *plotObjectHandler = new QCPDocumentObject(this);

        editor->document()->documentLayout()->registerHandler(QCPDocumentObject::PlotTextFormat, plotObjectHandler);
        cursor.insertText(QString(QChar::ObjectReplacementCharacter), QCPDocumentObject::generatePlotFormat(ui->plot, 0, 0));

        cursor.insertText("\n\n");
        cursor.insertText("Trend Line Equation: " + ui->trendLineTextEdit->toPlainText() + "\n\n");
        cursor.insertText("Covariance: " + ui->covTextEdit->toPlainText() + "\n\n");
        cursor.insertText("Correlation: " + ui->corrTextEdit->toPlainText() + "\n\n");
        cursor.insertText("R-squared: " + ui->RsquaredTextEdit->toPlainText() +  "\n\n");

        editor->setTextCursor(cursor);
        QString fileName = QFileDialog::getSaveFileName(this, "Save document...", qApp->applicationDirPath(), "*.pdf");
        if (!fileName.isEmpty())
        {
          fileName.append(".pdf");
          QPrinter printer;
          printer.setFullPage(true);
          printer.setPaperSize(QPrinter::A4);
          printer.setOrientation(QPrinter::Portrait);
          printer.setOutputFormat(QPrinter::PdfFormat);
          printer.setOutputFileName(fileName);
          editor->document()->setPageSize(printer.pageRect().size());
          editor->document()->print(&printer);
        }
}

void TableWindow::updatePredictedYValue(const QString& text)
{
    bool valid;
    double x = text.toDouble(&valid);
    if (valid && regressionValid) {
        ui->predictedYTextEdit->setText(QString::number(slope * x + yIntercept));
    }
}

void TableWindow::on_newXLineEdit_textEdited(const QString& text)
{
    updatePredictedYValue(text);
}

QVector<QVector<double>> TableWindow::createCorrelationTable (){
    QAbstractItemModel* model = ui->tableView->model();
    QVector<QVector<double>> matrix = QVector<QVector<double>>(model->columnCount());

    for(int i = 0; i < model->columnCount(); i++){
        matrix[i]=QVector<double>(model->columnCount());
    }

    for(int i = 0; i < model->columnCount(); i++){
        for(int j = 0; j < i; j++){
            QVector<double> first = getSeries(model, i);
            QVector<double> second = getSeries(model, j);
            if (first.size() != 0 & second.size() != 0){
                matrix[i][j] = calculateCorrelation(first, second, model->rowCount());
            }
        }
    }
    return matrix;
}

QVector<double> TableWindow::getSeries(QAbstractItemModel* model, int i){
    QVector<double> vals = QVector<double>(model->rowCount());
    bool checkDouble = false;
    for(int k = 0; k < vals.length(); k++){
        vals[k] = model->data(model->index(k,i)).toDouble(&checkDouble);
    }
    if (checkDouble)
        return vals;
    else
        return QVector<double>(0);
}

double TableWindow::calculateCorrelation(QVector<double> x, QVector<double> y, int count){
    double sumXY = 0, sumX = 0, sumY = 0, sumX2 = 0, sumY2 = 0, ans;
    for(int i = 0; i<count; i++){
        sumXY += x[i]*y[i];
        sumX += x[i];
        sumY += y[i];
        sumX2 += x[i]*x[i];
        sumY2 += y[i]*y[i];
    }
    ans = (count * sumXY - (sumX*sumY))/((sqrt((count*sumX2) - (sumX*sumX)))*(sqrt((count*sumY2) - (sumY*sumY))));
    return ans;
}

void TableWindow::on_correlationButton_clicked()
{
    QVector<QVector<double>> m = createCorrelationTable();

    QStandardItemModel* model = (QStandardItemModel*) (ui->tableView->model());
    QVector<QStandardItem*> headers = QVector<QStandardItem*>(m.size());
    for (int i = 0; i < m.size(); i++){
        headers[i] = model->horizontalHeaderItem(i);
    }
    Matrix* mat = new Matrix(m,headers);
    mat->show();
}

void TableWindow::on_actionClose_triggered()
{
    exit(0);
}
