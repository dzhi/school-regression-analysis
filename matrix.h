#ifndef MATRIX_H
#define MATRIX_H

#include <QWidget>
#include <QMainWindow>
#include <QVector>
#include <QString>

#include<QStandardItem>
#include"tablewindow.h"
namespace Ui {
class Matrix;
}

class Matrix : public QWidget
{
    Q_OBJECT

public:
    explicit Matrix(QVector<QVector<double>> data, QVector<QStandardItem*> headers, QWidget *parent = 0);
    ~Matrix();

private:
    Ui::Matrix *ui;
};

#endif // MATRIX_H
