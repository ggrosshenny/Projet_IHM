#include "serveur.h"

Serveur::Serveur()
{

    ----------------
    Dans le constructeur du serveur :

    connect(socket, SIGNAL(readyRead()), this, SLOT(readSocket()));

    //----------------
    //Dans le serveur.c :

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

    ----------------
    Lien vers la documentation :
    https://github.com/mpv-player/mpv/blob/master/DOCS/man/ipc.rst
    Liste des commandes :
    https://github.com/mpv-player/mpv/blob/master/DOCS/man/input.rst
}
