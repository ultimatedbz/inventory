#ifndef SCROLLEATER_H
#define SCROLLEATER_H

#include <QObject>

class ScrollEater : public QObject
{
    Q_OBJECT
public:
    explicit ScrollEater(QObject *parent = 0);
    ~ScrollEater();

signals:

protected:
    bool eventFilter(QObject *obj, QEvent *event);


};

#endif // SCROLLEATER_H
