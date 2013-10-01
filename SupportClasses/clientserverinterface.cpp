#include "clientserverinterface.h"

ClientServerInterface::ClientServerInterface()
{
}

//--------------------------Head Commands--------------------------------
const QString HeadCommands::MESSAGE_HEADER = QString("tmessage");
const QString HeadCommands::FRAME_HEADER = QString("tframe");
const QString HeadCommands::START = QString("send_start");
const QString HeadCommands::STOP = QString("send_stop");
const QString HeadCommands::GRAB = QString("send_grab");
const QString HeadCommands::START_SUCCESS_RESPONSE = QString("start_success");
const QString HeadCommands::STOP_SUCCESS_RESPONSE = QString("stop_success");
const QString HeadCommands::AUTHORIZE_REQUEST = QString("authorize_me");
const QString HeadCommands::AUTHORIZE_SUCCESS_RESPONSE = QString("auth_success");
const QString HeadCommands::AUTHORIZE_FAIL_RESPONSE = QString("auth_fail");
const QString HeadCommands::ANOTHER_CLIENT_CONNECTED = QString("another_connected");
const QString HeadCommands::STATUS = QString("get_status");
const QString HeadCommands::UNAUTHORIZED_REQUEST = QString("u'r_not_authorized");
const QString HeadCommands::WRONG_REQUEST_STRING = QString("wrong_request_string");
const QString HeadCommands::NOT_INITIALIZED = QString("not_initialized");
const QString HeadCommands::CAPTURE_CLOSED = QString("capture_closed");

//--------------------------Support Constants--------------------------------
const QString SupportClass::LOGIN_STRING = QString("here's_login");

QByteArray SupportClass::QString2QByteArray(QString strToBytes)
{
    QByteArray data;
    data.append(strToBytes);
    return data;
}
