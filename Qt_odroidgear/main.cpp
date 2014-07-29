#include "opiwatch.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    OpiWatch w;
    w.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    w.show();
    
    return a.exec();
}
