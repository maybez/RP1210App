#include "RP1210Window.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RP1210Window w;
    w.show();
    return a.exec();
}
