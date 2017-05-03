#include "serveur.h"

Serveur::Serveur()
{
    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}


void Serveur::readSocket()
{
    while (socket->canReadLine()) {
        QByteArray line = socket->readLine().trimmed();

        // Pour le passage en JSON
        /*QJsonParseError error;
        QJsonDocument jDoc=QJsonDocument::fromJson(line, &error);
        QJsonObject jsonObject=jDoc.object();
        qDebug() << jsonObject["error"].toString();*/

        // ICI ON A RECU UN MESSAGE QU'ON PEUT TRAITER

        qDebug() << QString::fromUtf8(line.constData(), line.length());
    }
}
