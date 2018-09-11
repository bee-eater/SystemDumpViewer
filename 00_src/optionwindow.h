#ifndef OPTIONWINDOW_H
#define OPTIONWINDOW_H

#include <QDialog>
#include <QMap>
#include <QCheckBox>
#include <QItemSelection>
#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace Ui {
    class OptionWindow;
}

/**
 * @brief The OptionWindow class is for the option dialog that can be called from the main window
 */
class OptionWindow : public QDialog
{
    Q_OBJECT

public:
    explicit OptionWindow(QWidget *parent, MainWindow *w);
    ~OptionWindow();

private slots:
    void on_btn_options_browseBRhelp_released(); /**< Slot for "Browse ..." button to search for BR.Help.Explorer.exe */
    void on_buttonBox_accepted();          /**< Slot for "OK" Button, -> Stuff is saved to ini-file */

    void on_check_option_autolanguage_toggled(bool checked);
    void on_navBarSelectionChanged();

private:
    Ui::OptionWindow *ui;
    MainWindow *Main;
    QStringList foundLanguages;
    QMap<QString, QString> map_lf2str;
    QMap<QString, QString> map_str2lf;
    QString lang_old;
    QString theme_old;
    Qt::CheckState checkAutoLang_old;

    void init_maps();

};

#endif // OPTIONWINDOW_H
