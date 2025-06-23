#include "optionwindow.h"
#include "ui_optionwindow.h"

#include <QStandardPaths>
#include <QFileDialog>
#include <QDebug>

//#include "./includes/SimpleIni.h"

void OptionWindow::on_btn_options_browseBRhelp_released()
{
    QStringList PathList = QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Browse for help explorer ..."),
                                                    PathList.at(0), tr("Files (Br.Help.Explorer.exe)"));
    if(fileName != QString("")){
        ui->label_options_helpexplorer->setText(fileName);
        loadHelpLanguages(fileName);
    }
}

void OptionWindow::on_buttonBox_accepted()
{
    this->Main->settings->setValue("autolanguage",(ui->check_option_autolanguage->checkState() == 2));
    if(!(ui->check_option_autolanguage->checkState() == 2)){
        this->Main->settings->setValue("languagefile",map_str2lf[ui->combo_option_language->currentText()]);
    }
    this->Main->settings->setValue("keeptempfiles",(ui->check_option_keepFiles->checkState() == 2));
    this->Main->settings->setValue("downloaddatafiles",(ui->check_option_downlaodData->checkState() == 2));
    this->Main->settings->setValue("autoCheckUpdates",(ui->check_option_autoCheckUpdates->checkState()==2));
    this->Main->settings->setValue("showsplash",(ui->check_option_splashScreen->checkState()==2));
    this->Main->settings->setValue("brhelpexplorer",ui->label_options_helpexplorer->text());
    this->Main->settings->setValue("brsearchwebprefix",ui->label_options_websearch_prefix->text());
    this->Main->settings->setValue("brsearchwebsuffix",ui->label_options_websearch_suffix->text());
    this->Main->settings->setValue("brsearchserprefix",ui->label_options_serialsearch_prefix->text());
    this->Main->settings->setValue("brsearchsersuffix",ui->label_options_serialsearch_suffix->text());
    this->Main->settings->setValue("ashelpwaittime",ui->label_options_timetowaitforhelp->text().toInt());
    this->Main->settings->setValue("hwdoubleclick",ui->combo_option_hardwaredoubleclick->currentIndex());
    this->Main->settings->setValue("hwsearchfor",ui->combo_option_hwSearch->currentIndex());
    this->Main->settings->setValue("ignoreSSLerrors",(ui->check_option_ignoresslerrors->checkState()==2));

    if(ui->combo_option_helplanguage->currentText() == "en" || ui->combo_option_helplanguage->currentText() == "de"){
        this->Main->settings->setValue("brhelplang",ui->combo_option_helplanguage->currentText());
    }


    if(static_cast<unsigned int>(ui->combo_option_themes->currentIndex()) < this->Main->vColorThemes.size())
        this->Main->settings->setValue("themefile",this->Main->vColorThemes[static_cast<unsigned int>(ui->combo_option_themes->currentIndex())].filename);

    // Language update
    //qDebug() << this->lang_old << " - " <<this->Main->settings->value("languagefile").toString();
    if(((this->lang_old != this->Main->settings->value("languagefile").toString()) && !(ui->check_option_autolanguage->checkState()==2)) || (this->checkAutoLang_old != ui->check_option_autolanguage->checkState()))
        this->Main->updateLanguage(1);

    // Color Theme update
    if(static_cast<unsigned int>(ui->combo_option_themes->currentIndex()) < this->Main->vColorThemes.size()){
        if(this->theme_old != this->Main->vColorThemes[static_cast<unsigned int>(ui->combo_option_themes->currentIndex())].filename)
            this->Main->updateColorTheme(ui->combo_option_themes->currentIndex());
    }

    this->Main->settings->sync();

    this->Main->statusbar->showMessage(tr("Options saved!"),DEFAULT_MESSAGE_TIME);
}

void OptionWindow::on_check_option_autolanguage_toggled(bool checked)
{

    QString langIcon;

    if(checked){
        ui->combo_option_language->clear();
        ui->combo_option_language->setDisabled(1);
    } else {
        ui->combo_option_language->setDisabled(0);

        // fill combobox
        for(int i=0;i<this->foundLanguages.count();i++){
            ui->combo_option_language->addItem(this->foundLanguages.at(i));

            langIcon = "://images/flag_" + this->map_str2lf[this->foundLanguages.at(i)].mid(5,2) + ".png";
            ui->combo_option_language->setItemIcon(i,QPixmap(langIcon));

            if(map_str2lf[ui->combo_option_language->itemText(i)] == this->Main->settings->value("languagefile").toString())
                ui->combo_option_language->setCurrentIndex(i);
        }
    }

}
