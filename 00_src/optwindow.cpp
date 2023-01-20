#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#include "optionwindow.h"
#include "ui_optionwindow.h"
#include "./includes/SimpleIni.h"

#include <QDebug>

OptionWindow::OptionWindow(QWidget *parent, MainWindow *w) :
    QDialog(parent),
    ui(new Ui::OptionWindow)
{
    // Basic stuff
    // ----------------------------------------------------->>>
        // Set fixed size for window
        //QWidget::setFixedSize(490,445);
        QString tmpString;

        // Copy pointers
        this->Main = w;

        ui->setupUi(this);

        // Background
        QGraphicsScene *scn = new QGraphicsScene(this);
        scn->addPixmap(QPixmap("://images/OptionBg.png"));
        ui->graphic_option_bg->setScene(scn);
        ui->graphic_option_bg->setGraphicsEffect(&this->Main->optionBgEffect);

        // Signals
        connect(ui->navBar_options->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(on_navBarSelectionChanged()));
        ui->navBar_options->setCurrentRow(0);
    // <<<-----------------------------------------------------


    // Preload options
    // ----------------------------------------------------->>>
        ui->label_options_helpexplorer->setText(this->Main->settings->value("brhelpexplorer").toString());
        ui->label_options_websearch_prefix->setText(this->Main->settings->value("brsearchwebprefix").toString());
        ui->label_options_websearch_suffix->setText(this->Main->settings->value("brsearchwebsuffix").toString());
        ui->label_options_serialsearch_prefix->setText(this->Main->settings->value("brsearchserprefix").toString());
        ui->label_options_serialsearch_suffix->setText(this->Main->settings->value("brsearchsersuffix").toString());
        ui->label_options_timetowaitforhelp->setText(this->Main->settings->value("ashelpwaittime").toString());
    // <<<-----------------------------------------------------


    // Data storage
    // ----------------------------------------------------->>>
        ui->check_option_splashScreen->setChecked(this->Main->settings->value("showsplash",true).toBool());
    // <<<-----------------------------------------------------


    // Update checking
    // ----------------------------------------------------->>>
        ui->check_option_autoCheckUpdates->setChecked(this->Main->settings->value("autoCheckUpdates",true).toBool());
        ui->label_options_updateserver->setText(this->Main->settings->value("updateserver").toString());
    // <<<-----------------------------------------------------

    // Load available help languages into dropdown
    // ----------------------------------------------------->>>
        loadHelpLanguages(this->Main->settings->value("brhelpexplorer").toString());
    // <<<-----------------------------------------------------

    // Load language files available
    // ----------------------------------------------------->>>
        QString langIcon;

        QStringList nameFilter("*.qm");
        QDir directory("lang/");
        w->languageFiles = directory.entryList(nameFilter);

        // Language mappings
        this->init_maps();

        // Add all found files to QStringList if known in map
        for(int i=0;i<w->languageFiles.count();i++){
            if(map_lf2str[w->languageFiles.at(i)]!=QString(""))
                tmpString = map_lf2str[w->languageFiles.at(i)];
            this->foundLanguages.append(tmpString);
        }
        ui->combo_option_language->clear();
        if(this->Main->settings->value("autolanguage",true).toBool()){
            ui->check_option_autolanguage->setChecked(1);
            ui->combo_option_language->setDisabled(1);
        } else {
            ui->check_option_autolanguage->setChecked(0);
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
    // <<<-----------------------------------------------------


    // Themes
    // ----------------------------------------------------->>>
        QStringList nameFilterTheme("*.sdtm");
        QDir directoryTheme("themes\\");

        ui->combo_option_themes->clear();
        for(int i=0;i<directoryTheme.entryList(nameFilterTheme).count();i++){
            // Open sdtm file and load values
            CSimpleIniA ini;
            ini.SetUnicode();
            QString themeFilePath = QString("themes\\") + directoryTheme.entryList(nameFilterTheme).at(i);
            ini.LoadFile(themeFilePath.toStdString().c_str());

            if(static_cast<unsigned int>(i) >= this->Main->vColorThemes.size())
                this->Main->vColorThemes.push_back(sColorTheme());

            this->Main->vColorThemes[static_cast<unsigned int>(i)].filename         = directoryTheme.entryList(nameFilterTheme).at(i);
            this->Main->vColorThemes[static_cast<unsigned int>(i)].name             = QString::fromUtf8(ini.GetValue("SDVTheme", "name", "Untitled Theme"));

            this->Main->vColorThemes[static_cast<unsigned int>(i)].cWindow          = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Window", "245,245,245")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cWindowText      = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_WindowText", "25,25,25")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cBase            = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Base", "235,235,235")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cAlternateBase   = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_AlternateBase", "245,245,245")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cToolTipBase     = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_ToolTipBase", "235,235,235")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cToolTipText     = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_ToolTipText", "25,25,25")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cText            = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Text", "25,25,25")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cButton          = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Button", "245,245,245")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cButtonText      = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_ButtonText", "25,25,25")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cBrightText      = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_BrightText", "255,0,0")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cLink            = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Link", "255,128,0")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cHighlight       = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_Highlight", "255,128,0")));
            this->Main->vColorThemes[static_cast<unsigned int>(i)].cHighlightedText = this->Main->QStringToQColor(QString::fromUtf8(ini.GetValue("SDVTheme", "col_HighlightedText", "235,235,235")));

            ui->combo_option_themes->addItem(this->Main->vColorThemes[static_cast<unsigned int>(i)].name);

            if(this->Main->vColorThemes[static_cast<unsigned int>(i)].filename==this->Main->settings->value("themefile").toString())
                ui->combo_option_themes->setCurrentIndex(i);
        }
    // <<<-----------------------------------------------------


    // Hardware doubleclick
    // ----------------------------------------------------->>>
        ui->combo_option_hardwaredoubleclick->addItems(w->HwDoubleClickActionsDrop);
        if(this->Main->settings->value("hwdoubleclick",0).toInt() < ui->combo_option_hardwaredoubleclick->count())
            ui->combo_option_hardwaredoubleclick->setCurrentIndex(this->Main->settings->value("hwdoubleclick",0).toInt());

        if(this->Main->settings->value("hwsearchfor",0).toInt() == HW_PLUGGED){
            ui->combo_option_hwSearch->setCurrentIndex(HW_PLUGGED);
        } else {
            ui->combo_option_hwSearch->setCurrentIndex(HW_CONFIGURED);
        }
    // <<<-----------------------------------------------------

    // Data storage
    // ----------------------------------------------------->>>
        if(this->Main->settings->value("keeptempfiles",true).toBool()){
            ui->check_option_keepFiles->setChecked(1);
        }
        if(this->Main->settings->value("downloaddatafiles",false).toBool()){
            ui->check_option_downlaodData->setChecked(1);
        }
    // <<<-----------------------------------------------------


    // Old
        this->lang_old = this->Main->settings->value("languagefile").toString();
        this->theme_old = this->Main->settings->value("themefile").toString();
        this->checkAutoLang_old = ui->check_option_autolanguage->checkState();

}

void OptionWindow::on_navBarSelectionChanged(){
    if(ui->navBar_options->currentRow() < ui->stacked_optionPages->count())
        ui->stacked_optionPages->setCurrentIndex(ui->navBar_options->currentRow());
}

OptionWindow::~OptionWindow()
{
    delete ui;
}

void OptionWindow::loadHelpLanguages(QString helpExplorer){

    QString helpLangIcon;
    QStringList helpLanguages = this->Main->getInstalledHelpLanguages(helpExplorer);
    if(helpLanguages.count() == 0){
        ui->combo_option_helplanguage->clear();
        ui->combo_option_helplanguage->addItem("--");
        ui->combo_option_helplanguage->setToolTip(tr("No installed help was found in the help path provided! Please check the path above!"));
        ui->combo_option_helplanguage->setDisabled(true);
    } else {
        ui->combo_option_helplanguage->clear();
        ui->combo_option_helplanguage->setDisabled(false);
        ui->combo_option_helplanguage->setToolTip(QString());
        for(int i=0;i<helpLanguages.count();i++){
            ui->combo_option_helplanguage->addItem(helpLanguages.at(i));
            helpLangIcon = "://images/flag_" + helpLanguages.at(i) + ".png";
            ui->combo_option_helplanguage->setItemIcon(i,QPixmap(helpLangIcon));
            if(ui->combo_option_helplanguage->itemText(i) == this->Main->settings->value("brhelplang").toString())
                ui->combo_option_helplanguage->setCurrentIndex(i);
        }
    }

}
