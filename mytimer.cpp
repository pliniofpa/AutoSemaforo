#include "mytimer.h"
#include <QTime>
#include <QDebug>
#include <QTimeEdit>
MyTimer::MyTimer(QObject *parent) :
    QTimer(parent)
{
    this->setSingleShot(true);
}
void MyTimer::setTimer(const QTime &time){
    int remainingTime = QTime::currentTime().msecsTo(time);
    if(remainingTime>=0){
         qDebug()<<"Timer: "<<remainingTime;
        this->start(remainingTime);
    }else{
         qDebug()<<"Timer: "<<(86400000+remainingTime);
        this->start(86400000+remainingTime);
    }
}
void MyTimer::activeTimer(bool active){
    QTimeEdit *timeEdit = (QTimeEdit*)this->parent();
    if(active){
        QObject::connect(timeEdit,SIGNAL(timeChanged(QTime)),this,SLOT(setTimer(QTime)));
        this->setTimer(timeEdit->time());

    }else{        
        timeEdit->disconnect();
    }
}
