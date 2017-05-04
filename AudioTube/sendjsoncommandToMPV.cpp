#include "sendjsoncommandToMPV.h"


SendjsoncommandToMPV::SendjsoncommandToMPV(QObject *parent, QString mpvSocket) :
    QObject(parent),
    mpv(new QLocalSocket(this))
{
    // Connecting to mpv server
    qDebug() << "Connecting to server....";
    mpv->connectToServer(mpvSocket);

    // Verify if we are connected
    if(mpv->waitForConnected())
    {
        qDebug() << "Connected to mpv server.";
    }
    else
    {
        mpv->error();
    }

}


SendjsoncommandToMPV::~SendjsoncommandToMPV()
{
    mpv->disconnectFromServer();
}


/**
* Method : sendRequestToMPV
* Param : QJsonObject, msg - message to send to the mpv server
* Desc : Send a JSON request to the mpv server
* Return : void
**/
void SendjsoncommandToMPV::sendRequestToMPV(QJsonObject msg)
{
    // Converting the JSON msg into bytes msg
    QByteArray bytes = QJsonDocument(msg).toJson(QJsonDocument::Compact)+"\n";
    if(mpv!=NULL)
    {
        mpv->write(bytes.data(), bytes.length());
        mpv->flush();
    }
}


/**
* Method : changeMusicOnMPV
* Param : QString, musicName - name of the music to play
* Desc : Create the QJsonObject with the music name and send it to the mpv server
* Return : bool, if success return true, else return false
**/
void SendjsoncommandToMPV::changeMusicOnMPV(QString musicName)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("loadfile");
    jsonArr.append(musicName);

    jsonObject["command"]=jsonArr;

    SendjsoncommandToMPV::sendRequestToMPV(jsonObject);

}


/**
* Method : changeVolumeOnMPV
* Param : int, newVolume - new volume to send to MPV server
* Desc : Create the QJsonObject with the volume value and send it to the mpv server
* Return : void
**/
void SendjsoncommandToMPV::changeVolumeOnMPV(int newVolume)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("set_property");
    jsonArr.append("volume");
    jsonArr.append(newVolume);

    jsonObject["command"]=jsonArr;

    SendjsoncommandToMPV::sendRequestToMPV(jsonObject);

}


/**
* Method : setPauseOnMPV
* Param : bool, newValue - the new boolean value to set
* Desc : Create the QJsonObject with the command to set pause on the mpv server
* Return : void
**/
void SendjsoncommandToMPV::setPauseOnMPV(bool newValue)
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("set_property");
    jsonArr.append("pause");
    jsonArr.append(newValue);

    jsonObject["command"]=jsonArr;

    SendjsoncommandToMPV::sendRequestToMPV(jsonObject);

}


/**
* Method : setStopOnMPV
* Param : void
* Desc : Stop the current played music
* Return : void
**/
void SendjsoncommandToMPV::setStopOnMPV()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("stop");

    jsonObject["command"]=jsonArr;

    SendjsoncommandToMPV::sendRequestToMPV(jsonObject);
}


/**
* Method : quitMPVServer
* Param : void
* Desc : shut down and quit the MPV server
* Return : void
**/
void SendjsoncommandToMPV::quitMPVServer()
{
    QJsonObject jsonObject;
    QJsonArray jsonArr;

    // Creating the JSON message
    jsonArr.append("quit");

    jsonObject["command"]=jsonArr;

    SendjsoncommandToMPV::sendRequestToMPV(jsonObject);
}


/**
 * @brief SendjsoncommandToMPV::CurrentUpdate
 * Param : void
 * Desc : Check for possible update of the current track on MPV
 */
void SendjsoncommandToMPV::CurrentUpdate(){

    QJsonObject jsonObject;
    QJsonArray jsonArr;

    jsonArr.append("observe_property");
    jsonArr.append(1);
    jsonArr.append("title");
    jsonArr.append("observe_property");
    jsonArr.append(2);
    jsonArr.append("volume");
    jsonArr.append("observe_property");
    jsonArr.append(3);
    jsonArr.append("percent-pos");
    jsonArr.append("observe_property");
    jsonArr.append(4);
    jsonArr.append("duration");
    jsonArr.append("observe_property");
    jsonArr.append(5);
    jsonArr.append("pause");

    jsonObject["command"] = jsonArr;

    SendjsoncommandToMPV::sendRequestToMPV(jsonObject);
}
