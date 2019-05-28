#include "tcpserver.h"

TcpServer::TcpServer(QObject *_parent) :
    QObject(_parent)
{
    qDebug() << "TcpServer::TcpServer" << m_server.listen(QHostAddress::Any, 8237);
    connect(&m_server, SIGNAL(newConnection()), this, SLOT(onNewConnection()));
}

void TcpServer::onNewConnection()
{
    qDebug() << "TcpServer::onNewConnection";

    QTcpSocket *socket = 0;
    while(socket=m_server.nextPendingConnection())
    {
        TcpConnection *tcp = new TcpConnection(socket, this);
    }
}


