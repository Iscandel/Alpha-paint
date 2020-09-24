#include "alphapaint.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AlphaPaint w;
    w.show();
    return a.exec();
}
