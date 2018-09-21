#ifndef SYSTEMDUMP_H
#define SYSTEMDUMP_H

#include <QMainWindow>
#include <QTranslator>
#include <QString>
#include <stdio.h>
#include <vector>

using namespace std;

// Values
// --------------------------------
    /**
     * @brief The sOperationalValues struct
     */
    struct sOperationalValues{
        QString NodeNumber;
        qint16 CurrentCpuMode;
        qint16 BatteryStatus;
        qint16 CPUtemperature;
        qint16 CurrentCpuUsage;
        QString TargetTime;
        qint32 OperatingHours;
        qint32 PowerOnCycles;
    };

    /**
     * @brief The sSNTPClient struct
     */
    struct sSNTPClient{
        QString SNTPclient;
        QString SNTPserver1;
        QString SNTPserver2;
        QString SNTPserver3;
        QString SNTPserver4;
        qint16 SNTPsynchInterval;
        qint16 RTCsynchInterval;
    };

    /**
     * @brief The sSNTPServer struct
     */
    struct sSNTPServer{
        QString SNTPserver;
    };

    /**
     * @brief The sTimeSynchronisation struct
     */
    struct sTimeSynchronisation{
        QString TimeZone;
        sSNTPServer Server;
        sSNTPClient Client;
    };

    /**
     * @brief The sSoftwareVersions struct
     */
    struct sSoftwareVersions{
        QString AR;
        QString VC;
        QString MC;
        QString CNC;
    };

    /**
     * @brief The sRebootMode struct
     */
    struct sRebootMode{
        qint16 AfterReset;
        qint16 AfterPowerFail;
        qint16 AfterCFChange;
    };

    /**
     * @brief The sPreservePVMemory struct
     */
    struct sPreservePVMemory{
        qint16 AfterCFChange;
    };

    /**
     * @brief The sCpuConfiguration struct
     */
    struct sCpuConfiguration{
        QString Hostname;
        QString DefaultDomain;
        QString CPUModeSwitch;
        sRebootMode RebootMode;
        sPreservePVMemory PreservePVMemory;
        bool Profiling;
        bool FTP;
        bool USBremoteInstall;
        bool CFremoteInstall;
        bool USBArwinInstall;
        bool UserPartitionInstall;
    };

    /**
     * @brief The sApplicationInfo struct
     */
    struct sApplicationInfo{
        QString ApplicationLink;
        qint16 ApplicationStatus;
    };

    /**
     * @brief The sValues struct
     */
    struct sValues{
        sOperationalValues OperationalValues;
        sTimeSynchronisation TimeSynchronisation;
        sSoftwareVersions SoftwareVersions;
        sCpuConfiguration CpuConfiguration;
        sApplicationInfo ApplicationInfo;
    };

// Ethernet
// --------------------------------
    /**
     * @brief The sInterface struct
     */
    struct sInterface{
        QString InterfaceID;
        QString IpAddress;
        QString SubnetMask;
        qint16 IpConfig;
        qint16 SNMPConfig;
        bool EthernetRemoteInstall;
        bool NetworkInstall;
        sInterface(){
                this->EthernetRemoteInstall = false;
                this->InterfaceID = QString("");
                this->IpAddress = QString("");
                this->IpConfig = 0;
                this->SNMPConfig = 0;
                this->SubnetMask = QString("");
        }
    };

    /**
     * @brief The sEthernet struct
     */
    struct sEthernet{
        QString DefaultGateway;
        std::vector<sInterface> vInterface;
    };

// Memory
// --------------------------------
    /**
     * @brief The sPartition struct
     */
    struct sPartition{
        QString description;
        QString id;
        qint32 size;
        qint32 available;
        qint32 used;
        sPartition(){
            this->available = 0;
            this->size = 0;
            this->used = 0;
            this->id = QString("");
            this->description = QString("");
        }
    };

    /**
     * @brief The sCompactFlash struct
     */
    struct sCompactFlash{
        qint16 NumberOfPartitions;
        qint32 NumberOfSectors;
        qint32 Size;
        qint32 SizePerSector;
        std::vector<sPartition> vPartition;
    };

    /**
     * @brief The sUSERRAM struct
     */
    struct sUSERRAM{
        qint32 size;
        qint32 available;
        qint32 used;
        qint32 LargestAvailableBlock;
        QString device;
    };

    /**
     * @brief The sREMMEM struct
     */
    struct sREMMEM{
        qint32 size;
        qint32 available;
        qint32 LargestAvailableBlock;
        qint32 permanent;
        qint32 remGlobal;
        qint32 remLocal;
        QString device;
    };

    /**
     * @brief The sDRAM struct
     */
    struct sDRAM{
        qint32 size;
        qint32 available;
        qint32 used;
        qint32 LargestAvailableBlock;
    };

    /**
     * @brief The sSRAM struct
     */
    struct sSRAM{
        qint32 size;
        qint32 notConfigured;
        sUSERRAM USERRAM;
        sREMMEM REMMEM;
    };

    /**
     * @brief The sMemory struct
     */
    struct sMemory{
        sCompactFlash CompactFlash;
        sDRAM DRAM;
        sSRAM SRAM;
    };

// Timing
// --------------------------------
    /**
     * @brief The sTimerConfig struct
     */
    struct sTimerConfig{
        QString SystemTimer;
        qint16 IdleTaskClass;
        qint16 MultipleValueOfSTC;
        qint16 systemTick;
        qint16 TaskClassIdleTime;
    };

    /**
     * @brief The sTaskClass struct
     */
    struct sTaskClass{
        qint16 Cyclic;
        qint32 CycleTime;
        QString InputDelay;
        QString OutputDelay;
        qint16 Stack;
        qint32 Tolerance;
        sTaskClass(){
            this->CycleTime = 0;
            this->Cyclic = 0;
            this->InputDelay = QString("");
            this->OutputDelay = QString("");
            this->Stack = 0;
            this->Tolerance = 0;
        }
    };

    /**
     * @brief The sTiming struct
     */
    struct sTiming{
        sTimerConfig TimerConfig;
        std::vector<sTaskClass> vTaskClass;
    };

// CPU Usage
// --------------------------------
    /**
     * @brief The sUsageValues struct
     */
    struct sUsageValues{
        std::vector<int> vAverage;
        std::vector<int> vMaximum;
            sUsageValues(){
                this->vAverage.swap(this->vAverage);
                this->vMaximum.swap(this->vMaximum);
            }
    };

    /**
     * @brief The sZoomInterval struct
     */
    struct sZoomInterval{
        QString description;
        qint16 id;
        qint16 average;
        qint16 maximum;
        sUsageValues values;
        sZoomInterval(){
            this->average = 0;
            this->average = 0;
            this->maximum = 0;
        }
    };

    /**
     * @brief The sCpuUsage struct
     */
    struct sCpuUsage{
        std::vector<sZoomInterval> vZoomInterval;
    };

// Software
// --------------------------------
    /**
     * @brief The sSoftwModule struct
     */
    struct sSoftwModule{
        qint16 id;
        qint32 size;
        qint16 type;
        QString name;
        QString timestamp;
        QString version;
        QString address;
        QString memory;
        sSoftwModule(){
            this->address = QString("");
            this->id = 0;
            this->memory = QString("");
            this->name = QString("");
            this->size = 0;
            this->timestamp = QString("");
            this->type = 0;
            this->version = QString("");
        }
    };

    /**
     * @brief The sAppModule struct
     */
    struct sAppModule{
        QString id;
        std::vector<sSoftwModule> vModules;
        sAppModule(){
            this->id = QString("");
            std::vector<sSoftwModule>().swap(this->vModules);
        }
    };

    /**
     * @brief The sSoftware struct
     */
    struct sSoftware{
        std::vector<sAppModule> vAppModule;
    };

// Hardware
// --------------------------------
    /**
     * @brief The sModuleStatus struct
     */
    struct sModuleStatus{
        QString Configured; /**< Configured module name */
        QString Plugged;    /**< Plugged module name */
        qint16 Supervised;  /**< Module is supervised */
        qint16 ModuleOk;    /**< ModuleOk flag */
    };

    /**
     * @brief The sIOInformation struct shows the basic IO data of a hardware moudle
     */
    struct sIOInformation{
        QString EquipmentID;
        QString FwVersion;
        QString HardwareRevision;
        QString HardwareVariant;
        QString ModulePath;
        QString Serialnumber;
    };

    /**
     * @brief The sChannel struct holds all attributes of a channel on a hardware module
     */
    struct sChannel{
        QString Name;
        QString Diagnose;
        QString ForceStatus;
        QString ForceValue;
        QString IECType;
        QString IECValue;
        QString PhysicalValue;
        qint16 Type;
        qint16 ID;
        sChannel(){
            this->Diagnose = QString("");
            this->ForceStatus = QString("");
            this->ForceValue = QString("");
            this->ID = 0;
            this->IECType = QString("");
            this->IECValue = QString("");
            this->Name = QString("");
            this->PhysicalValue = QString("");
            this->Type = 0;
        }
    };

    /**
     * @brief The sIOInfo struct contains the vector for the channels on the hardware module
     */
    struct sIOInfo{
        std::vector<sChannel> vChannel; /**< Vector for channels on hardware module */
    };

    /**
     * @brief The sNode struct contains the attributes of a hardwarenode
     */
    struct sNode{
        QString Configured;     /**< Configured module name */
        qint16 IOinfo;
        qint16 ID;
        qint16 Status;
        sModuleStatus ModuleStatus;
        sIOInformation IOInformation;
        sIOInfo IOChannels;
        sNode(){
            this->Configured = QString("");
            this->ID = 0;
            std::vector<sChannel>().swap(this->IOChannels.vChannel);
            this->IOinfo = 0;
            this->IOInformation.EquipmentID = QString("");
            this->IOInformation.FwVersion = QString("");
            this->IOInformation.HardwareRevision = QString("");
            this->IOInformation.HardwareVariant = QString("");
            this->IOInformation.ModulePath = QString("");
            this->IOInformation.Serialnumber = QString("");
            this->ModuleStatus.Configured = QString("");
            this->ModuleStatus.ModuleOk = 0;
            this->ModuleStatus.Plugged = QString("");
            this->ModuleStatus.Supervised = 0;
        }
    };

    /**
     * @brief The sHardware struct contains the complete hardware tree, that is read from the xml
     */
    struct sHardware{
        std::vector<sNode> vNode; /**< Vector for hardware nodes */
        bool HwError = false;
    };

// Motion
// --------------------------------
    /**
     * @brief The sAlarm struct holds the attributes of an axis alarm
     */
    struct sAlarm{
        qint32 ID;          /**< Number of alarm in SystemDump */
        qint32 Status;      /**< Info ... */
        qint32 ErrorNr;     /**< B&R error number of the alarm entry */
        qint32 ParID;       /**< Info ... */
        QString ErrorText;  /**< Error Text of the alarm entry */
        QString TextInfo;   /**< Info ... */
        qint32 TextParID;   /**< Info ... */
        QString TimeStamp;  /**< Timestamp of the alarm entry */
        sAlarm(){
            this->ErrorNr = 0;
            this->ErrorText = QString("");
            this->ID = 0;
            this->ParID = 0;
            this->Status = 0;
            this->TextInfo = QString("");
            this->TextParID = 0;
            this->TimeStamp = QString("");
        }
    };

    /**
     * @brief The sAxis struct shows all attributes of the axes on the current PLC
     */
    struct sAxis{
        qint32 ActPosition;     /**< Actual position of the axis */
        qint32 ActSpeed;        /**< Actual speed of the axis */
        QString Adress;         /**< Axis address string */
        QString Drive;          /**< Name of the drive the axis is assigned to */
        QString ID;             /**< Number of the axis in the SystemDump */
        QString Name;           /**< NcMapping name of the axis */
        qint16 MovementStatus;  /**< Shows if axis is moving */
        bool NegEndSw;          /**< Digital input value of negative end switch */
        bool PosEndSw;          /**< Digital input value of positive end switch */
        bool RefSw;             /**< Digital input value of ref switch */
        bool Simulation;        /**< Shows if drive simulation is active */
        bool Trigger1;          /**< Digital input value of trigger 1 switch */
        bool Trigger2;          /**< Digital input value of trigger 2 switch */
        bool Alarm;             /**< Shows if alarm is active on the drive */
        bool Controller;        /**< Shows controller state */
        bool Enable;            /**< Digital input value of enalbe */
        bool Homing;            /**< Shows if axis is homed */
        std::vector<sAlarm> vAlarm; /**< Vector for axis alarm entries */
        sAxis(){
           this->ActPosition = 0;
            this->ActSpeed = 0;
            this->Adress = QString("");
            this->Alarm = false;
            this->Controller = false;
            this->Drive = QString("");
            this->Enable = false;
            this->Homing = false;
            this->ID = QString("");
            this->MovementStatus = 0;
            this->Name = QString("");
            this->NegEndSw = false;
            this->PosEndSw = false;
            this->RefSw = false;
            this->Simulation = false;
            this->Trigger1 = false;
            this->Trigger2 = false;
            std::vector<sAlarm>().swap(this->vAlarm);
        }
    };

    /**
     * @brief Motion section of the SystemDump
     */
    struct sMotion{
        bool Available;             /**< Shows if motion is used in the current PLC */
        std::vector<sAxis> vAxis;   /**< Vector for the axes */
    };

// Logger
// --------------------------------
    /**
     * @brief Attribute of logger entry for Logger Version 1.00.0
     */
    struct sLogModuleEntry{
        qint32 ID;                  /**< Number of entry in SystemDump */
        qint16 Level;               /**< Level shows if it is information, error or warning */
        qint32 ErrorNr;             /**< B&R error number */
        QString ASCIIData;          /**< Additional error information */
        QString BinaryData;         /**< More additional error information */
        QString ErrorDescription;   /**< Textual error description */
        QString OSTask;             /**< Source task of the error entry */
        QString TimeStamp;          /**< Timestamp of the error entry */
        QString Logger;             /**< Name of the logger that contains the entry. This parameter does NOT come from the XML, it is set manualy to ease the handling in the table view! */
        sLogModuleEntry(){
            this->ASCIIData = QString("");
            this->BinaryData = QString("");
            this->ErrorDescription = QString("");
            this->ErrorNr = 0;
            this->ID = 0;
            this->Level = 0;
            this->OSTask = QString("");
            this->TimeStamp = QString("");
            this->Logger = QString("");
        }
    };

    /**
     * @brief Attribute of logger entry for Logger Version 1.02.0 or later
     */
    struct sLogModuleEntryV2{
        qint32 ID;                  /**< Number of entry in SystemDump */
        qint32 Severity;            /**< Severity shows if it is information, error or warning */
        qint32 Facility;            /**< Facility of error */
        qint32 OriginRecID;         /**< Origin Record ID -> goTo next Error */
        QString TimeStamp;          /**< Timestamp of the error entry */
        QString Info;               /**< Some B&R Information number */
        QString EventID;            /**< B&R EventID or B&R error number */
        QString EnteredBy;          /**< B&R Module / mapp Component which entered this error */
        QString Description;        /**< Textual error description */
        QString ASCIIData;          /**< Additional error information */
        QString BinaryData;         /**< More additional error information */
        QString Logger;             /**< Name of the logger that contains the entry. This parameter does NOT come from the XML, it is set manualy to ease the handling in the table view! */
        sLogModuleEntryV2(){
            this->ASCIIData = QString("");
            this->BinaryData = QString("");
            this->Description = QString("");
            this->EventID = QString("");
            this->ID = 0;
            this->Severity = 0;
            this->Facility = 0;
            this->OriginRecID = 0;
            this->EnteredBy = QString("");
            this->TimeStamp = QString("");
            this->Logger = QString("");
            this->Info = QString("");
        }
    };

    /**
     * @brief Attributes and entries of logger modules
     */
    struct sLogModule{
        QString Name;      /**< Name of the logger modules */
        QString Adress;    /**< Memory address of the logger module */
        QString Version;   /**< Some version information */
        qint16 ID;         /**< Number of the logger module in the SystemDump */
        std::vector<sLogModuleEntry> vEntry;     /**< Vector for Logger version 1.00.0 */
        std::vector<sLogModuleEntryV2> vEntryV2; /**< Vector for Logger version 1.02.0 and later */
        sLogModule(){
            this->Adress = QString("");
            this->ID = 0;
            this->Name = QString("");
            this->Version = QString("");
            std::vector<sLogModuleEntry>().swap(this->vEntry);
            std::vector<sLogModuleEntryV2>().swap(this->vEntryV2);
        }
    };

    /**
     * @brief Vector for logger modules
     */
    struct sLogger{
        std::vector<sLogModule> vModule;
    };

// Profiler
// --------------------------------
    /**
     * @brief Attributes of profiler Module
     */
    struct sProfModule{
        qint16 ID;              /**< Number of the profiler module in the SystemDump */
        qint32 Size;            /**< Size of the profiler module */
        QString Name;           /**< Name of the profiler module */
        QString TimeStamp;      /**< Timestamp of the profiler module */
        sProfModule(){
            this->ID = 0;
            this->Name = QString("");
            this->Size = 0;
            this->TimeStamp = QString("");
        }
    };

    /**
     * @brief Vector for profiler modules
     */
    struct sProfiler{
        std::vector<sProfModule> vModule;
    };

// Sections
// --------------------------------
    /**
     * @brief This struct contains all the sections of the SystemDump.
     */
    struct sSections{
        sValues Values;         /**< Dump section "Values" */
        sEthernet Ethernet;     /**< Dump section "Ethernet" */
        sMemory Memory;         /**< Dump section "Memory" */
        sTiming Timing;         /**< Dump section "Timing" */
        sCpuUsage CpuUsage;     /**< Dump section "CpuUsage" */
        sSoftware Software;     /**< Dump section "Software" */
        sHardware Hardware;     /**< Dump section "Hardware" */
        sMotion Motion;         /**< Dump section "Motion" */
        sLogger Logger;         /**< Dump section "Logger" */
        sProfiler Profiler;     /**< Dump section "Profiler" */
    };

// System Dump
// --------------------------------
    /**
     * @brief This struct is the main struct, that holds all the basic information about the SystemDump
     *          and also gives access to the different sections.
     */
    struct sSystemDump{
        QString Generator;  /**< Information about the generator of the SystemDump. Normally it is "BuR-SDM"! */
        QString Timestamp;  /**< Timestamp when the SystemDump was generated. Format: "2014-03-04 / 16:00:56" */
        QString Version;    /**< Systemdump version. Format: "2.0" */
        sSections Sections; /**< Sections of the SystemDump */
    };
// --------------------------------
// END OF DUMP
// --------------------------------
// --------------------------------

// CSS Stylesheets
    /**
     * @brief Contains mainly the styles for the motion section!
     *          The other styles for QTreeWidget and QTableWidget etc.
     *          are staticaly declared in the Qt-Designer -> Main -> Stylesheet.
     */
    struct sStyles{
        QString progressBarStyle;       /**< Holds css-string for style purposes. */
        QString scrollAreaSyle;         /**< Holds css-string for style purposes. */
        QString progressBarChunkStyle;  /**< Holds css-string for style purposes. */
        QString axisTopStyle;           /**< Holds css-string for style purposes. */
        QString axisOrangeStyle;        /**< Holds css-string for style purposes. */
        QString axisWhiteStyle;         /**< Holds css-string for style purposes. */
        QString axisBottomStyle;        /**< Holds css-string for style purposes. */
        QString axisHeadTopStyle;       /**< Holds css-string for style purposes. */
        QString axisHeadMidStyle;       /**< Holds css-string for style purposes. */
        QString axisHeadBottomStyle;    /**< Holds css-string for style purposes. */
    };

// Configuration from config.ini
    /**
     * @brief The sConfig struct contains all the information loaded from the
     *          config.ini file!
     */
    struct sConfig{
        int     position[4];        /**< Saves the last used window position and size */
        int     autolanguage;       /**< If this is activated, the language is automatically derived from the system language (Translations needed!!) */
        int     hwdoubleclick;      /**< Action when doubleclicking on a hardware modules */
        int     keeptempfiles;      /**< Sets if downloaded files are deleted after program closes */
        QString languagefile;       /**< Filename of the current selected language */
        QString themefile;          /**< Filename of the current selected color theme */
        QString brsearchwebprefix;  /**< Prefix for searching a product the B&R hompage */
        QString brsearchwebsuffix;  /**< Suffix for searching a product the B&R hompage */
        QString brsearchserprefix;  /**< Prefix for searching a serial the B&R hompage */
        QString brsearchsersuffix;  /**< Suffix for searching a serial the B&R hompage */
        QString brhelpexplorer;     /**< Path to the B&R.Help.Explorer.exe */
        QString timeforopeninghelp; /**< The B&R Help process needs a little time to open before the form can be accessed via the handle. Because
                                            of the time can vary from computer to computer, it can be set in the option dialog */

    };

// Themes
    struct sColorTheme{
        QString name;
        QString filename;
        QColor cWindow;
        QColor cWindowText;
        QColor cBase;
        QColor cAlternateBase;
        QColor cToolTipBase;
        QColor cToolTipText;
        QColor cText;
        QColor cButton;
        QColor cButtonText;
        QColor cBrightText;
        QColor cLink;
        QColor cHighlight;
        QColor cHighlightedText;
    };

#endif
