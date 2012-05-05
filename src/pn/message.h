#ifndef MESSAGE_H
#define MESSAGE_H

#include <QtCore>
#include "diagramscene.h"

class Message
{
public:
    Message();
    enum Commands {SLOGIN,CLOGIN,WRONGLOGIN,LOGGED,CLIST,SLIST, SEND, ERROR, SAVE, LOAD, SIMULATE, LOG};
    enum LogCodes {LOGGED, WRONGLOGIN,CONNECT,DISCONNECT,SIMULATE,SAVE,LOAD};

    Commands command;
    LogCodes logcode;
    QString user;
    QString password;
    QString netName;
    QString netVersion;
    QString netAuthor;
    QString errorText;
    int simulationSteps;
    QString activeTransitionName;
    QString messageText;
};

#endif // MESSAGE_H
