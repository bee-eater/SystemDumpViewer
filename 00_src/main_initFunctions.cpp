#include "mainwindow.h"
#include "ui_mainwindow.h"

// These are the meanings represented by the constant values in the SystemDump!
// If anything is changed by B&R it has to be changed here too!
void MainWindow::init_Names(){

    this->list_CpuModes << "-" << "BOOT" << "DIAG" << "SERV" << "RUN";
    this->list_BatteryStatus << "Empty!" << "OK";
    this->list_RebootMode << "-" << "warm start" << "cold start" << "-" << "service";
    this->list_RebootCF << "warm start" << "cold start";
    this->list_IpConfiguration << "static" << "DHCP";
    this->list_SNMPConfiguration << "disabled" << "read-only" << "enabled";
    this->list_ModuleOk << "TRUE" << "TRUE" << "FALSE" << "UNDEF" << "UNDEF" << "UNDEF";
    this->list_ChannelType << "DO" << "DI" << "AO" << "AI";

}

void MainWindow::init_Styles(){

    // Progress bar
    this->Styles.progressBarStyle = QString("");

    // ScrollArea style for transparent background
    this->Styles.scrollAreaSyle = QString("background-color:transparent;");

    // Scroll Area Axis Settings
    this->axisScrollArea = new QScrollArea(ui->tab_Motion);
    this->axisScrollArea->setGeometry(ui->widget_Axis->x(),ui->widget_Axis->y(),ui->widget_Axis->width(),ui->widget_Axis->height());
    this->axisScrollArea->setStyleSheet(this->Styles.scrollAreaSyle);
    this->axisScrollArea->setFrameShape(QFrame::NoFrame);
    this->axisScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Axis Stylesheets
    this->Styles.axisHeadTopStyle = QString("QLabel{"
                                                "color : #101010;"
                                                "background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1,"
                                                "stop: 0 #ffffff,"
                                                "stop: 1 #FFDBB2 );"
                                                "border-top: 1px solid black;"
                                                "border-left: 1px solid black;"
                                                "border-top-left-radius: 5px;"
                                                "border-right: 1px solid black;"
                                                "border-top-right-radius: 5px;"
                                            "}");

    this->Styles.axisHeadMidStyle = QString("QLabel{"
                                                "color : #101010;"
                                                "background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1,"
                                                "stop: 0 #FFDBB2,"
                                                "stop: 0.4999 #FFCF99,"
                                                "stop: 0.5 #FFB866,"
                                                "stop: 1 #FFAC4D );"
                                                "border-top: 0px solid white;"
                                                "border-left: 1px solid black;"
                                                "border-right: 1px solid black;"
                                            "}");

    this->Styles.axisHeadBottomStyle = QString("QLabel{"
                                                   "color : #101010;"
                                                   "background: QLinearGradient( x1: 0, y1: 0, x2: 0, y2: 1,"
                                                   "stop: 0 #FFAC4D,"
                                                   "stop: 1 #FF8800 );"
                                                   "border-top: 0px solid black;"
                                                   "border-left: 1px solid black;"
                                                   "border-bottom: 1px solid black;"
                                                   "border-bottom-left-radius: 5px;"
                                                   "border-right: 1px solid black;"
                                                   "border-bottom-right-radius: 5px;"
                                               "}");



    this->Styles.axisTopStyle = QString("QLabel{"
                                            "color : #101010;"
                                            "background: #FFDBB2;"
                                            "border-top: 1px solid black;"
                                            "border-bottom: 1px solid white;"
                                            "border-left: 1px solid black;"
                                            "border-right: 1px solid black;"
                                            "border-top-left-radius: 5px;"
                                            "border-top-right-radius: 5px;"
                                        "}");

    this->Styles.axisOrangeStyle = QString("QLabel{"
                                               "color : #101010;"
                                               "background: #FFDBB2;"
                                               "border-top: 1px solid white;"
                                               "border-bottom: 1px solid white;"
                                               "border-left: 1px solid black;"
                                               "border-right: 1px solid black;"
                                           "}");

    this->Styles.axisWhiteStyle = QString("QLabel{"
                                              "color : #101010;"
                                              "background: white;"
                                              "border-top: 1px solid #FFDBB2;"
                                              "border-left: 1px solid black;"
                                              "border-bottom: 1px solid #FFDBB2;"
                                              "border-right: 1px solid black;"
                                           "}");

    this->Styles.axisBottomStyle = QString("QLabel{"
                                               "color : #101010;"
                                               "background: white;"
                                               "border-top: 1px solid white;"
                                               "border-bottom: 1px solid black;"
                                               "border-left: 1px solid black;"
                                               "border-right: 1px solid black;"
                                               "border-bottom-left-radius: 5px;"
                                               "border-bottom-right-radius: 5px;"
                                           "}");

}

