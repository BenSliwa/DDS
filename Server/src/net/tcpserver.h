#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QDebug>
#include "tcpconnection.h"

class TcpServer : public QObject
{
    Q_OBJECT
public:
    TcpServer(QObject *_parent = 0);

private slots:
    void onNewConnection();

private:
    QTcpServer m_server;
};

#endif // TCPSERVER_H
