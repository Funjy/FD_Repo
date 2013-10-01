#ifndef NETWORKER_H
#define NETWORKER_H
#include <QObject>
#include <QtNetwork>

class ConnectionState
{
public:
    ConnectionState(QTcpSocket* soc);
    ~ConnectionState();
    QTcpSocket* Socket;
    bool IsAuthorized;
    bool IsWorking;
    QString Identifier();

    static QString IdentifySocket(QTcpSocket *soc);
};

class NetWorker : public QObject
{
    Q_OBJECT

public:
    NetWorker(QObject * parent = 0);
    ~NetWorker();
    bool StartListen(int port);
    void StopListen();
    void CloseConnection(ConnectionState *connection);
    bool IsListening();
    void SendData(QByteArray data2send, QTcpSocket* soc = NULL);
    void SendFrame(QByteArray data2send, QTcpSocket* soc = NULL);
    void SendMessage(QString message, QTcpSocket *soc = NULL);
    static ConnectionState EmptyConnection;

signals:
    void StartWorkingEvent();
    void StopWorkingEvent();
    void GrabFrameEvent();

private:
    QList<ConnectionState*> _connections;
    ConnectionState* _currentConnection;    
    QTcpServer* _serverSocket;
    //QTcpSocket* _connectedSocket;
    //QNetworkSession *network_session;
    bool _hasConnection;

private slots:
   void On_NewConnection();
   void On_DisconnectSocket(QString id);
   void On_Receive(QString str);
};





#endif // NETWORKER_H
