#ifndef SERVER_H
#define SERVER_H
#include<QTcpServer>
#include<QTcpSocket>
#include<QVector>
#include <QJsonObject>
#include <QJsonDocument>
class Server: public QTcpServer{
    Q_OBJECT
public:
public:
    explicit Server(QObject *parent = nullptr);
    ~Server();
    QTcpSocket *socket;
    QVector<QTcpSocket*> VectorOfSockets;
    QByteArray Data;
    void SendToClient();
    QTcpServer *server;
    QMap<QTcpSocket*, QString> clients;
public slots:
   // void incommingConnection(int socketDescriptor);
    void slotReadyRead();
    void sockDisc();
    void onNewConnection();


    //void newConnection();
};

#endif // SERVER_H
