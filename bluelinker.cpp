#include "bluelinker.h"
#include <QDebug>
#include <cstdlib>
#include <ctime>

BlueLinker::BlueLinker():
    m_serverInfo(0), socket(0), discoveryAgent(0), interval(5), m_resultLeft(0), m_resultRight(0),
    m_showDialog(false), m_role(0),  m_serviceFound(false)
{
    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

BlueLinker::~BlueLinker()
{
    delete m_timer;
    delete m_serverInfo;
    delete socket;
    delete discoveryAgent;
}

void BlueLinker::startTalk()
{
    m_showDialog = false;
    emit showDialogChanged();
    //! [Start the link]
    if (m_role == 1){// server
        ;
    }


    m_timer->start(500);
    //! [Start the link]
}

void BlueLinker::update()
{

    int random_variable = std::rand();
    if (m_role == 1){// server
        //m_msg = "sever says:" + QString::number(random_variable) + "\n";
        //m_msg = "sever says:" + QString::number(*m_num) + "\n";
        m_msg = "PANDAi says:" + *m_string + "\n";
        QByteArray size;
        size.append(m_msg);
        socket->write(size.constData());
        //setMessage("I say:" + QString::number(random_variable) + "\n");
    }else if (m_role == 2){
        m_msg = "client says:" + QString::number(random_variable) + "\n";
        QByteArray size;
        size.append(m_msg);
        socket->write(size.constData());
    }



}




void BlueLinker::startServer()
{
    setMessage(QStringLiteral("Starting the server"));
    //! [Starting the server]
    m_serverInfo = new QBluetoothServer(QBluetoothServiceInfo::RfcommProtocol, this);
    connect(m_serverInfo, SIGNAL(newConnection()), this, SLOT(clientConnected()));
    connect(m_serverInfo, SIGNAL(error(QBluetoothServer::Error)),
            this, SLOT(serverError(QBluetoothServer::Error)));
    const QBluetoothUuid uuid(serviceUuid);

    m_serverInfo->listen(uuid, QStringLiteral("BlueLinker server"));
    //! [Starting the server]
    setMessage(QStringLiteral("Server started, waiting for the client. "));
    // m_role is set to 1 if it is a server
    m_role = 1;
    emit roleChanged();
}

void BlueLinker::startClient()
{
    //! [Searching for the service]
    discoveryAgent = new QBluetoothServiceDiscoveryAgent(QBluetoothAddress());

    connect(discoveryAgent, SIGNAL(serviceDiscovered(QBluetoothServiceInfo)),
            this, SLOT(addService(QBluetoothServiceInfo)));
    connect(discoveryAgent, SIGNAL(finished()), this, SLOT(done()));
    connect(discoveryAgent, SIGNAL(error(QBluetoothServiceDiscoveryAgent::Error)),
            this, SLOT(serviceScanError(QBluetoothServiceDiscoveryAgent::Error)));
    discoveryAgent->setUuidFilter(QBluetoothUuid(serviceUuid));
    discoveryAgent->start(QBluetoothServiceDiscoveryAgent::FullDiscovery);
    //! [Searching for the service]
    setMessage(QStringLiteral("Starting server discovery."));
    // m_role is set to 2 if it is a client
    m_role = 2;
    emit roleChanged();
}

void BlueLinker::clientConnected()
{
    //! [Initiating server socket]
    if (!m_serverInfo->hasPendingConnections()) {
        setMessage("FAIL: expected pending server connection");
        return;
    }
    socket = m_serverInfo->nextPendingConnection();
    if (!socket)
        return;
    socket->setParent(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnected()));
    connect(socket, SIGNAL(error(QBluetoothSocket::SocketError)),
            this, SLOT(socketError(QBluetoothSocket::SocketError)));
    //! [Initiating server socket]
    setMessage(QStringLiteral("Client connected."));

    QByteArray size;
    size.append(' ');
    QByteArray size1;
    size.append(size1);
    size.append("client \n");
    socket->write(size.constData());

}

void BlueLinker::clientDisconnected()
{
    setMessage(QStringLiteral("Client disconnected"));
    m_timer->stop();
}

void BlueLinker::socketError(QBluetoothSocket::SocketError error)
{
    Q_UNUSED(error);
    m_timer->stop();
}

void BlueLinker::serverError(QBluetoothServer::Error error)
{
    Q_UNUSED(error);
    m_timer->stop();
}

void BlueLinker::done()
{
    qDebug() << "Service scan done";
    if (!m_serviceFound)
        setMessage("BlueLinker service not found");
}

void BlueLinker::addService(const QBluetoothServiceInfo &service)
{
    setMessage("Service found. Setting parameters...");
    //! [Connecting the socket]
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    socket->connectToService(service);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(socket, SIGNAL(connected()), this, SLOT(serverConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(serverDisconnected()));
    //! [Connecting the socket]
    m_serviceFound = true;
}

void BlueLinker::serviceScanError(QBluetoothServiceDiscoveryAgent::Error error)
{
    setMessage(QStringLiteral("Scanning error") + error);
}

bool BlueLinker::showDialog() const
{
    return m_showDialog;
}

QString BlueLinker::message() const
{
    return m_message;
}

void BlueLinker::serverConnected()
{
    setMessage("Server Connected");
    QByteArray size;
    size.append("sever \n");
    socket->write(size.constData());
}

void BlueLinker::serverDisconnected()
{
    setMessage("Server Disconnected");
    m_timer->stop();
}

void BlueLinker::readSocket()
{
    if (!socket)
        return;
    const char sep = ' ';
    QByteArray line;
    while (socket->canReadLine()) {
        line = socket->readLine();
        //qDebug() << QString::fromUtf8(line.constData(), line.length());
        setMessage(QString::fromUtf8(line.constData(), line.length()));
    }
    QTimer::singleShot(500, this, SLOT(startTalk()));


}



void BlueLinker::setMessage(const QString &message)
{
    m_showDialog = true;
    m_message = message;
    emit showDialogChanged();
}

int BlueLinker::role() const
{
    return m_role;
}

