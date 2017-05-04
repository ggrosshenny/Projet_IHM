#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QObject>
#include <QLocalSocket>
#include <QtConcurrent/QtConcurrent>
#include <QJsonObject>
#include "client.h"
#include "sendjsoncommandtoserver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent);
    ~MainWindow();

private slots:
    // Slots pour modifier l'UI
    void changePositionOffset(int offset);
    void changeTempsTotal(int tempsTotal);
    void changeTempsEcoule(int tempsEcoule);
    void changeVolume(int volume);

    // Slot pour lire sur le socket du serveur
    void readSocketOnServer();

    // Slots pour envoyer un message au serveur
    void setVolumeOnServer(int volume);
    void setRR();
    void setAR();
    void setPlay();
    void setPause();
    void setStop();
    void quit();

private:
    Ui::MainWindow *ui;
    QLocalSocket *m_socket;
    QLocalSocket *ServerSocket;
    SendJSONCommandToServer *jsonToSrv;

signals:
    // modification de l'UI
    void setVolume(int volume);
    void setoffsetMusique(int offset);
    void setTempsTotal(int tempsTotal);
    void setTempsEcoule(int tempsEcoule);
};

#endif // MAINWINDOW_H
