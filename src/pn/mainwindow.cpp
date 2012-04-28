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
    ui->actionMouse->setChecked(true);
    ui->mainToolBar->setMovable(false);
    ui->tabWidget->setTabsClosable(true);
    createActions();
    createMenus();
    tabCount = 0;
    addTab();

//    Message message;
//    message.user = "pepe";
//    message.password = "12345";
//    message.command = Message::SAVE;

//    XMLHandler xmlhandler(scenes.at(activeTab),&message);
//    QString str = xmlhandler.writeMessage();
//    qDebug() << str;
//    Message message2;

//    XMLHandler xmlhandler2(scenes.at(activeTab),&message2);
//    if (xmlhandler2.readMessage(str)==0 )
//        qDebug() << message2.command ;
//    else
//        qDebug() << "ERROR";

}

MainWindow::~MainWindow()
{
    delete ui;
}


/**
  * Vytváří menu pro Scene a Itemy
  */
void MainWindow::createMenus()
{
    placeMenu = new QMenu(tr("Place"),this);
    placeMenu->addAction(actionEditTokens);
    placeMenu->addSeparator();
    placeMenu->addAction(actionDeleteItem);

    transitionMenu = new QMenu(tr("Transition"),this);
    transitionMenu->addAction(actionEditGuard);
    transitionMenu->addAction(actionEditAction);
    transitionMenu->addSeparator();
    transitionMenu->addAction(actionDeleteItem);

    arrowMenu = new QMenu(tr("Arc"),this);
    arrowMenu->addAction(actionEditVariable);
    arrowMenu->addSeparator();
    arrowMenu->addAction(actionDeleteItem);
}

/**
  * Vytváří a spojuje všechnu akce hlavního
  */
void MainWindow::createActions()
{
    //výlučnost při vybírání kresleného předmětu z toolbaru
    actionGroup = new QActionGroup(this);
    actionGroup->setExclusive(true);
    actionGroup->addAction(ui->actionMouse);
    actionGroup->addAction(ui->actionArc);
    actionGroup->addAction(ui->actionTransition);
    actionGroup->addAction(ui->actionPlace);


    connect(ui->actionMouse,SIGNAL(triggered()),this, SLOT(selectMouse()));
    connect(ui->actionArc,SIGNAL(triggered()),this, SLOT(selectArc()));
    connect(ui->actionPlace,SIGNAL(triggered()),this, SLOT(selectPlace()));
    connect(ui->actionTransition,SIGNAL(triggered()),this, SLOT(selectTransition()));

    connect(ui->actionKonec,SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionNovakarta,SIGNAL(triggered()), this, SLOT(addTab()));
    connect(ui->actionUlozit_lokalne,SIGNAL(triggered()),this,SLOT(saveLocal()));
    connect(ui->actionOtevrit_lokalne,SIGNAL(triggered()),this,SLOT(loadLocal()));
    connect(ui->actionInformace_o_s_ti,SIGNAL(triggered()),this, SLOT(netInformation()));
    connect(ui->actionP_ipojit_k_serveru,SIGNAL(triggered()),this,SLOT(connectToServer()));


    connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
    connect(ui->tabWidget,SIGNAL(currentChanged(int)), this, SLOT(updateToolBar(int)));

    actionEditTokens = new QAction(tr("Edit tokens"),this);
    connect(actionEditTokens, SIGNAL(triggered()),
            this, SLOT(editTokens()));

    actionEditGuard = new QAction(tr("Edit guard"),this);
    connect(actionEditGuard, SIGNAL(triggered()),
            this, SLOT(editGuard()));
    actionEditAction = new QAction(tr("Edit action"),this);
    connect(actionEditAction, SIGNAL(triggered()),
            this, SLOT(editAction()));

    actionEditVariable = new QAction(tr("Edit variable"),this);
    connect(actionEditVariable, SIGNAL(triggered()),
            this, SLOT(editVariable()));

    actionDeleteItem = new QAction(tr("Delete item"),this);
    actionDeleteItem->setShortcut(Qt::Key_Delete);
    connect(actionDeleteItem, SIGNAL(triggered()),
            this, SLOT(deleteItem()));
}


/**
  * Slot, který přidává nový Tab do TabWidget, zároveň vytváří novou
  * DiagramScene a QGraphicsView v seznamech scenes a views. Nutno zachovat
  * pořadí
  *
  * @return index nově vytvořeného Tabu, -1 při chybě
  */
int MainWindow::addTab()
{
    DiagramScene *scene = new DiagramScene(placeMenu,transitionMenu,arrowMenu,this);
    scene->setSceneRect(QRectF(0,0,500,500));
    scenes.append(scene);

    QGraphicsView *view = new QGraphicsView(scene,this);
    views.append(view);

    return ui->tabWidget->addTab(view, QString("Unnamed %1").arg(++tabCount));
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

/**
  * Nastavuje správné zaškrtnutí v toolbaru podle scene
  * @param  tab tab, který je aktivní
  */
void MainWindow::updateToolBar(int tab)
{
    if (tab == -1)
        return;

    DiagramScene::Mode mode;

    mode = scenes.at(tab)->getMode();
    if (mode == DiagramScene::MoveItem)
    {
        ui->actionMouse->setChecked(true);
    }
    else if (mode == DiagramScene::InsertLine)
    {
        ui->actionArc->setChecked(true);
    }
    else if (mode == DiagramScene::InsertItem)
    {
        DiagramItem::DiagramType item = scenes.at(tab)->getItemType();

        if(item == DiagramItem::Place)
        {
            ui->actionPlace->setChecked(true);
        }
        else if (item == DiagramItem::Transition)
        {
            ui->actionTransition->setChecked(true);
        }
    }
    activeTab = tab;
}

/**
  * Slot pro ukládání na lokálním uložišti, zeptá se na jméno souboru a uloží tam
  * Při chybě vyskočí warning
  */
void MainWindow::saveLocal()
{
    XMLHandler xmlhandler(scenes.at(activeTab));
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save petri net"),
                                                    "",
                                                    tr("All Files(*)"));
    if(fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this,
                                     QString("The file can not be opened."),
                                     file.errorString());
            return;
        }
    xmlhandler.saveNetToFile(&file);
    file.close();
    }
}

/**
  * Nahraje xml síť z lokálního uložiště a vloží ji do nově otevřeného tabu
  */
void MainWindow::loadLocal()
{
    int tab = addTab();
    XMLHandler xmlhandler(scenes.at(tab));
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open petri net"),
                                                    "",
                                                    tr("All files(*)"));
    if(fileName.isEmpty())
        return;
    else
    {
        QFile file(fileName);

        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::information(this,
                                     tr("The file can not be opened."),
                                     file.errorString());
            return;
        }
        if( xmlhandler.loadNetFromFile(&file) != 0)
        {
            QMessageBox::critical(this,
                                  tr("XML parse problem"),
                                  tr("File can not be parsed."),
                                  QMessageBox::Ok);
        }
        file.close();
        scenes.at(tab)->update();
    }
}

/**
  * Otevře input dialog, kterým zadá nové tokeny do místa. Pokud nejsou místem
  * úspěšně zpracovány háže warning
  */
void MainWindow::editTokens()
{
    if (scenes.at(activeTab)->selectedItems().isEmpty())
        return;

    QGraphicsItem *item = scenes.at(activeTab)->selectedItems().first();
    Place *place = qgraphicsitem_cast<Place *>(item);

    bool ok;
    QString text = QInputDialog::getText(this,
                                         tr("Tokens edit"),
                                         tr("Tokens:"),
                                         QLineEdit::Normal,
                                         place->getTokenString(),
                                         &ok);

    if (ok && !text.isEmpty())
    {
        if(!place->setTokenString(text))
        {
            QMessageBox::warning(this,
                                 tr("Token input error"),
                                 tr("Input string (%1) of tokens is not valid.\nUse only number, comma and whitespaces.").arg(text),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        }
        scenes.at(activeTab)->update();
    }
}

/**
  * Otevře input dialog, kterým se zadává stráž do přechodu. Při neúspěšném
  * zpracování na straně přechodu vyhodí warning
  */
void MainWindow::editGuard()
{
    if (scenes.at(activeTab)->selectedItems().isEmpty())
        return;

    QGraphicsItem *item = scenes.at(activeTab)->selectedItems().first();
    Transition *transition = qgraphicsitem_cast<Transition *>(item);

    bool ok;
    QString text = QInputDialog::getText(this,
                                         tr("Guard edit"),
                                         tr("Guard:"),
                                         QLineEdit::Normal,
                                         transition->getGuard(),
                                         &ok);

    if (ok && !text.isEmpty())
    {
        if(!transition->setGuard(text))
        {
            QMessageBox::warning(this,
                                 tr("Guard input error"),
                                 tr("Input string (%1) is not valid guard.").arg(text),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        }
        scenes.at(activeTab)->update();
    }
}

/**
  * Otevře input dialog, kterým se zadává akce do přechodu. Při špatném řetězci
  * vyhodí warning
  */
void MainWindow::editAction()
{
    if (scenes.at(activeTab)->selectedItems().isEmpty())
        return;

    QGraphicsItem *item = scenes.at(activeTab)->selectedItems().first();
    Transition *transition = qgraphicsitem_cast<Transition *>(item);

    bool ok;
    QString text = QInputDialog::getText(this,
                                         tr("Action edit"),
                                         tr("Action:"),
                                         QLineEdit::Normal,
                                         transition->getAction(),
                                         &ok);

    if (ok && !text.isEmpty())
    {
        if(!transition->setAction(text))
        {
            QMessageBox::warning(this,
                                 tr("Action input error"),
                                 tr("Input string (%1) is not valid action.").arg(text),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        }
        scenes.at(activeTab)->update();
    }
}

/**
  * Otevře input dialog, jímž se zadává proměnná na hraně. Pokud není správně
  * přijata vyhazuje warning
  */
void MainWindow::editVariable()
{
    if (scenes.at(activeTab)->selectedItems().isEmpty())
        return;

    QGraphicsItem *item = scenes.at(activeTab)->selectedItems().first();
    Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);

    bool ok;
    QString text = QInputDialog::getText(this,
                                         tr("Variable edit"),
                                         tr("Variable:"),
                                         QLineEdit::Normal,
                                         arrow->getVariable(),
                                         &ok);

    if (ok && !text.isEmpty())
    {
        if(!arrow->setVariable(text))
        {
            QMessageBox::warning(this,
                                 tr("Variable input error"),
                                 tr("Input string (%1) is not valid variable.").arg(text),
                                 QMessageBox::Ok,
                                 QMessageBox::Ok);
        }
        scenes.at(activeTab)->update();
    }
}

/**
  * Maže všechny označené itemy na aktivní sceně
  */
void MainWindow::deleteItem()
{
    foreach (QGraphicsItem *item, scenes.at(activeTab)->selectedItems()) {
        if (item->type() == Arrow::Type)
        {
            scenes.at(activeTab)->removeItem(item);
            Arrow *arrow = qgraphicsitem_cast<Arrow *>(item);
            arrow->startItem()->removeArrow(arrow);
            arrow->endItem()->removeArrow(arrow);
            delete item;
        }
    }

    foreach (QGraphicsItem *item, scenes.at(activeTab)->selectedItems())
    {
         if (item->type() == Place::Type)
         {
             qgraphicsitem_cast<Place *>(item)->removeArrows();
         }
         else if(item->type() == Transition::Type)
         {
             qgraphicsitem_cast<Transition *>(item)->removeArrows();
         }
         scenes.at(activeTab)->removeItem(item);
         delete item;
    }
}

/**
  * Vyvolá formulář pro zápis informací o síti
  */
void MainWindow::netInformation()
{
    NetInformation diag(scenes.at(activeTab));
    diag.exec();
}

void MainWindow::connectToServer()
{
    Connect con(socket);
    con.exec();
}



















