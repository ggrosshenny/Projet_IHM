#ifndef SendjsoncommandToMPV_H
#define SendjsoncommandToMPV_H

#include <QObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>


class SendjsoncommandToMPV : public QObject
{
    Q_OBJECT
public:
    explicit SendjsoncommandToMPV(QObject *parent = 0, QString mpvSocket = "/tmp/mpv-socket");
    ~SendjsoncommandToMPV();
    void changeMusicOnMPV(QString musicName);
    void changeVolumeOnMPV(int newVolume);
    void setPauseOnMPV(bool newValue);
    void setStopOnMPV();
    void quitMPVServer();
    void getVolumeFromMPV();

private:
    QLocalSocket *mpv=NULL;
    void sendRequestToMPV(QJsonObject msg);
};

#endif // SendjsoncommandToMPV_H
