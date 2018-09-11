#include "mainwindow.h"
#include "ui_mainwindow.h"

std::map<std::string, MainWindow::en_SystemDump> MainWindow::map_SystemDump;
std::map<std::string, MainWindow::en_SystemDumpAttributes> MainWindow::map_SystemDumpAttributes;
std::map<std::string, MainWindow::en_Sections> MainWindow::map_Sections;
std::map<std::string, MainWindow::en_SectionValues> MainWindow::map_000_SectionValues;
std::map<std::string, MainWindow::en_SectionValuesOperationalValues> MainWindow::map_001_OperationalValues;
std::map<std::string, MainWindow::en_SectionValuesTimeSynchronisation> MainWindow::map_002_TimeSynchronisation;
std::map<std::string, MainWindow::en_SectionValuesTimeSyncClient> MainWindow::map_003_TimeSynchClient;
std::map<std::string, MainWindow::en_SectionValuesSoftware> MainWindow::map_004_Software;
std::map<std::string, MainWindow::en_SectionValuesCpuConf> MainWindow::map_005_CpuConfiguration;
std::map<std::string, MainWindow::en_SectionValuesCpuConfReboot> MainWindow::map_006_CpuConfigurationReboot;
std::map<std::string, MainWindow::en_SectionValuesCpuConfPreserve> MainWindow::map_007_CpuConfigurationPreserve;
std::map<std::string, MainWindow::en_SectionValuesCpuConfParam> MainWindow::map_008_CpuConfigurationParam;
std::map<std::string, MainWindow::en_SectionValuesAppInfo> MainWindow::map_009_ApplicationInfo;
std::map<std::string, MainWindow::en_SectionEthernet> MainWindow::map_010_Ethernet;
std::map<std::string, MainWindow::en_SectionEthernetPar> MainWindow::map_011_EthernetPar;
std::map<std::string, MainWindow::en_SectionEthernetInterface> MainWindow::map_012_EthernetIf;
std::map<std::string, MainWindow::en_SectionMemory> MainWindow::map_013_Memory;
std::map<std::string, MainWindow::en_SectionMemoryCF> MainWindow::map_014_MemoryCF;
std::map<std::string, MainWindow::en_SectionMemoryCfAttributes> MainWindow::map_029_CfAttributes;
std::map<std::string, MainWindow::en_SectionMemoryCfPartitionAttributes> MainWindow::map_030_PartitionAttributes;
std::map<std::string, MainWindow::en_SectionMemoryDRAMAttributes> MainWindow::map_031_DRAMAttributes;
std::map<std::string, MainWindow::en_SectionMemorySRAM> MainWindow::map_015_MemorySRAM;
std::map<std::string, MainWindow::en_SectionMemorySRAMAttributes> MainWindow::map_032_SRAMAttributes;
std::map<std::string, MainWindow::en_SectionMemorySRAMNotConfigured> MainWindow::map_035_NotConfigured;
std::map<std::string, MainWindow::en_SectionMemorySRAMUSERRAMAttributes> MainWindow::map_033_USERRAMAttributes;
std::map<std::string, MainWindow::en_SectionMemorySRAMREMMEMAttributes> MainWindow::map_034_REMMEMAttributes;
std::map<std::string, MainWindow::en_SectionTiming> MainWindow::map_016_Timing;
std::map<std::string, MainWindow::en_SectionTimingTimerConf> MainWindow::map_036_TimingTimerConfiguration;
std::map<std::string, MainWindow::en_SectionTimingTaskClass> MainWindow::map_017_TimingTaskClass;
std::map<std::string, MainWindow::en_SectionTimingTaskClassConfig> MainWindow::map_037_TimingTaskClassConfiguration;
std::map<std::string, MainWindow::en_SectionCpuUsage> MainWindow::map_018_CpuUsage;
std::map<std::string, MainWindow::en_SectionCpuUsageInterval> MainWindow::map_038_CpuUsageInterval;
std::map<std::string, MainWindow::en_SectionCpuUsageValues> MainWindow::map_038_CpuUsageValues;
std::map<std::string, MainWindow::en_SectionSoftware> MainWindow::map_019_Software;
std::map<std::string, MainWindow::en_SectionSoftwareAppModule> MainWindow::map_020_SoftwareAppModule;
std::map<std::string, MainWindow::en_SectionSoftwareModuleAttributes> MainWindow::map_039_ModuleAttributes;
std::map<std::string, MainWindow::en_Hardware> MainWindow::map_021_Hardware;
std::map<std::string, MainWindow::en_HardwareNode> MainWindow::map_022_HardwareNode;
std::map<std::string, MainWindow::en_HardwareNodeAttributes> MainWindow::map_023_HardwareNodeAttributes;
std::map<std::string, MainWindow::en_HardwareModuleStatusAttrb> MainWindow::map_024_HardwareModuleStatusAttributes;
std::map<std::string, MainWindow::en_HardwareIOInformationAttrb> MainWindow::map_025_HardwareIOInfoAttributes;
std::map<std::string, MainWindow::en_HardwareNodeIoInfo> MainWindow::map_026_HardwareNodeIoInfo;
std::map<std::string, MainWindow::en_HardwareChannelAttributes> MainWindow::map_027_HardwareChannelAttributes;
std::map<std::string, MainWindow::en_SectionMotion> MainWindow::map_040_Motion;
std::map<std::string, MainWindow::en_SectionMotionAttributes> MainWindow::map_041_MotionAttributes;
std::map<std::string, MainWindow::en_SectionMotionNodes> MainWindow::map_042_MotionNodes;
std::map<std::string, MainWindow::en_MotionAxisAttribute> MainWindow::map_045_MotionAxisAttribute;
std::map<std::string, MainWindow::en_MotionAlarm> MainWindow::map_043_MotionAlarm;
std::map<std::string, MainWindow::en_MotionAlarmAttributes> MainWindow::map_046_MotionAlarmAttributes;
std::map<std::string, MainWindow::en_Logger> MainWindow::map_050_Logger;
std::map<std::string, MainWindow::en_LoggerModuleAttributes> MainWindow::map_051_LoggerModuleAttributes;
std::map<std::string, MainWindow::en_LoggerModuleEntry> MainWindow::map_052_LoggerModuleEntry;
std::map<std::string, MainWindow::en_LoggerModuleEntryAttributes> MainWindow::map_053_LoggerModuleEntryAttributes;
std::map<std::string, MainWindow::en_LoggerModuleEntryAttributesV2> MainWindow::map_054_LoggerModuleEntryAttributesV2;
std::map<std::string, MainWindow::en_Profiler> MainWindow::map_060_Profiler;
std::map<std::string, MainWindow::en_ProfilerAttributes> MainWindow::map_061_ProfilerAttributes;

void MainWindow::init_Maps(){
    map_SystemDump["systemdump"] = SYSDUMP_DUMP;

    map_SystemDumpAttributes["generator"] = SYSDUMP_000_GENERATOR;
    map_SystemDumpAttributes["version"] = SYSDUMP_000_VERSION;
    map_SystemDumpAttributes["timestamp"] = SYSDUMP_000_TIMESTAMP;

    map_Sections["values"] = SECTION_VALUES;
    map_Sections["ethernet"] = SECTION_ETHERNET;
    map_Sections["memory"] = SECTION_MEMORY;
    map_Sections["timing"] = SECTION_TIMING;
    map_Sections["cpuusage"] = SECTION_CPUUSAGE;
    map_Sections["software"] = SECTION_SOFTWARE;
    map_Sections["hardware"] = SECTION_HARDWARE;
    map_Sections["motion"] = SECTION_MOTION;
    map_Sections["logger"] = SECTION_LOGGER;
    map_Sections["profiler"] = SECTION_PROFILER;
    map_Sections["additional"] = SECTION_ADDITIONAL;

    map_000_SectionValues["operational_values"] = VALUE_000_OPERATIONAL;
    map_000_SectionValues["time_synchronisation"] = VALUE_000_TIMESYNC;
    map_000_SectionValues["software_versions"] = VALUE_000_SOFTWARE;
    map_000_SectionValues["cpu_configuration"] = VALUE_000_CPUCONF;
    map_000_SectionValues["application_info"] = VALUE_000_APPINFO;

    map_001_OperationalValues["node number"] = VALUE_001_NODENUMBER;
    map_001_OperationalValues["current cpu mode"] = VALUE_001_CPUMODE;
    map_001_OperationalValues["battery status"] = VALUE_001_BATTERYSTATUS;
    map_001_OperationalValues["cpu temperature"] = VALUE_001_CPUTEMP;
    map_001_OperationalValues["current cpu usage"] = VALUE_001_CPUUSAGE;
    map_001_OperationalValues["target time"] = VALUE_001_TARGETTIME;
    map_001_OperationalValues["operating hours"] = VALUE_001_OPHOURS;
    map_001_OperationalValues["power-on cycles"] = VALUE_001_POWERCYCLES;

    map_002_TimeSynchronisation["param"] = VALUE_002_PARAM;
    map_002_TimeSynchronisation["server"] = VALUE_002_SERVER;
    map_002_TimeSynchronisation["client"] = VALUE_002_CLIENT;

    map_003_TimeSynchClient["sntp client"] = VALUE_003_CLIENT;
    map_003_TimeSynchClient["sntp server 1"] = VALUE_003_SERVER1;
    map_003_TimeSynchClient["sntp server 2"] = VALUE_003_SERVER2;
    map_003_TimeSynchClient["sntp server 3"] = VALUE_003_SERVER3;
    map_003_TimeSynchClient["sntp server 4"] = VALUE_003_SERVER4;
    map_003_TimeSynchClient["sntp synchronisation interval"] = VALUE_003_SNTP_SYNC_INT;
    map_003_TimeSynchClient["rtc synchronisation interval"] = VALUE_003_RTC_SYNC_INT;

    map_004_Software["automation runtime"] = VALUE_004_AR;
    map_004_Software["visual components"] = VALUE_004_VC;
    map_004_Software["motion control"] = VALUE_004_MC;
    map_004_Software["cnc software"] = VALUE_004_CNC;

    map_005_CpuConfiguration["param"] = VALUE_005_PARAM;
    map_005_CpuConfiguration["reboot_mode"] = VALUE_005_REBOOT;
    map_005_CpuConfiguration["preserve_permanent_pv_memory"] = VALUE_005_PRESERVE;

    map_006_CpuConfigurationReboot["after reset"] = VALUE_006_RESET;
    map_006_CpuConfigurationReboot["after powerfail"] = VALUE_006_PFAIL;
    map_006_CpuConfigurationReboot["after change of cf/hd"] = VALUE_006_CHANGECF;

    map_007_CpuConfigurationPreserve["after change of cf/hd"] = VALUE_007_CHANGECF;

    map_008_CpuConfigurationParam["host name"] = VALUE_008_HOSTNAME;
    map_008_CpuConfigurationParam["default domain"] = VALUE_008_DEFDOMAIN;
    map_008_CpuConfigurationParam["cpu mode switch"] = VALUE_008_CPUMODESWITCH;
    map_008_CpuConfigurationParam["profiling"] = VALUE_008_PROFILING;
    map_008_CpuConfigurationParam["ftp"] = VALUE_008_FTP;
    map_008_CpuConfigurationParam["usb remote install"] = VALUE_008_USBREMOTEINSTALL;
    map_008_CpuConfigurationParam["cf remote install"] = VALUE_008_CFREMOTEINSTALL;

    map_009_ApplicationInfo["application link"] = VALUE_009_APPLINK;
    map_009_ApplicationInfo["application status"] = VALUE_009_APPSTATUS;

    map_010_Ethernet["param"] = ETHERNET_PARAM;
    map_010_Ethernet["interface"] = ETHERNET_INTERFACE;

    map_011_EthernetPar["default gateway"] = ETHERNET_000_DEFGATE;

    map_012_EthernetIf["ip address"] = ETHERNET_001_IPADRESS;
    map_012_EthernetIf["subnet mask"] = ETHERNET_001_SUBNET;
    map_012_EthernetIf["ip configuration"] = ETHERNET_001_IPCONF;
    map_012_EthernetIf["snmp configuration"] = ETHERNET_001_SNMPCONF;
    map_012_EthernetIf["ethernet remote install"] = ETHERNET_001_ETHREMOTEINSTALL;

    map_013_Memory["cf"] = MEMORY_CF;
    map_013_Memory["dram"] = MEMORY_DRAM;
    map_013_Memory["sram"] = MEMORY_SRAM;

    map_014_MemoryCF["partition"] = MEMORY_000_PARTITION;

    map_015_MemorySRAM["not_configured"] = MEMORY_001_NOTCONFIGURED;
    map_015_MemorySRAM["userram"] = MEMORY_001_USERRAM;
    map_015_MemorySRAM["remmem"] = MEMORY_001_REMMEM;

    map_029_CfAttributes["number_of_partitions"] = MEMORY_002_NUMOFPARTITIONS;
    map_029_CfAttributes["number_of_sectors"] = MEMORY_002_NUMBEROFSECTORS;
    map_029_CfAttributes["size"] = MEMORY_002_SIZE;
    map_029_CfAttributes["size_per_sector"] = MEMORY_002_SIZEPERSECTOR;

    map_030_PartitionAttributes["available"] = MEMORY_003_AVAILABLE;
    map_030_PartitionAttributes["description"] = MEMORY_003_DESCRIPTION;
    map_030_PartitionAttributes["id"] = MEMORY_003_ID;
    map_030_PartitionAttributes["size"] = MEMORY_003_SIZE;
    map_030_PartitionAttributes["used"] = MEMORY_003_USED;

    map_031_DRAMAttributes["available"] = MEMORY_004_AVAILABLE;
    map_031_DRAMAttributes["largest_available_block"] = MEMORY_004_LARGESTBLOCK;
    map_031_DRAMAttributes["size"] = MEMORY_004_SIZE;
    map_031_DRAMAttributes["used"] = MEMORY_004_USED;

    map_033_USERRAMAttributes["available"] = MEMORY_006_AVAILABLE;
    map_033_USERRAMAttributes["device"] = MEMORY_006_DEVICE;
    map_033_USERRAMAttributes["largest_available_block"] = MEMORY_006_LARGESTBLOCK;
    map_033_USERRAMAttributes["size"] = MEMORY_006_SIZE;
    map_033_USERRAMAttributes["used"] = MEMORY_006_USED;

    map_032_SRAMAttributes["size"] = MEMORY_005_SIZE;

    map_035_NotConfigured["size"] = MEMORY_008_SIZE;

    map_034_REMMEMAttributes["available"] = MEMORY_007_AVAILABLE;
    map_034_REMMEMAttributes["device"] = MEMORY_007_DEVICE;
    map_034_REMMEMAttributes["largest_available_block"] = MEMORY_007_LARGESTBLOCK;
    map_034_REMMEMAttributes["permanent"] = MEMORY_007_PERMANENT;
    map_034_REMMEMAttributes["remanent_global"] = MEMORY_007_REMGLOBAL;
    map_034_REMMEMAttributes["remanent_local"] = MEMORY_007_REMLOCAL;
    map_034_REMMEMAttributes["size"] = MEMORY_007_SIZE;

    map_016_Timing["timer_configuration"] = TIMING_TIMERCONF;
    map_016_Timing["task_class_configuration"] = TIMING_TASKCLASSCONF;

    map_036_TimingTimerConfiguration["idle_task_class"] = TIMING_001_IDLETASKCLASS;
    map_036_TimingTimerConfiguration["multiple_value_of_system_timer_cycle"] = TIMING_001_MULTIPLEVALUE;
    map_036_TimingTimerConfiguration["dividing_value_of_system_timer_cycle"] = TIMING_001_DIVIDINGVALUE;
    map_036_TimingTimerConfiguration["system_tick"] = TIMING_001_SYSTICK;
    map_036_TimingTimerConfiguration["system_timer"] = TIMING_001_SYSTIMER;
    map_036_TimingTimerConfiguration["task_class_idle_time"] = TIMING_001_TCIDLETIME;

    map_017_TimingTaskClass["task_class"] = TIMING_000_TASKCLASS;

    map_037_TimingTaskClassConfiguration["cycle_time"] = TIMING_002_CYCLETIME;
    map_037_TimingTaskClassConfiguration["cyclic"] = TIMING_002_CYCLIC;
    map_037_TimingTaskClassConfiguration["input_delay"] = TIMING_002_INPUTDELAY;
    map_037_TimingTaskClassConfiguration["output_delay"] = TIMING_002_OUTPUTDELAY;
    map_037_TimingTaskClassConfiguration["stack"] = TIMING_002_STACK;
    map_037_TimingTaskClassConfiguration["tolerance"] = TIMING_002_TOLERANCE;

    map_018_CpuUsage["zoom_interval"] = CPUUSAGE_INTERVAL;

    map_038_CpuUsageInterval["average"] = CPUUSAGE_000_AVERAGE;
    map_038_CpuUsageInterval["description"] = CPUUSAGE_000_DESCRIPTION;
    map_038_CpuUsageInterval["id"] = CPUUSAGE_000_ID;
    map_038_CpuUsageInterval["maximum"] = CPUUSAGE_000_MAX;

    map_038_CpuUsageValues["average"] = CPUUSAGE_001_AVERAGE;
    map_038_CpuUsageValues["maximum"] = CPUUSAGE_001_MAXIMUM;

    map_019_Software["application_module"] = SOFTWARE_APPLICATIONMODULE;
    map_019_Software["module"] = SOFTWARE_MODULE;

    map_020_SoftwareAppModule["module"] = SOFTWARE_000_MODULE;

    map_039_ModuleAttributes["address"] = SOFTWARE_001_ADRESS;
    map_039_ModuleAttributes["id"] = SOFTWARE_001_ID;
    map_039_ModuleAttributes["memory"] = SOFTWARE_001_MEMORY;
    map_039_ModuleAttributes["name"] = SOFTWARE_001_NAME;
    map_039_ModuleAttributes["size"] = SOFTWARE_001_SIZE;
    map_039_ModuleAttributes["timestamp"] = SOFTWARE_001_TIMESTAMP;
    map_039_ModuleAttributes["type"] = SOFTWARE_001_TYPE;
    map_039_ModuleAttributes["version"] = SOFTWARE_001_VERSION;

    map_021_Hardware["node"] = HARDWARE_NODE;

    map_022_HardwareNode["module_status"] = HARDWARE_000_MODULESTATUS;
    map_022_HardwareNode["io_information"] = HARDWARE_000_IOINFORMATION;
    map_022_HardwareNode["io_info"] = HARDWARE_000_IOINFO;

    map_023_HardwareNodeAttributes["configured"] = HARDWARE_001_CONFIGURED;
    map_023_HardwareNodeAttributes["io_info"] = HARDWARE_001_IOINFO;
    map_023_HardwareNodeAttributes["id"] = HARDWARE_001_ID;
    map_023_HardwareNodeAttributes["status"] = HARDWARE_001_STATUS;

    map_024_HardwareModuleStatusAttributes["configured"] = HARDWARE_002_CONFIGURED;
    map_024_HardwareModuleStatusAttributes["moduleok"] = HARDWARE_002_MODULEOK;
    map_024_HardwareModuleStatusAttributes["plugged"] = HARDWARE_002_PLUGGED;
    map_024_HardwareModuleStatusAttributes["supervised"] = HARDWARE_002_SUPERVISED;

    map_025_HardwareIOInfoAttributes["equipment_id"] = HARDWARE_003_EQUIPMENTID;
    map_025_HardwareIOInfoAttributes["firmware_version"] = HARDWARE_003_FWVERSION;
    map_025_HardwareIOInfoAttributes["hardware_revision"] = HARDWARE_003_HWREVISION;
    map_025_HardwareIOInfoAttributes["hardware_variant"] = HARDWARE_003_HWVARIANT;
    map_025_HardwareIOInfoAttributes["module_path"] = HARDWARE_003_MODPATH;
    map_025_HardwareIOInfoAttributes["serialnumber"] = HARDWARE_003_SERIALNR;

    map_026_HardwareNodeIoInfo["channel_name"] = HARDWARE_004_CHANNEL;

    map_027_HardwareChannelAttributes["diagnose"] = HARDWARE_005_DIAGNOSE;
    map_027_HardwareChannelAttributes["force_status"] = HARDWARE_005_FORCESTATUS;
    map_027_HardwareChannelAttributes["force_value"] = HARDWARE_005_FORCEVALUE;
    map_027_HardwareChannelAttributes["iec_type"] = HARDWARE_005_IECTYPE;
    map_027_HardwareChannelAttributes["iec_value"] = HARDWARE_005_IECVALUE;
    map_027_HardwareChannelAttributes["id"] = HARDWARE_005_ID;
    map_027_HardwareChannelAttributes["name"] = HARDWARE_005_NAME;
    map_027_HardwareChannelAttributes["physical_value"] = HARDWARE_005_PHYSICALVALUE;
    map_027_HardwareChannelAttributes["type"] = HARDWARE_005_TYPE;

    map_040_Motion["motion"] = MOTION_000_MOTION;

    map_041_MotionAttributes["available"] = MOTION_004_AVAILABLE;

    map_042_MotionNodes["axis"] = MOTION_AXIS;
    map_042_MotionNodes["alarm"] = MOTION_ALARM;

    map_043_MotionAlarm["error"] = MOTION_003_ERRORENTRY;

    map_045_MotionAxisAttribute["act._position"] = MOTION_001_ACTPOS;
    map_045_MotionAxisAttribute["act._speed"] = MOTION_001_ACTSPEED;
    map_045_MotionAxisAttribute["address"] = MOTION_001_ADRESS;
    map_045_MotionAxisAttribute["alarm"] = MOTION_001_ALARM;
    map_045_MotionAxisAttribute["controller"] = MOTION_001_CONTROLLER;
    map_045_MotionAxisAttribute["drive"] = MOTION_001_DRIVE;
    map_045_MotionAxisAttribute["enable"] = MOTION_001_ENABLE;
    map_045_MotionAxisAttribute["homing"] = MOTION_001_HOMING;
    map_045_MotionAxisAttribute["id"] = MOTION_001_ID;
    map_045_MotionAxisAttribute["movement_status"] = MOTION_001_MOVEMENTSTATUS;
    map_045_MotionAxisAttribute["name"] = MOTION_001_NAME;
    map_045_MotionAxisAttribute["negative_endswitch"] = MOTION_001_NEGENDSW;
    map_045_MotionAxisAttribute["positive_endswitch"] = MOTION_001_POSENDSW;
    map_045_MotionAxisAttribute["reference_switch"] = MOTION_001_REFSW;
    map_045_MotionAxisAttribute["simulation"] = MOTION_001_SIMULATION;
    map_045_MotionAxisAttribute["trigger_1"] = MOTION_001_TRIGGER1;
    map_045_MotionAxisAttribute["trigger_2"] = MOTION_001_TRIGGER2;

    map_046_MotionAlarmAttributes["error_no."] = MOTION_002_ERRORNO;
    map_046_MotionAlarmAttributes["error_text"] = MOTION_002_ERRORTEXT;
    map_046_MotionAlarmAttributes["id"] = MOTION_002_ID;
    map_046_MotionAlarmAttributes["parid"] = MOTION_002_PARID;
    map_046_MotionAlarmAttributes["status"] = MOTION_002_STATUS;
    map_046_MotionAlarmAttributes["text_info"] = MOTION_002_TEXTINFO;
    map_046_MotionAlarmAttributes["text_parid"] = MOTION_002_TEXTPARID;
    map_046_MotionAlarmAttributes["timestamp"] = MOTION_002_TIMESTAMP;

    map_050_Logger["module"] = LOGGER_MODULE;

    map_051_LoggerModuleAttributes["address"] = LOGGER_000_ADRESS;
    map_051_LoggerModuleAttributes["id"] = LOGGER_000_ID;
    map_051_LoggerModuleAttributes["name"] = LOGGER_000_NAME;
    map_051_LoggerModuleAttributes["version"] = LOGGER_000_VERSION;

    map_052_LoggerModuleEntry["entry"] = LOGGER_001_ENTRY;

    // Logger version 1.00.0
    map_053_LoggerModuleEntryAttributes["ascii_data"] = LOGGER_002_ASCIIDATA;
    map_053_LoggerModuleEntryAttributes["binary_data"] = LOGGER_002_BINARYDATA;
    map_053_LoggerModuleEntryAttributes["error_description"] = LOGGER_002_ERRDESCRIPTION;
    map_053_LoggerModuleEntryAttributes["error_no."] = LOGGER_002_ERRORNR;
    map_053_LoggerModuleEntryAttributes["id"] = LOGGER_002_ID;
    map_053_LoggerModuleEntryAttributes["level"] = LOGGER_002_LEVEL;
    map_053_LoggerModuleEntryAttributes["os_task"] = LOGGER_002_OSTASK;
    map_053_LoggerModuleEntryAttributes["timestamp"] = LOGGER_002_TIMESTAMP;
    map_053_LoggerModuleEntryAttributes["event_id"] = LOGGER_002_EVENTID;
    map_053_LoggerModuleEntryAttributes["object_id"] = LOGGER_002_OBJECTID;

    // Logger version 1.02.0 and later
    map_054_LoggerModuleEntryAttributesV2["id"] = LOGGER_202_ID;
    map_054_LoggerModuleEntryAttributesV2["severity"] = LOGGER_202_SEVERITY;
    map_054_LoggerModuleEntryAttributesV2["facility"] = LOGGER_202_FACILITY;
    map_054_LoggerModuleEntryAttributesV2["timestamp"] = LOGGER_202_TIMESTAMP;
    map_054_LoggerModuleEntryAttributesV2["info"] = LOGGER_202_INFO;
    map_054_LoggerModuleEntryAttributesV2["error_no."] = LOGGER_202_EVENTID;
    map_054_LoggerModuleEntryAttributesV2["event_id"] = LOGGER_202_EVENTID;
    map_054_LoggerModuleEntryAttributesV2["entered_by"] = LOGGER_202_ENTEREDBY;
    map_054_LoggerModuleEntryAttributesV2["description"] = LOGGER_202_DESCRIPTION;
    map_054_LoggerModuleEntryAttributesV2["ascii_data"] = LOGGER_202_ASCIIDATA;
    map_054_LoggerModuleEntryAttributesV2["binary_data"] = LOGGER_202_BINARYDATA;
    map_054_LoggerModuleEntryAttributesV2["origin_record_id"] = LOGGER_202_ORIGINRECORDID;

    map_060_Profiler["module"] = PROFILER_MODULE;

    map_061_ProfilerAttributes["id"] = PROFILER_000_ID;
    map_061_ProfilerAttributes["name"] = PROFILER_000_NAME;
    map_061_ProfilerAttributes["size"] = PROFILER_000_SIZE;
    map_061_ProfilerAttributes["timestamp"] = PROFILER_000_TIMESTAMP;
}
