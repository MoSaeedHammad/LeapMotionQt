#include "MainWindow.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication oApplication(argc, argv);
    MainWindow oMainWindow;
    oMainWindow.show();
    return oApplication.exec();
}
