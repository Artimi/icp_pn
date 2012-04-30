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

    QHeaderView *headerView = ui->tableWidget->horizontalHeader();
    headerView->setResizeMode(0,QHeaderView::Stretch);
    headerView->setResizeMode(1,QHeaderView::Stretch);
    headerView->setResizeMode(2,QHeaderView::Stretch);
    headerView->setResizeMode(3,QHeaderView::ResizeToContents);

    updateTable();

    connect(ui->PBFind,SIGNAL(clicked()),this,SLOT(find()));
    connect(ui->PBOpen,SIGNAL(clicked()),this,SLOT(openRemote()));
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
    prototype.setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);

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
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
        ui->tableWidget->showRow(i);

    QList<QTableWidgetItem*> items = ui->tableWidget->findItems(ui->lineEdit->text(),Qt::MatchContains);
    QList<int> visibleRows;
    for (int i = 0; i < items.count(); i++)
    {
        visibleRows << items.at(i)->row();
    }

    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if (!visibleRows.contains(i))
            ui->tableWidget->hideRow(i);
    }

}

void NetList::openRemote()
{
    int curRow = ui->tableWidget->currentRow();

    QString name = ui->tableWidget->item(curRow,0)->text();
    QString version = ui->tableWidget->item(curRow,1)->text();
    QString author = ui->tableWidget->item(curRow,2)->text();

    emit remoteLoad(name,version,author);
    this->accept();

}
