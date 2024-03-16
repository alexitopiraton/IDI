#include "MyQLabel.h"
#include <iostream>
#include <cstdlib>

using namespace std;

MyQLabel::MyQLabel(QWidget *parent):QLabel(parent)
{
    time_t hora_actual = time(NULL);
    srand(hora_actual);
    stage = 1;
    score = 0;
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(iniciar_secuencia()));
    timer2 = new QTimer(this);
    connect(timer2, SIGNAL(timeout()), this, SLOT(mensajes_endgame()));
    open = true;

}

void MyQLabel::iniciar_secuencia() {
    if(neutro)
        setStyleSheet("QLabel { background-color : rgb(180, 180, 180)}");

    else {
        if(secuencia.size() == stage) {
            timer->stop();
            setStyleSheet("QLabel { background-color : rgb(180, 180, 180)}");
            emit habilitar_color();
            emit send_info("YOUR TURN!");
        }
        else {
            int random = rand();
            random = random%4;
            secuencia.push_back(random);

            // random = 0 -> rojo
            if(random == 0)
                setStyleSheet("QLabel { background-color : rgb(255, 0, 0)}");
            // random = 1 -> verde
            else if (random == 1)
                setStyleSheet("QLabel { background-color : rgb(85, 170, 0)}");
            // random = 2 -> azul
            else if (random == 2)
                setStyleSheet("QLabel { background-color : rgb(0, 170, 255)}");
            // random = 3 -> amarillo
            else if (random == 3)
                setStyleSheet("QLabel { background-color : rgb(255, 255, 0)}");
        }
    }

    neutro = not neutro;
    if(resetTimers) {
        timer->stop();
        setStyleSheet("QLabel { background-color : rgb(180, 180, 180)}");
    }

}

void MyQLabel::start() {
    if(open) open = false;
    neutro = false;
    resetTimers = false;
    end = 0;
    pos = 0;
    secuencia = QVector<int>();

    //emit signals
    emit deshabilitar_color();
    emit hide_start();
    emit print_score(QString::number(score,10));
    emit print_stage(QString::number(stage, 10));
    emit send_info("RUNNING SEQUENCE...");


    timer->start(2000);
}

void MyQLabel::sumaScore() {
    ++score;
    emit print_score(QString::number(score,10));
}

void MyQLabel::reset() {
    // reset atributos
    stage = 1;
    score = 0;
    setStyleSheet("QLabel { background-color : rgb(180, 180, 180)}");

    //reset timers
    resetTimers = true;

    //signals de reset
    emit print_stage(QString::number(stage,10));
    emit show_start();
    emit print_score(QString::number(score,10));
    emit send_info("PRESS PLAY");
}

void MyQLabel::mensajes_endgame() {
    if(resetTimers) timer2->stop();
    if (end == 0)
        emit send_info("YOU LOST :C");
    else {
        timer2->stop();
        reset();
    }
    ++end;

}

void MyQLabel::end_game() {
    if(not open) {
        emit send_info("YOU MISSED A COLOR!");
        emit deshabilitar_color();
        timer2->start(1000);
    }

}

void MyQLabel::next_stage() {
    ++stage;
    start();
}

void MyQLabel::boton_rojo() {
    if(0 == secuencia[pos] and not open) {
        ++pos;
        sumaScore();
        if(pos == stage) next_stage();
    }
    else end_game();
}

void MyQLabel::boton_verde() {
    if(1 == secuencia[pos] and not open) {
        ++pos;
        sumaScore();
        if(pos == stage) next_stage();
    }
    else end_game();
}

void MyQLabel::boton_azul() {
    if(2 == secuencia[pos] and not open) {
        ++pos;
        sumaScore();
        if(pos == stage) next_stage();
    }
    else end_game();
}

void MyQLabel::boton_amarillo() {
    if(3 == secuencia[pos] and not open) {
        ++pos;
        sumaScore();
        if(pos == stage) next_stage();
    }
    else end_game();
}
