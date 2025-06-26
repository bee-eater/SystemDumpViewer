#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "optionwindow.h"
#include "reportsettings.h"

#include <windowsx.h>
#include <cstdio>
#include <tlhelp32.h>
#include <commctrl.h>
#include <psapi.h>
#include <sstream>

#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QScreen>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QGraphicsEffect>
#include <QHttpPart>
#include <QHttpMultiPart>
#include <QInputDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QPrinter>
#include <QProcess>
#include <QSettings>
#include <QSize>
#include <QShortcut>
#include <QSplashScreen>
#include <QStandardPaths>
#include <QStyleFactory>
#include <QTextDocument>
#include <QGuiApplication>
#include <QScreen>
#include <QtNetwork>
#include <QSslSocket>

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

#include "qwt_plot.h"

#include <./includes/SimpleIni.h>
#include "stdio.h"
#include "stdlib.h"
#include "version.h"


DWORD MainWindow::HelpProcessId;
HWND MainWindow::hwndComboBox;
HWND MainWindow::hwndBrHelpExp;

const QString MainWindow::Version = tr(MY_APP_VERSION_STRING);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // Set fixed size for window
    QWidget::setMinimumSize(DEFAULT_WINDOW_WIDTH,DEFAULT_WINDOW_HEIGHT);

    // For update checking
    update_nwm = new QNetworkAccessManager();
    update_nw_request.setUrl(QUrl("https://api.github.com/repos/bee-eater/SystemDumpViewer/releases/latest"));
    QObject::connect(update_nwm, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_nwmCheckUpdate_finished(QNetworkReply*)));

    // Connect for SSL evaluation
    QObject::connect(&netAccManGET, &QNetworkAccessManager::sslErrors, this, &MainWindow::onIgnoreSslErrors);
    QObject::connect(&netAccManPOST, &QNetworkAccessManager::sslErrors, this, &MainWindow::onIgnoreSslErrors);
    QObject::connect(&netAccManDownload, &QNetworkAccessManager::sslErrors, this, &MainWindow::onIgnoreSslErrors);

    // Load the config.ini
    this->load_iniFile();

    // Show SplashScreen
    if(this->settings->value("showsplash",true).toBool())
        this->splashScreenShow(1500);

    // Initialize language (ScreenInit before!)
    this->updateLanguage();

    // Accept Drag'n Drop
    this->setAcceptDrops(true);

    ui->setupUi(this);

    // Start with start page
    this->startScreenInit();
    this->startScreenShow(true);

    // Execute custom stuff (Shortcuts etc...)
    this->setup_UI();

    // Show version info if first start
    if(this->settings->value("firststart",true).toBool()){
        this->show_InfoBox();
    }

    this->init_Names();
    this->init_Styles();

    // Initialize theme (<0 loads the saved themefile!)
    this->updateColorTheme(-1);

    // Check for updates :: Updateserver just for displaying purposes in options...
    this->settings->setValue("updateserver","https://api.github.com/repos/bee-eater/SystemDumpViewer/releases/latest");
    if(this->settings->value("autoCheckUpdates",true).toBool()){
        update_nwm->get(update_nw_request);
    }

    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    if(QDir(this->extractionPath).exists() && this->settings->value("keeptempfiles",false).toBool() == false){
        QDir(this->extractionPath).removeRecursively();
    }

    delete ui;
    delete update_nwm;
}

bool MainWindow::check_IsHelpOpen()
{
    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, static_cast<DWORD>(NULL));

    if (Process32First(snapshot, &entry))
        while (Process32Next(snapshot, &entry))
            if (!wcsicmp(entry.szExeFile, L"BR.Help.Explorer.exe")){
                exists = true;
                this->HelpProcessId = entry.th32ProcessID;
            }

    CloseHandle(snapshot);
    return exists;
}

void MainWindow::dataObjectsDisable(){
    std::vector<QAction*>().swap(this->vDataObjects);
    ui->menu_SaveDataObj->clear();
    ui->menu_SaveDataObj->setDisabled(true);
}

bool MainWindow::createPDFReport(int type, QString fileName, QString date){

    switch(type){

    case REPORT_TYPE_HARDWARE:
        /* TYP 1: HW + SERIALNUMBERS */
        if(fileName != QString("")){
                 QPrinter printer;
                 printer.setPageOrientation(QPageLayout::Portrait);
                 printer.setPageSize(QPageSize::A4);
                 printer.setOutputFileName(fileName);
                 printer.setOutputFormat(QPrinter::PdfFormat);

                 printer.setPageMargins(QMargins(16, 16, 16, 16), QPageLayout::Millimeter);

                 QFile inputCss("css/report.css");
                 inputCss.open(QIODevice::ReadOnly);
                 QTextStream in(&inputCss);
                 QString css = in.readAll();
                 inputCss.close();

                 QString htmlString = css + QString("<table width='100\%'>"
                                                    "<tr>"
                                                    "<td align='left' valign='bottom' style='font-size:14pt'>" + tr("Machine ID:") +" %1<br/>" + tr("Date:") + " %2</td>"
                                                    "<td align='right'><img width='200' src='://images/brlogo2.png'/></td>"
                                                    "</tr>"
                                                    "</table>"
                                                    "<p align='center' style='font-size:14pt;font-weight:bold'>%3</p>"
                                                    "<p/>"
                                                    "<table width='100\%' align='center' class='tg'>"
                                                    "<tr>"
                                                         "<th align='left' class='tg-031e'>"+tr("Hardware")+"</th>"
                                                         "<th align='center' class='tg-s6z2'>"+tr("Rev")+"</th>"
                                                         "<th align='center' class='tg-s6z2'>"+tr("Firmware")+"</th>"
                                                         "<th align='center' class='tg-s6z2'>"+tr("Serial number")+"</th>"
                                                    "</tr>"
                                                    ).arg(this->settings->value("reportmachid",tr("MachineID")).toString(),date,this->settings->value("reporttitle",tr("Hardware Report")).toString());

                 int ridx = 0;
                 for(unsigned int i = 0; i<this->SysDump.Sections.Hardware.vNode.size();i++){
                     if(this->SysDump.Sections.Hardware.vNode[i].IOInformation.Serialnumber != QString("-") && this->SysDump.Sections.Hardware.vNode[i].IOInformation.Serialnumber != QString("")){
                         if(ridx%2==0){
                             htmlString.append(QString("<tr>"
                                                       "  <td align'left' class='tg-z2zr'>%1</td>"
                                                       "  <td align='center' class='tg-lyaj'>%2</td>"
                                                       "  <td align='center' class='tg-lyaj'>%3</td>"
                                                       "  <td align='center' class='tg-lyaj'>%4</td>"
                                                       "</tr>      ").arg(this->SysDump.Sections.Hardware.vNode[i].ModuleStatus.Plugged,this->SysDump.Sections.Hardware.vNode[i].IOInformation.HardwareRevision,this->SysDump.Sections.Hardware.vNode[i].IOInformation.FwVersion,this->SysDump.Sections.Hardware.vNode[i].IOInformation.Serialnumber));
                         } else {
                             htmlString.append(QString("<tr>"
                                                       "  <td align='left' class='tg-031e'>%1</td>"
                                                       "  <td align='center' class='tg-s6z2'>%2</td>"
                                                       "  <td align='center' class='tg-s6z2'>%3</td>"
                                                       "  <td align='center' class='tg-s6z2'>%4</td>"
                                                       "</tr>      ").arg(this->SysDump.Sections.Hardware.vNode[i].ModuleStatus.Plugged,this->SysDump.Sections.Hardware.vNode[i].IOInformation.HardwareRevision,this->SysDump.Sections.Hardware.vNode[i].IOInformation.FwVersion,this->SysDump.Sections.Hardware.vNode[i].IOInformation.Serialnumber));
                         }
                         ridx++;
                     }
                 }

                 htmlString.append("</table>");

                 QTextDocument doc;
                 doc.setPageSize(QPageSize(QPageSize::A4).sizePoints());
                 doc.setHtml(htmlString);
                 doc.print(&printer);
                 printer.newPage();

         }

        break;

    case REPORT_TYPE_SOFTWARE:


        break;


    default:

        break;

    }

    return 0;

}

void MainWindow::dataObjectsUpdateList(){
    std::vector<QAction*>().swap(this->vDataObjects);
    ui->menu_SaveDataObj->clear();

    QDir dir;
    dir.setPath(QDir::cleanPath(this->extractionPath+"/"+this->ipAddress+"/Data Files/"));
    QDirIterator iterator(dir.absolutePath(), QDirIterator::Subdirectories);

    while(iterator.hasNext()){
        iterator.next();
        if(!iterator.fileInfo().isDir()){

            QAction *action = new QAction(ui->menu_SaveDataObj);
            this->vDataObjects.push_back(action);

            this->vDataObjects[this->vDataObjects.size()-1]->setText(iterator.fileName());
            this->vDataObjects[this->vDataObjects.size()-1]->setData(iterator.filePath());
            connect(this->vDataObjects[this->vDataObjects.size()-1], SIGNAL(triggered()), this, SLOT(on_download_dataObject()));
            ui->menu_SaveDataObj->addAction(this->vDataObjects[this->vDataObjects.size()-1]);
        }
        ui->menu_SaveDataObj->setDisabled(false);
    }

    if(this->vDataObjects.size()==0)
        ui->menu_SaveDataObj->setDisabled(true);

}

void MainWindow::dataSaveAllEnable(bool enable){
    ui->actionSave_all_as->setEnabled(enable);
}

void MainWindow::downloadedUpdateList(){
    std::vector<QAction*>().swap(this->vDownloadedFiles);
    ui->menu_openDownloaded->clear();

    QDir dir;
    dir.setPath(this->extractionPath);

    QDirIterator iterator(dir.absolutePath(), QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
       iterator.next();
       if (!iterator.fileInfo().isDir()) {
          QString filename = iterator.fileName();
          if (filename == "Systemdump.xml"){

              if(this->vDownloadedFiles.size() == 0){
                  QAction *delAction = new QAction(ui->menu_openDownloaded);
                  this->vDownloadedFiles.push_back(delAction);

                  // Add delete button and seperator
                  this->vDownloadedFiles[this->vDownloadedFiles.size()-1]->setText(tr("Delete downloaded files ..."));
                  this->vDownloadedFiles[this->vDownloadedFiles.size()-1]->setIcon(QPixmap("://images/ico_delete.png"));
                  connect(this->vDownloadedFiles[this->vDownloadedFiles.size()-1], SIGNAL(triggered()),this,SLOT(on_downloaded_removeFiles()));
                  ui->menu_openDownloaded->addAction(this->vDownloadedFiles[this->vDownloadedFiles.size()-1]);
                  ui->menu_openDownloaded->addSeparator();
                  ui->menu_openDownloaded->setDisabled(false);
              }

              // Extend vector
              QAction *action = new QAction(ui->menu_openDownloaded);
              this->vDownloadedFiles.push_back(action);

              QString text = QString("&%1 %2")
                      .arg(this->vDownloadedFiles.size()-1)
                      .arg(iterator.filePath().split("/").at(iterator.filePath().split("/").size()-2));

              this->vDownloadedFiles[this->vDownloadedFiles.size()-1]->setText(text);
              this->vDownloadedFiles[this->vDownloadedFiles.size()-1]->setData(iterator.filePath());
              connect(this->vDownloadedFiles[this->vDownloadedFiles.size()-1], SIGNAL(triggered()),this,SLOT(on_downloaded_openFile()));
              ui->menu_openDownloaded->addAction(this->vDownloadedFiles[this->vDownloadedFiles.size()-1]);

          }
       }
    }
    if(this->vDownloadedFiles.size()==0)
        ui->menu_openDownloaded->setDisabled(true);

}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QMimeData* mimeData = event->mimeData();

    // check for our needed mime type, here a file or a list of files
    if (mimeData->hasUrls())
    {
        QStringList pathList;
        QString extension;
        QList<QUrl> urlList = mimeData->urls();

        // extract the local paths of the files
        for (int i = 0; i < urlList.size() && i < 32; ++i)
        {
            pathList.append(urlList.at(i).toLocalFile());
        }

        // Check file extension
        if(pathList[0].length() > 5){

            QFileInfo fileInfo(pathList[0]);
            QString fullName = fileInfo.absoluteFilePath();

            if( fullName.endsWith(".xml"))
            {
                // call a function to open the files
                this->readXML(pathList[0].toUtf8());

                // Set UI
                ui->combo_SelectMenu->setCurrentIndex(0);
                ui->tabWidget_System->setCurrentIndex(0);
                ui->tabWidget_Motion->setCurrentIndex(0);

            }
            else if (fullName.endsWith(".tar.gz"))
            {
                readTarGz(pathList[0]);
                // Set UI
                ui->combo_SelectMenu->setCurrentIndex(0);
                ui->tabWidget_System->setCurrentIndex(0);
                ui->tabWidget_Motion->setCurrentIndex(0);
            }
            else {
                QMessageBox::warning(this,tr("Warning"),tr("Invalid file! Only .xml or .tar.gz files are allowed!"));
            }
        }

    }
}

bool MainWindow::extractTarGz(QString sourceFile, QString targetDir){

    if(!QFile("7za.exe").exists()){
        this->netError = tr("7za.exe not found! Extract and open file manually!");
        return 0;
    } else {
        QString program = "7za.exe";
        QStringList arguments;
        arguments << "x" << sourceFile << "-o"+targetDir << "-y";

        QProcess *myProcess = new QProcess(this);
        myProcess->setProcessChannelMode(QProcess::MergedChannels);
        myProcess->start(program, arguments);
        myProcess->waitForFinished();

        if(myProcess->exitStatus()==QProcess::NormalExit){
            return 1;
        } else {
            this->netError = myProcess->errorString();
            return 0;
        }
    }

}

bool MainWindow::readTarGz(QString sourceFile)
{
    QFileInfo fileInfo(sourceFile);
    QString tempPath = QDir::temp().absolutePath() + "/" + "sdvtemp";
    QDir(tempPath).removeRecursively(); // cleanup temp- dir
    QDir::temp().mkdir("sdvtemp");  // create subdir

    bool result = extractTarGz(sourceFile, tempPath );  // extract .tar.gz to .tar
    QString fullName = fileInfo.fileName();
    QString tarName = fullName.mid(0,fullName.length()-3);

    if(result){
        result = this->extractTarGz( tempPath+"/"+ tarName, tempPath ); // extract .tar
        if(result){
            this->readXML(QString( tempPath +"/Systemdump.xml").toStdString().c_str(), false,sourceFile);
            recentSetCurrentFile(sourceFile);
        }
        QDir(tempPath).removeRecursively(); // cleanup temp- dir
    }
    return result;
}



bool MainWindow::eventFilter(QObject *obj, QEvent *event){
    if (event->type() == QEvent::Enter){
        if(static_cast<QLabel *>(obj) == this->start_openxml){
            this->start_openxml->setText(QString("<html><img src='://images/start_openxml_hov.png'/></html>"));
            this->start_textopenxml->setVisible(true);

            return true;
        } else if(static_cast<QLabel *>(obj) == this->start_openplc){
            this->start_openplc->setText(QString("<html><img src='://images/start_openplc_hov.png' /></html>"));
            this->start_textopenplc->setVisible(true);
            return true;
        } else {
            return true;
        }

    } else if (event->type() == QEvent::Leave){
        if(static_cast<QLabel *>(obj) == this->start_openxml){
            this->start_openxml->setText(QString("<html><img src='://images/start_openxml.png' /></html>"));
            this->start_textopenxml->setVisible(false);
            return true;
        } else if(static_cast<QLabel *>(obj) == this->start_openplc){
            this->start_openplc->setText(QString("<html><img src='://images/start_openplc.png' /></html>"));
            this->start_textopenplc->setVisible(false);
            return true;
        } else {
            return true;
        }

    } else if (event->type() == QEvent::MouseButtonRelease){
        if(static_cast<QLabel *>(obj) == this->start_openxml){
            this->on_actionLoad_xml_triggered();
            this->start_openxml->setText(QString("<html><img src='://images/start_openxml.png' /></html>"));
            this->start_textopenxml->setVisible(false);
            return QObject::eventFilter(obj, event);

        } else if(static_cast<QLabel *>(obj) == this->start_openplc){
            this->on_actionLoad_from_PLC_triggered();
            this->start_openplc->setText(QString("<html><img src='://images/start_openplc.png' /></html>"));
            this->start_textopenplc->setVisible(false);
            return QObject::eventFilter(obj, event);

        } else {
            return QObject::eventFilter(obj, event);
        }

    }else if(event->type() == QEvent::Resize){
        if((static_cast<QMainWindow *>(obj)) == this){
            this->startScreenCenter();
            return QObject::eventFilter(obj, event);
        }
    }
    return false;
}

void MainWindow::loadingGifEnabled(bool enabled){
    if(enabled){
        lblLoadingGif->setVisible(true);
        lblLoadingGif->setMovie(movLoadingGif);
        movLoadingGif->start();
    } else {
        lblLoadingGif->setVisible(false);
        movLoadingGif->stop();
    }
    return;
}

void MainWindow::load_iniFile(){

    // Get system AppData/Local/<AppName> path
    QString SDVBasePath =  QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);

    // Check if AppDir exists, if not: copy settings file
    if(!QDir(SDVBasePath).exists()){
        QDir().mkdir(SDVBasePath);
        QFile::copy("system.cfg", SDVBasePath + "/system.cfg");
    }

    // Initialize settings (from file)
    this->settings = new QSettings(SDVBasePath + "/system.cfg",QSettings::IniFormat);

    // Internal Settings
    this->recentIpAddresses = settings->value("recentIpList").toStringList();
    this->themePath = settings->value("themePath","./themes/").toString();

    // Use AppData path because of write restrictions to program folder
    this->extractionPath = SDVBasePath + "/downloads/";

    // Es wurde ein Update durchgeführt
    if(this->settings->value("updated",false).toBool()){
        if(QFile(this->settings->value("updatefile").toString()).exists())
            QFile(this->settings->value("updatefile").toString()).remove();

        this->settings->remove("updated");
        this->settings->remove("updatefile");

        QMessageBox::information(this,tr("Information"),tr("Update successfully installed! Thanks for using!"));
    }

}

bool MainWindow::netHttpGET(QString GET){
    this->netRequestGET.setUrl(QUrl(GET));
    QNetworkReply *netReply = this->netAccManGET.get(netRequestGET);

    this->loadingGifEnabled(true);
    this->netTimeout.start(NET_TIMEOUT);
    netEventLoopGET.exec();
    this->loadingGifEnabled(false);

    if(this->netTimeout.isActive()){
        this->netTimeout.stop();
        if(netReply->error() == QNetworkReply::NoError){
            this->netReplyGET = netReply->readAll();
            return 1;
        } else {
            this->netError = netReply->errorString();
            return 0;
        }
    } else {
        netReply->abort();
        this->netError = tr("Timeout! No network connection or server not reachable!");
        return 0;
    }
}

bool MainWindow::netHttpPOST(QString ipPOST, QHttpMultiPart *httpPart){
    this->netRequestPOST.setUrl(QUrl(ipPOST));
    QNetworkReply *netReply = this->netAccManPOST.post(netRequestPOST,httpPart);

    this->loadingGifEnabled(true);
    this->netTimeout.start(NET_TIMEOUT);
    netEventLoopPOST.exec();
    this->loadingGifEnabled(false);

    if(this->netTimeout.isActive()){
        this->netTimeout.stop();
        if(netReply->error() == QNetworkReply::NoError){
            this->netReplyPOST = netReply->readAll();
            return 1;
        } else {
            this->netError = netReply->errorString();
            return 0;
        }
    } else {
        netReply->abort();
        this->netError = tr("Timeout! No network connection or server not reachable!");
        return 0;
    }
}

bool MainWindow::netHttpDownload(QString url, QString filePath){

    this->netRequestDownload.setUrl(QUrl(url));
    QNetworkReply *netReply = this->netAccManDownload.get(this->netRequestDownload);
    netReply->connect(netReply, &QNetworkReply::downloadProgress, [&](qint64 received, qint64 total){
        this->progressBar.setMaximum(static_cast<int>(total));
        this->progressBar.setValue(static_cast<int>(received));
        this->netTimeout.start(NET_TIMEOUT);
    });
    this->setStatusBarEnabled(true,tr("Downloading ..."));

    this->netTimeout.start(NET_TIMEOUT);
    netEventLoopDownload.exec();

    if(this->netTimeout.isActive()){
        this->netTimeout.stop();
        if(netReply->error() == QNetworkReply::NoError){

            QFile file(filePath);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::critical(this,tr("Error"),tr("Could not open %1 for writing: %2\n").arg(filePath,file.errorString()));
                return 0;
            }

            file.write(netReply->readAll());
            file.close();

            this->setStatusBarEnabled(false);
            return 1;
        } else {
            this->netError = netReply->errorString();
            this->setStatusBarEnabled(false);
            return 0;
        }
    } else {
        netReply->abort();
        this->setStatusBarEnabled(false);
        this->netError = tr("Timeout! No network connection or server not reachable!");
        return 0;
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox about(this);

    about.setWindowTitle(tr("About ..."));
    about.setTextFormat(Qt::RichText);
    about.setText(QString("<br/><br/>")+tr("This application is not an official B&R product! For this reason the use is at your own risk and there is NO claim on support or maintenance.")+QString("<br/><br/>Version: %1 <br/><br/>App: %2").arg(this->Version,qApp->applicationFilePath()));
    about.setInformativeText(tr("Developed with Qt 6.4.2")+QString("<br/><br/>")+tr("This program uses parts of the following Projects: ") + QString("<br/>- <a href='http://qwt.sf.net'>Qwt Project</a> (CPU-Usage graph)<br/>- <a href='http://rapidxml.sourceforge.net'>RapidXml</a><br/>- <a href='https://github.com/brofield/simpleini'>SimpleIni</a><br/><br/>Copyright %1 <a href='mailto:marcel_krausert@freenet.de'>Marcel Krausert</a> 2023").arg(QString::fromUtf8("\u00A9"))+QString("<br/><br/>")+tr("This work is licensed under the")+QString("<br/>&emsp;&emsp;&emsp;<a href='https://www.gnu.org/licenses/gpl-3.0.de.html'>GNU GENERAL PUBLIC LICENSE, Version 3</a><br/><br/>")+tr("Visit us on")+QString("<a href='https://github.com/bee-eater/SystemDumpViewer'> GitHub</a>!"));
    about.setStandardButtons(QMessageBox::Ok);
    about.setIconPixmap(QPixmap("://images/about.png").scaledToWidth(128));
    about.setDefaultButton(QMessageBox::Ok);
    about.show();
    about.exec();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionHelp_triggered()
{
    bool result;
    QString exePath = QString("file:///")+QDir::currentPath();
    QString msgText;

    result = QDesktopServices::openUrl(QUrl(exePath+"/help/BR.SystemdumpViewer.chm"));
    if(result==false){
        msgText = tr("No Help found at:\n\n%1/help/BR.SystemdumpViewer.chm!!").arg(exePath);
        QMessageBox::warning(this,tr("Warning!"),msgText);
    }

}

void MainWindow::onIgnoreSslErrors(QNetworkReply* reply, const QList<QSslError>& errors)
{
    Q_UNUSED(errors);
    if(this->settings->value("ignoreSSLerrors",true).toBool())
        reply->ignoreSslErrors();
}

void MainWindow::on_actionLoad_from_PLC_triggered()
{
    bool ok;
    this->ipAddress = QInputDialog::getItem(this ,tr("Target PLC ..."),
                                            tr("URL (e.g. http://127.0.0.1):"), this->recentIpAddresses,0,true,&ok);

    if(ok){

        QStringList url_lst = this->ipAddress.split("://");
        QStringList adr_lst;
        if(url_lst.size() > 1){
            adr_lst = url_lst[1].split(':');
        } else {
            adr_lst = this->ipAddress.split(':');
        }

        QHostAddress address(adr_lst[0]);
        QString host = adr_lst[0];

        bool port_ok = true;
        int port = 80;
        if (adr_lst.count() == 2){
            port = adr_lst[1].toInt(&port_ok, 10);
        }

        if (QAbstractSocket::IPv4Protocol != address.protocol() || adr_lst.count() > 2 || !port_ok || port < 0 || port > 65535){
            QMessageBox::warning(this,tr("Error!"),tr("No valid IP-address!"));
            return;
        }


        // if empty -> cancelled
        if(this->ipAddress != ""){

            if(!this->ipAddress.startsWith("http")){
                this->ipAddress = "http://" + this->ipAddress;
            }

            // Add to list of recent ip addresses
            this->recentSetCurrentIpAddress(this->ipAddress);

            bool result = this->netHttpGET(QString(this->ipAddress + "/sdm/svg.cgi?type=systemdump&action=delete&datafile=0&size=0"));
            if(result){
                QString downloadData;
                if(this->settings->value("downloaddatafiles",false).toBool()){
                    downloadData = QString("1");
                } else {
                    downloadData = QString("0");
                }
                result = this->netHttpGET(QString(this->ipAddress + "/sdm/svg.cgi?type=systemdump&action=start&datafile=%1&size=0").arg(downloadData));
                if(result){
                    // Nachfragen und warten bis fertig
                    int liveSign = 0;
                    int liveLimit = 100;
                    this->setStatusBarEnabled(true, tr("Waiting for PLC to create Systemdump ..."));

                    while(liveSign < liveLimit){
                        QHttpPart textHttpPOSTBody;
                        QHttpMultiPart httpPart;
                        textHttpPOSTBody.setBody(QString("<sdmWgMessage><GuiManager><guiManager><LifeSign>" + QString::number(liveSign) + "</LifeSign></guiManager></GuiManager><Edit><SDM_sysDump_progress></SDM_sysDump_progress><SDM_sysDump_existing></SDM_sysDump_existing></Edit><DynObjMgr><SDMDynObjMgr></SDMDynObjMgr></DynObjMgr></sdmWgMessage>").toLatin1());
                        httpPart.append(textHttpPOSTBody);

                        result = this->netHttpPOST(QString(this->ipAddress + "/sdm/cgi-bin/sdmWG.cgi"), &httpPart);

                        if(result){
                            liveSign = liveSign+2;

                            QString sysDumpProgress = this->netReplyPOST.mid(static_cast<int>(this->netReplyPOST.toStdString().find("<SDM_sysDump_progress><Text>")+QString("<SDM_sysDump_progress><Text>").toStdString().length()),static_cast<int>((this->netReplyPOST.toStdString().find("</Text></SDM_sysDump_progress>"))-(this->netReplyPOST.toStdString().find("<SDM_sysDump_progress><Text>")+QString("<SDM_sysDump_progress><Text>").toStdString().length())));
                            QString sysDumpExisting = this->netReplyPOST.mid(static_cast<int>(this->netReplyPOST.toStdString().find("<SDM_sysDump_existing><Text>")+QString("<SDM_sysDump_existing><Text>").toStdString().length()),static_cast<int>((this->netReplyPOST.toStdString().find("</Text></SDM_sysDump_existing>"))-(this->netReplyPOST.toStdString().find("<SDM_sysDump_existing><Text>")+QString("<SDM_sysDump_existing><Text>").toStdString().length())));
                            this->emitProgress(sysDumpProgress.toInt());

                            //qDebug() << sysDumpProgress << " - " << sysDumpExisting;
                            //qDebug() << this->netReplyPOST;

                            /* Because of the while loop and the QEventLoop don't really work together, here is a "EmergencyExit"
                             * -> If the program works in normal mode, the string contains !always! something
                             * -> Increase the limit because comm is ok!
                             * -> If the program gets closed, while it's in this loop, it would freeze in this loop
                             * -> But because of the string is empty -> liveSign reaches liveLimit very fast end the process quits!
                             */
                            if(this->netReplyPOST != "")
                                liveLimit = liveLimit+2;

                            if(sysDumpProgress.toInt()==100 && static_cast<bool>(sysDumpExisting.toInt()==true)){
                                this->setStatusBarEnabled(false);

                                // When temporary file is selected in the options, check if dir exists, otherwise create
                                if(!QDir(this->extractionPath).exists())
                                    QDir().mkdir(this->extractionPath);

                                if(QDir(this->extractionPath+"/"+host).exists())
                                    QDir(this->extractionPath+"/"+host).removeRecursively();

                                QDir().mkdir(this->extractionPath+"/"+host);

                                result = this->netHttpDownload(QString(this->ipAddress+"/sdm/cgiFileLoop.cgi?type=256"),QString(this->extractionPath+"/"+host+"/Systemdump.tar.gz"));
                                if(result){
                                    result = this->extractTarGz(QString(this->extractionPath+"/"+host+"/Systemdump.tar.gz"),this->extractionPath+"/"+host);
                                    QDir().remove(this->extractionPath+"/"+this->ipAddress+"/Systemdump.tar.gz");
                                    if(result){
                                        result = this->extractTarGz(QString(this->extractionPath+"/"+host+"/Systemdump.tar"),this->extractionPath+"/"+host);
                                        QDir().remove(this->extractionPath+"/"+host+"/Systemdump.tar");
                                        if(result){

                                            QString rememberedIP = this->ipAddress;
                                            this->readXML(QString(this->extractionPath+"/"+host+"/Systemdump.xml").toStdString().c_str());
                                            this->ipAddress = rememberedIP;

                                            this->downloadedUpdateList();
                                            this->dataObjectsUpdateList();
                                            this->dataSaveAllEnable(true);

                                            // Set UI
                                            ui->combo_SelectMenu->setCurrentIndex(0);
                                            ui->tabWidget_System->setCurrentIndex(0);
                                            ui->tabWidget_Motion->setCurrentIndex(0);
                                        } else {
                                            QMessageBox::warning(this,tr("Error!"),this->netError);
                                        }
                                    } else {
                                        QMessageBox::warning(this,tr("Error!"),this->netError);
                                    }
                                } else {
                                    QMessageBox::warning(this,tr("Error!"),this->netError);
                                }
                                break;
                            }
                        } else {
                            QMessageBox::warning(this,tr("Error!"),this->netError);
                            break;
                        }
                    }
                } else {
                    QMessageBox::warning(this,tr("Error!"),this->netError);
                }
            } else {
                QMessageBox::warning(this,tr("Error!"),this->netError);
            }

        } // IP address = ""

    } // Cancel or aborted

}

void MainWindow::on_actionLoad_xml_triggered()
{
    QStringList PathList = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Systemdump.xml ..."),
                                                    PathList.at(0), tr("Files (*.xml *tar.gz)"));
    if(QFile(fileName).exists() && fileName != QString("")){

        QFileInfo fileInfo(fileName);
        QString fullName = fileInfo.absoluteFilePath();

        // Check ending manually since Qt can't know at which point the extension starts
        if( fullName.endsWith(".xml"))
            this->readXML(fullName.toStdString().c_str());
        else if (fullName.endsWith(".tar.gz"))
            readTarGz(fullName);
        else
            QMessageBox::warning(this,tr("Warning"),tr("Invalid file! Only .xml or .tar.gz files are allowed!"));


        // Set UI
        ui->combo_SelectMenu->setCurrentIndex(0);
        ui->tabWidget_System->setCurrentIndex(0);
        ui->tabWidget_Motion->setCurrentIndex(0);
    }
}

void MainWindow::on_actionSave_xml_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save .xml as ..."),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/Systemdump.xml","*.xml");

    if(filename != QString("") && QFile(this->currentFileString).exists()){
        if(QFile::copy(this->currentFileString,filename)){
            this->statusbar->showMessage(tr("%1 successfully saved!").arg(filename),DEFAULT_MESSAGE_TIME);
        } else {
            QMessageBox::critical(this,tr("Error!"),tr("Error saving the %1! Check paths!").arg(filename));
        }
    }

}

void MainWindow::on_actionOptions_triggered()
{
    OptionWindow *options = new OptionWindow(this,this);
    options->show();
}

void MainWindow::on_actionSave_all_as_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this,tr("Save all as ..."),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+QString("/BR_SDM_%1.zip").arg(this->ipAddress),"*.zip");

    if(filename != QString("")){
        if(!QFile("7za.exe").exists()){
            QMessageBox::critical(this,tr("Error!"),tr("7za.exe not found! Extract and open file manually!"));
            return;
        } else {

            if(QFile(filename).exists())
                QFile(filename).remove();

            QString program = "7za.exe";
            QStringList arguments;
            //qDebug() << "a" << filename << this->currentFileString.mid(0,this->currentFileString.lastIndexOf("/")+1);
            arguments << "a" << filename << this->currentFileString.mid(0,this->currentFileString.lastIndexOf("/")+1);

            QProcess *myProcess = new QProcess(this);
            myProcess->setProcessChannelMode(QProcess::MergedChannels);
            myProcess->start(program, arguments);
            myProcess->waitForFinished();

            if(myProcess->exitStatus()==QProcess::NormalExit){
                this->statusbar->showMessage(tr("%1 successfully saved!").arg(filename),DEFAULT_MESSAGE_TIME);
                return;
            } else {
                QMessageBox::critical(this,tr("Error!"),myProcess->errorString());
                return;
            }
        }
    }
}

void MainWindow::on_actionSave_position_triggered()
{

    this->settings->setValue("xpos", this->x());
    this->settings->setValue("ypos", this->y());
    this->settings->setValue("width",this->width());
    this->settings->setValue("height",this->height());
    this->settings->sync();

    this->statusbar->showMessage(tr("Position saved: x=%1, y=%2, width=%3, height=%4").arg(QString::number(this->x()),QString::number(this->y()),QString::number(this->width()),QString::number(this->height())),DEFAULT_MESSAGE_TIME);

}

void MainWindow::on_actionCheck_for_updates_triggered()
{
    // Requires:
    // libcrypto-1_1-x64.dll
    // libssl-1_1-x64.dll
    // tls/qcertonlybackend.dll
    // tls/qopensslbackend.dll
    // tls/qschannelbackend.dll
    if(QSslSocket::supportsSsl()){
        this->updateClicked = true;
        update_nwm->get(update_nw_request);
        // --> on_nwmCheckUpdate_finished(...)
    } else {
        QMessageBox::critical(this,tr("Error!"),QString("Error checking for updates! (No SSL support)"));
    }
}


void MainWindow::ParseVersion(int result[4], const std::string& input)
{
    std::istringstream parser(input);
    parser >> result[0];
    for(int idx = 1; idx < 4; idx++)
    {
        parser.get(); //Skip period
        parser >> result[idx];
    }
}

bool MainWindow::LessThanVersion(const std::string& a,const std::string& b)
{
    int parsedA[4], parsedB[4];
    ParseVersion(parsedA, a);
    ParseVersion(parsedB, b);
    return std::lexicographical_compare(parsedA, parsedA + 4, parsedB, parsedB + 4);
}

void MainWindow::on_nwmCheckUpdate_finished(QNetworkReply *reply){

    try {
        if (reply->error()) {
            this->statusbar->showMessage(tr("Error checking for updates: ") + reply->errorString());
            return;
        }
        QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
        QJsonObject rootObj = document.object();

        QString version = rootObj["name"].toString();
        QString msg = rootObj["body"].toString();
        if(msg == ""){
            msg = tr("Found update!");
        } else {
            msg = "Changes: \n\n" + msg;
        }

        version.remove(0,1);
        qDebug() << this->Version << " :: " << version;

        QString ignore = this->settings->value("updateIgnoreVersion").toString();

        if(LessThanVersion(this->Version.toStdString(),version.toStdString())){
            if(LessThanVersion(ignore.toStdString(),version.toStdString()) || this->updateClicked){
                QMessageBox msgBox;
                QAbstractButton* pButtonGetIt = msgBox.addButton(tr("Get it!"), QMessageBox::YesRole);
                QAbstractButton* pButtonIgnore = msgBox.addButton(tr("Ignore version"), QMessageBox::ActionRole);
                msgBox.addButton(tr("Close"), QMessageBox::NoRole);
                msgBox.setText(msg);
                msgBox.exec();
                if (msgBox.clickedButton()==pButtonGetIt){
                    QDesktopServices::openUrl(QUrl("https://github.com/bee-eater/SystemDumpViewer/releases"));
                } else if(msgBox.clickedButton()==pButtonIgnore){
                    this->settings->setValue("updateIgnoreVersion", version);
                }
            } else {
                this->statusbar->showMessage(tr("You chose to ignore this version: ") + ignore);
            }
        } else {
            this->statusbar->showMessage(tr("You already have the newest version: ") + this->Version);
        }

        this->updateClicked = false;

    } catch (const std::exception& e) {
        qWarning() << "Caught std::exception:" << e.what();
    }catch (...){
        qWarning() << "Caught unknown exception";
    }
}

void MainWindow::on_actionClose_xml_triggered()
{
    if(this->currentFileString=="")
        return;

    setWindowTitle(tr("Systemdump Viewer"));

    this->statusbar->showMessage(tr("Closing %1 ...").arg(this->currentFileString),DEFAULT_MESSAGE_TIME);

    // Clear filepath
    this->ipAddress.clear();
    this->currentFileString.clear();

    // Disable menu entries
    ui->actionClose_xml->setDisabled(true);
    ui->actionSave_xml_as->setDisabled(true);
    ui->actionSave_PDF_Report->setDisabled(true);
    ui->actionSave_Datapoints->setDisabled(true);

    // Clear all TreeViews!
    ui->tree_Hardware->setColumnCount(1);
    QList<QTreeWidget *> uiTrees = ui->centralWidget->findChildren<QTreeWidget *>();
    foreach(QTreeWidget * uiTree, uiTrees){
        uiTree->clearSelection();
        uiTree->clear();
    }

    // Clears all QLabels if the name starts with "label_"
    // The descriptions are all beginning with "text_" the dynamic
    // Labels that are written are beginning with "label_" !!
    QList<QLabel *> uiLabels = ui->centralWidget->findChildren<QLabel *>();
    foreach(QLabel* uiLabel, uiLabels){
        if(uiLabel->objectName().toStdString().find("label_") == 0)
            uiLabel->clear();
    }

    // Clear all tables!
    QList<QTableWidget *> uiTables = ui->centralWidget->findChildren<QTableWidget *>();
    foreach(QTableWidget * uiTable, uiTables){
        uiTable->setRowCount(0);
    }

    // Clear EthIF
    for(int i=ui->tabWidget_EthernetInterfaces->count();i>=1;i--){
        ui->tabWidget_EthernetInterfaces->removeTab(i);
    }

    // Clear start screen
    qDeleteAll(ui->widget_overview_Center->findChildren<QWidget*>());
    qDeleteAll(ui->widget_overview_AppStatus->findChildren<QWidget*>());
    qDeleteAll(ui->widget_overview_CPUMode->findChildren<QWidget*>());
    qDeleteAll(ui->widget_overview_Hardware->findChildren<QWidget*>());
    qDeleteAll(ui->widget_overview_Motion->findChildren<QWidget*>());
    qDeleteAll(ui->widget_overview_Target->findChildren<QWidget*>());

    qDeleteAll(ui->grp_LoggerModules->findChildren<QCheckBox*>()); // Logger checkboxes
    qDeleteAll(ui->tab_CPUUsage->findChildren<QwtPlot*>());                 // CPU usage
    qDeleteAll(ui->widget_Partitions->findChildren<QWidget*>());            // Memory

    // Axis
    qDeleteAll(ui->widget_Axis->findChildren<QWidget*>());
    ui->text_Axis_Name->setGeometry(ui->text_Axis_Name->x(),ui->text_Axis_Name->y(),(AXIS_LABEL_WIDTH),ui->text_Axis_Name->height());
    ui->text_Axis_Drive->setGeometry(ui->text_Axis_Drive->x(),ui->text_Axis_Drive->y(),(AXIS_LABEL_WIDTH),ui->text_Axis_Drive->height());
    ui->text_Axis_Address->setGeometry(ui->text_Axis_Address->x(),ui->text_Axis_Address->y(),(AXIS_LABEL_WIDTH),ui->text_Axis_Address->height());
    ui->widget_Axis->adjustSize();
    this->axisScrollArea->hide();

    ui->combo_CpuUsage->clear();    // Dropdowns
    ui->combo_AxisError->clear();   // Dropdowns

    this->reset_UI();
    this->reset_DumpInfo();

    // Downloaded Data
    this->dataObjectsDisable();
    this->dataSaveAllEnable(false);
    this->startScreenShow(true);

}

void MainWindow::on_combo_SelectMenu_currentIndexChanged(int index)
{
    ui->stacked_MainFrame->setCurrentIndex(index);
}

void MainWindow::on_combo_softwareFilter_currentIndexChanged(int index)
{
    if(this->SysDump.Sections.Software.vAppModule.size() != 0 && ui->tree_ApplicationModules->currentItem()){
        QVariant UUID = ui->tree_ApplicationModules->currentItem()->data(0,Qt::UserRole);
        this->get_SoftwareModules(UUID.toInt(),index);
    }
}

void MainWindow::on_combo_CpuUsage_currentIndexChanged()
{
    this->draw_CpuUsage();
}

void MainWindow::on_combo_AxisError_currentIndexChanged()
{
    this->add_AxisErrorsToUi();
}

void MainWindow::on_createPDFReport_triggered(){
    ReportWindow *winReport = new ReportWindow(this,this);
    winReport->show();
}

void MainWindow::on_createDatapointExport_triggered(){

    QString DateStr = QString::number(QDate::currentDate().year()) + QString::number(QDate::currentDate().month())+ QString::number(QDate::currentDate().day());
    QString filename = QFileDialog::getSaveFileName(this, "Datapoint export", "DatapointExport_" + DateStr + ".csv", "CSV files (.csv)", nullptr);
    QFile data(filename);

    if(data.open(QFile::WriteOnly | QFile::Truncate))
    {     
        QTextStream output(&data);
        // Add header line with column description
        output << "ModulePath;EquipmentID;ModulePlugged;ModuleOk;ChannelName;ChannelIECType;ChannelValue\r\n";
        // Add datapoints for all modules
        for(std::vector<sNode>::iterator md = this->SysDump.Sections.Hardware.vNode.begin(); md != this->SysDump.Sections.Hardware.vNode.end(); ++md) {
            for(std::vector<sChannel>::iterator ch = md->IOChannels.vChannel.begin(); ch != md->IOChannels.vChannel.end(); ++ch) {
                output << md->IOInformation.ModulePath + ";" + md->IOInformation.EquipmentID + ";" + md->ModuleStatus.Plugged + ";" + ((md->ModuleStatus.ModuleOk < 2) ? QString::number(md->ModuleStatus.ModuleOk) : "") + ";" + ch->Name + ";" + ch->IECType + ";" + ch->PhysicalValue + "\r\n";
            }
        }

    }

}

void MainWindow::on_download_dataObject(){
    QAction *action = qobject_cast<QAction *>(sender());
    if(action){
        QString ext = action->text().split(".").at(action->text().split(".").size()-1);
        QString filename = QFileDialog::getSaveFileName(this,tr("Save .xml as ..."),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/"+action->text(),"*."+ext);

        if(filename != QString("") && QFile(action->data().toString()).exists()){
            if(QFile::copy(action->data().toString(),filename)){
                this->statusbar->showMessage(tr("%1 successfully saved!").arg(filename),DEFAULT_MESSAGE_TIME);
            } else {
                QMessageBox::critical(this,tr("Error!"),tr("Error saving the %1! Check paths!").arg(filename));
            }
        }
    }
}

void MainWindow::on_downloaded_openFile(){
    QAction *action = qobject_cast<QAction *>(sender());
    if(action){
        this->ipAddress = action->data().toString().split("/").at(action->data().toString().split("/").size()-2);
        bool result = this->readXML(action->data().toString().toStdString().c_str());

        ui->combo_SelectMenu->setCurrentIndex(0);
        ui->tabWidget_System->setCurrentIndex(0);
        ui->tabWidget_Motion->setCurrentIndex(0);
        if(!result)
            this->dataSaveAllEnable(true);
    }
    this->dataObjectsUpdateList();
}

void MainWindow::on_downloaded_removeFiles(){
    if(QDir(this->extractionPath).exists())
        QDir(this->extractionPath).removeRecursively();

    if(!((this->currentFileString.indexOf(QApplication::applicationDirPath()) == -1) && (this->currentFileString.indexOf(QDir::cleanPath(this->extractionPath+"/"+this->ipAddress+"/Systemdump.xml")) == -1)))
        this->on_actionClose_xml_triggered();

    this->downloadedUpdateList();
    this->dataObjectsUpdateList();
    this->dataSaveAllEnable(false);
}

void MainWindow::on_logger_checkbox_toggled(){
    this->add_LoggerModules();
}

void MainWindow::on_logger_section_clicked(int cidx){

    if(this->loggerClickedColumnOld == cidx){
        this->loggerSortOrder = !this->loggerSortOrder;
    } else {
        this->loggerSortOrder = 0;
    }
    this->loggerClickedColumnOld = cidx;

    if(cidx == LOGGER_TABLE_TIMEDATE){
        if(this->loggerSortOrder){
            ui->table_Logger->sortByColumn(LOGGER_TABLE_TIMEDATE,Qt::AscendingOrder);
        } else {
            ui->table_Logger->sortByColumn(LOGGER_TABLE_TIMEDATE,Qt::DescendingOrder);
        }
    } else if(cidx == LOGGER_TABLE_LEVELBMP) {
        if(this->loggerSortOrder){
            ui->table_Logger->sortByColumn(LOGGER_TABLE_LEVEL,Qt::DescendingOrder);
        } else {
            ui->table_Logger->sortByColumn(LOGGER_TABLE_LEVEL,Qt::AscendingOrder);
        }
    } else {
        if(this->loggerSortOrder){
            ui->table_Logger->sortByColumn(cidx,Qt::DescendingOrder);
        } else {
            ui->table_Logger->sortByColumn(cidx,Qt::AscendingOrder);
        }
    }

}

void MainWindow::on_logger_pressed_f1(){
    QModelIndexList selectedList = ui->table_Logger->selectionModel()->selectedRows();

    if(!selectedList.isEmpty()){
        int index = selectedList.at(0).data(Qt::UserRole).toInt();
        if(this->SysDump.Sections.Logger.vModule[0].Version==QString("1.00.0")){
            this->openInBuRHelp(QString::number(this->vLogSortView[static_cast<unsigned int>(index)].ErrorNr));
        } else {
            this->openInBuRHelp(this->vLogSortViewV2[static_cast<unsigned int>(index)].EventID);
        }
    }
}

void MainWindow::on_netAccManDownload_finished(QNetworkReply* netReply){

    if(netReply->error() == QNetworkReply::NoError){
        this->netReplyDownload = netReply->readAll();
        this->netAccManDownloadFinished = true;
    } else {
        this->netReplyDownload.clear();
        this->netAccManDownloadFinished = true;
    }

}

void MainWindow::on_recentFiles_clearList()
{
    this->settings->remove("recentFileList");
    this->recentUpdateFileActions();
    ui->menu_openRecent->setDisabled(true);
}

void MainWindow::on_recen_openFile()
{
    QAction *action = qobject_cast<QAction *>(sender());
    if(action){
        bool result = false;
        QString fileName = action->data().toString();
        QFileInfo fileInfo(fileName);
        QString fileExtension = fileInfo.completeSuffix();

        if( fileExtension == "xml")
            result = this->readXML(fileName.toStdString().c_str());
        else
            result = readTarGz(fileName);


        //bool result = this->readXML(fileName.toStdString().c_str());

        if(result){
            QStringList recentFilesList = this->settings->value("recentFileList").toStringList();
            recentFilesList.removeAll(action->data().toString());
            this->settings->setValue("recentFileList",recentFilesList);
            this->settings->sync();
            this->recentUpdateFileActions();
        }

        ui->combo_SelectMenu->setCurrentIndex(0);
        ui->tabWidget_System->setCurrentIndex(0);
        ui->tabWidget_Motion->setCurrentIndex(0);
    }
}

void MainWindow::on_tree_Hardware_selectionChanged(){
    if(ui->tree_Hardware != nullptr){
        if(ui->tree_Hardware->currentItem() != nullptr){
            QVariant HwNode = ui->tree_Hardware->currentItem()->data(ui->tree_Hardware->currentColumn(),Qt::UserRole);
            this->get_HardwareInformation(HwNode.toInt());
        }
    }

}

void MainWindow::on_tree_Hardware_itemDoubleClicked()
{

    switch(this->settings->value("hwdoubleclick",0).toInt()){

    case 0:
        on_tree_Hardware_custom_openOnHomepage();
        break;

    case 1:
        on_tree_Hardware_custom_openSerialOnHomepage();
        break;

    case 2:
        on_tree_Hardware_custom_openInHelp();
        break;

    case 3:
        on_tree_Hardware_custom_copySerial();
        break;

    default:
        on_tree_Hardware_custom_openOnHomepage();
        break;
    }

}

void MainWindow::on_tree_Hardware_customContextMenuRequested(const QPoint &pos)
{
    if(ui->tree_Hardware->currentItem() != nullptr){
        QMenu *menu=new QMenu(this);

        QAction *openOnHomepage = new QAction(this->HwDoubleClickActions.at(0), this);
        connect(openOnHomepage, SIGNAL(triggered()), this, SLOT(on_tree_Hardware_custom_openOnHomepage()));

        QAction *openSerialOnHomepage = new QAction(this->HwDoubleClickActions.at(1), this);
        connect(openSerialOnHomepage, SIGNAL(triggered()), this, SLOT(on_tree_Hardware_custom_openSerialOnHomepage()));

        QAction *openInHelp = new QAction(this->HwDoubleClickActions.at(2),this);
        connect(openInHelp, SIGNAL(triggered()), this, SLOT(on_tree_Hardware_custom_openInHelp()));

        QAction *copySerial = new QAction(this->HwDoubleClickActions.at(3), this);
        connect(copySerial, SIGNAL(triggered()), this, SLOT(on_tree_Hardware_custom_copySerial()));

        // Order must be correct!!!
        menu->addAction(openOnHomepage);
        menu->addAction(openSerialOnHomepage);
        menu->addAction(openInHelp);
        menu->addAction(copySerial);
        menu->popup(ui->tree_Hardware->viewport()->mapToGlobal(pos));
    }
}

void MainWindow::on_tree_Hardware_custom_openOnHomepage(){
    QVariant UUID = ui->tree_Hardware->currentItem()->data(0,Qt::UserRole);
    QString link;
    if(this->settings->value("hwsearchfor",0).toInt() == HW_PLUGGED){
        link = this->settings->value("brsearchwebprefix","https://www.br-automation.com/en/search/?q=").toString()+this->SysDump.Sections.Hardware.vNode[UUID.toUInt()].ModuleStatus.Plugged+this->settings->value("brsearchwebsuffix","").toString();
        this->statusbar->showMessage(tr("Searching for hardware \"%1\" on B&R homepage ...").arg(this->SysDump.Sections.Hardware.vNode[UUID.toUInt()].ModuleStatus.Plugged),DEFAULT_MESSAGE_TIME);
    } else {
        link = this->settings->value("brsearchwebprefix","https://www.br-automation.com/en/search/?q=").toString()+this->SysDump.Sections.Hardware.vNode[UUID.toUInt()].ModuleStatus.Configured+this->settings->value("brsearchwebsuffix","").toString();
        this->statusbar->showMessage(tr("Searching for hardware \"%1\" on B&R homepage ...").arg(this->SysDump.Sections.Hardware.vNode[UUID.toUInt()].ModuleStatus.Configured),DEFAULT_MESSAGE_TIME);
    }
    QDesktopServices::openUrl(QUrl(link));

}

void MainWindow::on_tree_Hardware_custom_openSerialOnHomepage(){
    QVariant UUID = ui->tree_Hardware->currentItem()->data(0,Qt::UserRole);
    QString link = this->settings->value("brsearchserprefix","https://www.br-automation.com/en/search/?q=").toString()+this->SysDump.Sections.Hardware.vNode[UUID.toUInt()].IOInformation.Serialnumber+this->settings->value("brsearchsersuffix","").toString();
    this->statusbar->showMessage(tr("Searching for serial \"%1\" on B&R homepage ...").arg(this->SysDump.Sections.Hardware.vNode[UUID.toUInt()].IOInformation.Serialnumber),DEFAULT_MESSAGE_TIME);
    QDesktopServices::openUrl(QUrl(link));
}

void MainWindow::on_tree_Hardware_custom_openInHelp(){
    QVariant UUID = ui->tree_Hardware->currentItem()->data(0,Qt::UserRole);
    if(this->settings->value("hwsearchfor",0).toInt() == HW_PLUGGED){
        this->openInBuRHelp(this->SysDump.Sections.Hardware.vNode[UUID.toUInt()].ModuleStatus.Plugged);
    } else {
        this->openInBuRHelp(this->SysDump.Sections.Hardware.vNode[UUID.toUInt()].ModuleStatus.Configured);
    }
}

void MainWindow::on_tree_Hardware_custom_copySerial(){
    QVariant UUID = ui->tree_Hardware->currentItem()->data(0,Qt::UserRole);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(this->SysDump.Sections.Hardware.vNode[UUID.toUInt()].IOInformation.Serialnumber);
    this->statusbar->showMessage(tr("Copied serial \"%1\" to clipboard ...").arg(this->SysDump.Sections.Hardware.vNode[UUID.toUInt()].IOInformation.Serialnumber));
}

void MainWindow::on_tree_Hardware_Channels_selectionChanged(){
    if(ui->tree_Hardware_Channels != nullptr){
        if(ui->tree_Hardware_Channels->currentItem() != nullptr){
            QVariant UUID = ui->tree_Hardware_Channels->currentItem()->data(ui->tree_Hardware_Channels->currentColumn(),Qt::UserRole);
            QVariant HwNode = ui->tree_Hardware->currentItem()->data(ui->tree_Hardware_Channels->currentColumn(),Qt::UserRole);
            this->get_HardwareChannelInformation(HwNode.toInt(), UUID.toInt());
        }
    }
}

void MainWindow::on_tree_ApplicationModules_itemClicked(QTreeWidgetItem *item, int column)
{
    QVariant UUID = item->data(column,Qt::UserRole);
    ui->combo_softwareFilter->setCurrentIndex(0);
    this->get_SoftwareModules(UUID.toInt(),ui->combo_softwareFilter->currentIndex());
}

void MainWindow::on_uiLabelClicked(int index){
    //qDebug()<<"Clicked: " << index;
    switch(index){
    case 1: // Overview Target
        ui->combo_SelectMenu->setCurrentIndex(1);
        ui->tabWidget_System->setCurrentIndex(0);
        break;
    case 2: // Overview Hardware
        ui->combo_SelectMenu->setCurrentIndex(3);
        break;

    case 3: // Overview Motion
        ui->tabWidget_Motion->setCurrentIndex(0);
        ui->combo_SelectMenu->setCurrentIndex(4);
        break;

    case 4: // Overview Application
        ui->combo_SelectMenu->setCurrentIndex(2);
        break;

    case 5: // Overview CPUMode
        ui->combo_SelectMenu->setCurrentIndex(1);
        ui->tabWidget_System->setCurrentIndex(0);
        break;

    case 250: // Overview Center
        break;

    default:
        break;
    }

    // Axis Error
    if(index < 200 && index >= 100){
            ui->tabWidget_Motion->setCurrentIndex(1);
            ui->combo_AxisError->setCurrentIndex(index-100);
    }
}

void MainWindow::on_hwtree_pressed_f1(){
    if(ui->tree_Hardware->currentIndex().isValid()){
        on_tree_Hardware_custom_openInHelp();
    }
}

void MainWindow::on_central_pressed_esc(){
    ui->combo_SelectMenu->setCurrentIndex(0);
}

BOOL CALLBACK MainWindow::EnumWindowsProc(HWND hwnd,LPARAM lparam){

    UNUSED(lparam);

    //if(!hwnd) return false;
    DWORD processId;
    DWORD threadId = GetWindowThreadProcessId(hwnd,&processId);
    UNUSED(threadId);

    if(processId==HelpProcessId)
    {
        QString Title;
        char buffer[21];
        PTSTR pTitle = static_cast<PTSTR>(malloc ( 20 * sizeof (TCHAR)));
        GetWindowText(hwnd,pTitle,20);

        // Copy title to QString
        for(int i = 0;i<21;i++){
            buffer[i] = static_cast<char>(*(pTitle+i));
            Title.append(buffer[i]);
        }

        // If title contains B&R Help Explorer, found the main window!
        if(Title.indexOf("B&R Help Explorer")!=-1){
            hwndBrHelpExp = hwnd;
        }
    }
return true;
}

BOOL CALLBACK MainWindow::EnumChildWindowsProc(HWND hwnd, LPARAM lParam){

    UNUSED(lParam);

    QString ClassName;
    char buffer[30];
    PTSTR pClassName = static_cast<PTSTR>(malloc ( 30 * sizeof (TCHAR)));
    GetClassName(hwnd,pClassName,30);

    // Copy class name to QString
    for(int i = 0;i<30;i++){
        buffer[i] = static_cast<char>(*(pClassName+i));
        ClassName.append(buffer[i]);
    }

    // If class name contains COMBOBOX, found the one and only ;-)
    if(ClassName.indexOf("COMBOBOX")!=-1){
        hwndComboBox = hwnd;
    }

    return true;
}

void MainWindow::openInBuRHelp(QString search){

    wchar_t ButtonCaption[MAX_PATH];
    //this->vASHelpHwnd.clear();

    // Get selected help language and create cmd args
    QString helpLang = this->settings->value("brhelplang","").toString();
    QString cmdArgs;
    if(helpLang == "en" || helpLang == "de"){
        cmdArgs = " /L=" + helpLang;
    } else {
        // If nothing configured / provided just ignore the language for now!
        cmdArgs = "";
    }

    if(!this->check_IsHelpOpen())
        if(!this->start_Application(this->settings->value("brhelpexplorer").toString().toStdWString().c_str(), cmdArgs))
            return;

    // Get hwnd
    EnumWindows(&MainWindow::EnumWindowsProc,MAKELPARAM(0,0));

    if(hwndBrHelpExp){

        // Have hwnd now, get child with class title containing "COMBOBOX"
        EnumChildWindows(hwndBrHelpExp,&MainWindow::EnumChildWindowsProc,MAKELPARAM(0,0));

        // Should have hwnd of the COMBOBOX now!
        if(this->hwndComboBox){
            HWND hwnd = FindWindowEx(GetParent(this->hwndComboBox), nullptr, nullptr, nullptr);
            while(hwnd){
                GetWindowText(hwnd, ButtonCaption, MAX_PATH);

                if(!wcsicmp(ButtonCaption,L"Suchen") || !wcsicmp(ButtonCaption,L"Search")){


                    ComboBox_InsertString(this->hwndComboBox,0,search.toStdWString().c_str());
                    ComboBox_SelectString(this->hwndComboBox,0,search.toStdWString().c_str());
                    SetForegroundWindow(hwndBrHelpExp);
                    SendMessage(GetParent(GetParent(this->hwndComboBox)),TCM_SETCURFOCUS,0,0);
                    SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(0, 0));
                    SendMessage(hwnd, WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM(0, 0));
                    this->statusbar->showMessage(tr("Successfully connected to B&R help!"),DEFAULT_MESSAGE_TIME);
                }

                hwnd = FindWindowEx(GetParent(this->hwndComboBox), hwnd, nullptr, nullptr);
            }
        } else {
            QMessageBox::critical(this,tr("Error!"),tr("<p>Internal error!\n\nError nr.: 30200</p>"));
        }
    } else {
        QMessageBox::critical(this,tr("Error!"),tr("<p>Help not opened! Check path and time in options!\n\nIf your help is open, please report Bug!</p>"));
    }

}


// Search for Help-xx\Data\brhelpcontent.xml files in configured help path
QStringList MainWindow::getInstalledHelpLanguages(QString helpExplorer){

    QStringList helpLangs;

    if(QFile::exists(helpExplorer)){

        QDir ASDir = QFileInfo(helpExplorer).absoluteDir();
        ASDir.cdUp(); // remove bin-en / bin-de folder by going one up
        QString ASPath = ASDir.absolutePath();
        if(QFile::exists(ASPath + "\\Help-de\\Data\\brhelpcontent.xml")){
            helpLangs.append("de");
        }
        if(QFile::exists(ASPath + "\\Help-en\\Data\\brhelpcontent.xml")){
            helpLangs.append("en");
        }

    } else {
        this->statusbar->showMessage(tr("Invalid path to help specified! Check options!"));
    }

    return helpLangs;
}


char* MainWindow::pCharToUtf8(const char *charsrc){
    return const_cast<char *> (charsrc);
}

QColor MainWindow::QStringToQColor(QString rgbString){
    QStringList tmpStringList;
    tmpStringList = rgbString.split(",");
    if(tmpStringList.size() == 3)
         return QColor::fromRgb(tmpStringList[0].toInt(),tmpStringList[1].toInt(),tmpStringList[2].toInt());

    return QColor(Qt::red);
}

void MainWindow::recentUpdateFileActions()
{
    std::vector<QAction *>().swap(this->recentFilesActions);
    ui->menu_openRecent->clear();

    QStringList recentFilesList = this->settings->value("recentFileList").toStringList();

    while(recentFilesList.size()>MaxRecentFiles)
        recentFilesList.removeLast();

    int numRecentFiles = recentFilesList.size();

    for(int i=0;i<numRecentFiles;i++){

        if(this->recentFilesActions.size() == 0){
            QAction *delAction = new QAction(ui->menu_openRecent);
            this->recentFilesActions.push_back(delAction);

            // Add delete button and seperator
            this->recentFilesActions[this->recentFilesActions.size()-1]->setText(tr("Clear list ..."));
            this->recentFilesActions[this->recentFilesActions.size()-1]->setIcon(QPixmap("://images/ico_delete.png"));
            connect(this->recentFilesActions[this->recentFilesActions.size()-1], SIGNAL(triggered()),this,SLOT(on_recentFiles_clearList()));
            ui->menu_openRecent->addAction(this->recentFilesActions[this->recentFilesActions.size()-1]);
            ui->menu_openRecent->addSeparator();
            ui->menu_openRecent->setDisabled(false);
        }

        QString text = QString("&%1 %2")
                .arg(i+1)
                .arg(QFileInfo(recentFilesList[i]).fileName());

        QAction *action = new QAction(ui->menu_openRecent);
        this->recentFilesActions.push_back(action);

        this->recentFilesActions[this->recentFilesActions.size()-1]->setText(text);
        this->recentFilesActions[this->recentFilesActions.size()-1]->setData(recentFilesList[i]);
        this->recentFilesActions[this->recentFilesActions.size()-1]->setVisible(true);
        connect(this->recentFilesActions[this->recentFilesActions.size()-1],SIGNAL(triggered()),this,SLOT(on_recen_openFile()));
        ui->menu_openRecent->addAction(this->recentFilesActions[this->recentFilesActions.size()-1]);
        ui->menu_openRecent->setDisabled(false);
    }

    if(this->recentFilesActions.size()==0)
        ui->menu_openRecent->setDisabled(true);
}

void MainWindow::recentSetCurrentFile(const QString &fileName)
{
  /*
    qDebug() << fileName;
    qDebug() << QApplication::applicationDirPath();
    qDebug() << this->extractionPath;
    qDebug() << QDir::cleanPath(this->extractionPath+"/"+this->ipAddress+"/Systemdump.xml");
   */
    // No temporary files in the list! For temp downloads is the downloaded menu!
    if((fileName.indexOf(QApplication::applicationDirPath()) == -1) && (fileName.indexOf(QDir::cleanPath(this->extractionPath+"/"+this->ipAddress+"/Systemdump.xml")) == -1)){
        QStringList recentFilesList = this->settings->value("recentFileList").toStringList();

        recentFilesList.removeAll(QDir::cleanPath(fileName));
        recentFilesList.prepend(QDir::cleanPath(fileName));
        settings->setValue("recentFileList",recentFilesList);
        this->settings->sync();
        recentUpdateFileActions();
    }
}

void MainWindow::recentSetCurrentIpAddress(const QString &ipAddress)
{
    QStringList recentIpList = this->settings->value("recentIpList").toStringList();
    recentIpList.removeAll(ipAddress);
    recentIpList.prepend(ipAddress);
    while(recentIpList.size() > MaxRecentFiles)
        recentIpList.removeLast();

    this->settings->setValue("recentIpList",recentIpList);
    this->settings->sync();
    recentIpAddresses = recentIpList;
}

void MainWindow::setStatusBarEnabled(bool enabled, QString statusMessage){
    if(enabled){
        this->progressBar.setVisible(true);
        this->statusbarLabel.setText(statusMessage);
    } else {
        this->progressBar.setVisible(false);
        this->statusbarLabel.clear();
        qApp->processEvents();
    }
}

void MainWindow::setup_UI(){

    // Get primary screen size for initial value
    QSize sSize = QGuiApplication::primaryScreen()->size();
    qreal monWidth = sSize.width();
    qreal monHeight = sSize.height();

    // Get total desktop size to validate the saved position (all monitors together!)
    QScreen *desktop = QGuiApplication::primaryScreen();
    int totalHeight=desktop->geometry().height();
    int totalWidth=desktop->geometry().width();

    // Load positions and geometry from settings if saved
    int xpos = settings->value("xpos",(monWidth - this->width())/2).toInt();
    int ypos = settings->value("ypos",(monHeight - this->height())/2).toInt();
    int width = settings->value("width",DEFAULT_WINDOW_WIDTH).toInt();
    int height = settings->value("height",DEFAULT_WINDOW_HEIGHT).toInt();

    // Make sure app is in visible area of the screen(s)
    if(xpos + width > totalWidth){
        xpos = totalWidth - width;
    }
    if(ypos + height > totalHeight){
        ypos = totalHeight - height;
    }

    // Set window geometry and position
    this->resize(width,height);
    this->move(xpos, ypos);

    // Alternating Rows
    ui->table_AxisError->setAlternatingRowColors(true);
    ui->table_Logger->setAlternatingRowColors(true);
    ui->table_Profiler->setAlternatingRowColors(true);
    ui->table_Software->setAlternatingRowColors(true);
    ui->table_TaskClasses->setAlternatingRowColors(true);

    // Setup Shortcuts
    QShortcut* shortcut00 = new QShortcut(QKeySequence(Qt::Key_F1), ui->table_Logger);
    connect(shortcut00, SIGNAL(activated()), this, SLOT(on_logger_pressed_f1()));

    QShortcut* shortcut01 = new QShortcut(QKeySequence(Qt::Key_F1), ui->tree_Hardware);
    connect(shortcut01, SIGNAL(activated()), this, SLOT(on_hwtree_pressed_f1()));

    QShortcut* shortcut02 = new QShortcut(QKeySequence(Qt::Key_Escape), ui->centralWidget);
    connect(shortcut02, SIGNAL(activated()), this, SLOT(on_central_pressed_esc()));

    // Hardware Tree Selection Signals (for arrow keys)
    ui->tree_Hardware->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->tree_Hardware, SIGNAL(itemSelectionChanged()), this, SLOT(on_tree_Hardware_selectionChanged()), Qt::QueuedConnection);
    ui->tree_Hardware_Channels->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(ui->tree_Hardware_Channels, SIGNAL(itemSelectionChanged()), this, SLOT(on_tree_Hardware_Channels_selectionChanged()), Qt::QueuedConnection);

    // Selection Mode for all Tables
    QList<QTableWidget *> uiTables = ui->centralWidget->findChildren<QTableWidget *>();
    foreach(QTableWidget * uiTable, uiTables){
        uiTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        uiTable->setSelectionMode(QAbstractItemView::SingleSelection);
    }

    // Network slots
    QObject::connect(&netAccManGET, SIGNAL(finished(QNetworkReply*)), &netEventLoopGET, SLOT(quit()));
    QObject::connect(&netAccManPOST, SIGNAL(finished(QNetworkReply*)), &netEventLoopPOST, SLOT(quit()));
    QObject::connect(&netAccManDownload, SIGNAL(finished(QNetworkReply*)), &netEventLoopDownload, SLOT(quit()));

    // Export pdf
    connect(ui->actionSave_PDF_Report,SIGNAL(triggered()),this,SLOT(on_createPDFReport_triggered()),Qt::UniqueConnection);

    // Export datapoints
    connect(ui->actionSave_Datapoints,SIGNAL(triggered()),this,SLOT(on_createDatapointExport_triggered()),Qt::UniqueConnection);

    // Connect Logger Table Clicked
    connect(ui->table_Logger->horizontalHeader(),SIGNAL(sectionClicked(int)), this,SLOT(on_logger_section_clicked(int)));

    // Config Network Timer
    this->netTimeout.setSingleShot(true);
    connect(&this->netTimeout, SIGNAL(timeout()), &netEventLoopGET, SLOT(quit()));
    connect(&this->netTimeout, SIGNAL(timeout()), &netEventLoopPOST, SLOT(quit()));
    connect(&this->netTimeout, SIGNAL(timeout()), &netEventLoopDownload, SLOT(quit()));

    // Status / Progressbar
    this->statusbar = new QStatusBar(this);
    this->statusbar->setStyleSheet(QString("*{background-color: transparent;}"));
    this->setStatusBar(this->statusbar);

    // Loading GIF
    movLoadingGif = new QMovie("://images/spinner.gif");
    movLoadingGif->setScaledSize(QSize(28,15));
    lblLoadingGif = new QLabel(this);
    lblLoadingGif->setFixedSize(28,15);
    lblLoadingGif->setVisible(false);

    this->statusbar->addWidget(this->lblLoadingGif);
    this->statusbar->addWidget(&this->statusbarLabel);
    this->progressBar.setMaximum(this->progressMax());
    this->progressBar.setMinimum(this->progressMin());
    this->progressBar.resize(PROGRESSBAR_WIDTH,this->statusbar->height());
    this->progressBar.setFixedWidth(PROGRESSBAR_WIDTH);
    connect(this, SIGNAL(signalProgress(int)), &this->progressBar, SLOT(setValue(int)));
    this->statusbar->addPermanentWidget(&this->progressBar);
    this->setStatusBarEnabled(false);

    // Recent files
    this->recentUpdateFileActions();

    // Downloaded files
    this->downloadedUpdateList();

}

void MainWindow::startScreenCenter(){

    int bgwidth = 1920;
    int bgheight = 1280;
    int stripewidth = 3800;
    int stripeheight = 268;
    int iconLength = 256;
    int xpos,ypos;

    xpos = (this->width()/2 - (bgwidth/2));
    ypos = (this->height()/2 - (bgheight/2));
    this->startBg->move(xpos,ypos);

    xpos = (this->width()/2 -(stripewidth/2));
    ypos = (this->height()/2 - (stripeheight/2));
    this->start_stripe->move(xpos,ypos);

    xpos = (this->width()/2 - iconLength);
    ypos = (this->height()/2 - static_cast<int>(0.5*iconLength));
    this->start_openxml->move(xpos,ypos);

    xpos = xpos - ((STARTPAGE_LABEL_WIDTH/2)-(iconLength/2));
    ypos = ypos + iconLength;
    this->start_textopenxml->move(xpos,ypos);

    xpos = (this->width()/2);
    ypos = (this->height()/2 - static_cast<int>(0.5*iconLength));
    this->start_openplc->move(xpos,ypos);


    xpos = xpos - ((STARTPAGE_LABEL_WIDTH/2)-(iconLength/2));
    ypos = ypos + iconLength;
    this->start_textopenplc->move(xpos,ypos);

    xpos = this->width()/2 - this->start_sdvLogo->width()/2;
    ypos = ypos+40;
    this->start_sdvLogo->move(xpos,ypos);

    xpos = (this->width() - (this->start_brLogo->width()+5));
    ypos = (ui->menuBar->height() + 5);
    this->start_brLogo->move(xpos,ypos);
}

void MainWindow::startScreenInit(){

    QFontDatabase fontDatabase;
    fontDatabase.addApplicationFont("://fonts/miso-regular.otf");

    QFont font;
    font.setBold(true);
    font.setFamily("Miso");
    font.setPixelSize(24);

    QFont logoFont;
    logoFont.setFamily("Miso");
    logoFont.setPixelSize(54);

    int bgwidth = 1920;
    int bgheight = 1280;
    int stripewidth = 3800;
    int stripeheight = 268;
    int iconLength = 256;
    int xpos,ypos;

    xpos = (this->width()/2 - (bgwidth/2));
    ypos = (this->height()/2 - (bgheight/2));
    this->scn = new QGraphicsScene(this);
    this->scn->addPixmap(QPixmap("://images/startBg.png"));
    this->startBg = new QGraphicsView(this);
    this->startBg->setScene(this->scn);
    this->startBg->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->startBg->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->startBg->setGraphicsEffect(&this->startBgEffect);
    this->startBg->resize(bgwidth,bgheight);
    this->startBg->move(xpos,ypos);

    this->start_stripe = new QLabel (this);
    xpos = (this->width()/2 -(stripewidth/2));
    ypos = (this->height()/2 - (stripeheight/2));
    this->start_stripe->setText(QString("<html><img src='://images/startStripe.png'/></html>"));
    this->start_stripe->resize(stripewidth,stripeheight);
    this->start_stripe->move(xpos,ypos);

    this->start_openxml = new QLabel(this);
    xpos = (this->width()/2 - iconLength);
    ypos = (this->height()/2 - static_cast<int>(0.5*iconLength));
    this->start_openxml->setText(QString("<html><img src='://images/start_openxml.png'/></html>"));
    this->start_openxml->resize(iconLength,iconLength);
    this->start_openxml->move(xpos,ypos);

    this->start_textopenxml = new QLabel(this);
    xpos = xpos - ((STARTPAGE_LABEL_WIDTH/2)-(iconLength/2));
    ypos = ypos + iconLength;
    this->start_textopenxml->setFont(font);
    this->start_textopenxml->setAlignment(Qt::AlignCenter);
    this->start_textopenxml->setText(tr("Load .xml / .tar.gz"));
    this->start_textopenxml->resize(STARTPAGE_LABEL_WIDTH,STARTPAGE_LABEL_HEIGHT);
    this->start_textopenxml->move(xpos,ypos);
    this->start_textopenxml->setVisible(false);

    this->start_openplc = new QLabel(this);
    xpos = (this->width()/2);
    ypos = (this->height()/2 - static_cast<int>(0.5*iconLength));
    this->start_openplc->setText(QString("<html><img src='://images/start_openplc.png'/></html>"));
    this->start_openplc->resize(iconLength,iconLength);
    this->start_openplc->move(xpos,ypos);

    this->start_textopenplc = new QLabel(this);
    xpos = xpos - ((STARTPAGE_LABEL_WIDTH/2)-(iconLength/2));
    ypos = ypos + iconLength;
    this->start_textopenplc->setFont(font);
    this->start_textopenplc->setAlignment(Qt::AlignCenter);
    this->start_textopenplc->setText(tr("Load from PLC"));
    this->start_textopenplc->resize(STARTPAGE_LABEL_WIDTH,STARTPAGE_LABEL_HEIGHT);
    this->start_textopenplc->move(xpos,ypos);
    this->start_textopenplc->setVisible(false);

    this->start_sdvLogo = new QLabel(this);
    this->start_sdvLogo->resize(400,40);
    this->start_sdvLogo->setFont(logoFont);
    xpos = this->width()/2 - this->start_sdvLogo->width()/2;
    ypos = ypos + 50;
    this->start_sdvLogo->move(xpos,ypos);
    this->start_sdvLogo->setAlignment(Qt::AlignCenter);
    QStringList Versions = this->Version.split(".");
    this->start_sdvLogo->setText("SdV v" + Versions[0] + "." + Versions[1]);

    this->start_brLogo = new QLabel(this);
    this->start_brLogo->resize(118,79);
    xpos = (this->width() - (this->start_brLogo->width()+5));
    ypos = (ui->menuBar->height() + 5);
    this->start_brLogo->setText(QString("<html><img src='://images/brlogo.png'/></html>"));
    this->start_brLogo->move(xpos,ypos);

    ui->menuBar->raise();
    ui->statusBar->raise();

}

void MainWindow::startScreenShow(bool enabled){

    if(enabled){

        // Hide first page and show startpage
        ui->combo_SelectMenu->setVisible(false);
        ui->stacked_MainFrame->setVisible(false);
        ui->widget_overview->setVisible(false);

        this->startScreenCenter();

        this->startBg->setVisible(true);
        this->start_openplc->setVisible(true);
        this->start_openxml->setVisible(true);
        this->start_stripe->setVisible(true);
        this->start_sdvLogo->setVisible(true);
        this->start_brLogo->setVisible(true);
        this->start_textopenplc->setVisible(true);
        this->start_textopenxml->setVisible(true);

    } else {

        // Hide startpage and show first page
        ui->combo_SelectMenu->setVisible(true);
        ui->stacked_MainFrame->setVisible(true);
        ui->widget_overview->setVisible(true);

        this->startBg->setVisible(false);
        this->start_openplc->setVisible(false);
        this->start_openxml->setVisible(false);
        this->start_stripe->setVisible(false);
        this->start_sdvLogo->setVisible(false);
        this->start_brLogo->setVisible(false);
        this->start_textopenplc->setVisible(false);
        this->start_textopenxml->setVisible(false);

    }
}

bool MainWindow::sort_LogSortByTimeStampRev(const sLogModuleEntry &lhs, const sLogModuleEntry &rhs){
    return lhs.TimeStamp > rhs.TimeStamp;
}

bool MainWindow::sort_LogSortByTimeStampRevV2(const sLogModuleEntryV2 &lhs, const sLogModuleEntryV2 &rhs){
    return lhs.TimeStamp > rhs.TimeStamp;
}

void MainWindow::splashScreenShow(int time){
    QPixmap pixmap("://images/splash.png");
    QSplashScreen splash(pixmap);
    splash.setFont(QFont("Helvetica", 8, QFont::Normal));
    splash.show();
    splash.showMessage(
          QString("Version: ") + Version,
          Qt::AlignRight|Qt::AlignTop,
          Qt::white
       );
    qApp->processEvents();

    Sleep(static_cast<DWORD>(time));
    splash.finish(this);
}

bool MainWindow::start_Application(LPCTSTR lpApplicationName, QString cmdArgs)
{
    // additional information
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // set the size of the structures
    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );


    LPTSTR lptArgs = (LPTSTR)cmdArgs.utf16();

     // start the program up
     if(!CreateProcess( lpApplicationName,   // the path
     lptArgs,           // Command line
     nullptr,           // Process handle not inheritable
     nullptr,           // Thread handle not inheritable
     FALSE,          // Set handle inheritance to FALSE
     NORMAL_PRIORITY_CLASS,           	// No creation flags
     nullptr,           // Use parent's environment block
     nullptr,           // Use parent's starting directory
     &si,            // Pointer to STARTUPINFO structure
     &pi )           // Pointer to PROCESS_INFORMATION structure
     ){
         QMessageBox::critical(this,tr("Error!"),tr("Error opening Help!\nCheck path in options!"));
         return 0;
     }

     // Sleep because :: When WaitForSingleObject fires, the window
     // sometimes is not built up completely yet... -> Hwnd errors...
     Sleep(static_cast<DWORD>(this->settings->value("ashelpwaittime",DEFAULT_ASHELP_WAITTIME).toInt()));
     WaitForSingleObject(pi.hProcess,static_cast<DWORD>(this->settings->value("ashelpwaittime",DEFAULT_ASHELP_WAITTIME).toInt()));

     this->HelpProcessId = pi.dwProcessId;

     // Close process and thread handles.
     CloseHandle( pi.hProcess );
     CloseHandle( pi.hThread );
     return 1;
}

int MainWindow::start_ApplicationQt(const QString &program, const QStringList &arguments){
    QProcess process;
    //process.setReadChannelMode(QProcess::ForwardedChannels);
    process.start(program, arguments);

    if (!process.waitForFinished(-1))
        return -2;
    return process.exitStatus() == QProcess::NormalExit ? process.exitCode() : -1;
}

void MainWindow::trigger_ApplicationQt(const QString &program, const QStringList &arguments){
    QString cmdString = program;
    foreach (QString str, arguments) {
        cmdString.append(" "+str);
    }

    // qDebug() << cmdString;
    system(QString("start "+cmdString).toStdString().c_str());
    return;
}

void MainWindow::translateQStringLists(){

    this->HwDoubleClickActions.clear();
    this->HwDoubleClickActions << tr("Search &Hardware on B&&R Homepage ...")
                               << tr("Search &Serialnumber on B&&R Homepage ...")
                               << tr("S&earch Hardware in B&&R Help ...")
                               << tr("&Copy Serialnumber to clipboard ...");

    this->HwDoubleClickActionsDrop.clear();
    this->HwDoubleClickActionsDrop << tr("Search Hardware on B&R Homepage ...")
                                   << tr("Search Serialnumber on B&R Homepage ...")
                                   << tr("Search Hardware in B&R Help ...")
                                   << tr("Copy Serialnumber to clipboard ...");

}

void MainWindow::updateColorTheme(int themeIndex){
    if(themeIndex >= 0){
        qApp->setStyle(QStyleFactory::create(this->vColorThemes[static_cast<unsigned int>(themeIndex)].name));

        QPalette themePalette;
        themePalette.setColor(QPalette::Window, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cWindow);
        themePalette.setColor(QPalette::WindowText, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cWindowText);
        themePalette.setColor(QPalette::Base, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cBase);
        themePalette.setColor(QPalette::AlternateBase, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cAlternateBase);
        themePalette.setColor(QPalette::ToolTipBase, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cToolTipBase);
        themePalette.setColor(QPalette::ToolTipText, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cToolTipText);
        themePalette.setColor(QPalette::Text, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cText);
        themePalette.setColor(QPalette::Button, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cButton);
        themePalette.setColor(QPalette::ButtonText, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cButtonText);
        themePalette.setColor(QPalette::BrightText, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cBrightText);
        themePalette.setColor(QPalette::Link, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cLink);
        themePalette.setColor(QPalette::Highlight, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cHighlight);
        themePalette.setColor(QPalette::HighlightedText, this->vColorThemes[static_cast<unsigned int>(themeIndex)].cHighlightedText);
        themePalette.setColor(QPalette::Disabled,QPalette::Text,this->vColorThemes[static_cast<unsigned int>(themeIndex)].cBrightText);

        qApp->setPalette(themePalette);
        qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

        // Because of the axis design is fixed and the colors are overwritten by the global settings
        // the stylessheets have to be reloaded!
        ui->text_Axis_Name->setStyleSheet(this->Styles.axisHeadTopStyle);
        ui->text_Axis_Drive->setStyleSheet(this->Styles.axisHeadMidStyle);
        ui->text_Axis_Address->setStyleSheet(this->Styles.axisHeadBottomStyle);

        ui->text_Axis_Homing->setStyleSheet(this->Styles.axisTopStyle);
        ui->text_Axis_Alarm->setStyleSheet(this->Styles.axisBottomStyle);

        ui->text_Axis_Controller->setStyleSheet(this->Styles.axisOrangeStyle);
        ui->text_Axis_Simulation->setStyleSheet(this->Styles.axisOrangeStyle);
        ui->text_Axis_Enable->setStyleSheet(this->Styles.axisOrangeStyle);
        ui->text_Axis_Trigger1->setStyleSheet(this->Styles.axisOrangeStyle);
        ui->text_Axis_Trigger2->setStyleSheet(this->Styles.axisOrangeStyle);
        ui->text_Axis_PosEndSw->setStyleSheet(this->Styles.axisOrangeStyle);
        ui->text_Axis_NegEndSw->setStyleSheet(this->Styles.axisOrangeStyle);
        ui->text_Axis_RefSw->setStyleSheet(this->Styles.axisOrangeStyle);

        ui->text_Axis_Movement->setStyleSheet(this->Styles.axisWhiteStyle);
        ui->text_Axis_Position->setStyleSheet(this->Styles.axisWhiteStyle);
        ui->text_Axis_Speed->setStyleSheet(this->Styles.axisWhiteStyle);

        // Option BG
        this->optionBgEffect.setColor(this->vColorThemes[static_cast<unsigned int>(themeIndex)].cHighlight);
        this->optionBgEffect.setStrength(1.0);

        // Report BG
        this->reportBgEffect.setColor(this->vColorThemes[static_cast<unsigned int>(themeIndex)].cHighlight);
        this->reportBgEffect.setStrength(1.0);

        // Take over color for start page
        this->startBgEffect.setColor(this->vColorThemes[static_cast<unsigned int>(themeIndex)].cHighlight);
        this->startBgEffect.setStrength(1.0);

        // Dump has to be reopened in order to take over the axis design!
        QFile TestFile(this->currentFileString);
        if(TestFile.exists()){
            //qDebug() << "Reopening Dump ...";
            QString rememberedFile = this->currentFileString;
            QString rememberedIP = this->ipAddress;
            this->on_actionClose_xml_triggered();
            this->readXML(rememberedFile.toStdString().c_str());
            this->ipAddress = rememberedIP;
            this->dataObjectsUpdateList();
            this->dataSaveAllEnable(!(this->ipAddress.isEmpty()));

        }
    } else {

        CSimpleIniA ini;
        ini.SetUnicode();
        QString themeFilePath = this->themePath + this->settings->value("themefile","lang_en.qm").toString();
        ini.LoadFile(themeFilePath.toStdString().c_str());

        if(this->vColorThemes.size() < 1)
            this->vColorThemes.push_back(sColorTheme());

        this->vColorThemes[0].filename        = this->settings->value("themefile","lang_en.qm").toString();
        this->vColorThemes[0].name            = QString::fromUtf8(ini.GetValue("SDVTheme", "name", "Untitled Theme"));

        this->vColorThemes[0].cWindow         = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Window", "245,245,245")));
        this->vColorThemes[0].cWindowText     = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_WindowText", "25,25,25")));
        this->vColorThemes[0].cBase           = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Base", "235,235,235")));
        this->vColorThemes[0].cAlternateBase  = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_AlternateBase", "245,245,245")));
        this->vColorThemes[0].cToolTipBase    = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_ToolTipBase", "235,235,235")));
        this->vColorThemes[0].cToolTipText    = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_ToolTipText", "25,25,25")));
        this->vColorThemes[0].cText           = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Text", "25,25,25")));
        this->vColorThemes[0].cButton         = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Button", "245,245,245")));
        this->vColorThemes[0].cButtonText     = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_ButtonText", "25,25,25")));
        this->vColorThemes[0].cBrightText     = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_BrightText", "255,0,0")));
        this->vColorThemes[0].cLink           = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Link", "255,128,0")));
        this->vColorThemes[0].cHighlight      = this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Highlight", "255,128,0")));
        this->vColorThemes[0].cHighlightedText= this->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_HighlightedText", "235,235,235")));

        this->updateColorTheme(0);
    }
}

void MainWindow::updateLanguage(bool update) {

    QString translations;
    QString local;
    QStringList nameFilter;

    // if autolanguage is active, try to derive language from system language
    if(this->settings->value("autolanguage",true).toBool()){

        if(QLocale().name().toStdString().find("de")==0) local = QString("de");
        if(QLocale().name().toStdString().find("en")==0) local = QString("en");
        if(QLocale().name().toStdString().find("fr")==0) local = QString("fr");
        if(QLocale().name().toStdString().find("pl")==0) local = QString("pl");
        if(QLocale().name().toStdString().find("ru")==0) local = QString("ru");
        if(QLocale().name().toStdString().find("cs")==0) local = QString("cs");
        if(QLocale().name().toStdString().find("da")==0) local = QString("da");
        if(QLocale().name().toStdString().find("el")==0) local = QString("el");
        if(QLocale().name().toStdString().find("es")==0) local = QString("es");
        if(QLocale().name().toStdString().find("et")==0) local = QString("et");
        if(QLocale().name().toStdString().find("eu")==0) local = QString("eu");
        if(QLocale().name().toStdString().find("fi")==0) local = QString("fi");
        if(QLocale().name().toStdString().find("he")==0) local = QString("he");
        if(QLocale().name().toStdString().find("hr")==0) local = QString("hr");
        if(QLocale().name().toStdString().find("hu")==0) local = QString("hu");
        if(QLocale().name().toStdString().find("hy")==0) local = QString("hy");
        if(QLocale().name().toStdString().find("is")==0) local = QString("is");
        if(QLocale().name().toStdString().find("it")==0) local = QString("it");
        if(QLocale().name().toStdString().find("ja")==0) local = QString("ja");
        if(QLocale().name().toStdString().find("ko")==0) local = QString("ko");
        if(QLocale().name().toStdString().find("lt")==0) local = QString("lt");
        if(QLocale().name().toStdString().find("nl")==0) local = QString("nl");
        if(QLocale().name().toStdString().find("pt")==0) local = QString("pt");
        if(QLocale().name().toStdString().find("ro")==0) local = QString("ro");
        if(QLocale().name().toStdString().find("sk")==0) local = QString("sk");
        if(QLocale().name().toStdString().find("sl")==0) local = QString("sl");
        if(QLocale().name().toStdString().find("sr")==0) local = QString("sr");
        if(QLocale().name().toStdString().find("sv")==0) local = QString("sv");
        if(QLocale().name().toStdString().find("uk")==0) local = QString("uk");
        if(QLocale().name().toStdString().find("zh")==0) local = QString("zh");


        translations = QString("lang/lang_%1.qm").arg(local);
        nameFilter.append(QString("lang_%1.qm").arg(local));
    } else {
        // else, take configured language!
        translations = QString("lang/") + this->settings->value("languagefile").toString();
        nameFilter.append(this->settings->value("languagefile","lang_en.qm").toString());
    }

    QDir directory("lang/");

    if(directory.entryList(nameFilter).count() > 0){

        //qDebug() << "Found: " + directory.entryList(nameFilter).at(0);

        // Apply new language
        QApplication::instance()->removeTranslator(&this->mTranslator);

        if (this->mTranslator.load(translations)) {
            QApplication::instance()->installTranslator(&this->mTranslator);

        } else {
            QMessageBox::information(this,tr("Information"),tr("The language file %1 could not be found! Maybe the file is corrupt!").arg(translations));
            this->settings->setValue("languagefile","lang_en.qm");
        }

        if(update){
            ui->retranslateUi(this);
            this->start_textopenplc->setText(tr("Load from PLC"));
            this->start_textopenxml->setText(tr("Load .xml / .tar.gz"));
        }

        QFile TestFile(this->currentFileString);
        if(TestFile.exists()){     
            QString rememberedFile = this->currentFileString;
            QString rememberedIP = this->ipAddress;
            this->on_actionClose_xml_triggered();
            this->readXML(rememberedFile.toStdString().c_str());
            this->ipAddress = rememberedIP;
            this->dataObjectsUpdateList();
            this->dataSaveAllEnable(!(this->ipAddress.isEmpty()));
        }

    }

    this->translateQStringLists();
}
