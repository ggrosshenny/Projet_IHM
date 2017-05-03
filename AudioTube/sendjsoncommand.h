#ifndef SENDJSONCOMMAND_H
#define SENDJSONCOMMAND_H

#include <QJsonObject>
#include <QLocalServer>
#include <QLocalSocket>
#include <QDebug>


class SendJSONCommand : public QObject
{
    Q_OBJECT
public:
    explicit SendJSONCommand(QObject *parent = 0);
    ~SendJSONCommand();
private:
    QLocalSocket *mpv=NULL;
    void sendRequestToMPV();
};

#endif // SENDJSONCOMMAND_H
