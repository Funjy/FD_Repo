#include "networker.h"
#include <QtNetwork>
#include <QObject>
#include "clientserverinterface.h"

//-------------------------------

ConnectionState NetWorker::EmptyConnection = ConnectionState(NULL);

quint16 _port;

bool NetWorker:: IsListening()
{
    return _serverSocket->isListening();
}

NetWorker::NetWorker(QObject *parent): QObject(parent)
{
    _currentConnection = &EmptyConnection;

    _serverSocket = new QTcpServer(this);

    _hasConnection = false;

    connect(_serverSocket, SIGNAL(newConnection()), this, SLOT(On_NewConnection()));
}

NetWorker::~NetWorker()
{
    ConnectionState* con;
    foreach (con, _connections) {        
        CloseConnection(con);
    }
    _serverSocket->close();
    delete _serverSocket;
}

bool NetWorker::StartListen(int port)
{
    if(_serverSocket->isListening())
        return true;
    _port = port;
    return _serverSocket->listen(QHostAddress::Any, port);
}

void NetWorker::StopListen()
{
    if(!_serverSocket->isListening())
        return;
    _port = 0;
    _serverSocket->close();
}

void NetWorker::CloseConnection(ConnectionState* connection)
{
    /*
    if(_currentConnection->Identifier().compare(connection->Identifier(), Qt::CaseInsensitive) == 0)
    {
        _hasConnection = false;
        if(connection->IsWorking)
            emit StopWorking();
        _currentConnection = &EmptyConnection;
    }*/
    //connection->Socket->close();
    //connection->Socket = NULL;
    _connections.removeOne(connection);
    //connection->~ConnectionState();
    delete connection;
    connection = NULL;
}

void NetWorker::On_NewConnection()
{
    /*
    if(_hasConnection)
    {
        //Сказать, что занят.
        _serverSocket->nextPendingConnection()->close();
        return;
    }*/
    QTcpSocket* soc = _serverSocket->nextPendingConnection();
    ConnectionState* con = new ConnectionState(soc);
    _connections.append(con);
    //On Received Event
    QSignalMapper* mapper = new QSignalMapper(this);
    connect(soc, SIGNAL(readyRead()), mapper, SLOT(map()));
    mapper->setMapping(soc, con->Identifier());
    connect(mapper, SIGNAL(mapped(QString)), this, SLOT(On_Receive(QString)));
    //---------------------------------------

    //On Disconnect Event
    QSignalMapper* diconnectMapper = new QSignalMapper(this);
    connect(soc, SIGNAL(disconnected()), diconnectMapper, SLOT(map()));
    diconnectMapper->setMapping(soc, con->Identifier());
    connect(diconnectMapper, SIGNAL(mapped(QString)), this, SLOT(On_DisconnectSocket(QString)));
    //connect(soc, SIGNAL(disconnected()), this, SLOT(On_DisconnectSocket(con)));
    _hasConnection = true;
}

void NetWorker::SendData(QByteArray data2send, QTcpSocket *soc)
{
    if(!_hasConnection)
        return;
    //_currentConnection->Socket->write(data2send->data());
    if(soc == NULL)
    {
        ConnectionState* con;
        foreach (con, _connections)
        {
            con->Socket->write(data2send);
        }

        //_connections.at(0)->Socket->write(data2send);
    }
    else
        soc->write(data2send);
    //_connections.at(0)->Socket->write()

}

void NetWorker::SendFrame(QByteArray data2send, QTcpSocket *soc)
{
    QByteArray ba;
    //header
    int hlen = HeadCommands::FRAME_HEADER.length();
    char(& ih_bytes)[4] = *(char(*)[4]) &hlen;
    ba.append(ih_bytes[0]);
    ba.append(ih_bytes[1]);
    ba.append(ih_bytes[2]);
    ba.append(ih_bytes[3]);
    ba.append(HeadCommands::FRAME_HEADER);
    //frame
    int dlen = data2send.length();
    char(& id_bytes)[4] = *(char(*)[4]) &dlen;
    ba.append(id_bytes[0]);
    ba.append(id_bytes[1]);
    ba.append(id_bytes[2]);
    ba.append(id_bytes[3]);
    ba.append(data2send);
    SendData(ba, soc);
}

void NetWorker::SendMessage(QString message, QTcpSocket *soc)
{
    QByteArray ba;
    //header
    int hlen = HeadCommands::MESSAGE_HEADER.length();
    char(& ih_bytes)[4] = *(char(*)[4]) &hlen;
    ba.append(ih_bytes[0]);
    ba.append(ih_bytes[1]);
    ba.append(ih_bytes[2]);
    ba.append(ih_bytes[3]);
    ba.append(HeadCommands::MESSAGE_HEADER);
    QByteArray b = SupportClass::QString2QByteArray(message);
    //message
    int dlen = b.length();
    char(& id_bytes)[4] = *(char(*)[4]) &dlen;
    ba.append(id_bytes[0]);
    ba.append(id_bytes[1]);
    ba.append(id_bytes[2]);
    ba.append(id_bytes[3]);
    ba.append(b);
    SendData(ba, soc);
}

void NetWorker::On_Receive(QString str/*ConnectionState* connection*/)
{
    ConnectionState* connection;
    foreach (connection, _connections) {
        if(connection->Identifier().compare(str) == 0)
        {
            break;
        }
    }
    QTcpSocket* soc = connection->Socket;
    quint16 avData = soc->bytesAvailable();
    if(avData < 1)
        return;
    QByteArray data = soc->readAll();
    QString incomeString = QString::fromUtf8(data);
    QStringList commands = incomeString.split(';', QString::SkipEmptyParts);
    QString command;
    if(commands.length() < 1)
    {
        SendMessage(HeadCommands::WRONG_REQUEST_STRING);
        return;
    }
    //--------Check all commands----------
    foreach (command, commands) {
        QStringList commandPair = command.split(':');
        if(commandPair.length() != 2)
        {
            SendMessage(HeadCommands::WRONG_REQUEST_STRING);
            continue;
        }
        if(commandPair[0] == HeadCommands::AUTHORIZE_REQUEST)       //Authorization
        {
            //Autorization
            if(commandPair[1].compare(SupportClass::LOGIN_STRING) != 0)
            {
                SendMessage(HeadCommands::AUTHORIZE_FAIL_RESPONSE);
                continue;
            }
            connection->IsAuthorized = true;
            SendMessage(HeadCommands::AUTHORIZE_SUCCESS_RESPONSE);
            continue;

        } else if(!connection->IsAuthorized)        //Check autorization
        {
            //Send him home
            SendMessage(HeadCommands::UNAUTHORIZED_REQUEST);
            continue;
        }
        if(commandPair[0] == HeadCommands::START)            //Start Working
        {
            //if(_currentConnection->Identifier().compare(connection->Identifier()) == 0)
              //  return;

            /*if(_currentConnection->IsWorking)
            {
                //Only 1 working client at a time is available
                SendMessage(HeadCommands::ANOTHER_CLIENT_CONNECTED);
                continue;
            } else
            {*/
                //Start working with client
                //_currentConnection = connection;
                //_currentConnection->IsWorking = true;
                //if(!_hasConnection)
                //    _hasConnection = true;
                emit StartWorkingEvent();
            //}

        } else if(commandPair[0] == HeadCommands::STOP)         //Stop Working
        {
            //if(_currentConnection->Identifier().compare(connection->Identifier()) == 0)
            //{
                /*if(!_currentConnection->IsWorking)
                    continue;
                else
                {*/
                    //_currentConnection->IsWorking = false;
                    //_hasConnection = false;
                    emit StopWorkingEvent();
                //}
            //}
            //else
            //{
            //    SendMessage(HeadCommands::ANOTHER_CLIENT_CONNECTED);
            //}
        } else if(commandPair[0] == HeadCommands::GRAB)
        {
            emit GrabFrameEvent();
        }
    }

    return;
}

void NetWorker::On_DisconnectSocket(QString id)
{
    ConnectionState* connection;
    foreach (connection, _connections) {
        if(connection->Identifier().compare(id) == 0)
        {
            break;
        }
    }
    CloseConnection(connection);
}

//---------------Connection State--------------------


ConnectionState::ConnectionState(QTcpSocket* soc)
{
    Socket = soc;   
    IsAuthorized = false;
    IsWorking = false;
}

ConnectionState::~ConnectionState()
{
    if(Socket == NULL)
        return;
    Socket->close();
    delete Socket;
    Socket = NULL;
}

QString ConnectionState::Identifier()
{
    if(Socket == NULL)
        return "-1";
    return Socket->peerAddress().toString() + QString::number(Socket->peerPort());
}

QString ConnectionState::IdentifySocket(QTcpSocket* soc)
{
    return soc->peerAddress().toString() + QString::number(soc->peerPort());
}
