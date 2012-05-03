/**
  * @file thread.cpp
  *
  * @brief Zakladni trida pro vlakna na serveru
  * @author xsebek02, xsimon14
  */
#include "thread.h"

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
    qDebug() <<  socketDescriptor <<"Starting thread";
    socket = new QTcpSocket();
    if (!socket->setSocketDescriptor(socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()),this, SLOT(handleRequest()),Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()),Qt::DirectConnection);

    qDebug() << socketDescriptor << "Client connected";

    exec();
}


/**
  * Zpracuje a spravne zvoli akce podle doruceneho packetu
  */
void Thread::handleRequest()
{
    QByteArray rawdata = socket->readAll();
    qDebug() << socketDescriptor << "Data in:" << rawdata;

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
            qDebug() << socketDescriptor << "Sended lists:" << rawdata;
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
                resultXml.setMessage(&resultMsg);
                out = resultXml.writeMessage();
                rawdata = out.toUtf8();
                socket->write(rawdata);
                socket->flush();
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
            }
            else
            {
                /* Sit byla uspesne otevrena */
                resultMsg.command = Message::SEND;
                resultXml.setPetriNet(petriNet);
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
                    qDebug() << socketDescriptor << "Logged as user" << username;
                    file.close();
                    return true;
                }
                else
                {
                    /* Heslo uzivatele neodpovida */
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
  * Volá se při odhlášení klienta
  */
void Thread::disconnected()
{
    qDebug() << socketDescriptor << "Disconected";
    socket->deleteLater();
    exit(0);
}
