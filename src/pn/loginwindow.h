#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

namespace Ui {
class loginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT
    
public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
    
public slots:

private:
    Ui::loginWindow *ui;
};

#endif // LOGINWINDOW_H
