#include <QtGui/QApplication>
#include "mainform.h"
#include "QStyle"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle("plastique");

    MainForm w;

    return a.exec();
}
