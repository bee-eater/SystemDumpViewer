#include "mainwindow.h"
#include <QApplication>
#include <QStatusBar>
#include <QtPlugin>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");

    /* neccessary because if you start the application with an argument
     * - open file with or dropping a file on the executable -
     * the working path will be wrong and the config and languages etc.
     * won't be found!
     */
    QDir::setCurrent(QApplication::applicationDirPath());

    MainWindow w;

    w.show();

    if(argc > 1){
        w.readXML(argv[1]);
    }

    return a.exec();
}
