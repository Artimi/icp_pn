/**
  * @file thread.cpp
  *
  * @brief Zakladni trida pro vlakna na serveru
  * @author xsebek02, xsimon14
  */
#include "thread.h"
#include "simulate.h"

/**
  * Konstruktor vlakna
  */
Thread::Thread(int socketDescriptor, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketDescriptor;
    this->username = "guest";
    this->apppath = QFileInfo(QCoreApplication::applicationFilePath()).path() + "/";
}

/**
  * Spouštěcí procedura vlákna
  */
void Thread::run()
{
//    qDebug() <<  socketDescriptor <<"Starting thread";
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()),this, SLOT(handleRequest()),Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()),Qt::DirectConnection);

    qDebug() << socketDescriptor << "Client connected";

    writeLogConnect();
    exec();
}


/**
  * Zpracuje a spravne zvoli akce podle doruceneho packetu
  */
void Thread::handleRequest()
{
    QByteArray rawdata = socket->readAll();

    Message message;
    ManipulateNet tool;
    PetriNet *petriNet = new PetriNet;
    XMLHandler xmlhandler;
    QList<PetriNet *> netList;

    xmlhandler.setMessage(&message);
    xmlhandler.setPetriNet(petriNet);
    xmlhandler.setNetList(&netList);
    xmlhandler.readMessage(QString(rawdata));
    Message resultMsg;
    XMLHandler resultXml;
    QString out;
    PetriNet *resultNet = new PetriNet;
    Simulate simulate;

    switch (message.command)
    {
        case Message::SLOGIN:
            break;
        case Message::CLOGIN:
            /* Pozadavek na prihlaseni uzivatele */
            if(!authenticate(message.user, message.password))
            {
                resultMsg.command = Message::WRONGLOGIN;
                resultXml.setMessage(&resultMsg);
            }
            else
            {
                resultMsg.command = Message::LOGGED;
                resultXml.setMessage(&resultMsg);
            }
            out = resultXml.writeMessage();
            rawdata = out.toUtf8();
            socket->write(rawdata);
            socket->flush();
            break;
        case Message::WRONGLOGIN:
            break;
        case Message::LOGGED:
            break;
        case Message::CLIST:
            /* Pozadavek na seznam siti */
            qDebug() << socketDescriptor << "Request for list of nets";
            tool.getNetList(this->getUsername(),&netList);
            if (tool.error)
            {
                /* Nastala chyba pri ukladani */
                resultMsg.command = Message::ERROR;
                resultMsg.errorText = "Error while getting list of nets";
            }
            else
            {
                resultMsg.command = Message::SLIST;
                resultXml.setNetList(&netList);
                resultXml.setMessage(&resultMsg);
            }
            resultXml.setMessage(&resultMsg);
            out = resultXml.writeMessage();
            rawdata = out.toUtf8();
            socket->write(rawdata);
            socket->flush();
            /* Dealokuju seznam siti */
            for (int i = 0; i < netList.size(); i++)
            {
                delete netList.at(i);
            }
            netList.clear();
            break;
        case Message::SLIST:
            break;
        case Message::SEND:
            break;
        case Message::ERROR:
            break;
        case Message::SAVE:
            /* Pozadavek na ulozeni site */
            qDebug() << socketDescriptor << "Request for save a net";
            tool.saveNet(petriNet->getName(),this->getUsername(),&xmlhandler);
            if (tool.error)
            {
                /* Nastala chyba pri ukladani */
                resultMsg.command = Message::ERROR;
                resultMsg.errorText = "Error while saving";
                writeLogSave(petriNet->getName(), "FAIL");
                resultXml.setMessage(&resultMsg);
                out = resultXml.writeMessage();
                rawdata = out.toUtf8();
                socket->write(rawdata);
                socket->flush();
            }
            else
            {
                writeLogSave(petriNet->getName(), "PASS");
            }

            break;
        case Message::LOAD:
            /* Pozadavek na nacteni site */
            qDebug() << socketDescriptor << "Request for load the net";

            tool.loadNet(petriNet->getName(),this->getUsername(),petriNet->getVersion(),petriNet);
            if (tool.error)
            {
                /* Nastala chyba pri cteni, sit neexistuje? */
                resultMsg.command = Message::ERROR;
                resultMsg.errorText = "Error while loading, perhaps the net does not exist";
                writeLogLoad(petriNet->getName(), "FAIL");
            }
            else
            {
                /* Sit byla uspesne otevrena */
                resultMsg.command = Message::SEND;
                resultXml.setPetriNet(petriNet);
                writeLogLoad(petriNet->getName(), "PASS");
            }

            /* At uz se stala chyba nebo ne, vse je treba zabalit a odeslat */
            resultXml.setMessage(&resultMsg);
            out = resultXml.writeMessage();
            rawdata = out.toUtf8();
            socket->write(rawdata);
            socket->flush();
            break;
        case Message::SIMULATE:
            /* Pozadavek na simulaci */
            qDebug() << socketDescriptor << "Request for the simulation";
            if (message.simulationSteps == 0)
            {
                simulate.simulateAll(petriNet);
            }
            else
            {
                if(!simulate.simulateStep(petriNet))
                {
                    simulate.error = true;
                }
            }

            if (simulate.error)
            {
                /* Nastala chyba pri simulaci */
                resultMsg.command = Message::ERROR;
                resultMsg.errorText = "Error while simulation, perhaps there aren't any possible steps";
                writeLogSimulate(petriNet->getName(),petriNet->getVersion(),"FAIL");
            }
            else
            {
                /* Sit byla v poradku odsimulovana */
                resultMsg.command = Message::SEND;
                resultXml.setPetriNet(petriNet);
                writeLogSimulate(petriNet->getName(),petriNet->getVersion(),"PASS");
            }

            /* Zprava bude odeslana */
            resultXml.setMessage(&resultMsg);
            out = resultXml.writeMessage();
            rawdata = out.toUtf8();
            socket->write(rawdata);
            socket->flush();
            break;
        case Message::LOG:
            /* Klient zada o log */
            qDebug() << socketDescriptor << "Request for logs";
            resultMsg.user = getUsername();
            resultMsg.command = message.command;
            resultXml.setMessage(&resultMsg);
            out = resultXml.writeMessage();
            rawdata = out.toUtf8();
            socket->write(rawdata);
            socket->flush();
            break;
    }
    delete petriNet; // maže se opravdu vždycky?
    delete resultNet;
}

/**
  * Overi, jestli uzivatel zadal spravne jmeno a odpovidajici heslo
  * @param username Uzivatelske jmeno
  * @param passwd Heslo
  */
bool Thread::authenticate(QString username, QString passwd)
{
    QString path = getPath();
    QFile file(path + "passwd");
    if(!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QRegExp rx("^([^:]*):(.*)$");
    QString name,pass;
    QTextStream in(&file);
    QString line = in.readLine();
    while (!line.isNull())
    {
        if (rx.indexIn(line) != -1 )
        {
            /* Nacteny radek je validni zapis jmena s heslem */
            name = rx.cap(1);
            if (name == username)
            {
                /* Jsem na spravne radce */
                pass = rx.cap(2);
                if (passwd == pass)
                {
                    /* Heslo uzivatele odpovida */
                    this->username = username;
                    writeLogLogin();
                    qDebug() << socketDescriptor << "Logged as user" << username;
                    file.close();
                    return true;
                }
                else
                {
                    /* Heslo uzivatele neodpovida */
                    writeLogWronglogin(name);
                    return false;
                }
            }
        }
        else
        {
            /* Zaznamy v passwd nejsou validni! */
            return false;
        }
        line = in.readLine();
    }

    file.close();
    return false;
}

/**
  * Zapise do logu libovolny zaznam
  * @param event Udalost, ktera se ma do logu zapsat
  */
void Thread::writeLog(QString event)
{
    QFile file(getPath() + "server.log");
    mutex.lock();
    if(!file.open(QIODevice::Append))
    {
        mutex.unlock();
        qDebug() << "Error while opening file for logging";
        return;
    }
    QByteArray msg = QDateTime::currentDateTime().toString().toUtf8() + "#" + event.toUtf8() + "\n";
    file.write(msg);
    file.close();
    mutex.unlock();
}

/**
  * Zapise do logu zaznam o uspesnem prihlaseni
  */
void Thread::writeLogLogin()
{
    QString msg = getUsername() + "#" + QString::number(Message::LOG_LOGGED) + "#Successfully logged";
    writeLog(msg);
}

/**
  * Zapise do logu zapis o neuspesnem prihlaseni
  * @param name Prihlasovaci jmeno, ke kteremu se neuspesne prihlaseni vaze
  */
void Thread::writeLogWronglogin(QString name)
{
    QString msg = name + "#" + QString::number(Message::LOG_WRONGLOGIN) + "#Attempt to unauthorized access!";
    writeLog(msg);
}


/**
  * Zapise do logu zaznam o pripojeni klienta
  */
void Thread::writeLogConnect()
{
    QString msg = "#" + QString::number(Message::LOG_CONNECT) + "#" + QString::number(socketDescriptor);
    writeLog(msg);
}

/**
  * Zapise do logu zaznam o odpojeni klienta
  */
void Thread::writeLogDisconnect()
{
    QString msg = "#" + QString::number(Message::LOG_DISCONNECT) + "#" + QString::number(socketDescriptor);
    writeLog(msg);
}

/**
  * Zapise do logu zaznam o uspesnem prihlaseni
  */
void Thread::writeLogSimulate(QString netName, QString version, QString state)
{
    QString msg = getUsername() + "#" + QString::number(Message::LOG_SIMULATE) + "#The net \"" + netName\
            + ":" + version + "\" has been simulated. Result: " + state;
    writeLog(msg);
}


/**
  * Zapise do logu zaznam o uspesnem prihlaseni
  */
void Thread::writeLogSave(QString netName, QString state)
{
    QString msg = getUsername() + "#" + QString::number(Message::LOG_SAVE) + "#The net " + netName + " has been saved. Result: " + state;
    writeLog(msg);
}

/**
  * Zapise do logu zaznam o uspesnem prihlaseni
  */
void Thread::writeLogLoad(QString netName, QString state)
{
    QString msg = getUsername() + "#" + QString::number(Message::LOG_LOAD) + "#The net " + netName + " has been loaded. Result: " + state;
    writeLog(msg);
}

/**
  * Volá se při odhlášení klienta
  */
void Thread::disconnected()
{
    qDebug() << socketDescriptor << "Disconected";
    writeLogDisconnect();
    socket->deleteLater();
    exit(0);
}
