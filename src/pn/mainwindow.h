/**
  * @file mainwindow.h
  *
  * @brief Hlavičkový soubor pro mainwindow.cpp
  * @author xsebek02 xsimon14
  *
  */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QActionGroup>
#include <QGraphicsView>
#include <QtGui/QMessageBox>
#include <QDebug>
#include <QMessageBox>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QRegExp>
#include "diagramscene.h"
#include "diagramitem.h"
#include "arrow.h"
#include "place.h"
#include "transition.h"
#include "xmlhandler.h"
#include "netinformation.h"
#include "message.h"
#include "connect.h"
#include "ui_loginwindow.h"
#include "netlist.h"

class QActionGroup;
class QGraphicsView;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<DiagramScene*> scenes;
    QList<QGraphicsView*> views;
    QActionGroup *actionGroup;

    int tabCount;

    void createMenus();
    void createActions();
    void printError(QString errorText);

    int activeTab;

    QMenu *placeMenu;
    QMenu *transitionMenu;
    QMenu *arrowMenu;

    QAction * actionEditTokens;
    QAction * actionEditGuard;
    QAction * actionEditVariable;
    QAction * actionEditAction;
    QAction * actionDeleteItem;

    QTcpSocket *socket;

    QList<DiagramScene *> petriNetList;

    NetList *netListForm;



public slots:
    int addTab(DiagramScene *scene = 0);
    void closeTab(int tab);
    void selectMouse();
    void selectArc();
    void selectPlace();
    void selectTransition();
    void updateToolBar(int tab);
    void saveLocal();
    void loadLocal();

    void editTokens();
    void editGuard();
    void editAction();
    void editVariable();
    void deleteItem();
    void netInformation();
    void connectToServer();

    void gotConnected();
    void gotDisconnected();
    void gotError(QAbstractSocket::SocketError);
    void handleReply();

    void disconnectFromServer();

    void login();
    void saveRemote();
    void openRemote();

    void sendRemoteLoadRequest(QString name, QString version, QString author);

};

#endif // MAINWINDOW_H
