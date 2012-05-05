/**
  * @file userlogs.h
  *
  * @author xsebek02,xsimon14
  */
#ifndef USERLOGS_H
#define USERLOGS_H

#include <QDialog>
#include <QtCore>
#include "message.h"

namespace Ui {
class UserLogs;
}

class UserLogs : public QDialog
{
    Q_OBJECT
    
public:
    UserLogs(QList< QList<QString> > *logs, QWidget *parent = 0);
    ~UserLogs();

public slots:
    void updateTable();
    void find();

signals:
    void updateUserLog();
    
private:
    Ui::UserLogs *ui;
    QList< QList<QString> > * myLogs;


    QString getActionName(int action);
};

#endif // USERLOGS_H
