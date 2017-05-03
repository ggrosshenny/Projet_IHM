#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(mpv_socket, SIGNAL(progressUpdate(int)), sliderLecteur, SLOT(setValue(int)));
    QObject::connect(ui->buttonPlay, SIGNAL(playSignal()), this, SLOT());
    QObject:: connect(ui->buttonStop, SIGNAL(stopSignal()), this, SLOT());
    QObject:: connect(ui->buttonPause, SIGNAL(pauseSignal()), this, SLOT());
    QObject:: connect(ui->buttonFFrwd, SIGNAL(FFrwdSignal()), this, SLOT());
    QObject:: connect(ui->buttonFRtrn, SIGNAL(FRtrnSignal()), this, SLOT());
    QObject:: connect(ui->buttonQuit, SIGNAL(quit()), this, SLOT());
    QObject::connect(this, SIGNAL(pressed())), ui->controleVolume, SLOT(setVolume());


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setVolume(float volume){
    ui->controleVolume->setValue(volume);
}
