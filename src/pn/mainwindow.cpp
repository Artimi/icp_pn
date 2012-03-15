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

    connect(ui->actionMouse,SIGNAL(triggered()),this, SLOT(selectMouse()));
    connect(ui->actionArc,SIGNAL(triggered()),this, SLOT(selectArc()));
    connect(ui->actionPlace,SIGNAL(triggered()),this, SLOT(selectPlace()));
    connect(ui->actionTransition,SIGNAL(triggered()),this, SLOT(selectTransition()));

    ui->mainToolBar->setMovable(false);

    ui->tabWidget->setTabsClosable(true);
    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));

    tabCount = 0;
    addTab();
    connect(ui->actionNovakarta,SIGNAL(triggered()), this, SLOT(addTab()));
}

MainWindow::~MainWindow()
{
    delete ui;
    QGraphicsView *view;
    foreach(view,views)
    {
        delete view;
    }

    DiagramScene *scene;
    foreach (scene, scenes)
    {
        delete scene;
    }

}

/**
  * Slot, který přidává nový Tab do TabWidget, zároveň vytváří novou
  * DiagramScene a QGraphicsView v seznamech scenes a views. Nutno zachovat
  * pořadí
  *
  * @retun index nově vytvořeného Tabu, -1 při chybě
  */
int MainWindow::addTab()
{
    tabCount++;
    DiagramScene *scene = new DiagramScene;
    scenes.append(scene);

    QGraphicsView *view = new QGraphicsView(scene);
    views.append(view);

    return ui->tabWidget->addTab(view, QString("unnamed %1").arg(tabCount));
}


/**
  * Zavření tabu a uvolnění DiagramScene a QGraphicsView z paměti a seznamů
  * scenes a views.
  *
  * /todo Otázka na uložení při změně obsahu scene
  *
  * @param tab index mazaného tabu
  */
void MainWindow::closeTab(int tab)
{
    QGraphicsView *view = views.at(tab);
    DiagramScene *scene = scenes.at(tab);
    ui->tabWidget->removeTab(tab);
    views.removeAt(tab);
    scenes.removeAt(tab);
    delete scene;
    delete view;
}

/**
  * Vybrání myši jako nástroje v současném tabu(scene)
  */
void MainWindow::selectMouse()
{
    scenes.at(ui->tabWidget->currentIndex())->setMode(DiagramScene::MoveItem);
}

/**
  * Vybrání hrany jako nástroje v současném tabu (scene)
  */
void MainWindow::selectArc()
{
    scenes.at(ui->tabWidget->currentIndex())->setMode(DiagramScene::InsertLine);
}

/**
  * Vybrání místa jako nástroje v současném tabu (scene)
  */
void MainWindow::selectPlace()
{
    scenes.at(ui->tabWidget->currentIndex())->setMode(DiagramScene::InsertItem);
    scenes.at(ui->tabWidget->currentIndex())->setItemType(DiagramItem::Place);
}

/**
  * Vybrání přechodu jako nástroje v současném tabu (scene)
  */
void MainWindow::selectTransition()
{
    scenes.at(ui->tabWidget->currentIndex())->setMode(DiagramScene::InsertItem);
    scenes.at(ui->tabWidget->currentIndex())->setItemType(DiagramItem::Transition);
}
