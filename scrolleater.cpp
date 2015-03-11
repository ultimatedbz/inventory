#include "scrolleater.h"
#include <QDebug>
#include <QEvent>
ScrollEater::ScrollEater(QObject *parent) : QObject(parent)
{

}

ScrollEater::~ScrollEater()
{

}

bool ScrollEater::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Wheel ) {
        return true;
    } else {
        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}
