#include "userlogs.h"
#include "ui_userlogs.h"

UserLogs::UserLogs(QList< QList<QString> > *logs, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserLogs)
{
    ui->setupUi(this);
    myLogs = logs;
    ui->tableWidget->setColumnCount(3);
    QStringList headerList;
    headerList << "Date" << "Action" << "Description";
    ui->tableWidget->setHorizontalHeaderLabels(headerList);

    QHeaderView * headerView = ui->tableWidget->horizontalHeader();
    headerView->setResizeMode(0,QHeaderView::Stretch);
    headerView->setResizeMode(1,QHeaderView::Stretch);
    headerView->setResizeMode(2,QHeaderView::ResizeToContents);

    updateTable();

    connect(ui->PBFind,SIGNAL(clicked()),this,SLOT(find()));
    connect(ui->PBUpdate,SIGNAL(clicked()),this,SIGNAL(updateUserLog()));
}

UserLogs::~UserLogs()
{
    delete ui;
}

void UserLogs::updateTable()
{
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(myLogs->count());

    int i = 0;
    QTableWidgetItem prototype;
    prototype.setFlags(Qt::ItemIsEnabled|Qt::ItemIsSelectable);

    foreach(QList<QString>  record,*myLogs)
    {
        QTableWidgetItem *timeItem = prototype.clone();
        QTableWidgetItem *actionItem = prototype.clone();
        QTableWidgetItem *descriptionItem = prototype.clone();

        timeItem->setText(record.at(0));
        actionItem->setText(getActionName(record.at(1).toInt()));
        descriptionItem->setText(record.at(2));

        ui->tableWidget->setItem(i,0,timeItem);
        ui->tableWidget->setItem(i,1,actionItem);
        ui->tableWidget->setItem(i,2,descriptionItem);

        i++;
    }

}

void UserLogs::find()
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

QString UserLogs::getActionName(int action)
{
    switch(action)
    {
    case Message::LOG_CONNECT:
        return "connect";
        break;
    case Message::LOG_DISCONNECT:
        return "disconnect";
        break;
    case Message::LOG_LOAD:
        return "load";
        break;
    case Message::LOG_LOGGED:
        return "logged";
        break;
    case Message::LOG_SAVE:
        return "save";
        break;
    case Message::LOG_SIMULATE:
        return "simulate";
        break;
    case Message::LOG_WRONGLOGIN:
        return "wronglogin";
        break;
    default:
        return "unknown";
    }
}


