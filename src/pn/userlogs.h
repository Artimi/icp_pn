#ifndef USERLOGS_H
#define USERLOGS_H

#include <QDialog>

namespace Ui {
class UserLogs;
}

class UserLogs : public QDialog
{
    Q_OBJECT
    
public:
    explicit UserLogs(QWidget *parent = 0);
    ~UserLogs();
    
private:
    Ui::UserLogs *ui;
};

#endif // USERLOGS_H
