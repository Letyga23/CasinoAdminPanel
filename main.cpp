#include "adminpanelwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AdminPanelWindow w;
    w.show();
    return a.exec();
}
