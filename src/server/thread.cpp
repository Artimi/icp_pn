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
    DiagramScene *scene = new DiagramScene();
    QList<DiagramScene *> netList;

    XMLHandler xmlhandler;
    xmlhandler.readMessage(QString(rawdata));

    //message = *(xmlhandler.getMessage());
    //scene = xmlhandler.getScene();
    //netList = *(xmlhandler.getNetList());

    //qDebug() << socketDescriptor << "rozparsoval data";
    switch (message.command)
    {
        case Message::SLOGIN:
            break;
        case Message::CLOGIN:
            //rozparsovani a potvrzeni/odmitnuti prihlaseni
            if(!authenticate(message.user, message.password))
            {
                Message errmessage;
                XMLHandler result;
                errmessage.command = Message::WRONGLOGIN;
                result.setMessage(&errmessage);
                QString out = result.writeMessage();
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

            break;
        case Message::SLIST:
            break;
        case Message::SEND:
            break;
        case Message::ERROR:
            //klient udelal nejakou chybu
            break;
        case Message::SAVE:
            //dorucena sit urcena k ulozeni
            qDebug() << socketDescriptor << "Dorucena sit k ulozeni";
            tool.saveNet(scene->getName(),this->getUsername(),&xmlhandler);
            if (tool.error)
            {
                /* Nastala chyba pri ukladani */
                Message errmessage;
                XMLHandler result;
                errmessage.command = Message::ERROR;
                errmessage.errorText = "Error while saving";
                result.setMessage(&errmessage);
                QString out = result.writeMessage();
                rawdata = out.toUtf8();
                socket->write(rawdata);
                socket->flush();
            }
            else
            {
                /* Ulozeni probehlo v poradku */

            }
            break;
        case Message::LOAD:
            //pozadavek na nacteni site
        qDebug() << socketDescriptor << "Dorucen pozadavek na nacteni site";
            break;
        case Message::SIMULATE:
            //klient zada o simulaci site
            break;
    }
    delete scene;
}

/**
  * Overi, jestli uzivatel zadal spravne jmeno a heslo
  */
bool Thread::authenticate(QString username, QString passwd)
{
    //bool result = false;
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
