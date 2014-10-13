#ifndef MYTIMER_H
#define MYTIMER_H

#include <QTimer>

class MyTimer : public QTimer
{
    Q_OBJECT
public:
    explicit MyTimer(QObject *parent = 0);

signals:

public slots:
    void setTimer(const QTime &time);
    void activeTimer(bool active);

};

#endif // MYTIMER_H
