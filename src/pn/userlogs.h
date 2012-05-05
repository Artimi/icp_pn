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
    explicit UserLogs(QList<QVector<QString>> *logs, QWidget *parent = 0);
    ~UserLogs();
    
private:
    Ui::UserLogs *ui;
    QList<QVector<QString>> * myLogs;
};

#endif // USERLOGS_H
