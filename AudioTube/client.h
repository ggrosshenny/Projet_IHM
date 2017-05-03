#ifndef CLIENT_H
#define CLIENT_H
#include <QObject>
#include <QLocalSocket>
#include <QtConcurrent/QtConcurrent>
#include <QJsonObject>

class Client : public QObject
{
  Q_OBJECT
public:
  Client(QObject *parent = 0);
  ~Client();
  void readSocketOnServer();

private:
  QLocalSocket *m_socket;

private slots:
  void readSocket;
};

#endif // CLIENT_H
