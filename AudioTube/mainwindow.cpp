#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, Client client) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->Play, SIGNAL(playSignal()),client , SLOT(Client::setPlay()));
    QObject::connect(ui->Arret, SIGNAL(stopSignal()), client, SLOT(Client::setStop()));
    QObject::connect(ui->Pause, SIGNAL(pauseSignal()), client, SLOT(Client::setPause()));
    QObject::connect(ui->AvanceRapide, SIGNAL(FFrwdSignal()), client, SLOT(Client::setAR()));
    QObject::connect(ui->RetourRapide, SIGNAL(FRtrnSignal()), client, SLOT(Client::setRR()));
    QObject::connect(ui->buttonQuit, SIGNAL(quit()), client, SLOT(Client::quit()));
    QObject::connect(this, SIGNAL(pressed())), ui->controleVolume, SLOT(Client::setVolume(float));

}

MainWindow::~MainWindow()
{
    delete ui;
}


//connect signal reçu UI vers SLOT dans Client qui effectue l'action
