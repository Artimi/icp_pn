/**
  * @file mainwindow.cpp
  * Používá vytvořený formulář mainwindow.ui. Posléze definuje další signály,
  * sloty a propojení.
  *
  * @brief Hlavní okno programu
  * @author xsebek02 xsimon14
  *
  */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->actionKonec,SIGNAL(triggered()), qApp, SLOT(quit()));

    //výlučnost při vybírání kresleného předmětu z toolbaru
    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    actionGroup->addAction(ui->actionMouse);
    actionGroup->addAction(ui->actionArc);
    actionGroup->addAction(ui->actionTransition);
    actionGroup->addAction(ui->actionPlace);
    ui->actionMouse->setChecked(true);

    ui->mainToolBar->setMovable(false);

    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));

    addTab();
    connect(ui->actionAddTab,SIGNAL(triggered()), this, SLOT(addTab()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addTab()
{
    DiagramScene *scene = new DiagramScene;
    scenes.append(scene);

    QGraphicsView *view = new QGraphicsView(scene);
    views.append(view);

    ui->tabWidget->addTab(view, "unnamed");
}

void MainWindow::closeTab(int tab)
{

    QWidget *widget = ui->tabWidget->widget(tab);
    QGraphicsView *view = widget;
    DiagramScene *scene = view->scene();

    views.removeOne(view);
    scenes.removeOne(scene);
    delete scene;
    delete view;


    ui->tabWidget->removeTab(tab);
}
