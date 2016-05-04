#include "explorerwindow.h"
#include "ui_explorerwindow.h"
#include <QString>
#include "tablewindow.h"
using namespace std;

ExplorerWindow::ExplorerWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ExplorerWindow)
{
    ui->setupUi(this);
    QString sPath = "C:/";
    dirmodel = new QFileSystemModel(this);
    dirmodel->setRootPath(sPath);
    QStringList filters;
    filters << "*.csv";
    dirmodel->setNameFilters(filters);
    dirmodel->setNameFilterDisables(false);
    ui->treeView->setModel(dirmodel);
    /*QStandardItemModel *model = new QStandardItemModel(5,5,this);
    string line;
    for(int i = 0; i < 6; i++)
    {
        getline(file,line);
        stringstream lineStream(line);
        string cell;
        for(int j = 0; j < 5; j++)
        {
            getline(lineStream,cell,',');
            if(i == 0)
                model->setHorizontalHeaderItem(j, new QStandardItem(QString::fromStdString(cell)));
            else
                model->setItem(i-1,j, new QStandardItem(QString::fromStdString(cell)));
        }
    }
        ui->tableView->setModel(model);
        ui->tableView->resize(1000,1000);*/
}

ExplorerWindow::~ExplorerWindow()
{
    delete ui;
}

void ExplorerWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    QString sPath = dirmodel->fileInfo(index).absoluteFilePath();
    if(sPath.contains(".csv")) {
        TableWindow *mw2 = new TableWindow(sPath);
        mw2->show();
        this->hide();
    }
}
