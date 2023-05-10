#include "server.h"

Server::Server(QObject *parent) :
    server(new QTcpServer(this))
{
    connect(server, &QTcpServer::newConnection, this, &Server::onNewConnection);
    server->listen(QHostAddress::Any, 3333);
    qDebug() << "start";
}
Server::~Server()
{
    qDebug()<<"Server destroyed";
}

/*void Server::incommingConnection(int socketDescriptor)
{
    qDebug() << "connect";
    socket = new QTcpSocket(this);
    bool connected = (socket->state() == QTcpSocket::ConnectedState);
    qDebug() << connected;
    socket->setSocketDescriptor(socketDescriptor);
    connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));
    connect(socket, SIGNAL(disconnected()),this,SLOT(socDisc()));
    socket->write("hello");
    //VectorOfSockets.push_back(socket);
    qDebug() << "client connected"<< socketDescriptor;
}*/
void Server::onNewConnection()
{
    socket = server->nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &Server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::sockDisc);
    qDebug() << "new socket"<< socket->socketDescriptor();

}

void Server::slotReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
        QByteArray data = socket->readAll();
        //qDebug() << "Received data:" << data;
        QVector<QString> package(2);
        QDataStream in(data);
        in.setVersion(QDataStream::Qt_5_15);
        in >> package;
        qDebug() << "Received message of "<<socket->socketDescriptor() <<" for" << package[0] << ":" << package[1];
        if (clients[socket] == "")
        {
            clients[socket] = package[1];
        } else
            if (package[0] !="srever")
        {
            //отправка сообщения по логину
            QString recipient = package[0];
            package[0] = clients[socket];
            //QString message = package[1];
            for (auto it = clients.begin(); it != clients.end(); ++it)
            {
                if (it.value() == recipient)
                {
                    QByteArray block;
                    QDataStream out(&block, QIODevice::WriteOnly);
                    out.setVersion(QDataStream::Qt_5_15);
                    out << package;
                    it.key()->write(block);
                    break;
                }
            }
        }

}
void Server::sockDisc()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    qDebug()<<clients[socket]<<" is out";
    clients.remove(socket);
    socket->deleteLater();

}
