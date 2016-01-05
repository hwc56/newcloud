#include <QtGui/QApplication>
#include "mainview.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainview  m;
    m.setWindowIcon(QIcon("./images/logo.png"));
    m.setWindowTitle("suncloud client");
    //m.resize(400,800);
    m.showFullScreen();
    m.show();
    m.CheckNetworkState();

    return a.exec();
}
