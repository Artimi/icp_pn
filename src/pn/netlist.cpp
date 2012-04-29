#include "netlist.h"
#include "ui_netlist.h"

NetList::NetList(QList<DiagramScene *> *list,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetList)
{
    myList = list;
    ui->setupUi(this);
    QStringList headerList;
    headerList << "Name" << "Version" << "Author" << "Description";
    ui->tableWidget->setHorizontalHeaderLabels(headerList);
    updateTable();

    connect(ui->PBFind,SIGNAL(clicked()),this,SLOT(find()));
}

NetList::~NetList()
{
    delete ui;
}

void NetList::updateTable()
{
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(myList->count());

    int i = 0;

    QTableWidgetItem prototype;
    prototype.setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    foreach(DiagramScene * scene,*myList)
    {
        QTableWidgetItem *nameItem = prototype.clone();
        QTableWidgetItem *versionItem = prototype.clone();
        QTableWidgetItem *authorItem = prototype.clone();
        QTableWidgetItem *descriptionItem = prototype.clone();

        nameItem->setText(scene->getName());
        versionItem->setText(scene->getVersion());
        authorItem->setText(scene->getAuthor());
        descriptionItem->setText(scene->getDescription());

        ui->tableWidget->setItem(i,0, nameItem);
        ui->tableWidget->setItem(i,1,versionItem);
        ui->tableWidget->setItem(i,2,authorItem);
        ui->tableWidget->setItem(i,3,descriptionItem);

        i++;
    }
}

void NetList::find()
{
    QList<QTableWidgetItem*> items = ui->tableWidget->findItems(ui->lineEdit->text(),Qt::MatchContains);;
    QMap<int, int> rowsMap;
    for(int i = 0; i < items.count(); i++)
    {
      rowsMap[items.at(i)->row()] = 1;
    }
    QList<int> rowsList = rowsMap.uniqueKeys();
    qSort(rowsList);

    for(int i = 0 ; i < ui->tableWidget->rowCount(); i++)
    {
        if(!rowsList.contains(i))
            ui->tableWidget->removeRow(i);
    }
}
