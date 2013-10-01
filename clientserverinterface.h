#ifndef CLIENTSERVERINTERFACE_H
#define CLIENTSERVERINTERFACE_H

#include <QtCore>

class ClientServerInterface
{
public:
    ClientServerInterface();
};

namespace NetworkConstans {
class HeadCommands;
class SupportConstants;
}

class HeadCommands
{
public:
    static const QString START;
    static const QString STOP;
    static const QString GRAB;
    static const QString START_SUCCESS_RESPONSE;
    static const QString STOP_SUCCESS_RESPONSE;
    static const QString AUTHORIZE_REQUEST;
    static const QString STATUS;
    static const QString AUTHORIZE_SUCCESS_RESPONSE;
    static const QString AUTHORIZE_FAIL_RESPONSE;
    static const QString UNAUTHORIZED_REQUEST;
    static const QString ANOTHER_CLIENT_CONNECTED;
    static const QString WRONG_REQUEST_STRING;
    static const QString NOT_INITIALIZED;
    static const QString MESSAGE_HEADER;
    static const QString FRAME_HEADER;
    static const QString CAPTURE_CLOSED;
};

class SupportClass
{
public:
    static const QString LOGIN_STRING;
    static QByteArray QString2QByteArray(QString strToBytes);
};

#endif // CLIENTSERVERINTERFACE_H
