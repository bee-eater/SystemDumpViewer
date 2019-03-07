#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

#define UNUSED(expr) do { (void)(expr); } while (0)

/*----------------------------------
 *
 * GUI GUI CONSTANTS
 *
 * -------------------------------*/

#define AXIS_LABEL_HEIGHT           25
#define AXIS_LABEL_HEIGHT_BIG       33
#define AXIS_LABEL_HEIGHT_SMALL     19
#define AXIS_LABEL_WIDTH            111
#define AXIS_TEXT_LENGTH            11
#define AXISERR_TABLE_COLUMNS       4

#define CPU_MODE_RUN                4

#define DEFAULT_WINDOW_WIDTH        708
#define DEFAULT_WINDOW_HEIGHT       555
#define DEFAULT_ASHELP_WAITTIME     1000
#define DEFAULT_MESSAGE_TIME        2000

#define HW_PLUGGED                  0
#define HW_CONFIGURED               1

#define PARTITION_LABEL_HEIGHT      16
#define PARTITION_LABEL_WIDTH       111
#define PROGRESSBAR_WIDTH           250

#define LOGGER_TABLE_COLUMNS        10
#define LOGGER_V2_TABLE_COLUMNS     11
#define LOGGER_TABLE_TIMEDATE       1
#define LOGGER_TABLE_LEVELBMP       0
#define LOGGER_TABLE_LEVEL          7

#define LOGGER_IMAGE_COLUMN         0
#define LOGGER_DATEX_COLUMN         1
#define LOGGER_ERRNO_COLUMN         2
#define LOGGER_LOGGR_COLUMN         3
#define LOGGER_EVENT_COLUMN         4
#define LOGGER_OBJCT_COLUMN         5
#define LOGGER_OSTSK_COLUMN         6
#define LOGGER_DESCR_COLUMN         7
#define LOGGER_ASCII_COLUMN         8
#define LOGGER_LEVEL_COLUMN         9

#define LOGGER_V2_IMAGE_COLUMN      0
#define LOGGER_V2_DATEX_COLUMN      1
#define LOGGER_V2_FACIL_COLUMN      2
#define LOGGER_V2_LOGGR_COLUMN      3
#define LOGGER_V2_EVTID_COLUMN      4
#define LOGGER_V2_ENTBY_COLUMN      5
#define LOGGER_V2_DESCR_COLUMN      6
#define LOGGER_V2_ASCII_COLUMN      7
#define LOGGER_V2_BINAR_COLUMN      8
#define LOGGER_V2_INFOS_COLUMN      9
#define LOGGER_V2_ORIGN_COLUMN      10

#define PROFILER_TABLE_COLUMNS      4

#define REPORT_TYPE_HARDWARE        0
#define REPORT_TYPE_SOFTWARE        1

#define SOFTWARE_TABLE_COLUMNS      7
#define STARTPAGE_LABEL_WIDTH       250
#define STARTPAGE_LABEL_HEIGHT      30


#define TABLE_ROW_HEIGHT            25
#define TASK_CLASS_TABLE_COLUMNS    7

/*----------------------------------
 *
 * GUI NETWORK CONSTANTS
 *
 * -------------------------------*/
#define NET_TIMEOUT 5000

/*----------------------------------
 *
 * OTHER CONSTANTS
 *
 * -------------------------------*/
#define BYTE_PER_GB 1073741824
#define BYTE_PER_MB 1048576
#define BYTE_PER_KB 1024


/*----------------------------------
 *
 * Qt - INCLUDES
 *
 * -------------------------------*/
// GUI AND ANIMATION
#include <QCheckBox>
#include <QGraphicsColorizeEffect>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QProgressBar>
#include <QMainWindow>
#include <QMovie>
#include <QScrollArea>
#include <QSettings>
#include <QStringList>
#include <QTranslator>
#include <QTreeWidget>

// FILES AND NETWORKING
#include <QEventLoop>
#include <QFileSystemModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>


/*----------------------------------
 *
 * 3rd - PARTY - INCLUDES
 *
 * -------------------------------*/
#include <winsock2.h>
#include <windows.h>
#include "qwt_plot.h"
#include "qwt_plot_picker.h"
#include <array>
#include <map>
#include <string>

#include "systemdump.h"
#include "./includes/rapidxml_utils.hpp"
#include "./includes/rapidxml_print.hpp"

using namespace rapidxml;


namespace Ui {
    class MainWindow;
}

/**
 * @brief The MainWindow class is the main class of this application and derives from QMainWindow
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    // GLOBAL MAIN WINDOW
    explicit MainWindow(QWidget *parent = 0);   /**< Constructor */
    ~MainWindow();  /**< Destructor */

    static const QString Version;               /**< String that contains the version information of the application */
    QStatusBar *statusbar;                      /**< MainWindow statusbar */


    // COLOR SCHEME AND LANGUAGE
    void updateLanguage(bool update=0);         /**< Updates the UI Langauge */
    void updateColorTheme(int themeIndex);      /**< Updates the color sheme of the UI */
    QColor QStringToQColor(QString rgbString);  /**< Converts "128,128,128" to a QColor */

    std::vector<sColorTheme> vColorThemes;      /**< Vector holding all the color themes found in the theme folder, extension .sdtm */
    QString themePath;
    QGraphicsColorizeEffect optionBgEffect;     /**< ColorEffect for the OptionBackground */
    QGraphicsColorizeEffect reportBgEffect;
    QStringList languageFiles;                  /**< StringList with all available Language files */


    // READ AND DISPLAY XML
    bool readXML(const char* filename, bool updateRecentFileNameList = true );         /**< Function that reads the xml file */
    bool displayValues();                       /**< Function that sends / writes / arranges etc. the read stuff on the window */    

    // DUMP AND INFORMATION STORAGE
    sSystemDump SysDump;                        /**< Variable to store complete SystemDump information in. All values from the read XML-File are going in here! */
    sStyles Styles;                             /**< CSS stylesheets for internal use */
    QSettings *settings;                         /**< Storage for program settings */
    int ErrorNr;                                /**< Error number for bug reporting and debugging purposes */
    QString Error;                              /**< String for attribute error output */

    // PRINT PDF REPORT
    bool createPDFReport(int type,QString fileName,QString date);     /**< Create a PDF report from the actual loaded Systemdump.xml file ... */

    // "CONSTANT" STRINGS
    QStringList list_CpuModes;                  /**< Contains the texts to the numbers from XML, e.g. CPU mode 4 = RUN etc ... */
    QStringList list_BatteryStatus;             /**< Contains the texts to the numbers from XML, e.g. CPU mode 4 = RUN etc ... */
    QStringList list_RebootMode;                /**< Contains the texts to the numbers from XML, e.g. CPU mode 4 = RUN etc ... */
    QStringList list_RebootCF;                  /**< Contains the texts to the numbers from XML, e.g. CPU mode 4 = RUN etc ... */
    QStringList list_IpConfiguration;           /**< Contains the texts to the numbers from XML, e.g. CPU mode 4 = RUN etc ... */
    QStringList list_SNMPConfiguration;         /**< Contains the texts to the numbers from XML, e.g. CPU mode 4 = RUN etc ... */
    QStringList list_ModuleOk;                  /**< Contains the texts to the numbers from XML, e.g. CPU mode 4 = RUN etc ... */
    QStringList list_ChannelType;               /**< Contains the texts to the numbers from XML, e.g. CPU mode 4 = RUN etc ... */


    // CONFIGURATION
    QStringList HwDoubleClickActions;           /**< Contains the possible Actions on a double click on a hw module */
    QStringList HwDoubleClickActionsDrop;       /**< Contains the same information as HwDoubleClickActions but without ampersands for shortcuts ... */


    // MAPS FOR SWITCH CASE STATEMENTS IN readXML FUNCTION
    // VALUES SET IN init_Maps()
    enum en_SystemDump { SYSDUMP_ERR, SYSDUMP_DUMP};
    static std::map<std::string, en_SystemDump> map_SystemDump;

    enum en_SystemDumpAttributes {SYSDUMP_000_ERR, SYSDUMP_000_GENERATOR, SYSDUMP_000_VERSION, SYSDUMP_000_TIMESTAMP};
    static std::map<std::string, en_SystemDumpAttributes> map_SystemDumpAttributes;

    enum en_Sections {SECTION_ERR, SECTION_VALUES, SECTION_ETHERNET, SECTION_MEMORY, SECTION_TIMING, SECTION_CPUUSAGE, SECTION_SOFTWARE,
                      SECTION_HARDWARE, SECTION_MOTION, SECTION_LOGGER, SECTION_PROFILER, SECTION_ADDITIONAL};
    static std::map<std::string, en_Sections> map_Sections;

    enum en_SectionValues {VALUE_000_ERR, VALUE_000_OPERATIONAL, VALUE_000_TIMESYNC, VALUE_000_SOFTWARE, VALUE_000_CPUCONF, VALUE_000_APPINFO};
    static std::map<std::string, en_SectionValues> map_000_SectionValues;

    enum en_SectionValuesOperationalValues {VALUE_001_ERR, VALUE_001_NODENUMBER, VALUE_001_CPUMODE, VALUE_001_BATTERYSTATUS, VALUE_001_CPUTEMP,
                                            VALUE_001_CPUUSAGE, VALUE_001_TARGETTIME, VALUE_001_OPHOURS, VALUE_001_POWERCYCLES};
    static std::map<std::string, en_SectionValuesOperationalValues> map_001_OperationalValues;

    enum en_SectionValuesTimeSynchronisation {VALUE_002_ERR, VALUE_002_PARAM,VALUE_002_SERVER,VALUE_002_CLIENT};
    static std::map<std::string, en_SectionValuesTimeSynchronisation> map_002_TimeSynchronisation;

    enum en_SectionValuesTimeSyncClient {VALUE_003_ERR, VALUE_003_CLIENT, VALUE_003_SERVER1, VALUE_003_SERVER2, VALUE_003_SERVER3,
                                         VALUE_003_SERVER4, VALUE_003_SNTP_SYNC_INT, VALUE_003_RTC_SYNC_INT};
    static std::map<std::string, en_SectionValuesTimeSyncClient> map_003_TimeSynchClient;

    enum en_SectionValuesSoftware {VALUE_004_ERR, VALUE_004_AR, VALUE_004_VC, VALUE_004_MC, VALUE_004_CNC};
    static std::map<std::string, en_SectionValuesSoftware> map_004_Software;

    enum en_SectionValuesCpuConf {VALUE_005_ERR, VALUE_005_PARAM, VALUE_005_REBOOT, VALUE_005_PRESERVE};
    static std::map<std::string, en_SectionValuesCpuConf> map_005_CpuConfiguration;

    enum en_SectionValuesCpuConfReboot {VALUE_006_ERR, VALUE_006_RESET, VALUE_006_PFAIL, VALUE_006_CHANGECF};
    static std::map<std::string, en_SectionValuesCpuConfReboot> map_006_CpuConfigurationReboot;

    enum en_SectionValuesCpuConfPreserve {VALUE_007_ERR, VALUE_007_CHANGECF};
    static std::map<std::string, en_SectionValuesCpuConfPreserve> map_007_CpuConfigurationPreserve;

    enum en_SectionValuesCpuConfParam {VALUE_008_ERR, VALUE_008_HOSTNAME, VALUE_008_DEFDOMAIN, VALUE_008_CPUMODESWITCH, VALUE_008_PROFILING,
                                       VALUE_008_FTP, VALUE_008_USBREMOTEINSTALL, VALUE_008_CFREMOTEINSTALL, VALUE_008_USBARWININSTALL, VALUE_008_USERPARTITIONINSTALL };
    static std::map<std::string, en_SectionValuesCpuConfParam> map_008_CpuConfigurationParam;

    enum en_SectionValuesAppInfo {VALUE_009_ERR, VALUE_009_APPLINK, VALUE_009_APPSTATUS};
    static std::map<std::string, en_SectionValuesAppInfo> map_009_ApplicationInfo;

    enum en_SectionEthernet {ETHERNET_ERR, ETHERNET_PARAM, ETHERNET_INTERFACE};
    static std::map<std::string, en_SectionEthernet> map_010_Ethernet;

    enum en_SectionEthernetPar {ETHERNET_000_ERR, ETHERNET_000_DEFGATE};
    static std::map<std::string, en_SectionEthernetPar> map_011_EthernetPar;

    enum en_SectionEthernetInterface {ETHERNET_001_ERR, ETHERNET_001_IPADRESS, ETHERNET_001_SUBNET, ETHERNET_001_IPCONF, ETHERNET_001_SNMPCONF,
                                      ETHERNET_001_ETHREMOTEINSTALL, ETHERNET_001_NETWORKINSTALL};
    static std::map<std::string, en_SectionEthernetInterface> map_012_EthernetIf;

    enum en_SectionMemory {MEMORY_ERR, MEMORY_CF, MEMORY_DRAM, MEMORY_SRAM};
    static std::map<std::string, en_SectionMemory> map_013_Memory;

    enum en_SectionMemoryCF {MEMORY_000_ERR, MEMORY_000_PARTITION};
    static std::map<std::string, en_SectionMemoryCF> map_014_MemoryCF;

    enum en_SectionMemoryCfAttributes {MEMORY_002_ERR, MEMORY_002_NUMOFPARTITIONS, MEMORY_002_NUMBEROFSECTORS, MEMORY_002_SIZE, MEMORY_002_SIZEPERSECTOR};
    static std::map<std::string, en_SectionMemoryCfAttributes> map_029_CfAttributes;

    enum en_SectionMemoryCfPartitionAttributes {MEMORY_003_ERR, MEMORY_003_AVAILABLE, MEMORY_003_DESCRIPTION, MEMORY_003_ID, MEMORY_003_SIZE, MEMORY_003_USED};
    static std::map<std::string, en_SectionMemoryCfPartitionAttributes> map_030_PartitionAttributes;

    enum en_SectionMemoryDRAMAttributes {MEMORY_004_ERR, MEMORY_004_AVAILABLE, MEMORY_004_LARGESTBLOCK, MEMORY_004_SIZE, MEMORY_004_USED};
    static std::map<std::string, en_SectionMemoryDRAMAttributes> map_031_DRAMAttributes;

    enum en_SectionMemorySRAM {MEMORY_001_ERR, MEMORY_001_NOTCONFIGURED,MEMORY_001_USERRAM, MEMORY_001_REMMEM};
    static std::map<std::string, en_SectionMemorySRAM> map_015_MemorySRAM;

    enum en_SectionMemorySRAMAttributes {MEMORY_005_ERR, MEMORY_005_SIZE};
    static std::map<std::string, en_SectionMemorySRAMAttributes> map_032_SRAMAttributes;

    enum en_SectionMemorySRAMNotConfigured {MEMORY_008_ERR, MEMORY_008_SIZE};
    static std::map<std::string, en_SectionMemorySRAMNotConfigured> map_035_NotConfigured;

    enum en_SectionMemorySRAMUSERRAMAttributes {MEMORY_006_ERR, MEMORY_006_AVAILABLE, MEMORY_006_DEVICE, MEMORY_006_LARGESTBLOCK, MEMORY_006_SIZE, MEMORY_006_USED};
    static std::map<std::string, en_SectionMemorySRAMUSERRAMAttributes> map_033_USERRAMAttributes;

    enum en_SectionMemorySRAMREMMEMAttributes {MEMORY_007_ERR, MEMORY_007_AVAILABLE, MEMORY_007_DEVICE, MEMORY_007_LARGESTBLOCK, MEMORY_007_PERMANENT, MEMORY_007_REMGLOBAL, MEMORY_007_REMLOCAL, MEMORY_007_SIZE};
    static std::map<std::string, en_SectionMemorySRAMREMMEMAttributes> map_034_REMMEMAttributes;

    enum en_SectionTiming {TIMING_ERR, TIMING_TIMERCONF, TIMING_TASKCLASSCONF};
    static std::map<std::string, en_SectionTiming> map_016_Timing;

    enum en_SectionTimingTimerConf {TIMING_001_ERR, TIMING_001_IDLETASKCLASS, TIMING_001_MULTIPLEVALUE, TIMING_001_DIVIDINGVALUE, TIMING_001_SYSTICK, TIMING_001_SYSTIMER, TIMING_001_TCIDLETIME};
    static std::map<std::string, en_SectionTimingTimerConf> map_036_TimingTimerConfiguration;

    enum en_SectionTimingTaskClass {TIMING_000_ERR, TIMING_000_TASKCLASS};
    static std::map<std::string, en_SectionTimingTaskClass> map_017_TimingTaskClass;

    enum en_SectionTimingTaskClassConfig {TIMING_002_ERR, TIMING_002_CYCLETIME, TIMING_002_CYCLIC, TIMING_002_INPUTDELAY, TIMING_002_OUTPUTDELAY, TIMING_002_STACK, TIMING_002_TOLERANCE};
    static std::map<std::string, en_SectionTimingTaskClassConfig> map_037_TimingTaskClassConfiguration;

    enum en_SectionCpuUsage {CPUUSAGE_ERR, CPUUSAGE_INTERVAL};
    static std::map<std::string, en_SectionCpuUsage> map_018_CpuUsage;

    enum en_SectionCpuUsageInterval {CPUUSAGE_000_ERR, CPUUSAGE_000_AVERAGE, CPUUSAGE_000_DESCRIPTION, CPUUSAGE_000_ID, CPUUSAGE_000_MAX};
    static std::map<std::string, en_SectionCpuUsageInterval> map_038_CpuUsageInterval;

    enum en_SectionCpuUsageValues {CPUUSAGE_001_ERR, CPUUSAGE_001_AVERAGE, CPUUSAGE_001_MAXIMUM};
    static std::map<std::string, en_SectionCpuUsageValues> map_038_CpuUsageValues;

    enum en_SectionSoftware {SOFTWARE_ERR, SOFTWARE_APPLICATIONMODULE, SOFTWARE_MODULE};
    static std::map<std::string, en_SectionSoftware> map_019_Software;

    enum en_SectionSoftwareAppModule {SOFTWARE_000_ERR, SOFTWARE_000_MODULE};
    static std::map<std::string, en_SectionSoftwareAppModule> map_020_SoftwareAppModule;

    enum en_SectionSoftwareModuleAttributes {SOFTWARE_001_ERR, SOFTWARE_001_ADRESS, SOFTWARE_001_ID, SOFTWARE_001_MEMORY, SOFTWARE_001_NAME, SOFTWARE_001_SIZE, SOFTWARE_001_TIMESTAMP, SOFTWARE_001_TYPE, SOFTWARE_001_VERSION};
    static std::map<std::string, en_SectionSoftwareModuleAttributes> map_039_ModuleAttributes;

    enum en_Hardware {HARDWARE_ERR, HARDWARE_NODE};
    static std::map<std::string, en_Hardware> map_021_Hardware;

    enum  en_HardwareNode {HARDWARE_000_ERR, HARDWARE_000_MODULESTATUS, HARDWARE_000_IOINFORMATION, HARDWARE_000_IOINFO};
    static std::map<std::string, en_HardwareNode> map_022_HardwareNode;

    enum en_HardwareNodeAttributes {HARDWARE_001_ERR, HARDWARE_001_CONFIGURED, HARDWARE_001_IOINFO, HARDWARE_001_ID, HARDWARE_001_STATUS};
    static std::map<std::string, en_HardwareNodeAttributes> map_023_HardwareNodeAttributes;

    enum en_HardwareModuleStatusAttrb {HARDWARE_002_ERR, HARDWARE_002_CONFIGURED, HARDWARE_002_MODULEOK, HARDWARE_002_PLUGGED, HARDWARE_002_SUPERVISED};
    static std::map<std::string, en_HardwareModuleStatusAttrb> map_024_HardwareModuleStatusAttributes;

    enum en_HardwareIOInformationAttrb {HARDWARE_003_ERR, HARDWARE_003_EQUIPMENTID, HARDWARE_003_FWVERSION, HARDWARE_003_HWREVISION, HARDWARE_003_HWVARIANT, HARDWARE_003_MODPATH, HARDWARE_003_SERIALNR};
    static std::map<std::string, en_HardwareIOInformationAttrb> map_025_HardwareIOInfoAttributes;

    enum en_HardwareNodeIoInfo {HARDWARE_004_ERR, HARDWARE_004_CHANNEL};
    static std::map<std::string, en_HardwareNodeIoInfo> map_026_HardwareNodeIoInfo;

    enum en_HardwareChannelAttributes {HARDWARE_005_ERR, HARDWARE_005_DIAGNOSE, HARDWARE_005_FORCESTATUS, HARDWARE_005_FORCEVALUE, HARDWARE_005_IECTYPE, HARDWARE_005_IECVALUE, HARDWARE_005_ID, HARDWARE_005_NAME, HARDWARE_005_PHYSICALVALUE, HARDWARE_005_TYPE};
    static std::map<std::string, en_HardwareChannelAttributes> map_027_HardwareChannelAttributes;

    enum en_SectionMotion{MOTION_000_ERR, MOTION_000_MOTION};
    static std::map<std::string, en_SectionMotion> map_040_Motion;

    enum en_SectionMotionAttributes{MOTION_004_ERR, MOTION_004_AVAILABLE};
    static std::map<std::string, en_SectionMotionAttributes> map_041_MotionAttributes;

    enum en_SectionMotionNodes {MOTION_ERR, MOTION_AXIS, MOTION_ALARM};
    static std::map<std::string, en_SectionMotionNodes> map_042_MotionNodes;

    enum en_MotionAxisAttribute {MOTION_001_ERR, MOTION_001_ACTPOS, MOTION_001_ACTSPEED, MOTION_001_ADRESS, MOTION_001_ALARM, MOTION_001_CONTROLLER, MOTION_001_DRIVE, MOTION_001_ENABLE, MOTION_001_HOMING, MOTION_001_ID, MOTION_001_MOVEMENTSTATUS, MOTION_001_NAME, MOTION_001_NEGENDSW, MOTION_001_POSENDSW, MOTION_001_REFSW, MOTION_001_SIMULATION, MOTION_001_TRIGGER1, MOTION_001_TRIGGER2};
    static std::map<std::string, en_MotionAxisAttribute> map_045_MotionAxisAttribute;

    enum en_MotionAlarm {MOTION_003_ERR, MOTION_003_ERRORENTRY};
    static std::map<std::string, en_MotionAlarm> map_043_MotionAlarm;

    enum en_MotionAlarmAttributes {MOTION_002_ERR, MOTION_002_ERRORNO, MOTION_002_ERRORTEXT, MOTION_002_ID, MOTION_002_PARID, MOTION_002_STATUS, MOTION_002_TEXTINFO, MOTION_002_TEXTPARID, MOTION_002_TIMESTAMP};
    static std::map<std::string, en_MotionAlarmAttributes> map_046_MotionAlarmAttributes;

    enum en_Logger {LOGGER_ERR, LOGGER_MODULE};
    static std::map<std::string, en_Logger> map_050_Logger;

    enum en_LoggerModuleAttributes {LOGGER_000_ERR, LOGGER_000_ADRESS, LOGGER_000_ID, LOGGER_000_NAME, LOGGER_000_VERSION};
    static std::map<std::string, en_LoggerModuleAttributes> map_051_LoggerModuleAttributes;

    enum en_LoggerModuleEntry {LOGGER_001_ERR, LOGGER_001_ENTRY};
    static std::map<std::string, en_LoggerModuleEntry> map_052_LoggerModuleEntry;

    enum en_LoggerModuleEntryAttributes {LOGGER_002_ERR, LOGGER_002_ASCIIDATA, LOGGER_002_BINARYDATA, LOGGER_002_ERRDESCRIPTION, LOGGER_002_ERRORNR, LOGGER_002_EVENTID, LOGGER_002_OBJECTID, LOGGER_002_ID, LOGGER_002_LEVEL, LOGGER_002_OSTASK, LOGGER_002_TIMESTAMP};
    static std::map<std::string, en_LoggerModuleEntryAttributes> map_053_LoggerModuleEntryAttributes;

    enum en_LoggerModuleEntryAttributesV2 {LOGGER_202_ERR, LOGGER_202_ID, LOGGER_202_SEVERITY, LOGGER_202_FACILITY, LOGGER_202_TIMESTAMP, LOGGER_202_INFO, LOGGER_202_EVENTID, LOGGER_202_ORIGINRECORDID, LOGGER_202_ENTEREDBY, LOGGER_202_DESCRIPTION, LOGGER_202_ASCIIDATA, LOGGER_202_BINARYDATA};
    static std::map<std::string, en_LoggerModuleEntryAttributesV2> map_054_LoggerModuleEntryAttributesV2;

    enum en_Profiler {PROFILER_ERR, PROFILER_MODULE};
    static std::map<std::string, en_Profiler> map_060_Profiler;

    enum en_ProfilerAttributes {PROFILER_000_ERR, PROFILER_000_ID, PROFILER_000_NAME, PROFILER_000_SIZE, PROFILER_000_TIMESTAMP};
    static std::map<std::string, en_ProfilerAttributes> map_061_ProfilerAttributes;

private slots:
    // GUI SLOTS
    void on_actionExit_triggered();         /**< Slot for menu button */
    void on_actionLoad_xml_triggered();     /**< Slot for menu button */
    void on_actionAbout_triggered();        /**< Slot for menu button */
    void on_actionHelp_triggered();         /**< Slot for menu button */
    void on_actionOptions_triggered();      /**< Slot for menu button */
    void on_actionClose_xml_triggered();    /**< Slot for menu button */
    void on_actionLoad_from_PLC_triggered();/**< Slot for menu button */
    void on_actionSave_position_triggered();/**< Slot for menu button */
    void on_actionSave_xml_as_triggered();  /**< Slot for menu button */
    void on_actionSave_all_as_triggered();  /**< Slot for menu button */

    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent* event);

    // MANUAL GUI SLOTS
    void on_combo_SelectMenu_currentIndexChanged(int index);        /**< Slot for catching change of main section combobox */
    void on_combo_softwareFilter_currentIndexChanged(int index);    /**< Slot for catching change of software filter combobox */
    void on_combo_CpuUsage_currentIndexChanged();                   /**< Slot for catching change of cpu usage combobox */
    void on_combo_AxisError_currentIndexChanged();                  /**< Slot for catching change of axis error list combobox */
    void on_tree_ApplicationModules_itemClicked(QTreeWidgetItem *item, int column); /**< Slot for catching click on an application module */
    void on_tree_Hardware_Channels_selectionChanged();
    void on_tree_Hardware_itemDoubleClicked();
    void on_tree_Hardware_customContextMenuRequested(const QPoint &pos);
    void on_tree_Hardware_custom_openOnHomepage();
    void on_tree_Hardware_custom_openSerialOnHomepage();
    void on_tree_Hardware_custom_openInHelp();
    void on_tree_Hardware_custom_copySerial();
    void on_tree_Hardware_selectionChanged();
    void on_logger_checkbox_toggled();
    void on_logger_section_clicked(int cidx);
    void on_uiLabelClicked(int index);
    void on_createPDFReport_triggered();

    // RECENT FILES
    void on_recentFiles_clearList();
    void on_recen_openFile();

    // DOWNLAODED FILES
    void on_downloaded_removeFiles();
    void on_downloaded_openFile();

    // DATA OBJECTS
    void on_download_dataObject();
    void dataObjectsDisable();

    // GLOBAL SHORTCUTS
    void on_logger_pressed_f1();
    void on_hwtree_pressed_f1();
    void on_central_pressed_esc();

    // NETWORKING
    void on_netAccManDownload_finished(QNetworkReply *netReply);

    void on_actionCheck_for_updates_triggered();

public slots:

    // NO PUBLIC SLOTS YET

private:
    // GLOBAL MAIN WINDOW
    Ui::MainWindow *ui;

    void show_InfoBox();
    void splashScreenShow(int time);
    void setup_UI();

    void init_Maps();
    void init_Names();
    void init_Styles();

    void load_iniFile();

    void startScreenInit();
    void startScreenShow(bool enabled);
    void startScreenCenter();
    bool eventFilter(QObject *obj, QEvent *event);
    QGraphicsColorizeEffect startBgEffect;         /**< ColorEffect for the startPage */
    QLabel *start_stripe;
    QLabel *start_openxml;
    QLabel *start_textopenxml;
    QLabel *start_openplc;
    QLabel *start_textopenplc;
    QLabel *start_sdvLogo;
    QLabel *start_brLogo;
    QGraphicsScene *scn;
    QGraphicsView *startBg;

    // PATH OF CURRENT OPENED SYSTEMDUMP
    QString currentFileString;


    // READING AND PAINTING THE SYSDUMP FILE
    bool get_SystemDumpMainInfo(xml_document<> *doc);
    int get_SystemDumpSections(xml_document<> *doc);

    int add_EthernetTab(int index);
    int add_Hardware();
    int add_Hardware_Item(unsigned int *index, string *path, QTreeWidgetItem *parent);
    int add_Hardware_Item(unsigned int *index, string *path, QTreeWidget *parent);
    int add_Hardware_Channel(int HwNode, unsigned int *index);
    int add_PartitionBar(int x, int y, int index);
    int add_ApplicationModule(unsigned int index);
    int add_TaskClass(unsigned int index);
    int add_LoggerModules();
    int add_ProfilerModules();
    int add_AxisToUi(int x, int index);
    int add_AxisErrorsToUi();

    int get_HardwareInformation(int uuid);
    int get_HardwareChannelInformation(int HwNode, int uuid);
    int get_SoftwareModules(int uuid, int filter);
    int get_CpuUsageValues(vector<int>* values, char* cValues);
    int get_LoggerModules();
    int get_StringsForIdAndValue(xml_node<> *pItem, string * const &tmpString, string * const &tmpString2);

    int draw_CpuUsage();
    int draw_Overview();

    bool loggerSortOrder;
    int loggerClickedColumnOld;

    // RESETTING THE UI AND
    void reset_DumpInfo();
    void reset_UI();

    // RECENT FILES AND RECENT IP ADDRESSES
    enum {MaxRecentFiles = 5};
    std::vector<QAction *> recentFilesActions;
    void recentUpdateFileActions();
    void recentSetCurrentFile(const QString &fileName);
    void recentLoadFile(const QString &fileName);

    enum {MaxRecentIpAddresses = 5};
    void recentSetCurrentIpAddress(const QString &ipAddress);
    QStringList recentIpAddresses;

    // EXPORT FILES
    enum {MaxDataObj = 10};
    QAction *exportFilesActs[MaxDataObj];

    // DOWNLOADED FILES
    std::vector<QAction *> vDownloadedFiles;
    void downloadedUpdateList();

    // DATAOBJECTS
    std::vector<QAction *> vDataObjects;
    void dataObjectsUpdateList();
    void dataSaveAllEnable(bool enable);

    // LANGUAGE TRANSLATOR
    QTranslator mTranslator;
    void translateQStringLists();       /**< StringList initialization, is called when language changes in order to get the lists translated! */


    // PROGRESS AND STATUSBAR
    int progressMax(){return 100;}
    int progressMin(){return 0;}
    void emitProgress(int progress){emit signalProgress(progress);}
    QProgressBar progressBar;

    QLabel statusbarLabel;
    void loadingGifEnabled(bool enabled);
    QMovie *movLoadingGif;
    QLabel *lblLoadingGif;

    QScrollArea *axisScrollArea;
    QFileSystemModel* hwTreeModel;

    std::vector<QCheckBox*> vLogger_Checkbox;       /**< Holds the checked boxes from Logger view */
    std::vector<sLogModuleEntry> vLogSortView;      /**< Holds all Entries from all selected modules from Logger Table */
    std::vector<sLogModuleEntryV2> vLogSortViewV2;  /**< Holds all new Entries from all selected modules from Logger Table (v1.02.0 and newer) */
    std::vector<QStringList> vHardwareErrorPaths;   /**< Stores all paths where an Error occured */

    // ACCESSING B&R HELP
    static HWND hwndBrHelpExp;
    static HWND hwndComboBox;
    static DWORD HelpProcessId;
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lparam);
    static BOOL CALLBACK EnumChildWindowsProc(HWND hwnd,LPARAM lparam);

    // NETWORKING (GET, POST AND DOWNLOAD)
    QNetworkAccessManager netAccManGET;
    QNetworkAccessManager netAccManPOST;
    QNetworkAccessManager netAccManDownload;
    QNetworkRequest netRequestGET;
    QNetworkRequest netRequestPOST;
    QNetworkRequest netRequestDownload;
    QEventLoop netEventLoopGET;
    QEventLoop netEventLoopPOST;
    QEventLoop netEventLoopDownload;
    QString netReplyGET;
    QString netReplyPOST;
    QString netReplyDownload;
    QString netError;
    QTimer netTimeout;
    bool netHttpGET(QString GET);
    bool netHttpPOST(QString ipPOST, QHttpMultiPart *httpPart);
    bool netHttpDownload(QString url, QString filePath);
    void setStatusBarEnabled(bool enabled, QString statusMessage=QString(""));
    QString ipAddress;
    bool netAccManDownloadFinished;


    // DUMP EXTRACTION
    bool extractTarGz(QString sourceFile, QString targetDir);
    bool readTarGz(QString sourceFile);
    QString extractionPath;


    // HELPER FUNCTIONS
    char* pCharToUtf8(const char* charsrc); /**< Function to convert a const char * into a QString! Usage: QString::fromUtf8(pCharToUtf8(src)) */
    bool start_Application(LPCTSTR lpApplicationName);
    int start_ApplicationQt(const QString &program, const QStringList &arguments);
    void trigger_ApplicationQt(const QString &program, const QStringList &arguments);
    static bool sort_LogSortByTimeStampRev(const sLogModuleEntry &lhs, const sLogModuleEntry  &rhs);
    static bool sort_LogSortByTimeStampRevV2(const sLogModuleEntryV2 &lhs, const sLogModuleEntryV2  &rhs);
    void openInBuRHelp(QString search);
    bool check_IsHelpOpen();


signals:
    // SIGNALS
    void signalProgress(int);   /**< Is emitted to set the progressbar value while */

};


// HELPER FUNCTIONS
void writeByteToLabel(QLabel *pLabel, long long size);


#endif // MAINWINDOW_H

