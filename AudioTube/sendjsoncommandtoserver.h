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
    explicit SendJSONCommandToServer(QObject *parent = 0);
    ~SendJSONCommandToServer();
private:
    QLocalSocket *mpv=NULL;
};

#endif // SENDJSONCOMMANDTOSERVER_H
