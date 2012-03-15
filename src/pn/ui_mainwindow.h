/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Thu Mar 15 13:51:50 2012
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionOtev_t_lok_ln;
    QAction *actionUlo_it_lok_ln;
    QAction *actionOtev_t_ze_serveru;
    QAction *actionUlo_it_na_server;
    QAction *actionMouse;
    QAction *actionPlace;
    QAction *actionTransition;
    QAction *actionArc;
    QAction *actionNov;
    QAction *actionNov_karta;
    QAction *actionKonec;
    QAction *actionAddTab;
    QAction *actionCloseTab;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QTabWidget *tabWidget;
    QMenuBar *menuBar;
    QMenu *menuSoubor;
    QMenu *menuNastaven;
    QMenu *menuAbout;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(737, 461);
        actionOtev_t_lok_ln = new QAction(MainWindow);
        actionOtev_t_lok_ln->setObjectName(QString::fromUtf8("actionOtev_t_lok_ln"));
        actionUlo_it_lok_ln = new QAction(MainWindow);
        actionUlo_it_lok_ln->setObjectName(QString::fromUtf8("actionUlo_it_lok_ln"));
        actionOtev_t_ze_serveru = new QAction(MainWindow);
        actionOtev_t_ze_serveru->setObjectName(QString::fromUtf8("actionOtev_t_ze_serveru"));
        actionUlo_it_na_server = new QAction(MainWindow);
        actionUlo_it_na_server->setObjectName(QString::fromUtf8("actionUlo_it_na_server"));
        actionMouse = new QAction(MainWindow);
        actionMouse->setObjectName(QString::fromUtf8("actionMouse"));
        actionMouse->setCheckable(true);
        actionPlace = new QAction(MainWindow);
        actionPlace->setObjectName(QString::fromUtf8("actionPlace"));
        actionPlace->setCheckable(true);
        actionTransition = new QAction(MainWindow);
        actionTransition->setObjectName(QString::fromUtf8("actionTransition"));
        actionTransition->setCheckable(true);
        actionArc = new QAction(MainWindow);
        actionArc->setObjectName(QString::fromUtf8("actionArc"));
        actionArc->setCheckable(true);
        actionNov = new QAction(MainWindow);
        actionNov->setObjectName(QString::fromUtf8("actionNov"));
        actionNov_karta = new QAction(MainWindow);
        actionNov_karta->setObjectName(QString::fromUtf8("actionNov_karta"));
        actionKonec = new QAction(MainWindow);
        actionKonec->setObjectName(QString::fromUtf8("actionKonec"));
        actionAddTab = new QAction(MainWindow);
        actionAddTab->setObjectName(QString::fromUtf8("actionAddTab"));
        actionCloseTab = new QAction(MainWindow);
        actionCloseTab->setObjectName(QString::fromUtf8("actionCloseTab"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 737, 25));
        menuSoubor = new QMenu(menuBar);
        menuSoubor->setObjectName(QString::fromUtf8("menuSoubor"));
        menuNastaven = new QMenu(menuBar);
        menuNastaven->setObjectName(QString::fromUtf8("menuNastaven"));
        menuAbout = new QMenu(menuBar);
        menuAbout->setObjectName(QString::fromUtf8("menuAbout"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuSoubor->menuAction());
        menuBar->addAction(menuNastaven->menuAction());
        menuBar->addAction(menuAbout->menuAction());
        menuSoubor->addAction(actionNov);
        menuSoubor->addAction(actionNov_karta);
        menuSoubor->addSeparator();
        menuSoubor->addAction(actionOtev_t_lok_ln);
        menuSoubor->addAction(actionUlo_it_lok_ln);
        menuSoubor->addSeparator();
        menuSoubor->addAction(actionOtev_t_ze_serveru);
        menuSoubor->addAction(actionUlo_it_na_server);
        menuSoubor->addSeparator();
        menuSoubor->addAction(actionKonec);
        mainToolBar->addAction(actionAddTab);
        mainToolBar->addAction(actionCloseTab);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionMouse);
        mainToolBar->addAction(actionPlace);
        mainToolBar->addAction(actionTransition);
        mainToolBar->addAction(actionArc);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionOtev_t_lok_ln->setText(QApplication::translate("MainWindow", "Otev\305\231\303\255t lok\303\241ln\304\233", 0, QApplication::UnicodeUTF8));
        actionUlo_it_lok_ln->setText(QApplication::translate("MainWindow", "Ulo\305\276it lok\303\241ln\304\233", 0, QApplication::UnicodeUTF8));
        actionOtev_t_ze_serveru->setText(QApplication::translate("MainWindow", "Otev\305\231\303\255t ze serveru", 0, QApplication::UnicodeUTF8));
        actionUlo_it_na_server->setText(QApplication::translate("MainWindow", "Ulo\305\276it na server", 0, QApplication::UnicodeUTF8));
        actionMouse->setText(QApplication::translate("MainWindow", "Mouse", 0, QApplication::UnicodeUTF8));
        actionPlace->setText(QApplication::translate("MainWindow", "Place", 0, QApplication::UnicodeUTF8));
        actionTransition->setText(QApplication::translate("MainWindow", "Transition", 0, QApplication::UnicodeUTF8));
        actionArc->setText(QApplication::translate("MainWindow", "Arc", 0, QApplication::UnicodeUTF8));
        actionNov->setText(QApplication::translate("MainWindow", "&Nov\303\275", 0, QApplication::UnicodeUTF8));
        actionNov_karta->setText(QApplication::translate("MainWindow", "Nov\303\241 &karta", 0, QApplication::UnicodeUTF8));
        actionKonec->setText(QApplication::translate("MainWindow", "&Konec", 0, QApplication::UnicodeUTF8));
        actionAddTab->setText(QApplication::translate("MainWindow", "AddTab", 0, QApplication::UnicodeUTF8));
        actionCloseTab->setText(QApplication::translate("MainWindow", "CloseTab", 0, QApplication::UnicodeUTF8));
        menuSoubor->setTitle(QApplication::translate("MainWindow", "Soubor", 0, QApplication::UnicodeUTF8));
        menuNastaven->setTitle(QApplication::translate("MainWindow", "\303\232pravy", 0, QApplication::UnicodeUTF8));
        menuAbout->setTitle(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
