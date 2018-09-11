#ifndef REPORTSETTINGS_H
#define REPORTSETTINGS_H

#include <QDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace Ui {
    class ReportWindow;
}

/**
 * @brief The ReportSettings class is for the "Create PDF Report" dialog that can be called from the main window
 */
class ReportWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ReportWindow(QWidget *parent, MainWindow *w);
    ~ReportWindow();

private slots:
    void on_buttonBox_accepted();
    void on_btn_report_browsefile_released();

private:
    Ui::ReportWindow *ui;
    MainWindow *Main;
    QString reportFile;

};

#endif // OPTIONWINDOW_H
