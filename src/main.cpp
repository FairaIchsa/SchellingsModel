#include "MainWindow.h"


int main(int argc, char *argv[])
{
    MainWindow *pWindow = new MainWindow;

    int res = Run(argc, argv, pWindow, 1300, 900);

    delete pWindow;

    return res;
}
