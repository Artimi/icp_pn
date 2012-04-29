#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtCore>
#include "diagramscene.h"

class Message
{
public:
    Message();
    enum Commands {SLOGIN,CLOGIN,WRONGLOGIN,LOGGED,CLIST,SLIST, SEND, ERROR, SAVE, LOAD, SIMULATE};

    Commands command;
    QString user;
    QString password;
    QString netName;
    QString netVersion;
    QString netAuthor;
    QString errorText;
    int simulationSteps;

    QString messageText;
};

#endif // MESSAGE_H
