#include "bluelinker.h"
#include <QDebug>
#include <cstdlib>
#include <ctime>

BlueLinker::BlueLinker()
{
    // TODO
}

BlueLinker::~BlueLinker()
{
    // TODO
}

void BlueLinker::startTalk()
{
    // TODO
}

void BlueLinker::update()
{

    // TODO

}




void BlueLinker::startServer()
{
    // TODO
}

void BlueLinker::startClient()
{
    // TODO
}

void BlueLinker::clientConnected()
{
    // TODO

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
    // TODO
}

void BlueLinker::done()
{
    // TODO
}

void BlueLinker::addService(const QBluetoothServiceInfo &service)
{
	// TODO
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
    // TODO
}

void BlueLinker::serverDisconnected()
{
	// TODO
}

void BlueLinker::readSocket()
{
	// TODO


}



void BlueLinker::setMessage(const QString &message)
{
	// TODO
}

int BlueLinker::role() const
{
    return m_role;
}

