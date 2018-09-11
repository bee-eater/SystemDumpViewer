#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#include "reportsettings.h"
#include "ui_reportsettings.h"

#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>

ReportWindow::ReportWindow(QWidget *parent, MainWindow *w) :
    QDialog(parent),
    ui(new Ui::ReportWindow)
{
    // Basic stuff
    // ----------------------------------------------------->>>
        // Copy pointers
        this->Main = w;

        ui->setupUi(this);

        // Background
        QGraphicsScene *scn = new QGraphicsScene(this);
        scn->addPixmap(QPixmap("://images/OptionBg.png"));
        ui->graphic_option_bg->setScene(scn);
        ui->graphic_option_bg->setGraphicsEffect(&this->Main->reportBgEffect);
    // <<<-----------------------------------------------------


    // Fill stuff
    // ----------------------------------------------------->>>
        ui->label_report_machid->setText(this->Main->settings->value("reportmachid",tr("Machine XY")).toString());
        ui->label_report_title->setText(this->Main->settings->value("reporttitle",tr("PDFReport")).toString());
        ui->date_report_date->setDate(QDate::currentDate());
    // <<<-----------------------------------------------------

}

void ReportWindow::on_buttonBox_accepted()
{
    this->Main->settings->setValue("reportmachid",ui->label_report_machid->text());
    this->Main->settings->setValue("reporttitle",ui->label_report_title->text());

    this->Main->createPDFReport(ui->combo_report_type->currentIndex(),this->reportFile,ui->date_report_date->date().toString("dd.MM.yyyy"));
}

ReportWindow::~ReportWindow()
{
    delete ui;
}


void ReportWindow::on_btn_report_browsefile_released()
{
    QString filename = QFileDialog::getSaveFileName(NULL,tr("Save PDF report as..."),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"\\Report_" + QDate::currentDate().toString("yyyyMMdd") + ".pdf","*.pdf");
    this->reportFile = filename;
    ui->label_report_savefile->setText(this->reportFile);
}
