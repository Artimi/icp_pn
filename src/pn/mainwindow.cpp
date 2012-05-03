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

//QMenu MainWindow::*PLACEMENU = NULL;
//QMenu MainWindow::*TRANSITIONMENU = NULL;
//QMenu MainWindow::*ARROWMENU = NULL;

//int MainWindow::FONTSIZE = 10;
//int MainWindow::OBJECTSIZE = 100;
QFont MainWindow::FONT = QFont("times",10);
QColor MainWindow::LINECOLOR = Qt::black;
QColor MainWindow::DASHLINECOLOR= Qt::darkBlue;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    netListForm = new NetList(&netList);
    ui->actionMouse->setChecked(true);
    ui->mainToolBar->setMovable(false);
    ui->tabWidget->setTabsClosable(true);
    createActions();
    createMenus();
    userName = "guest";
    tabCount = 0;
    activeTab = -1;

    xmlFormat = QSettings::registerFormat("xml",readXMLSettings,writeXMLSettings);
    mySettings = new QSettings("settings.xml",xmlFormat,this);
    loadSettings();

    addTab();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete netListForm;
    delete socket;
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
  * Vytváří a spojuje všechny akce hlavního okna
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

    connect(ui->actionQuit,SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(ui->actionNewTab,SIGNAL(triggered()), this, SLOT(addTab()));
    connect(ui->actionSaveLocally,SIGNAL(triggered()),this,SLOT(saveLocal()));
    connect(ui->actionOpenLocally,SIGNAL(triggered()),this,SLOT(loadLocal()));
    connect(ui->actionSaveRemote,SIGNAL(triggered()),this,SLOT(saveRemote()));
    connect(ui->actionOpenRemote,SIGNAL(triggered()),this,SLOT(openRemote()));
    connect(ui->actionNetInformation,SIGNAL(triggered()),this, SLOT(netInformation()));
    connect(ui->actionConnectToServer,SIGNAL(triggered()),this,SLOT(connectToServer()));
    connect(ui->actionDisconnectFromServer,SIGNAL(triggered()),this,SLOT(disconnectFromServer()));
    connect(ui->actionLogin,SIGNAL(triggered()),this,SLOT(login()));
    connect(ui->actionSettings,SIGNAL(triggered()),this,SLOT(settingsWindow()));

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

    connect(socket,SIGNAL(connected()),SLOT(gotConnected()));
    connect(socket,SIGNAL(disconnected()),SLOT(gotDisconnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)),
            SLOT(gotError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(readyRead()),SLOT(handleReply()));

    connect(netListForm,SIGNAL(remoteLoad(QString,QString,QString)),
            this,SLOT(sendLoadRequest(QString,QString,QString)));
    connect(this,SIGNAL(netListArrived()),netListForm,SLOT(updateTable()));
    connect(netListForm,SIGNAL(updateNetList()),this,SLOT(sendListRequest()));

    connect(ui->actionSimulate,SIGNAL(triggered()), this, SLOT(simulate()));
    connect(ui->actionSimulate_Step,SIGNAL(triggered()),this,SLOT(simulateStep()));

}
/**
  * Vytiskne chybovou hlášku do status baru a vyhodí warning okno
  * @param errorText    chybová hláška, která se má vypsat
  */
void MainWindow::printError(QString errorText)
{
    ui->statusBar->showMessage(errorText);
    QMessageBox::warning(this,
                         tr("Error"),
                         errorText,
                         QMessageBox::Ok,
                         QMessageBox::Ok);
}


/**
  * Vrátí číslo tabu, na kterém se nachází síť zadaného jména
  * @param netName  hledané jméno sítě
  * @return číslo tabu, -1 nenalezeno
  */
int MainWindow::findTab(QString netName)
{
    for(int i = 0; i < ui->tabWidget->count() ; i++)
    {
        if(scenes.at(i)->getName() == netName)
            return i;
    }
    return -1;
}


/**
  * Načítá soubor s nastavením
  * @param device   device obsahující soubor pro načtení nastavení
  * @param map  mapa obsahující nastavení pro QSettings
  * @return true v pořádku, false chyba
  */
bool MainWindow::readXMLSettings(QIODevice &device, QSettings::SettingsMap &map)
{// http://www.openshots.de/2011/03/qsettings-mit-xml-format/
    QXmlStreamReader xmlReader(&device);
    QStringList elements;

    while (!xmlReader.atEnd() && !xmlReader.hasError())
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement() && xmlReader.name() != "settings")
        {
            elements.append(xmlReader.name().toString());
        }
        else if (xmlReader.isEndElement())
        {
            if(!elements.isEmpty()) elements.removeLast();
        }
        else if (xmlReader.isCharacters() && !xmlReader.isWhitespace())
        {
            QString key;

            for(int i = 0; i < elements.size(); i++) {
                if(i != 0) key += "/";
                key += elements.at(i);
            }

            map[key] = xmlReader.text().toString();
        }
    }

    if (xmlReader.hasError())
    {
        qWarning() << xmlReader.errorString();
        return false;
    }

    return true;
}

/**
  * Zapisuje do xml souboru
  * @param device soubor, do kterého se bude zapisovat
  * @param map  mapa, ze které se hodnoty čtou
  * @return true v pořádku, false chyba
  */
bool MainWindow::writeXMLSettings(QIODevice &device, const QSettings::SettingsMap &map)
{
        QXmlStreamWriter xmlWriter(&device);
    xmlWriter.setAutoFormatting(true);
    xmlWriter.writeStartDocument();
    xmlWriter.writeStartElement("settings");

    QStringList prev_elements;
    QSettings::SettingsMap::ConstIterator map_i;

    for (map_i = map.begin(); map_i != map.end(); map_i++)
    {

        QStringList elements = map_i.key().split("/");

        int x = 0;
        while(x < prev_elements.size() && elements.at(x) == prev_elements.at(x))
        {
            x++;
        }

        for(int i = prev_elements.size() - 1; i >= x; i--)
        {
            xmlWriter.writeEndElement();
        }

        for (int i = x; i < elements.size(); i++)
        {
            xmlWriter.writeStartElement(elements.at(i));
        }

        xmlWriter.writeCharacters(map_i.value().toString());

        prev_elements = elements;
    }

    for(int i = 0; i < prev_elements.size(); i++)
    {
        xmlWriter.writeEndElement();
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndDocument();

    return true;
}


/**
  * Slot, který přidává nový Tab do TabWidget, zároveň vytváří novou
  * DiagramScene a QGraphicsView v seznamech scenes a views. Nutno zachovat
  * pořadí
  *
  * @return index nově vytvořeného Tabu, -1 při chybě
  */
int MainWindow::addTab(DiagramScene * scene)
{
    int result;

    if (scene == 0)
        scene = new DiagramScene(placeMenu,transitionMenu,arrowMenu,this);

    scene->setSceneRect(QRectF(0,0,500,500));
    scenes.append(scene);

    QGraphicsView *view = new QGraphicsView(scene,this);
    views.append(view);

    if (scene->getName() == "")
        result = ui->tabWidget->addTab(view, QString("Unnamed %1").arg(++tabCount));
    else
        result = ui->tabWidget->addTab(view, scene->getName());
    return result;
}

/**
  * Nahradí scenu na tabu
  * @param scene scena, kterou se bude nahrazovat
  * @param tab  tab na kterém se nahrazovaná scéna nachází
  */
void MainWindow::replaceTab(DiagramScene *scene, int tab)
{
    DiagramScene * deleteScene = scenes.at(tab);
    scenes.replace(tab,scene);
    views.at(tab)->setScene(scene);
    delete deleteScene;
    scene->update();
    if(scene->getName() != "")
        ui->tabWidget->setTabText(tab,scene->getName());
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

    if(scenes.isEmpty())
         activeTab = -1;
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
    if(activeTab >= 0)
    {
        DiagramScene * scene = scenes.at(activeTab);
        XMLHandler xmlhandler;
        xmlhandler.setScene(scene);
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
                printError(tr("File can not be opened.")+file.errorString());
                return;
            }

        xmlhandler.saveNetToFile(&file);

        //jméno sítě
        QRegExp re(".*/(\\w+)(?:\\.*)?");
        re.indexIn(fileName);
        QString netName = re.cap(1);

        if(scene->getName() == "")
            scene->setName(netName);

        ui->tabWidget->setTabText(activeTab,netName);

        ui->statusBar->showMessage(tr("File saved"));
        file.close();
        }
    }
    else
    {
        printError(tr("No scene to handle"));
        return;
    }
}

/**
  * Nahraje xml síť z lokálního uložiště a vloží ji do nově otevřeného tabu
  */
void MainWindow::loadLocal()
{
    int tab = addTab();
    DiagramScene * scene = scenes.at(tab);
    XMLHandler xmlhandler;
    xmlhandler.setScene(scene);
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
            printError(tr("File can not be opened."));
            return;
        }
        if( xmlhandler.loadNetFromFile(&file) != 0)
        {
            printError(tr("File can not be parsed. Invalid XML."));
            return;
        }
        file.close();

        //jméno sítě
        QRegExp re(".*/(\\w+)(?:\\.*)?");
        re.indexIn(fileName);
        QString netName = re.cap(1);

        if ((scene)->getName() == "")
            scene->setName(netName);

        ui->tabWidget->setTabText(tab,netName);

        scene->update();
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
            printError(tr("Input string (%1) of tokens is not valid.\nUse only number, comma and whitespaces.").arg(text));
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
            printError(tr("Input string (%1) is not valid guard.").arg(text));
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
            printError(tr("Input string (%1) is not valid action.").arg(text));
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
            printError(tr("Input string (%1) is not valid variable.").arg(text));
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
int MainWindow::netInformation()
{
    if(activeTab >= 0)
    {
        NetInformation diag(scenes.at(activeTab),userName, this);
        return diag.exec();
    }
    else
    {
        printError(tr("No scene to handle"));
        return -1;
    }
}

/**
  * Otevře okno, ve kterém se vyplňuje ip adresa a port na připojení k serveru
  */
void MainWindow::connectToServer()
{
    Connect con(socket);
    con.exec();
}

/**
  * Slot volaný při připojení k serveru, vypíše zprávu do status baru
  */
void MainWindow::gotConnected()
{
    ui->statusBar->showMessage(tr("Connected to server ")+socket->peerName());
    ui->actionConnectToServer->setEnabled(false);

    ui->actionLogin->setEnabled(true);
    ui->actionDisconnectFromServer->setEnabled(true);
    ui->actionOpenRemote->setEnabled(true);
    ui->actionSaveRemote->setEnabled(true);

    ui->actionSimulate->setEnabled(true);
    ui->actionSimulate_Step->setEnabled(true);
}

/**
  * Slot volaný při disconnected() ze serveru, vypíše zprávu do status baru
  */
void MainWindow::gotDisconnected()
{
    ui->statusBar->showMessage(tr("Disconnected from server."));
    ui->actionConnectToServer->setEnabled(true);

    ui->actionLogin->setEnabled(false);
    ui->actionDisconnectFromServer->setEnabled(false);
    ui->actionOpenRemote->setEnabled(false);
    ui->actionSaveRemote->setEnabled(false);

    ui->actionSimulate->setEnabled(false);
    ui->actionSimulate_Step->setEnabled(false);
}

/**
  * Vypíše do status baru pokud se na socketu objevil problém
  */
void MainWindow::gotError(QAbstractSocket::SocketError error)
{
    ui->statusBar->showMessage(socket->errorString());
}

/**
  * Zpracuje příchozí zprávy od serveru
  */
void MainWindow::handleReply()
{
    QByteArray rawdata = socket->readAll();
    int tab;
    bool deleteScene = true;
//    qDebug()<< "Handle reply: " << rawdata;

    Message message;
    DiagramScene *scene = new DiagramScene(placeMenu, transitionMenu,arrowMenu,this);

    XMLHandler xmlhandler;
    xmlhandler.setScene(scene);
    xmlhandler.setMessage(&message);
    xmlhandler.setNetList(&netList);
    xmlhandler.readMessage(QString(rawdata));

    switch(message.command)
    {
        case Message::SLOGIN:
            login();
            break;
        case Message::CLOGIN:
            break;
        case Message::WRONGLOGIN:
            printError(tr("Wrong user name or password."));
            login();
            break;
        case Message::LOGGED:
            ui->statusBar->showMessage(tr("Logged as: ") + userName);
            break;
        case Message::CLIST:
            break;
        case Message::SLIST:
            emit netListArrived();
            break;
        case Message::SEND:
            //vložím na místo souhlasně pojmenované sítě v mainWindow, pokud
            // není žádná souhlasně pojmenovaná, vytvořím nový tab
            if((tab = findTab(scene->getName())) >= 0)
                replaceTab(scene,tab);
            else
                addTab(scene);
            deleteScene = false;
            break;
        case Message::ERROR:
            printError("Server sent: "+ message.errorText);
            break;
        case Message::SAVE:
            break;
        case Message::LOAD:
            break;
        case Message::SIMULATE:
            if((tab = findTab(scene->getName())) >= 0)
                replaceTab(scene,tab);
            else
                addTab(scene);
            deleteScene = false;
            break;
    }
    if(deleteScene)
        delete scene;
}

/**
  * Slot volaný pro odhlášení ze serveru, emituje disconnected()
  */
void MainWindow::disconnectFromServer()
{
    socket->disconnectFromHost();
}

/**
  * Vyskočí okno s výzvou k přihlášení k serveru jako uživatel s heslem, poté
  * pošle na server žádos o přihlášení
  */
void MainWindow::login()
{

    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        int x;
        QDialog dlg;
        QString user;
        QString password;

        Ui::loginWindow loginWindowUI;

        loginWindowUI.setupUi(&dlg);
        dlg.adjustSize();

        if ((x = dlg.exec()) == QDialog::Accepted)
        {
            user = loginWindowUI.LEUser->text();
//            password = loginWindowUI.LEPassword->text();
            password = QString(QCryptographicHash::hash(loginWindowUI.LEPassword->text().toLocal8Bit(),QCryptographicHash::Md5).toHex());
        }
        else if (x == QDialog::Rejected)
        {
            return;
        }

        userName = user;

        Message message;
        message.command = Message::CLOGIN;
        message.user = user;
        message.password = password;

        XMLHandler xmlhandler;
        xmlhandler.setMessage(&message);
        socket->write(xmlhandler.writeMessage().toUtf8());
        socket->flush();
    }
    else
    {
        printError("You have to be connected to server to log in.");
    }

}

/**
  * Uloží aktuální síť na vzdálené uložiště
  *
  */
void MainWindow::saveRemote()
{
    if(activeTab >= 0)
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
        {
            Message message;
            message.command = Message::SAVE;

            XMLHandler xmlhandler;
            xmlhandler.setScene(scenes.at(activeTab));
            xmlhandler.setMessage(&message);

            while(scenes.at(activeTab)->getName() == "")
            {
                if (netInformation() == QDialog::Rejected)
                    return;
                ui->statusBar->showMessage(tr("You have to insert at least name of the net."));
            }
            socket->write(xmlhandler.writeMessage().toUtf8());
            socket->flush();
            ui->statusBar->showMessage(tr("Net was sent to server."));
        }
        else
        {
            printError("You have to be connected to server to save Petri Net to remote repository.");
        }
    }
    else
    {
        printError(tr("No scene to handle"));
        return;
    }
}

/**
  * Otevře okno s výběrem sítí a možností stažení některé
  */
void MainWindow::openRemote()
{
    sendListRequest();
    netListForm->exec();

}

void MainWindow::sendLoadRequest(QString name, QString version, QString author)
{
    Message message;
    message.command = Message::LOAD;
    message.netName = name;
    message.netVersion = version;
    message.netAuthor = author;

    XMLHandler xmlhandler;
    xmlhandler.setMessage(&message);

    socket->write(xmlhandler.writeMessage().toUtf8());
    socket->flush();
    ui->statusBar->showMessage(tr("Request to open net was sent to server."));
}

/**
  * Pošle žádost o seznam sítí na server
  */
void MainWindow::sendListRequest()
{
    Message message;
    message.command = Message::CLIST;

    XMLHandler xmlhandler;
    xmlhandler.setMessage(&message);

    socket->write(xmlhandler.writeMessage().toUtf8());
    socket->flush();
}

/**
  * Pošle serveru žádost o simulaci aktivní scény až do konce
  */
void MainWindow::simulate()
{
    if(activeTab >=0)
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
        {
            Message message;
            message.command = Message::SIMULATE;
            message.simulationSteps = 0;

            XMLHandler xmlhandler;
            xmlhandler.setMessage(&message);
            xmlhandler.setScene(scenes.at(activeTab));

            socket->write(xmlhandler.writeMessage().toUtf8());
            socket->flush();
        }
        else
        {
            printError(tr("You have to be connected to server to simulate net."));
        }
    }
    else
    {
        printError(tr("No scene to handle"));
        return;
    }
}

/**
  * Pošle serveru žádost o simulaci aktivní scény o jeden krok, nejdříve je
  * třeba vybrat prováděný přechod
  */
void MainWindow::simulateStep()
{
    if(activeTab >=0)
    {
        if (socket->state() == QAbstractSocket::ConnectedState)
        {
            DiagramScene * scene = scenes.at(activeTab);
            QList<QGraphicsItem *> list = scene->selectedItems();

            if(list.isEmpty() || list.first()->type() != Transition::Type)
            {
                printError(tr("You have to choose transition to simulate one step of petri net"));
                return;
            }

            Transition * transition =qgraphicsitem_cast<Transition *>(list.first());
            transition->chosen = true;

            Message message;
            message.command = Message::SIMULATE;
            message.simulationSteps = 1;

            XMLHandler xmlhandler;
            xmlhandler.setMessage(&message);
            xmlhandler.setScene(scene);

            socket->write(xmlhandler.writeMessage().toUtf8());
            socket->flush();
        }
        else
        {
            printError(tr("You have to be connected to server to simulate net."));
        }
    }
    else
    {
        printError(tr("No scene to handle"));
        return;
    }
}

void MainWindow::settingsWindow()
{
    Settings sett(mySettings);
    sett.exec();
    loadSettings();
    scenes.at(activeTab)->update();

}

void MainWindow::loadSettings()
{
    if(mySettings->value("mainWindowMaximized","false").toBool())
        setWindowState(Qt::WindowMaximized);
    else
        setWindowState(Qt::WindowNoState);

    LINECOLOR = mySettings->value("lineColor","#000080").value<QColor>();
    DASHLINECOLOR = mySettings->value("dashLineColor","#000000").value<QColor>();
    FONT = mySettings->value("font","Sans Serif,10,-1,5,50,0,0,0,0,0").value<QFont>();
    qDebug()<< FONT;
//    FONTSIZE = mySettings->value("fontSize",10).toInt();
//    OBJECTSIZE = mySettings->value("objectSize",100).toInt();

}
















