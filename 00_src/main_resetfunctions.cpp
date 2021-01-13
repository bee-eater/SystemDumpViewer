#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::reset_DumpInfo(){
    // Clear Vectors
    std::vector<sInterface>().swap(this->SysDump.Sections.Ethernet.vInterface);
    std::vector<sPartition>().swap(this->SysDump.Sections.Memory.CompactFlash.vPartition);
    std::vector<sTaskClass>().swap(this->SysDump.Sections.Timing.vTaskClass);
    std::vector<sZoomInterval>().swap(this->SysDump.Sections.CpuUsage.vZoomInterval);
    std::vector<sAppModule>().swap(this->SysDump.Sections.Software.vAppModule);
    std::vector<sNode>().swap(this->SysDump.Sections.Hardware.vNode);
    std::vector<sAxis>().swap(this->SysDump.Sections.Motion.vAxis);
    std::vector<sLogModule>().swap(this->SysDump.Sections.Logger.vModule);
    std::vector<sProfModule>().swap(this->SysDump.Sections.Profiler.vModule);

    std::vector<sLogModuleEntry>().swap(this->vLogSortView);
    std::vector<QCheckBox*>().swap(this->vLogger_Checkbox);
    std::vector<QStringList>().swap(this->vHardwareErrorPaths);

    this->SysDump.Generator = QTranslator::tr("");
    this->SysDump.Timestamp = QTranslator::tr("");
    this->SysDump.Version = QTranslator::tr("");

    this->SysDump.Sections.Ethernet.DefaultGateway = QTranslator::tr("");

    this->SysDump.Sections.Memory.CompactFlash.NumberOfPartitions = 0;
    this->SysDump.Sections.Memory.CompactFlash.NumberOfSectors = 0;
    this->SysDump.Sections.Memory.CompactFlash.Size = 0;
    this->SysDump.Sections.Memory.CompactFlash.SizePerSector = 0;
    this->SysDump.Sections.Memory.CompactFlash.SerialNumber = "";
    this->SysDump.Sections.Memory.CompactFlash.ModelNumber = "";
    this->SysDump.Sections.Memory.CompactFlash.StorageWear = 0;
    this->SysDump.Sections.Memory.DRAM.available = 0;
    this->SysDump.Sections.Memory.DRAM.LargestAvailableBlock = 0;
    this->SysDump.Sections.Memory.DRAM.size = 0;
    this->SysDump.Sections.Memory.DRAM.used = 0;
    this->SysDump.Sections.Memory.SRAM.notConfigured = 0;
    this->SysDump.Sections.Memory.SRAM.REMMEM.available = 0;
    this->SysDump.Sections.Memory.SRAM.REMMEM.device = QTranslator::tr("");
    this->SysDump.Sections.Memory.SRAM.REMMEM.LargestAvailableBlock = 0;
    this->SysDump.Sections.Memory.SRAM.REMMEM.permanent = 0;
    this->SysDump.Sections.Memory.SRAM.REMMEM.remGlobal = 0;
    this->SysDump.Sections.Memory.SRAM.REMMEM.remLocal = 0;
    this->SysDump.Sections.Memory.SRAM.REMMEM.size = 0;
    this->SysDump.Sections.Memory.SRAM.size = 0;
    this->SysDump.Sections.Memory.SRAM.USERRAM.available = 0;
    this->SysDump.Sections.Memory.SRAM.USERRAM.device = QTranslator::tr("");
    this->SysDump.Sections.Memory.SRAM.USERRAM.LargestAvailableBlock = 0;
    this->SysDump.Sections.Memory.SRAM.USERRAM.size = 0;
    this->SysDump.Sections.Memory.SRAM.USERRAM.used = 0;

    this->SysDump.Sections.Motion.Available = false;

    this->SysDump.Sections.Timing.TimerConfig.IdleTaskClass = 0;
    this->SysDump.Sections.Timing.TimerConfig.MultipleValueOfSTC = 0;
    this->SysDump.Sections.Timing.TimerConfig.systemTick = 0;
    this->SysDump.Sections.Timing.TimerConfig.SystemTimer = QTranslator::tr("");
    this->SysDump.Sections.Timing.TimerConfig.TaskClassIdleTime = 0;

    this->SysDump.Sections.Values.ApplicationInfo.ApplicationLink = QTranslator::tr("");
    this->SysDump.Sections.Values.ApplicationInfo.ApplicationStatus = 0;

    this->SysDump.Sections.Values.CpuConfiguration.CFremoteInstall = false;
    this->SysDump.Sections.Values.CpuConfiguration.CPUModeSwitch = QTranslator::tr("");
    this->SysDump.Sections.Values.CpuConfiguration.DefaultDomain = QTranslator::tr("");
    this->SysDump.Sections.Values.CpuConfiguration.FTP = false;
    this->SysDump.Sections.Values.CpuConfiguration.Hostname = QTranslator::tr("");
    this->SysDump.Sections.Values.CpuConfiguration.PreservePVMemory.AfterCFChange = 0;
    this->SysDump.Sections.Values.CpuConfiguration.Profiling = false;
    this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterCFChange = 0;
    this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterPowerFail = 0;
    this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterReset = 0;
    this->SysDump.Sections.Values.CpuConfiguration.USBremoteInstall = false;

    this->SysDump.Sections.Values.OperationalValues.BatteryStatus = 0;
    this->SysDump.Sections.Values.OperationalValues.CPUtemperature = 0;
    this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode = 0;
    this->SysDump.Sections.Values.OperationalValues.NodeNumber = QTranslator::tr("");
    this->SysDump.Sections.Values.OperationalValues.OperatingHours = 0;
    this->SysDump.Sections.Values.OperationalValues.PowerOnCycles = 0;
    this->SysDump.Sections.Values.OperationalValues.TargetTime = QTranslator::tr("");

    this->SysDump.Sections.Values.SoftwareVersions.AR = QTranslator::tr("");
    this->SysDump.Sections.Values.SoftwareVersions.VC = QTranslator::tr("");
    this->SysDump.Sections.Values.SoftwareVersions.MC = QTranslator::tr("");
    this->SysDump.Sections.Values.SoftwareVersions.CNC = QTranslator::tr("");

    this->SysDump.Sections.Values.TimeSynchronisation.Client.RTCsynchInterval = 0;
    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPclient = QTranslator::tr("");
    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver1 = QTranslator::tr("");
    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver2 = QTranslator::tr("");
    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver3 = QTranslator::tr("");
    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver4 = QTranslator::tr("");
    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPsynchInterval = 0;

    this->SysDump.Sections.Values.TimeSynchronisation.Server.SNTPserver = QTranslator::tr("");
    this->SysDump.Sections.Values.TimeSynchronisation.TimeZone = QTranslator::tr("");
}

void MainWindow::reset_UI(){

    ui->tree_Hardware_Channels->clear();
    ui->label_Hardware_Channel_Diagnose->clear();
    ui->label_Hardware_Channel_ForceStatus->clear();
    ui->label_Hardware_Channel_ForceValue->clear();
    ui->label_Hardware_Channel_ID->clear();
    ui->label_Hardware_Channel_IECType->clear();
    ui->label_Hardware_Channel_IECValue->clear();
    ui->label_Hardware_Channel_Name->clear();
    ui->label_Hardware_Channel_PhysicalValue->clear();
    ui->label_Hardware_Channel_Type->clear();

    ui->label_Hardware_Configured->clear();
    ui->label_Hardware_EquipmentID->clear();
    ui->label_Hardware_Firmware->clear();
    ui->label_Hardware_HwRevision->clear();
    ui->label_Hardware_HwVariant->clear();
    ui->label_Hardware_ModuleOk->clear();
    ui->label_Hardware_Supervised->clear();
    ui->label_Hardware_ModulePath->clear();
    ui->label_Hardware_Plugged->clear();
    ui->label_Hardware_Serialnumber->clear();

    ui->label_CPUUsage_Average->clear();
    ui->label_CPUUsage_Id->clear();
    ui->label_CPUUsage_Maximum->clear();

    ui->combo_SelectMenu->setCurrentIndex(0);
    ui->tabWidget_System->setCurrentIndex(0);
    ui->tabWidget_Motion->setCurrentIndex(0);
}
