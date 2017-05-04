#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QLocalSocket>
#include <QtConcurrent/QtConcurrent>
#include <QJsonObject>
#include "sendjsoncommandtoserver.h"

class Client : public QObject
{
  Q_OBJECT
public:
  Client(QObject *parent = 0);
  ~Client();
  void readSocketOnServer();

private:
  QLocalSocket *m_socket;
  SendJSONCommandToServer JSONToSrv;

private slots:
  void readSocket();
  void setVolume(float volume);
  void setRR();
  void setAR();
  void setPlay();
  void setPause();
  void setStop();
  void quit();
};

#endif // CLIENT_H
