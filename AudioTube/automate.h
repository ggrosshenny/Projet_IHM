#ifndef AUTOMATE_H
#define AUTOMATE_H

#include <QObject>
#include <QStateMachine>
#include <QState>
#include <QHistoryState>
#include <QFinalState>
#include <QSignalTransition>
#include <QTimer>
#include <QDebug>



class Automate
{
    Q_OBJECT
public:
    explicit Automate(QObject *parent = 0);

private:
    // Machine à états
    QStateMachine *machine;

    // Les états de la machine
    QState *pause;
    QState *play;
    QState *retourRapide;
    QState *avanceRapide;
    QState *stop;

    // Un état global contenant les états de la machine hors pause avec l'historique
    QState *fonction;
    QHistoryState *fonctionHistory;

    // Etat de sortie de l'automate
    QFinalState *fin;

    // Transition entre les états
    QSignalTransition *test;

};

#endif // AUTOMATE_H
