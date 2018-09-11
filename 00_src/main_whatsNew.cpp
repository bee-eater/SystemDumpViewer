#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QSettings>
#include <QTextStream>
#include <QFile>

void MainWindow::show_InfoBox(){
    QMessageBox whatsNew;
    QString content;

    const QString file("whatsnew.txt");
    QFile xmlString(file);
    bool ret = xmlString.open(QIODevice::ReadOnly | QIODevice::Text);
    if( ret )
    {
        QTextStream stream(&xmlString);
        content = stream.readAll();
    }

    whatsNew.setWindowTitle(tr("Whats new?"));
    whatsNew.setText(content);
    whatsNew.setIconPixmap(QPixmap("://images/BR.SystemdumpViewer.png").scaledToWidth(32));
    whatsNew.setStandardButtons(QMessageBox::Ok);
    whatsNew.setDefaultButton(QMessageBox::Ok);
    whatsNew.show();
    whatsNew.exec();

    this->settings->setValue("firststart",false);
    this->settings->sync();
}
