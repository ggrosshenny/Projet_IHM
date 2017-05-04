#ifndef SENDJSONCOMMANDTOSERVER_H
#define SENDJSONCOMMANDTOSERVER_H



#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>


class SendJSONCommandToServer : public QObject
{
    Q_OBJECT
public:
    explicit SendJSONCommandToServer(QObject *parent = 0, QString servSocket = "/tmp/server-IHM");
    ~SendJSONCommandToServer();

    void sendRequestToServer(QJsonObject msg);
    void SendPlayToServ();
    void SendPauseToServ();
    void SendVolumeToServ(float);
    void SendStopToServ();
    void SendFastForwardToServ();
    void SendFastReturnToServ();
    void sendMusicToServer(QString musicName);
    void sendQuitToServer();
    void sendOffsetToServer(int offset);

private:
    QLocalSocket *server=NULL;
};

#endif // SENDJSONCOMMANDTOSERVER_H
