#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "client.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, Client client);
    ~MainWindow();

private slots:
    void setVolume(float volume);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
