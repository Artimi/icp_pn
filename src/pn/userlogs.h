#ifndef USERLOGS_H
#define USERLOGS_H

#include <QDialog>
#include <QtCore>

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
};

#endif // USERLOGS_H
