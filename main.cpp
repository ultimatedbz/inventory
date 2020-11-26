#include "dialog.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Dialog w;
    w.setWindowTitle("Inventory 3.0");
    w.show();

    return a.exec();
}
