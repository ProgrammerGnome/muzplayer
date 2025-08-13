#include <QApplication>
#include "MusicPlayerWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MusicPlayerWindow musicPlayerWindow;
    musicPlayerWindow.show();
    return a.exec();
}
