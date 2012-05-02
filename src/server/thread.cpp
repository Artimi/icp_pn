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
    this->username = "host";
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
  * volá se při příchozích datech, data převezme zpracuje a pošle odpověď pokud
  * je třeba
  */
/*void Thread::readyRead()
{
    QByteArray data = socket->readAll();
    qDebug() << socketDescriptor << "Data in:" << data;
//    Message message;

//    DiagramScene scene(NULL,NULL,NULL);

//    XMLHandler xmlhandler(&scene,&message);
//    xmlhandler.readMessage(QString(data));

//    QString out = xmlhandler.writeMessage(); //jen zkouším parsing, mělo by se vrátit cojsem poslal

//    data = out.toLatin1();

//    qDebug() << socketDescriptor << "Data out:" << data;
    socket->write(data);
    socket->flush();
}*/

/**
  * Zpracuje a spravne zvoli akce podle doruceneho packetu
  */
void Thread::handleRequest()
//void Thread::readyRead()
{
    QByteArray rawdata = socket->readAll();
    qDebug() << socketDescriptor << "Data in:" << rawdata;
    Message message;
    ManipulateNet tool;
    qDebug() << "pred scenou";

    PetriNet *petriNet = new PetriNet;
    qDebug()<< "za scenou";
    QList<PetriNet *> netList;


    XMLHandler xmlhandler;//FIXME -------------------------------
    //přidáno setXXX do handleru se to vkládá ručně venkem protože to tam potřebuji jen naplnit
    xmlhandler.setMessage(&message); //FIXME ------------------------------
//    xmlhandler.setPetriNet(petriNet); //TODO v xmlhandler
//    xmlhandler.setPetriNetList(&netList); //TODO v xmlhandler
    xmlhandler.readMessage(QString(rawdata));//FIXME ------------------------------------



    Message resultMsg;
    XMLHandler resultXml;//FIXME -------------------------------
    QString out;
    PetriNet *resultNet = new PetriNet;

    switch (message.command)
    {
        case Message::SLOGIN:
            break;
        case Message::CLOGIN:
            //rozparsovani a potvrzeni/odmitnuti prihlaseni
            if(!authenticate(message.user, message.password))
            {
                resultMsg.command = Message::WRONGLOGIN;
                resultXml.setMessage(&resultMsg); //FIXME ----------------------------
                out = resultXml.writeMessage();//FIXME -------------------------
                rawdata = out.toUtf8();
                socket->write(rawdata);
                socket->flush();
            }
            break;
        case Message::WRONGLOGIN:
            break;
        case Message::LOGGED:
            break;
        case Message::CLIST:
            //vrati seznam siti
            netList = tool.getNetList(this->getUsername());
            if (tool.error)
            {
                /* Nastala chyba pri ukladani */
                resultMsg.command = Message::ERROR;
                resultMsg.errorText = "Error while getting list of nets";
            }
            else
            {
                resultMsg.command = Message::SLIST;
                //resultXml.setNetList(netList); //FIXME --------------------------
            }
            resultXml.setMessage(&resultMsg); //FIXME ------------------------------
            out = resultXml.writeMessage(); //FIXME ----------------------------
            rawdata = out.toUtf8();
            socket->write(rawdata);
            socket->flush();
            break;
        case Message::SLIST:
            break;
        case Message::SEND:
            break;
        case Message::ERROR:
            break;
        case Message::SAVE:
            //dorucena sit urcena k ulozeni
            qDebug() << socketDescriptor << "Dorucena sit k ulozeni";
            tool.saveNet(petriNet->getName(),this->getUsername(),&xmlhandler); //FIXME --------------------------
            if (tool.error)
            {
                /* Nastala chyba pri ukladani */
                resultMsg.command = Message::ERROR;
                resultMsg.errorText = "Error while saving";
                resultXml.setMessage(&resultMsg);//FIXME ---------------------------
                out = resultXml.writeMessage();//FIXME ---------------------------
                rawdata = out.toUtf8();
                socket->write(rawdata);
                socket->flush();
            }

            break;
        case Message::LOAD:
            //pozadavek na nacteni site
            qDebug() << socketDescriptor << "Dorucen pozadavek na nacteni site";

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
                //resultXml.setScene(&petriNet); //FIXME ----------------------
            }

            /* At uz se stala chyba nebo ne, vse je treba zabalit a odeslat */
            resultXml.setMessage(&resultMsg);//FIXME -----------------------
            out = resultXml.writeMessage();//FIXME ----------------------------------
            rawdata = out.toUtf8();
            socket->write(rawdata);
            socket->flush();
            break;
        case Message::SIMULATE:
            //klient zada o simulaci site
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
    QFile file("passwd");
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
                pass = QString(QCryptographicHash::hash(rx.cap(2).toLocal8Bit(),QCryptographicHash::Md5).toHex());
                if (passwd == pass)
                {
                    /* Heslo uzivatele odpovida */
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
    return true;
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
