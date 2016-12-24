#ifndef BLUELINKER_H
#define BLUELINKER_H

#include <QTimer>
#include <QObject>
#include <qbluetoothserver.h>
#include <qbluetoothserviceinfo.h>
#include <qbluetoothlocaldevice.h>
#include <qbluetoothservicediscoveryagent.h>

static const QString serviceUuid(QStringLiteral("e8e10f95-1a70-4b27-9ccf-02010264e9c9"));

class BlueLinker: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool showDialog READ showDialog NOTIFY showDialogChanged)
    Q_PROPERTY(QString message READ message NOTIFY showDialogChanged)
    Q_PROPERTY(int role READ role NOTIFY roleChanged)

public:
    BlueLinker();
    ~BlueLinker();
    int *m_num;
    QString *m_string;

    bool showDialog() const;
    QString message() const;
    void setMessage(const QString &message);
    int role() const;



public slots:
    void startTalk();
    void update();



    void startServer();
    void startClient();
    void clientConnected();
    void clientDisconnected();
    void serverConnected();
    void serverDisconnected();
    void socketError(QBluetoothSocket::SocketError);
    void serverError(QBluetoothServer::Error);
    void serviceScanError(QBluetoothServiceDiscoveryAgent::Error);
    void done();
    void addService(const QBluetoothServiceInfo &);
    void readSocket();

Q_SIGNALS:

    void showDialogChanged();
    void roleChanged();
    void resultChanged();

private:
    QBluetoothServer *m_serverInfo;
    QBluetoothServiceInfo m_serviceInfo;
    QBluetoothSocket *socket;
    QBluetoothServiceDiscoveryAgent *discoveryAgent;

    QTimer *m_timer;

    float m_direction;
    float m_targetX;
    float m_targetY;
    int interval;
    int m_resultLeft;
    int m_resultRight;
    bool m_showDialog;
    QString m_message;
    int m_role;

    bool m_serviceFound;



    QString m_msg;//
};

#endif // BLUELINKER_H
