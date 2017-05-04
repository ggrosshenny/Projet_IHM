#ifndef SENDJSONCOMMANDTOSERVER_H
#define SENDJSONCOMMANDTOSERVER_H



#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>


class SendJSONCommandToServer : public QObject
{
    Q_OBJECT
public:
    explicit SendJSONCommandToServer(QObject *parent = 0, QString servSocket);
    ~SendJSONCommandToServer();

    void sendRequestToServer(QJsonObject msg);
    void SendPlayToServ();
    void SendPauseToServ();
    void SendVolumeToServ(float);
    void SendStopToServ();
    void SendFastForwardToServ();
    void SendFastReturnToServ();
private:
    QLocalSocket *server=NULL;
};

#endif // SENDJSONCOMMANDTOSERVER_H
