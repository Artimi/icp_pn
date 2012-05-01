#include "thread.h"

Thread::Thread(int socketDescriptor, QObject *parent) :
    QThread(parent)
{
    this->socketDescriptor = socketDescriptor;
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

    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()),Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()),Qt::DirectConnection);

    qDebug() << socketDescriptor << "Client connected";

    exec();
}

/**
  * volá se při příchozích datech, data převezme zpracuje a pošle odpověď pokud
  * je třeba
  */
void Thread::readyRead()
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
}

/**
  * Zpracuje a spravne zvoli akce podle doruceneho packetu
  */
void Thread::handleRequest()
{
    QByteArray rawdata = socket->readAll();

    Message message;

    XMLHandler xmlhandler;
    //xmlhandler.setScene(scene);
    xmlhandler.setMessage(&message);
    //xmlhandler.setNetList(&netList);
    xmlhandler.readMessage(QString(rawdata));

    switch (message.command)
    {
        case Message::SLOGIN:
            break;
        case Message::CLOGIN:
            //rozparsovani a potvrzeni/odmitnuti prihlaseni
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
            break;
        case Message::LOAD:
            //pozadavek na nacteni site
            break;
        case Message::SIMULATE:
            //klient zada o simulaci site
            break;
    }
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
