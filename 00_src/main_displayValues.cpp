#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCheckBox>
#include <QGraphicsDropShadowEffect>
#include <QVariant>
#include <QStringRef>
#include <QIcon>

#include "qwt.h"
#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_legend.h"
#include "qwt_abstract_scale_draw.h"
#include "qwt_scale_draw.h"
#include "qwt_plot_grid.h"
#include "qwt_plot_picker.h"

#include <algorithm>

#include <./includes/clickLabel.h>

/**
 * @brief This function is called to pass the read values from the XML to the elements of the MainWindow
 * @return is always 0
 */
bool MainWindow::displayValues(){

    // General Information
    ui->label_General_Generator->setText(this->SysDump.Generator);
    ui->label_General_Timestamp->setText(this->SysDump.Timestamp);
    ui->label_General_Version->setText(this->SysDump.Version);

    // System
        // General :: Operational Values
             // When using Lists, always check that the index is in a valid range!
            ((this->SysDump.Sections.Values.OperationalValues.BatteryStatus < this->list_BatteryStatus.count()) && (this->SysDump.Sections.Values.OperationalValues.BatteryStatus >= 0)) ? ui->label_Values_OPV_BattStatus->setText(this->list_BatteryStatus.at(this->SysDump.Sections.Values.OperationalValues.BatteryStatus)) : ui->label_Values_OPV_BattStatus->setText(QString::number(this->SysDump.Sections.Values.OperationalValues.BatteryStatus)) ;
            ((this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode < this->list_CpuModes.count()) && (this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode >= 0)) ? ui->label_Values_OPV_CPUMode->setText(this->list_CpuModes.at(this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode)) : ui->label_Values_OPV_CPUMode->setText(QString::number(this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode)) ;
            ui->label_Values_OPV_CPUTemp->setText(QString::number(this->SysDump.Sections.Values.OperationalValues.CPUtemperature)+tr(" °C"));
            ui->label_Values_OPV_NodeNumber->setText(this->SysDump.Sections.Values.OperationalValues.NodeNumber);
            ui->label_Values_OPV_OPHours->setText(QString::number(this->SysDump.Sections.Values.OperationalValues.OperatingHours));
            ui->label_Values_OPV_PowerCycles->setText(QString::number(this->SysDump.Sections.Values.OperationalValues.PowerOnCycles));
            ui->label_Values_OPV_TargetTime->setText(this->SysDump.Sections.Values.OperationalValues.TargetTime);

        // General :: CPU Configuration
            ui->label_Values_CPUConf_HostName->setText(this->SysDump.Sections.Values.CpuConfiguration.Hostname);
            ui->label_Values_CPUConf_defdomain->setText(this->SysDump.Sections.Values.CpuConfiguration.DefaultDomain);
            ui->label_Values_CPUConf_CPUmodeSwitch->setText(this->SysDump.Sections.Values.CpuConfiguration.CPUModeSwitch);
            if(this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterReset == 0x20){
                ui->label_Values_CPUConf_afterReset->setText(tr("diagnose"));
            } else {
                (this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterReset < this->list_RebootMode.count()) ? ui->label_Values_CPUConf_afterReset->setText(this->list_RebootMode.at(this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterReset)) : ui->label_Values_CPUConf_afterReset->setText(QString::number(this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterReset)) ;
            }
            if(this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterPowerFail == 0x20){
                ui->label_Values_CPUConf_powerfail->setText(tr("diagnose"));
            } else {
                (this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterPowerFail < this->list_RebootMode.count()) ? ui->label_Values_CPUConf_powerfail->setText(this->list_RebootMode.at(this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterPowerFail)) : ui->label_Values_CPUConf_powerfail->setText(QString::number(this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterPowerFail)) ;
            }
            (this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterCFChange < this->list_RebootCF.count() && this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterCFChange>=0)? ui->label_Values_CPUConf_changecf->setText(this->list_RebootCF.at(this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterCFChange)) : ui->label_Values_CPUConf_changecf->setText(QString::number(this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterCFChange));
            this->SysDump.Sections.Values.CpuConfiguration.PreservePVMemory.AfterCFChange==0 ? ui->label_Values_CPUConf_pvchangecf->setText(tr("no")) : ui->label_Values_CPUConf_pvchangecf->setText(tr("yes"));
            this->SysDump.Sections.Values.CpuConfiguration.CFremoteInstall==0 ? ui->label_Values_CPUConf_cfremote->setText(tr("-")) : ui->label_Values_CPUConf_cfremote->setText(tr("enabled"));
            this->SysDump.Sections.Values.CpuConfiguration.USBremoteInstall==0 ? ui->label_Values_CPUConf_usbremote->setText(tr("-")) : ui->label_Values_CPUConf_usbremote->setText(tr("enabled"));
            this->SysDump.Sections.Values.CpuConfiguration.Profiling==0 ? ui->label_Values_CPUConf_profiling->setText(tr("-")) : ui->label_Values_CPUConf_profiling->setText(tr("enabled"));
            this->SysDump.Sections.Values.CpuConfiguration.FTP==0 ? ui->label_Values_CPUConf_ftp->setText(tr("-")) : ui->label_Values_CPUConf_ftp->setText(tr("enabled"));

        // General :: Time Synchronisation
            ui->label_TimeSync_TimeZone->setText(this->SysDump.Sections.Values.TimeSynchronisation.TimeZone);
            ui->label_TimeSync_SNTPServer->setText(this->SysDump.Sections.Values.TimeSynchronisation.Server.SNTPserver);
            ui->label_TimeSync_SNTPClient->setText(this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPclient);
            ui->label_TimeSync_SNTPClient_Server1->setText(this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver1);
            ui->label_TimeSync_SNTPClient_Server2->setText(this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver2);
            ui->label_TimeSync_SNTPClient_Server3->setText(this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver3);
            ui->label_TimeSync_SNTPClient_Server4->setText(this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver4);
            ui->label_TimeSync_SNTPClient_sntpSync->setText(QString::number(this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPsynchInterval));
            ui->label_TimeSync_SNTPClient_rtcSync->setText(QString::number(this->SysDump.Sections.Values.TimeSynchronisation.Client.RTCsynchInterval));

        // General :: Software Versions
            ui->label_Software_AR->setText(this->SysDump.Sections.Values.SoftwareVersions.AR);
            ui->label_Software_VC->setText(this->SysDump.Sections.Values.SoftwareVersions.VC);
            ui->label_Software_MC->setText(this->SysDump.Sections.Values.SoftwareVersions.MC);
            ui->label_Software_CNC->setText(this->SysDump.Sections.Values.SoftwareVersions.CNC);

    // System :: Ethernet :: Ethernet Interfaces
        // Def Gateway
            ui->label_EthIf_DefaultGateway->setText(this->SysDump.Sections.Ethernet.DefaultGateway);
        // Remove all old Tabs
            for(int i=ui->tabWidget_EthernetInterfaces->count();i>=1;i--){
                ui->tabWidget_EthernetInterfaces->removeTab(i);
            }
        //Create New Tabs
            for(unsigned int i=0;i<this->SysDump.Sections.Ethernet.vInterface.size();i++){
                this->add_EthernetTab(static_cast<int>(i));
            }

    // System :: Memory
        // CF/HD Partitions
            ui->label_Memory_Partitions_NoOfPartitions->setText(QString::number(this->SysDump.Sections.Memory.CompactFlash.NumberOfPartitions));
            ui->label_Memory_Partitions_NoOfSectors->setText(QString::number(this->SysDump.Sections.Memory.CompactFlash.NumberOfSectors));
            writeByteToLabel(ui->label_Memory_Partitions_SizePerSector, this->SysDump.Sections.Memory.CompactFlash.SizePerSector);
            writeByteToLabel(ui->label_Memory_Partitions_TotalCapacity, this->SysDump.Sections.Memory.CompactFlash.Size);
            ui->label_Memory_SerialNumber->setText(this->SysDump.Sections.Memory.CompactFlash.SerialNumber);
            ui->label_Memory_ModelNumber->setText(this->SysDump.Sections.Memory.CompactFlash.ModelNumber);
            qDeleteAll(ui->gridLayoutPartitions->findChildren<QWidget*>());
            for(unsigned int i=0;i<this->SysDump.Sections.Memory.CompactFlash.vPartition.size();i++){
                this->add_PartitionBar(10,(12+static_cast<int>(i)*(4*PARTITION_LABEL_HEIGHT+45)),static_cast<int>(i));
            }
        // DRAM
            writeByteToLabel(ui->label_Memory_DRAM_Size, this->SysDump.Sections.Memory.DRAM.size);
            ui->label_Memory_DRAM_Size->setAlignment(Qt::AlignRight | Qt::AlignCenter);

            writeByteToLabel(ui->label_Memory_DRAM_Used, this->SysDump.Sections.Memory.DRAM.used);
            ui->label_Memory_DRAM_Used->setAlignment(Qt::AlignRight | Qt::AlignCenter);

            writeByteToLabel(ui->label_Memory_DRAM_available, this->SysDump.Sections.Memory.DRAM.available);
            ui->label_Memory_DRAM_available->setAlignment(Qt::AlignRight | Qt::AlignCenter);

            writeByteToLabel(ui->label_Memory_DRAM_LBlock, this->SysDump.Sections.Memory.DRAM.LargestAvailableBlock);
            ui->label_Memory_DRAM_LBlock->setAlignment(Qt::AlignRight | Qt::AlignCenter);

        // SRAM
            writeByteToLabel(ui->label_Memory_SRAM_Size, this->SysDump.Sections.Memory.SRAM.size);
            writeByteToLabel(ui->label_Memory_SRAM_NotUsed, this->SysDump.Sections.Memory.SRAM.notConfigured);
            // USERROM
                writeByteToLabel(ui->label_Memory_USERRAM_Available, this->SysDump.Sections.Memory.SRAM.USERRAM.available);
                ui->label_Memory_USERRAM_Device->setText(this->SysDump.Sections.Memory.SRAM.USERRAM.device);
                writeByteToLabel(ui->label_Memory_USERRAM_LBlock, this->SysDump.Sections.Memory.SRAM.USERRAM.LargestAvailableBlock);
                writeByteToLabel(ui->label_Memory_USERRAM_Size, this->SysDump.Sections.Memory.SRAM.USERRAM.size);
                writeByteToLabel(ui->label_Memory_USERRAM_Used, this->SysDump.Sections.Memory.SRAM.USERRAM.used);
            // REMMEM
                writeByteToLabel(ui->label_Memory_SRAM_REMMEM_Available, this->SysDump.Sections.Memory.SRAM.REMMEM.available);
                ui->label_Memory_SRAM_REMMEM_Device->setText(this->SysDump.Sections.Memory.SRAM.REMMEM.device);
                writeByteToLabel(ui->label_Memory_SRAM_REMMEM_LBlock, this->SysDump.Sections.Memory.SRAM.REMMEM.LargestAvailableBlock);
                writeByteToLabel(ui->label_Memory_SRAM_REMMEM_Size, this->SysDump.Sections.Memory.SRAM.REMMEM.size);
                writeByteToLabel(ui->label_Memory_SRAM_REMMEM_Permanent, this->SysDump.Sections.Memory.SRAM.REMMEM.permanent);
                writeByteToLabel(ui->label_Memory_SRAM_REMMEM_RemGlobal, this->SysDump.Sections.Memory.SRAM.REMMEM.remGlobal);
                writeByteToLabel(ui->label_Memory_SRAM_REMMEM_RemLocal, this->SysDump.Sections.Memory.SRAM.REMMEM.remLocal);

    // System :: Timing
        // Timer Configuration
            ui->label_TimerConf_IdleTaskClass->setText(tr("Cyclic #")+QString::number(this->SysDump.Sections.Timing.TimerConfig.IdleTaskClass));
            ui->label_TimerConf_mvostc->setText(QString::number(this->SysDump.Sections.Timing.TimerConfig.MultipleValueOfSTC));
            ui->label_TimerConf_SystemTick->setText(QString::number(this->SysDump.Sections.Timing.TimerConfig.systemTick)+tr(" µs"));
            ui->label_TimerConf_SystemTimer->setText((this->SysDump.Sections.Timing.TimerConfig.SystemTimer));
            ui->label_TimerConf_TaskClassIdleTime->setText(QString::number(this->SysDump.Sections.Timing.TimerConfig.TaskClassIdleTime)+tr(" µs"));
        // Task class configuration
            QStringList labels;
            // Clear old
                ui->table_TaskClasses->setRowCount(0);
            // Configure
                labels << tr("") << tr("Task class") << tr("Cycle time") << tr("Tolerance") << tr("Stack") << tr("Input delay") << tr("Output delay");
                ui->table_TaskClasses->setColumnCount(TASK_CLASS_TABLE_COLUMNS);

                ui->table_TaskClasses->setColumnWidth(0,20);
                ui->table_TaskClasses->setColumnWidth(1,80);
                ui->table_TaskClasses->setColumnWidth(2,90);
                ui->table_TaskClasses->setColumnWidth(3,90);
                ui->table_TaskClasses->setColumnWidth(4,80);
                ui->table_TaskClasses->setColumnWidth(5,152);
                ui->table_TaskClasses->setColumnWidth(6,152);

                ui->table_TaskClasses->setHorizontalHeaderLabels(labels);
                ui->table_TaskClasses->verticalHeader()->hide();
                ui->table_TaskClasses->setShowGrid(true);
                for(unsigned int i=0;i<this->SysDump.Sections.Timing.vTaskClass.size();i++){
                    this->add_TaskClass(i);
                }

    // Systen CPU Usage
        // Add filter entries
            ui->combo_CpuUsage->clear();

            for( auto zi : SysDump.Sections.CpuUsage.vZoomInterval )
            {
                ui->combo_CpuUsage->addItem(zi.description);
            }
        // Draw points
            this->draw_CpuUsage();

    // Software
        // Clear old
            ui->tree_ApplicationModules->clear();
            ui->table_Software->setRowCount(0);
        // Configure
            labels.clear();
            labels << tr("Modules") << tr("Version") << tr("Date/Time") << tr("Type") << tr("Memory") << tr("Adress") << tr("Size (Byte)");
            ui->table_Software->setColumnCount(SOFTWARE_TABLE_COLUMNS);

            ui->table_Software->setColumnWidth(0,90);
            ui->table_Software->setColumnWidth(1,70);
            ui->table_Software->setColumnWidth(2,150);
            ui->table_Software->setColumnWidth(3,100);
            ui->table_Software->setColumnWidth(4,80);
            ui->table_Software->setColumnWidth(5,80);
            ui->table_Software->setColumnWidth(6,90);

            ui->table_Software->setHorizontalHeaderLabels(labels);
            ui->table_Software->verticalHeader()->hide();
            ui->table_Software->setShowGrid(true);
            for(unsigned int i=0;i<this->SysDump.Sections.Software.vAppModule.size();i++){
                this->add_ApplicationModule(i);
            }

    // Hardware
        // Clear old ErrorPaths
            std::vector<QStringList>().swap(this->vHardwareErrorPaths);
        // Create nodes
            ui->tree_Hardware->setColumnCount(1);
            ui->tree_Hardware->setHeaderLabels(QStringList() << tr("Hardware Tree"));
        // Remove previous hardware tree
            ui->tree_Hardware->clear();
        // Add new hardware
            this->add_Hardware();

    // Motion
        // Delete Old
            qDeleteAll(ui->widget_Axis->findChildren<QWidget*>());
            ui->text_Axis_Name->setGeometry(ui->text_Axis_Name->x(),ui->text_Axis_Name->y(),(AXIS_LABEL_WIDTH),ui->text_Axis_Name->height());
            ui->text_Axis_Drive->setGeometry(ui->text_Axis_Drive->x(),ui->text_Axis_Drive->y(),(AXIS_LABEL_WIDTH),ui->text_Axis_Drive->height());
            ui->text_Axis_Address->setGeometry(ui->text_Axis_Address->x(),ui->text_Axis_Address->y(),(AXIS_LABEL_WIDTH),ui->text_Axis_Address->height());

        // Add new axis
            for(unsigned int i=0;i<this->SysDump.Sections.Motion.vAxis.size();i++){
                this->add_AxisToUi((12+static_cast<int>(i)*(10+AXIS_LABEL_WIDTH)),static_cast<int>(i));
                ui->widget_Axis->adjustSize();
            }
            this->axisScrollArea->setWidget(ui->widget_Axis);
            this->axisScrollArea->show();

       // Remove Old Errors
            ui->table_AxisError->setRowCount(0);
       // Configuration
            labels.clear();
            labels << tr("Error nr.") << tr("Date/Time") << tr("Text info") << tr("Error text");
            ui->table_AxisError->setColumnCount(AXISERR_TABLE_COLUMNS);

            ui->table_AxisError->setColumnWidth(0,80);
            ui->table_AxisError->setColumnWidth(1,140);
            ui->table_AxisError->setColumnWidth(2,220);
            ui->table_AxisError->setColumnWidth(3,400);

            ui->table_AxisError->setHorizontalHeaderLabels(labels);
            ui->table_AxisError->verticalHeader()->hide();
            ui->table_AxisError->setShowGrid(true);

       // Add filter entries
            ui->combo_AxisError->clear();
            for(unsigned int i = 0; i<this->SysDump.Sections.Motion.vAxis.size();i++){
                ui->combo_AxisError->addItem(this->SysDump.Sections.Motion.vAxis[i].Name);
            }


    // Logger
        // Add logger entries
            this->get_LoggerModules();

    // Profiler
        // Delete old
            ui->table_Profiler->setRowCount(0);
        // Configuration
            labels.clear();
            labels << tr("ID") << tr("Name") << tr("Date/Time") << tr("Size");
            ui->table_Profiler->setColumnCount(PROFILER_TABLE_COLUMNS);

            ui->table_Profiler->setColumnWidth(0,30);
            ui->table_Profiler->setColumnWidth(1,140);
            ui->table_Profiler->setColumnWidth(2,200);
            ui->table_Profiler->setColumnWidth(3,140);

            ui->table_Profiler->setHorizontalHeaderLabels(labels);
            ui->table_Profiler->verticalHeader()->hide();
            ui->table_Profiler->setShowGrid(true);

            this->add_ProfilerModules();

    // Overview
        //qDeleteAll(ui->widget_overview->findChildren<QWidget*>());
        this->draw_Overview();

    return 0;
}

int MainWindow::add_EthernetTab(int index){
    if(index==0){
        this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].EthernetRemoteInstall==0 ? ui->label_EthIf_RemoteInstall->setText(tr("disabled")) : ui->label_EthIf_RemoteInstall->setText(tr("enabled"));
        ui->label_EthIf_IPadress->setText(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].IpAddress);
        ui->label_EthIf_IFID->setText(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].InterfaceID);
        ui->label_EthIf_SubnetMask->setText(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].SubnetMask);
        (this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].IpConfig < this->list_IpConfiguration.count())? ui->label_EthIf_IPConfig->setText(this->list_IpConfiguration.at(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].IpConfig)) : ui->label_EthIf_IPConfig->setText(QString::number(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].IpConfig));
        (this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].SNMPConfig < this->list_SNMPConfiguration.count()) ? ui->label_EthIf_SNMPConfig->setText(this->list_SNMPConfiguration.at(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].SNMPConfig)) : ui->label_EthIf_SNMPConfig->setText(QString::number(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].SNMPConfig));
    } else {
        // Create new Tabs
        QWidget *EthIF=new QWidget(this);
        const QString names = tr("IF-")+QString::number(index+1);
        QLabel *Text00_EthIf = new QLabel(EthIF);
        QLabel *Label00_EthIf = new QLabel(EthIF);
        QLabel *Text01_EthIf = new QLabel(EthIF);
        QLabel *Label01_EthIf = new QLabel(EthIF);
        QLabel *Text02_EthIf = new QLabel(EthIF);
        QLabel *Label02_EthIf = new QLabel(EthIF);
        QLabel *Text03_EthIf = new QLabel(EthIF);
        QLabel *Label03_EthIf = new QLabel(EthIF);
        QLabel *Text04_EthIf = new QLabel(EthIF);
        QLabel *Label04_EthIf = new QLabel(EthIF);
        QLabel *Text05_EthIf = new QLabel(EthIF);
        QLabel *Label05_EthIf = new QLabel(EthIF);

        Text00_EthIf->move(QPoint(16,4));
        Label00_EthIf->move(QPoint(208,4));
        Text00_EthIf->setText(tr("Remote install:"));
        this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].EthernetRemoteInstall==0 ? Label00_EthIf->setText(tr("disabled")) : Label00_EthIf->setText(tr("enabled"));

        Text01_EthIf->move(QPoint(16,20));
        Label01_EthIf->move(QPoint(208,20));
        Text01_EthIf->setText(tr("Interface ID:"));
        Label01_EthIf->setText(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].InterfaceID);

        Text02_EthIf->move(QPoint(16,36));
        Label02_EthIf->move(QPoint(208,36));
        Text02_EthIf->setText(tr("IP adress:"));
        Label02_EthIf->setText(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].IpAddress);

        Text03_EthIf->move(QPoint(16,52));
        Label03_EthIf->move(QPoint(208,52));
        Text03_EthIf->setText(tr("SubNet mask:"));
        Label03_EthIf->setText(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].SubnetMask);

        Text04_EthIf->move(QPoint(16,68));
        Label04_EthIf->move(QPoint(208,68));
        Text04_EthIf->setText(tr("IP configuration:"));
        (this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].IpConfig < this->list_IpConfiguration.count()) ? Label04_EthIf->setText(this->list_IpConfiguration.at(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].IpConfig)) : Label04_EthIf->setText(QString::number(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].IpConfig));

        Text05_EthIf->move(QPoint(16,84));
        Label05_EthIf->move(QPoint(208,84));
        Text05_EthIf->setText(tr("SNMP configuration:"));
        (this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].SNMPConfig < this->list_SNMPConfiguration.count()) ? Label05_EthIf->setText(this->list_SNMPConfiguration.at(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].SNMPConfig)) : Label05_EthIf->setText(QString::number(this->SysDump.Sections.Ethernet.vInterface[static_cast<unsigned int>(index)].SNMPConfig));

        EthIF->show();
        ui->tabWidget_EthernetInterfaces->addTab(EthIF,names);
    }
    return 0;
}

int MainWindow::add_Hardware(){
    string actualPath = "";

    unsigned int i = 0;

    // reset harware error
    this->SysDump.Sections.Hardware.HwError = false;
    // add items
    while(i<this->SysDump.Sections.Hardware.vNode.size()){
        // Get path from actual element
        actualPath = this->SysDump.Sections.Hardware.vNode[i].IOInformation.ModulePath.toStdString();
        this->add_Hardware_Item(&i,&actualPath,ui->tree_Hardware);
        i++;
    }

    // Iterate through all Tree-Items and set ErrIcon if the itempath matches an Error Path
    QTreeWidgetItemIterator it(ui->tree_Hardware);
    QString ErrPath;

    while(*it){

        QTreeWidgetItem* item = *it;

        for(unsigned int i=0;i<this->vHardwareErrorPaths.size();i++){
            ErrPath.clear();
            for(int j=0;j<this->vHardwareErrorPaths[i].count()-1;j++){
                (j==0) ? ErrPath.append(this->vHardwareErrorPaths[i][j]) : ErrPath.append(QString(".")+this->vHardwareErrorPaths[i][j]) ;
                if((j==this->vHardwareErrorPaths[i].count()-1) && (ErrPath == item->data(0,Qt::UserRole+1))){
                    item->setIcon(0,QIcon(QPixmap("://images/hwtr_err.png")));
                } else if(ErrPath == item->data(0,Qt::UserRole+1)) {
                    // Check if the node itself has an error
                    if (item->data(0,Qt::UserRole+2) == 0 || item->data(0,Qt::UserRole+2) == 1 || item->data(0,Qt::UserRole+2) == 5 ){
                        item->setIcon(0,QIcon(QPixmap("://images/hwtr_okatt.png")));
                    } else {
                        item->setIcon(0,QIcon(QPixmap("://images/hwtr_erratt.png")));
                    }
                }
            }
        }
        ++it;
    }


    return 0;
}

// Attention!!! :: Both functions execute exactly the same code!!! Just the last parameter-type is different!!!
// The reason is, that QTreeWidgetItem does not inherit from QObject! So it is not possible to
// pass the main class... -.-
// ------------------------------------------------------------------------------------------------------->>>
    int MainWindow::add_Hardware_Item(unsigned int* index, string* path, QTreeWidgetItem *parent){

        // Because of the Systemdump contains the Hardware in a simple list, but we want to have a topological overview,
        // every tree item gets its index as UserRole and its module Path in UserRole+1. So later if we click on the item,
        // we can identify the correct vector item via the UUID
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(parent);
        QVariant UUID = *index;
        treeItem->setText(0, this->SysDump.Sections.Hardware.vNode[*index].ModuleStatus.Configured);
        treeItem->setData(0,Qt::UserRole, UUID);
        treeItem->setData(0,Qt::UserRole+1,this->SysDump.Sections.Hardware.vNode[(*index)].IOInformation.ModulePath);
        treeItem->setData(0,Qt::UserRole+2,this->SysDump.Sections.Hardware.vNode[(*index)].Status);

        // Set own icon
        // Status 0 / 1:    Configured = Plugged (ModuleOk 1/2) -> "check"
        // Status 2:        not plugged -> "cross"
        // Status 5:        Something Plugged / not configured -> "question"
        if(this->SysDump.Sections.Hardware.vNode[*index].Status == 0 || this->SysDump.Sections.Hardware.vNode[*index].Status == 1){
            treeItem->setIcon(0,QIcon(QPixmap("://images/hwtr_ok.png")));
        } else if(this->SysDump.Sections.Hardware.vNode[*index].Status == 8 && this->SysDump.Sections.Hardware.vNode[*index].ModuleStatus.Configured == "X20ZF0000"){
            treeItem->setIcon(0,QIcon(QPixmap("://images/hwtr_ok.png")));
        } else if(this->SysDump.Sections.Hardware.vNode[*index].Status == 2 && !(this->SysDump.Sections.Hardware.vNode[*index].ModuleStatus.Configured == "not configured")) {
            treeItem->setIcon(0,QIcon(QPixmap("://images/hwtr_err.png")));
            this->SysDump.Sections.Hardware.HwError = true;
            this->vHardwareErrorPaths.push_back(QStringList());
            this->vHardwareErrorPaths[this->vHardwareErrorPaths.size()-1] = this->SysDump.Sections.Hardware.vNode[(*index)].IOInformation.ModulePath.split(QString("."));            
        } else {
            this->SysDump.Sections.Hardware.HwError = true;
            treeItem->setIcon(0,QIcon(QPixmap("://images/hwtr_warn.png")));
        }

        // Search for childs
        if((*index)+1 < this->SysDump.Sections.Hardware.vNode.size()){

            string tmpString = this->SysDump.Sections.Hardware.vNode[(*index)+1].IOInformation.ModulePath.toStdString();

            while(tmpString.find(*path+".") == 0 && (path->empty()) == 0){

                (*index)++;

                this->add_Hardware_Item(index,&tmpString,treeItem);

                if((*index)+1 < this->SysDump.Sections.Hardware.vNode.size()){
                    tmpString = this->SysDump.Sections.Hardware.vNode[(*index)+1].IOInformation.ModulePath.toStdString();
                } else {
                    return 0;
                }
            }

            return 0;

        } else {

            return 0;
        }
    }

// Second function with same body!!
    int MainWindow::add_Hardware_Item(unsigned int* index, string* path, QTreeWidget *parent){

        QTreeWidgetItem *treeItem = new QTreeWidgetItem(parent);
        QVariant UUID = *index;
        treeItem->setText(0, this->SysDump.Sections.Hardware.vNode[*index].ModuleStatus.Configured);
        treeItem->setData(0,Qt::UserRole, UUID);
        treeItem->setData(0,Qt::UserRole+1,this->SysDump.Sections.Hardware.vNode[(*index)].IOInformation.ModulePath);
        treeItem->setData(0,Qt::UserRole+2,this->SysDump.Sections.Hardware.vNode[(*index)].Status);

        // Set own icon
        // Status 0 / 1:    Configured = Plugged (ModuleOk 1/2) -> "check"
        // Status 2:        not plugged -> "cross"
        // Status 5:        Something Plugged / not configured -> "question"
        if(this->SysDump.Sections.Hardware.vNode[*index].Status == 0 || this->SysDump.Sections.Hardware.vNode[*index].Status == 1){
            treeItem->setIcon(0,QIcon(QPixmap("://images/hwtr_ok.png")));
        } else if(this->SysDump.Sections.Hardware.vNode[*index].Status == 8 && this->SysDump.Sections.Hardware.vNode[*index].ModuleStatus.Configured == "X20ZF0000"){
            treeItem->setIcon(0,QIcon(QPixmap("://images/hwtr_ok.png")));
        } else if(this->SysDump.Sections.Hardware.vNode[*index].Status == 2 && !(this->SysDump.Sections.Hardware.vNode[*index].ModuleStatus.Configured == "not configured")) {
            treeItem->setIcon(0,QIcon(QPixmap("://images/hwtr_err.png")));
            this->SysDump.Sections.Hardware.HwError = true;
            this->vHardwareErrorPaths.push_back(QStringList());
            this->vHardwareErrorPaths[this->vHardwareErrorPaths.size()-1] = this->SysDump.Sections.Hardware.vNode[(*index)].IOInformation.ModulePath.split(QString("."));
        } else {
            this->SysDump.Sections.Hardware.HwError = true;
            treeItem->setIcon(0,QIcon(QPixmap("://images/hwtr_warn.png")));
        }

        // Search for childs
        if((*index)+1 < this->SysDump.Sections.Hardware.vNode.size()){

            string tmpString = this->SysDump.Sections.Hardware.vNode[(*index)+1].IOInformation.ModulePath.toStdString();

            while(tmpString.find(*path+".") == 0 && (path->empty()) == 0){

                (*index)++;

                this->add_Hardware_Item(index,&tmpString,treeItem);

                if((*index)+1 < this->SysDump.Sections.Hardware.vNode.size()){
                    tmpString = this->SysDump.Sections.Hardware.vNode[(*index)+1].IOInformation.ModulePath.toStdString();
                } else {
                    return 0;
                }
            }

            return 0;

        } else {

            return 0;
        }
    }
// <<<-------------------------------------------------------------------------------------------------------
// END OF CRAP

int MainWindow::get_HardwareInformation(int uuid){
    // Set the hardware information
    ui->label_Hardware_Configured->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].ModuleStatus.Configured);
    ui->label_Hardware_EquipmentID->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].IOInformation.EquipmentID);
    ui->label_Hardware_Firmware->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].IOInformation.FwVersion);
    ui->label_Hardware_HwRevision->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].IOInformation.HardwareRevision);
    ui->label_Hardware_HwVariant->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].IOInformation.HardwareVariant);
    ui->label_Hardware_ModuleOk->setText(QString::number(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].ModuleStatus.ModuleOk));
    //(this->SysDump.Sections.Hardware.vNode[uuid].ModuleStatus.ModuleOk < this->list_ModuleOk.count()) ? ui->label_Hardware_ModuleOk->setText(this->list_ModuleOk.at(this->SysDump.Sections.Hardware.vNode[uuid].ModuleStatus.ModuleOk)) : ui->label_Hardware_ModuleOk->setText(QString::number(this->SysDump.Sections.Hardware.vNode[uuid].ModuleStatus.ModuleOk));
    ui->label_Hardware_ModulePath->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].IOInformation.ModulePath);
    ui->label_Hardware_Plugged->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].ModuleStatus.Plugged);
    ui->label_Hardware_Supervised->setText(QString::number(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].ModuleStatus.Supervised));
    ui->label_Hardware_Serialnumber->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].IOInformation.Serialnumber);

    // Clear channel information (is set when a channel is selected)
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

    // Clear old channel tree
    ui->tree_Hardware_Channels->clear();

    // add the channel information
    for(unsigned int i=0;i<this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(uuid)].IOChannels.vChannel.size();i++){
        this->add_Hardware_Channel(uuid,&i);
    }
    return 0;
}

int MainWindow::add_Hardware_Channel(int HwNode, unsigned int *index){
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->tree_Hardware_Channels);
    QVariant UUID = *index;
    treeItem->setText(0, this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[*index].Name);
    treeItem->setData(0,Qt::UserRole, UUID);
    return 0;
}

int MainWindow::get_HardwareChannelInformation(int HwNode, int uuid){
    if(static_cast<unsigned int>(uuid) < this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel.size()){
        ui->label_Hardware_Channel_Diagnose->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].Diagnose);
        ui->label_Hardware_Channel_ForceStatus->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].ForceStatus);
        ui->label_Hardware_Channel_ForceValue->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].ForceValue);
        ui->label_Hardware_Channel_ID->setText(QString::number(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].ID));
        ui->label_Hardware_Channel_IECType->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].IECType);
        ui->label_Hardware_Channel_IECValue->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].IECValue);
        ui->label_Hardware_Channel_Name->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].Name);
        ui->label_Hardware_Channel_PhysicalValue->setText(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].PhysicalValue);
        (this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].Type < this->list_ChannelType.count()) ? ui->label_Hardware_Channel_Type->setText(this->list_ChannelType.at(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].Type)) : ui->label_Hardware_Channel_Type->setText(QString::number(this->SysDump.Sections.Hardware.vNode[static_cast<unsigned int>(HwNode)].IOChannels.vChannel[static_cast<unsigned int>(uuid)].Type));
    }
    return 0;
}

int MainWindow::add_PartitionBar(int x, int y, int index){

    // Fonts
    QFont header( "MS Shell Dlg 2", 10, QFont::Bold);
    QFont normal( "MS Shell Dlg 2", 8, QFont::Normal);
    int width = static_cast<int>(0.8*ui->group_Memory_Partitions->width());

    // Create labels
    QLabel *partitionName = new QLabel(this);
    QLabel *partitionSizeText = new QLabel(this);
    QLabel *partitionSize = new QLabel(this);
    QLabel *partitionAvailableText = new QLabel(this);
    QLabel *partitionAvailable = new QLabel(this);
    QLabel *partitionUsedText = new QLabel(this);
    QLabel *partitionUsed = new QLabel(this);

    // Header
    partitionName->setFont(header);
    partitionName->setText(this->SysDump.Sections.Memory.CompactFlash.vPartition[static_cast<unsigned int>(index)].id + QString(": (") + this->SysDump.Sections.Memory.CompactFlash.vPartition[static_cast<unsigned int>(index)].description + QString(")"));
    partitionName->setGeometry(x,y,PARTITION_LABEL_WIDTH,PARTITION_LABEL_HEIGHT);

    // Bargraph of used size
    QProgressBar *pBar = new QProgressBar(this);
    pBar->setMinimum(0);
    pBar->setMaximum(100); // 0 - 100%
    int tmpValue = static_cast<int>((this->SysDump.Sections.Memory.CompactFlash.vPartition[static_cast<unsigned int>(index)].used*100/this->SysDump.Sections.Memory.CompactFlash.vPartition[static_cast<unsigned int>(index)].size));
    pBar->setValue(tmpValue);
    pBar->setGeometry(x,(y+PARTITION_LABEL_HEIGHT+5),width,30);

    // Size label / text
    partitionSizeText->setFont(normal);
    partitionSizeText->setText(tr("Size:"));
    partitionSizeText->setGeometry(x,(y+PARTITION_LABEL_HEIGHT+35),89,PARTITION_LABEL_HEIGHT);
    partitionSize->setFont(normal);
    partitionSize->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    writeByteToLabel(partitionSize,this->SysDump.Sections.Memory.CompactFlash.vPartition[static_cast<unsigned int>(index)].size);
    partitionSize->setGeometry((x+PARTITION_LABEL_WIDTH),(y+PARTITION_LABEL_HEIGHT+35),62,PARTITION_LABEL_HEIGHT);

    // Used label / text
    partitionUsedText->setFont(normal);
    partitionUsedText->setText(tr("Used:"));
    partitionUsedText->setGeometry(x,(y+2*PARTITION_LABEL_HEIGHT+35),89,PARTITION_LABEL_HEIGHT);
    partitionUsed->setFont(normal);
    partitionUsed->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    writeByteToLabel(partitionUsed,this->SysDump.Sections.Memory.CompactFlash.vPartition[static_cast<unsigned int>(index)].used);
    partitionUsed->setGeometry((x+PARTITION_LABEL_WIDTH),(y+2*PARTITION_LABEL_HEIGHT+35),62,PARTITION_LABEL_HEIGHT);

    // Available label / text
    partitionAvailableText->setFont(normal);
    partitionAvailableText->setText(tr("Available:"));
    partitionAvailableText->setGeometry(x,(y+3*PARTITION_LABEL_HEIGHT+35),89,PARTITION_LABEL_HEIGHT);
    partitionAvailable->setFont(normal);
    partitionAvailable->setAlignment(Qt::AlignRight | Qt::AlignCenter);
    writeByteToLabel(partitionAvailable,this->SysDump.Sections.Memory.CompactFlash.vPartition[static_cast<unsigned int>(index)].available);
    partitionAvailable->setGeometry((x+PARTITION_LABEL_WIDTH),(y+3*PARTITION_LABEL_HEIGHT+35),62,PARTITION_LABEL_HEIGHT);

    // Add the widgets to the grid layout (has ScrollArea!)
    ui->gridLayoutPartitions->addWidget(partitionName,(index*5)+1,1,1,2);
    ui->gridLayoutPartitions->addWidget(pBar,(index*5)+2,1,1,2);
    ui->gridLayoutPartitions->addWidget(partitionSizeText,(index*5)+3,1,1,1);
    ui->gridLayoutPartitions->addWidget(partitionSize,(index*5)+3,2,1,1);
    ui->gridLayoutPartitions->addWidget(partitionAvailableText,(index*5)+4,1,1,1);
    ui->gridLayoutPartitions->addWidget(partitionAvailable,(index*5)+4,2,1,1);
    ui->gridLayoutPartitions->addWidget(partitionUsedText,(index*5)+5,1,1,1);
    ui->gridLayoutPartitions->addWidget(partitionUsed,(index*5)+5,2,1,1);

    return 0;
}

int MainWindow::add_ApplicationModule(unsigned int index){
    if(index < this->SysDump.Sections.Software.vAppModule.size()){
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui->tree_ApplicationModules);
        QVariant UUID = index;
        treeItem->setText(0, this->SysDump.Sections.Software.vAppModule[index].id);
        treeItem->setData(0,Qt::UserRole, UUID);
    }
    return 0;
}

int MainWindow::get_SoftwareModules(int uuid, int filter){
    if(static_cast<unsigned int>(uuid) < this->SysDump.Sections.Software.vAppModule.size()){
        // Remove old stuff
        ui->table_Software->setRowCount(0);
        int ridx = 0;

        // Add new stuff
        for(unsigned int i = 0;i<this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules.size();i++){

            // Check if filter is off, or ID applies to filter
            if(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].type == filter || filter == 0){

                ui->table_Software->insertRow(ridx);
                ui->table_Software->setRowHeight(ridx,TABLE_ROW_HEIGHT);

                for(int cidx=0;cidx<SOFTWARE_TABLE_COLUMNS;cidx++){
                    QTableWidgetItem* item = new QTableWidgetItem();

                    switch(cidx){
                        case 0:
                            item->setText(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].name);
                            break;

                        case 1:
                            item->setText(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].version);
                            break;

                        case 2:
                            item->setText(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].timestamp);
                            break;

                        case 3:
                            if(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].type==1)
                                item->setText(tr("Cyclic object"));
                            if(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].type==2)
                                item->setText(tr("Data object"));
                            if(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].type==3)
                                item->setText(tr("Motion object"));
                            if(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].type==4)
                                item->setText(tr("Config object"));
                            if(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].type==5)
                                item->setText(tr("Other object"));
                            break;

                        case 4:
                            item->setText(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].memory);
                            break;

                        case 5:
                            item->setText(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].address);
                            break;

                        case 6:
                            item->setText(QString::number(this->SysDump.Sections.Software.vAppModule[static_cast<unsigned int>(uuid)].vModules[i].size));
                            break;

                    default:
                        break;
                    }
                    ui->table_Software->setItem(ridx,cidx,item);
                }
                ridx++;
            }
        }
    }

    return 0;
}

int MainWindow::add_TaskClass(unsigned int index){

    QPixmap cyclePixmap("://images/cycleClass.png");
    QLabel *cyclePmLabel = new QLabel(this);
    cyclePmLabel->setPixmap(cyclePixmap.scaledToWidth(20));
    cyclePmLabel->setAlignment(Qt::AlignCenter);
    cyclePmLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");

    ui->table_TaskClasses->insertRow(static_cast<int>(index));
    ui->table_TaskClasses->setRowHeight(static_cast<int>(index),TABLE_ROW_HEIGHT);

    for(int cidx=0;cidx<TASK_CLASS_TABLE_COLUMNS;cidx++){
        QTableWidgetItem* item = new QTableWidgetItem();

        switch(cidx){
            case 0:
                ui->table_TaskClasses->setCellWidget(static_cast<int>(index),cidx,cyclePmLabel);
                break;

            case 1:
                item->setText(tr("Cyclic #")+QString::number(this->SysDump.Sections.Timing.vTaskClass[index].Cyclic));
                break;

            case 2:
                item->setText(QString::number(this->SysDump.Sections.Timing.vTaskClass[index].CycleTime/1000.0,'f',1)+tr(" ms"));
                break;

            case 3:
                item->setText(QString::number(this->SysDump.Sections.Timing.vTaskClass[index].Tolerance/1000.0,'f',1)+tr(" ms"));
                break;

            case 4:
                item->setText(QString::number(this->SysDump.Sections.Timing.vTaskClass[index].Stack)+tr(" Bytes"));
                break;

            case 5:
                item->setText(this->SysDump.Sections.Timing.vTaskClass[index].InputDelay);
                break;

            case 6:
                item->setText(this->SysDump.Sections.Timing.vTaskClass[index].OutputDelay);
                break;

        default:
            break;
        }
        ui->table_TaskClasses->setItem(static_cast<int>(index),cidx,item);
    }

    return 0;
}

int MainWindow::get_LoggerModules(){

    QStringList labels;

    // Delete old
    ui->table_Logger->setRowCount(0);

    // DEPENDING ON VERSION OF THE LOGGER, DIFFERENT COLUMNS ARE SET
    // FIRST LOGGER DECIDES VERSION FOR ALL!! NO DIFFERENT VERSIONS SUPPORTED AT THIS TIME!!
    if(this->SysDump.Sections.Logger.vModule.size()>0){
        if(this->SysDump.Sections.Logger.vModule[0].Version=="1.00.0"){

            // Get version of Automation Runtime (old Logger <> EventLogging)
            labels << tr("Lvl") << tr("Date/Time") << tr("Error nr.") << tr("Logger") << tr("Event-ID") << tr("Object-ID") << tr("OS Task") << tr("Description") << tr("ASCII Data");

            ui->table_Logger->setColumnCount(LOGGER_TABLE_COLUMNS);

            ui->table_Logger->setColumnWidth(LOGGER_IMAGE_COLUMN,30);
            ui->table_Logger->setColumnWidth(LOGGER_DATEX_COLUMN,140);

            ui->table_Logger->setColumnWidth(LOGGER_ERRNO_COLUMN,60);
            ui->table_Logger->setColumnHidden(LOGGER_ERRNO_COLUMN,this->SysDump.Sections.Values.SoftwareVersions.AR.mid(1,3).toDouble() >= 4.10);

            ui->table_Logger->setColumnWidth(LOGGER_LOGGR_COLUMN,60);
            ui->table_Logger->setColumnHidden(LOGGER_LOGGR_COLUMN,this->SysDump.Sections.Values.SoftwareVersions.AR.mid(1,3).toDouble() >= 4.10);

            ui->table_Logger->setColumnWidth(LOGGER_EVENT_COLUMN,100);
            ui->table_Logger->setColumnHidden(LOGGER_EVENT_COLUMN,this->SysDump.Sections.Values.SoftwareVersions.AR.mid(1,3).toDouble() < 4.10);

            ui->table_Logger->setColumnWidth(LOGGER_OBJCT_COLUMN,100);
            ui->table_Logger->setColumnHidden(LOGGER_OBJCT_COLUMN,this->SysDump.Sections.Values.SoftwareVersions.AR.mid(1,3).toDouble() < 4.10);

            ui->table_Logger->setColumnWidth(LOGGER_OSTSK_COLUMN,100);
            ui->table_Logger->setColumnHidden(LOGGER_OSTSK_COLUMN,this->SysDump.Sections.Values.SoftwareVersions.AR.mid(1,3).toDouble() >= 4.10);

            ui->table_Logger->setColumnWidth(LOGGER_DESCR_COLUMN,350);
            ui->table_Logger->setColumnWidth(LOGGER_ASCII_COLUMN,350);
            ui->table_Logger->setColumnWidth(LOGGER_LEVEL_COLUMN,30);
            ui->table_Logger->setColumnHidden(LOGGER_LEVEL_COLUMN,true);

        } else {

            // Get version of Automation Runtime (old Logger <> EventLogging)
            labels << tr("Lvl") << tr("Date/Time") << tr("Fac.") << tr("Logger") << tr("Event-ID") << tr("Entered by") << tr("Description") << tr("ASCII Data") << tr("Binary Data") << tr("Info") << tr("Origin Record");

            ui->table_Logger->setColumnCount(LOGGER_V2_TABLE_COLUMNS);

            ui->table_Logger->setColumnWidth(LOGGER_V2_IMAGE_COLUMN,30);
            ui->table_Logger->setColumnHidden(LOGGER_V2_IMAGE_COLUMN,false);
            ui->table_Logger->setColumnWidth(LOGGER_V2_DATEX_COLUMN,140);
            ui->table_Logger->setColumnHidden(LOGGER_V2_DATEX_COLUMN,false);
            ui->table_Logger->setColumnWidth(LOGGER_V2_FACIL_COLUMN,40);
            ui->table_Logger->setColumnHidden(LOGGER_V2_FACIL_COLUMN,false);
            ui->table_Logger->setColumnWidth(LOGGER_V2_LOGGR_COLUMN,60);
            ui->table_Logger->setColumnHidden(LOGGER_V2_LOGGR_COLUMN,false);
            ui->table_Logger->setColumnWidth(LOGGER_V2_EVTID_COLUMN,100);
            ui->table_Logger->setColumnHidden(LOGGER_V2_EVTID_COLUMN,false);
            ui->table_Logger->setColumnWidth(LOGGER_V2_ENTBY_COLUMN,100);
            ui->table_Logger->setColumnHidden(LOGGER_V2_ENTBY_COLUMN,false);
            ui->table_Logger->setColumnWidth(LOGGER_V2_DESCR_COLUMN,350);
            ui->table_Logger->setColumnHidden(LOGGER_V2_DESCR_COLUMN,false);
            ui->table_Logger->setColumnWidth(LOGGER_V2_ASCII_COLUMN,350);
            ui->table_Logger->setColumnHidden(LOGGER_V2_ASCII_COLUMN,false);
            ui->table_Logger->setColumnWidth(LOGGER_V2_BINAR_COLUMN,100);
            ui->table_Logger->setColumnHidden(LOGGER_V2_BINAR_COLUMN,false);
            ui->table_Logger->setColumnWidth(LOGGER_V2_INFOS_COLUMN,60);
            ui->table_Logger->setColumnHidden(LOGGER_V2_INFOS_COLUMN,false);
            ui->table_Logger->setColumnWidth(LOGGER_V2_ORIGN_COLUMN,60);
            ui->table_Logger->setColumnHidden(LOGGER_V2_ORIGN_COLUMN,false);
        }
    } else {
        // No Logger entries!
        ui->table_Logger->setColumnCount(0);
    }

    ui->table_Logger->setHorizontalHeaderLabels(labels);
    ui->table_Logger->verticalHeader()->hide();
    ui->table_Logger->setShowGrid(true);

    this->loggerClickedColumnOld = LOGGER_TABLE_TIMEDATE;
    this->loggerSortOrder = 1;

    // Remove all old checkboxes
    qDeleteAll(ui->grp_LoggerModules->findChildren<QCheckBox*>());
    // Clear vector
    this->vLogger_Checkbox.clear();

    // Create one checkbox for each LoggerModule found in the xml-File
    for(unsigned int i=0;i<this->SysDump.Sections.Logger.vModule.size();i++){

        this->vLogger_Checkbox.push_back(new QCheckBox(this->SysDump.Sections.Logger.vModule[i].Name));
        //this->vLogger_Checkbox[i]->setGeometry(10,8+(i*20),ui->scrollArea_LogModuleLayout->width()-10,20);
        //this->vLogger_Checkbox[i]->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
        ui->vertLayoutLogger->addWidget(this->vLogger_Checkbox[i]);
        connect(this->vLogger_Checkbox[i], SIGNAL(toggled(bool)), this, SLOT(on_logger_checkbox_toggled()));
        this->vLogger_Checkbox[i]->show();
    }   

    return 0;
}

int MainWindow::add_LoggerModules(){

    QPixmap level0("://images/level0.png");
    QPixmap level1("://images/level1.png");
    QPixmap level2("://images/level2.png");
    QPixmap level3("://images/level3.png");

    ui->table_Logger->setRowCount(0);

    // DEPENDING ON VERSION OF THE LOGGER, DIFFERENT COLUMNS ARE SET
    // FIRST LOGGER DECIDES VERSION FOR ALL!! NO DIFFERENT VERSIONS SUPPORTED AT THIS TIME!!
    if(this->SysDump.Sections.Logger.vModule.size()>0){
        if(this->SysDump.Sections.Logger.vModule[0].Version=="1.00.0" || this->SysDump.Sections.Logger.vModule[0].Version=="1.01.0"){

            // Delete old stuff
            this->vLogSortView.clear();

            // In this function: Loop through all Logger modules and check if the checkbox is selected,
            // if yes copy the the entries into the output vector and then sort the outputvector by date.
            // .. perh. implement a sorting algorithm
            for(unsigned int i=0;i<this->SysDump.Sections.Logger.vModule.size();i++){
                if(this->vLogger_Checkbox[i]->checkState()==2){
                    for(unsigned int u=0;u<this->SysDump.Sections.Logger.vModule[i].vEntry.size();u++){
                        this->vLogSortView.push_back(this->SysDump.Sections.Logger.vModule[i].vEntry[u]);
                    }
                }
            }

            // Could be deleted, sorting is done by TableView
            // Just for nicer initial view
            std::sort(this->vLogSortView.begin(), this->vLogSortView.end(), this->sort_LogSortByTimeStampRev);

            for(unsigned int j=0;j<this->vLogSortView.size();j++){
                ui->table_Logger->insertRow(static_cast<int>(j));
                ui->table_Logger->setRowHeight(static_cast<int>(j),TABLE_ROW_HEIGHT);

                for(int cidx=0;cidx<LOGGER_TABLE_COLUMNS;cidx++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    QLabel *levelLabel = new QLabel(this);

                    switch(cidx){
                        case LOGGER_IMAGE_COLUMN:
                            if(this->vLogSortView[j].Level==1) levelLabel->setPixmap(level1);
                            if(this->vLogSortView[j].Level==2) levelLabel->setPixmap(level2);
                            if(this->vLogSortView[j].Level==3) levelLabel->setPixmap(level3);
                            levelLabel->setAlignment(Qt::AlignCenter);
                            levelLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
                            ui->table_Logger->setCellWidget(static_cast<int>(j),cidx,levelLabel);
                            break;

                        case LOGGER_DATEX_COLUMN:
                            item->setText(this->vLogSortView[j].TimeStamp);
                            break;

                        case LOGGER_EVENT_COLUMN:
                        case LOGGER_ERRNO_COLUMN:
                            item->setData(0,this->vLogSortView[j].ErrorNr);
                            break;

                        case LOGGER_LOGGR_COLUMN:
                            item->setText(this->vLogSortView[j].Logger);
                            break;

                        case LOGGER_OSTSK_COLUMN:
                            item->setText(this->vLogSortView[j].OSTask);
                            break;

                        case LOGGER_OBJCT_COLUMN:

                            break;

                        case LOGGER_DESCR_COLUMN:
                            item->setText(this->vLogSortView[j].ErrorDescription);
                            break;

                        case LOGGER_ASCII_COLUMN:
                            item->setText(this->vLogSortView[j].ASCIIData);
                            break;

                        case LOGGER_LEVEL_COLUMN:
                            item->setData(0,this->vLogSortView[j].Level);
                            break;

                    default:
                        break;
                    }
                    item->setData(Qt::UserRole, j);
                    ui->table_Logger->setItem(static_cast<int>(j),cidx,item);

                } // FOR COLUMNS

            } // FOR LOGSORTVIEW SIZE

        } else {

            // Delete old stuff
            this->vLogSortViewV2.clear();

            // In this function: Loop through all Logger modules and check if the checkbox is selected,
            // if yes copy the the entries into the output vector and then sort the outputvector by date.
            // .. perh. implement a sorting algorithm
            for(unsigned int i=0;i<this->SysDump.Sections.Logger.vModule.size();i++){
                if(this->vLogger_Checkbox[i]->checkState()==2){
                    for(unsigned int u=0;u<this->SysDump.Sections.Logger.vModule[i].vEntryV2.size();u++){
                        this->vLogSortViewV2.push_back(this->SysDump.Sections.Logger.vModule[i].vEntryV2[u]);
                    }
                }
            }

            // Could be deleted, sorting is done by TableView
            // Just for nicer initial view
            //std::sort(this->vLogSortViewV2.begin(), this->vLogSortViewV2.end(), this->sort_LogSortByTimeStampRevV2);

            for(unsigned int j=0;j<this->vLogSortViewV2.size();j++){

                ui->table_Logger->insertRow(static_cast<int>(j));
                ui->table_Logger->setRowHeight(static_cast<int>(j),TABLE_ROW_HEIGHT);

                for(int cidx=0;cidx<LOGGER_V2_TABLE_COLUMNS;cidx++){
                    QTableWidgetItem* item = new QTableWidgetItem();
                    QLabel *levelLabel = new QLabel(this);

                    switch(cidx){
                        case LOGGER_V2_IMAGE_COLUMN:
                            if(this->vLogSortViewV2[j].Severity==0) levelLabel->setPixmap(level0);
                            if(this->vLogSortViewV2[j].Severity==1) levelLabel->setPixmap(level1);
                            if(this->vLogSortViewV2[j].Severity==2) levelLabel->setPixmap(level2);
                            if(this->vLogSortViewV2[j].Severity==3) levelLabel->setPixmap(level3);
                            levelLabel->setAlignment(Qt::AlignCenter);
                            levelLabel->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
                            ui->table_Logger->setCellWidget(static_cast<int>(j),cidx,levelLabel);
                            break;

                        case LOGGER_V2_DATEX_COLUMN:
                            item->setText(this->vLogSortViewV2[j].TimeStamp);
                            break;

                        case LOGGER_V2_FACIL_COLUMN:
                            item->setData(0,this->vLogSortViewV2[j].Facility);
                            break;

                        case LOGGER_V2_LOGGR_COLUMN:
                            item->setText(this->vLogSortViewV2[j].Logger);
                            break;

                        case LOGGER_V2_EVTID_COLUMN:
                            item->setText(this->vLogSortViewV2[j].EventID);
                            break;

                        case LOGGER_V2_ENTBY_COLUMN:
                            item->setText(this->vLogSortViewV2[j].EnteredBy);
                            break;

                        case LOGGER_V2_DESCR_COLUMN:
                            item->setText(this->vLogSortViewV2[j].Description);
                            break;

                        case LOGGER_V2_ASCII_COLUMN:
                            item->setText(this->vLogSortViewV2[j].ASCIIData);
                            break;

                        case LOGGER_V2_BINAR_COLUMN:
                            item->setText(this->vLogSortViewV2[j].BinaryData);
                            break;

                        case LOGGER_V2_INFOS_COLUMN:
                            item->setText(this->vLogSortViewV2[j].Info);
                            break;

                        case LOGGER_V2_ORIGN_COLUMN:
                            item->setData(0,this->vLogSortViewV2[j].OriginRecID);
                            break;

                    default:
                        break;
                    }
                    item->setData(Qt::UserRole, j);
                    ui->table_Logger->setItem(static_cast<int>(j),cidx,item);

                } // FOR COLUMNS

            } // FOR LOGSORTVIEW SIZE

        } // IF NEW /OLD LOGGER

    } // IF NO LOGGER THERE

    //ui->table_Logger->sortByColumn(1,Qt::AscendingOrder);

    return 0;
}

int MainWindow::draw_CpuUsage(){

    int index=-1;

    for(unsigned int i=0;i<this->SysDump.Sections.CpuUsage.vZoomInterval.size();i++){
        if(this->SysDump.Sections.CpuUsage.vZoomInterval[i].description == ui->combo_CpuUsage->currentText())
            index = static_cast<int>(i);
    }

    if(index >= 0){
        // Display Information
        ui->label_CPUUsage_Average->setText(QString::number(this->SysDump.Sections.CpuUsage.vZoomInterval[static_cast<unsigned int>(index)].average)+tr(" %"));
        ui->label_CPUUsage_Maximum->setText(QString::number(this->SysDump.Sections.CpuUsage.vZoomInterval[static_cast<unsigned int>(index)].maximum)+tr(" %"));
        ui->label_CPUUsage_Id->setText(QString::number(this->SysDump.Sections.CpuUsage.vZoomInterval[static_cast<unsigned int>(index)].id));

        qDeleteAll(ui->tab_CPUUsage->findChildren<QwtPlot*>());

        QwtText Average;
        Average.setText(tr("Average"));
        QwtText Maximum;
        Maximum.setText(tr("Maximum"));

        QwtPlot *qwt_cpuUsage = new QwtPlot(ui->tab_CPUUsage);
        QwtLegend *legend = new QwtLegend();
        QwtPlotGrid *grid = new QwtPlotGrid();
        grid->setPen(Qt::gray);
        grid->attach(qwt_cpuUsage);

        // Plot Configuration
        qwt_cpuUsage->setMouseTracking(true);
        qwt_cpuUsage->setGeometry(15,100,660,330);
        qwt_cpuUsage->setAxisScale(QwtPlot::yLeft,0.0,100.0,20);
        qwt_cpuUsage->setAxisTitle(QwtPlot::xBottom,"Time");
        qwt_cpuUsage->setAxisTitle(QwtPlot::yLeft,"CPU Usage [%]");
        qwt_cpuUsage->setCanvasBackground( Qt::white );

        if (this->SysDump.Sections.CpuUsage.vZoomInterval[static_cast<unsigned int>(index)].values.vAverage.size()>0){
            QwtPlotCurve *curveAverage = new QwtPlotCurve(Average);

            double *x = new double[200*sizeof(double)];
            double *y = new double[200*sizeof(double)];

            for(unsigned int i=0;i<this->SysDump.Sections.CpuUsage.vZoomInterval[static_cast<unsigned int>(index)].values.vAverage.size();i++){
                x[i] = static_cast<double>(200-(i+1));
                y[i] = static_cast<double>(this->SysDump.Sections.CpuUsage.vZoomInterval[static_cast<unsigned int>(index)].values.vAverage[i]);
            }

            curveAverage->setRawSamples(x, y, 200);
            curveAverage->setPen(QPen("#ff8800"));
            curveAverage->attach(qwt_cpuUsage);
        } else {

        }

        if (this->SysDump.Sections.CpuUsage.vZoomInterval[static_cast<unsigned int>(index)].values.vMaximum.size()>0){
            QwtPlotCurve *curveMaximum = new QwtPlotCurve(Maximum);

            double *xM = new double[200*sizeof(double)];
            double *yM = new double[200*sizeof(double)];

            for(unsigned int i=0;i<this->SysDump.Sections.CpuUsage.vZoomInterval[static_cast<unsigned int>(index)].values.vMaximum.size();i++){
                xM[i] = static_cast<double>(200-(i+1));
                yM[i] = static_cast<double>(this->SysDump.Sections.CpuUsage.vZoomInterval[static_cast<unsigned int>(index)].values.vMaximum[i]);
            }

            curveMaximum->setRawSamples(xM, yM, 200);
            curveMaximum->setPen(QPen("#7b7c7e"));
            curveMaximum->attach(qwt_cpuUsage);
        } else {

        }
        qwt_cpuUsage->insertLegend(legend, QwtPlot::RightLegend);
        qwt_cpuUsage->axisScaleDraw(QwtPlot::xBottom)->enableComponent(QwtAbstractScaleDraw::Labels, false );
        qwt_cpuUsage->show();

        QwtPlotPicker *picker = new QwtPlotPicker(qwt_cpuUsage->xBottom,qwt_cpuUsage->yLeft, QwtPlotPicker::EllipseRubberBand, QwtPicker::AlwaysOn,(qwt_cpuUsage->canvas()));
        picker->setParent(qwt_cpuUsage->canvas());

    } else {

    }

    return 0;
}

int MainWindow::add_ProfilerModules(){

    ui->table_Profiler->setRowCount(0);

    for(unsigned int ridx=0;ridx<this->SysDump.Sections.Profiler.vModule.size();ridx++){
        ui->table_Profiler->insertRow(static_cast<int>(ridx));
        ui->table_Profiler->setRowHeight(static_cast<int>(ridx),TABLE_ROW_HEIGHT);

        for(int cidx=0;cidx<PROFILER_TABLE_COLUMNS;cidx++){
            QTableWidgetItem* item = new QTableWidgetItem();

            switch(cidx){
                case 0:
                    item->setText(QString::number(this->SysDump.Sections.Profiler.vModule[ridx].ID));
                    break;

                case 1:
                    item->setText((this->SysDump.Sections.Profiler.vModule[ridx].Name));
                    break;

                case 2:
                    item->setText((this->SysDump.Sections.Profiler.vModule[ridx].TimeStamp));
                    break;

                case 3:
                    item->setText(QString::number(this->SysDump.Sections.Profiler.vModule[ridx].Size)+tr("Byte"));
                    break;

                default:
                    break;

            }
            ui->table_Profiler->setItem(static_cast<int>(ridx),cidx,item);
        }
    }

    return 0;
}

int MainWindow::draw_Overview(){

    qDeleteAll(ui->widget_overview_Center->findChildren<QWidget*>());
    qDeleteAll(ui->widget_overview_AppStatus->findChildren<QWidget*>());
    qDeleteAll(ui->widget_overview_CPUMode->findChildren<QWidget*>());
    qDeleteAll(ui->widget_overview_Hardware->findChildren<QWidget*>());
    qDeleteAll(ui->widget_overview_Motion->findChildren<QWidget*>());
    qDeleteAll(ui->widget_overview_Target->findChildren<QWidget*>());

    // Texts and labels
    QFont title( "MS Shell Dlg 2", 14, QFont::Bold);
    QFont header( "MS Shell Dlg 2", 12, QFont::Bold);
    QFont subheader( "MS Shell Dlg 2", 10, QFont::Bold);
    QFont normal( "MS Shell Dlg 2", 8, QFont::Normal);

    // Picture Labels
    clickLabel *Center = new clickLabel(250,this,ui->widget_overview_Center);

    clickLabel *Outside[5];// = new QLabel[5*sizeof(QLabel)];
    clickLabel *LabelOutside_Heading;

    //bool HwError = false;
    int AxisErrCnt = 0;

    for(int i=0; i<5; i++){

        switch(i){
        case 0:
            Outside[i] = new clickLabel(i+1,this,ui->widget_overview_Target);
            LabelOutside_Heading = new clickLabel(i+1,this,ui->widget_overview_Target);
            break;
        case 1:
            Outside[i] = new clickLabel(i+1,this,ui->widget_overview_Hardware);
            LabelOutside_Heading = new clickLabel(i+1,this,ui->widget_overview_Hardware);
            break;
        case 2:
            Outside[i] = new clickLabel(i+1,this,ui->widget_overview_Motion);
            LabelOutside_Heading = new clickLabel(i+1,this,ui->widget_overview_Motion);
            break;
        case 3:
            Outside[i] = new clickLabel(i+1,this,ui->widget_overview_AppStatus);
            LabelOutside_Heading = new clickLabel(i+1,this,ui->widget_overview_AppStatus);
            break;
        case 4:
            Outside[i] = new clickLabel(i+1,this,ui->widget_overview_CPUMode);
            LabelOutside_Heading = new clickLabel(i+1,this,ui->widget_overview_CPUMode);
            break;
        default:
            LabelOutside_Heading = new clickLabel(i+1,this,this); // <- just to silence warnings...
            Outside[i] = new clickLabel(0,this);
        }

        // Label
        LabelOutside_Heading->setFont(header);
        LabelOutside_Heading->setStyleSheet("QLabel{color: white;}");
        LabelOutside_Heading->setGeometry(55,35,100,20);

        QLabel *LabelOutside_Sub[4];

            QGraphicsDropShadowEffect* Effect = new QGraphicsDropShadowEffect();
            Effect->setBlurRadius(30);
            Effect->setColor(QColor(120,120,120,255));
            Effect->setOffset(2,2);

            switch(i){
                case 0:
                    Outside[i]->setPixmap(QPixmap("://images/Outside_Green_S.png"));
                    LabelOutside_Heading->setText(tr("Target"));
                    for(int j=0;j<4;j++){
                        LabelOutside_Sub[j] = new clickLabel(i+1,this,ui->widget_overview_Target);
                    }
                    LabelOutside_Sub[0]->setFont(subheader);
                    LabelOutside_Sub[0]->setText(tr("Type:"));
                    LabelOutside_Sub[1]->setFont(normal);
                    if(this->SysDump.Sections.Hardware.vNode.size()!=0)
                        LabelOutside_Sub[1]->setText(QString("  ")+this->SysDump.Sections.Hardware.vNode[0].ModuleStatus.Plugged);
                    LabelOutside_Sub[2]->setFont(subheader);
                    LabelOutside_Sub[2]->setText(tr("Host name:"));
                    LabelOutside_Sub[3]->setFont(normal);
                    LabelOutside_Sub[3]->setText(QString("  ")+this->SysDump.Sections.Values.CpuConfiguration.Hostname);
                    break;

                case 1:
                    LabelOutside_Heading->setText("Hardware");
                    for(int j=0;j<4;j++){
                        LabelOutside_Sub[j] = new clickLabel(i+1,this,ui->widget_overview_Hardware);
                    }
                    LabelOutside_Sub[1]->setFont(normal);
                    // 30.01.2017 :: krausertm
                    // Fehlerbit direkt beim Auslesen der Datei bilden und hier nur noch abfragen!
                    /*for(unsigned int j=0;j<this->SysDump.Sections.Hardware.vNode.size();j++){
                        if(this->SysDump.Sections.Hardware.vNode[j].ModuleStatus.ModuleOk == 0 || (this->SysDump.Sections.Hardware.vNode[j].Status != 0 && this->SysDump.Sections.Hardware.vNode[j].Status != 1))
                            HwError = true;
                    }*/
                    if(!this->SysDump.Sections.Hardware.HwError){
                        Outside[i]->setPixmap(QPixmap("://images/Outside_Green_S.png"));
                        LabelOutside_Sub[1]->setText("OK");
                    } else {
                        Outside[i]->setPixmap(QPixmap("://images/Outside_Orange_S.png"));
                        LabelOutside_Sub[1]->setText(tr("Error!"));
                    }
                    break;

                case 2:
                    Outside[i]->setPixmap(QPixmap("://images/Outside_Green_S.png"));
                    LabelOutside_Heading->setText("Motion");
                    for(int j=0;j<4;j++){
                        LabelOutside_Sub[j] = new clickLabel(i+1,this,ui->widget_overview_Motion);
                    }
                    LabelOutside_Sub[1]->setFont(normal);
                    LabelOutside_Sub[2]->setFont(normal);
                    if(this->SysDump.Sections.Motion.Available==true){
                        for(unsigned int j=0; j<this->SysDump.Sections.Motion.vAxis.size();j++){
                            if(this->SysDump.Sections.Motion.vAxis[j].Alarm!=false)
                                AxisErrCnt++;
                        }
                        LabelOutside_Sub[1]->setText(tr("Axes: ")+QString::number(this->SysDump.Sections.Motion.vAxis.size()));
                        LabelOutside_Sub[2]->setText(tr("In Error: ")+QString::number(AxisErrCnt));
                        if(AxisErrCnt!=0)
                            Outside[i]->setPixmap(QPixmap("://images/Outside_Orange_S.png"));
                    } else {
                        LabelOutside_Sub[1]->setText(tr("Axes: -"));
                        LabelOutside_Sub[2]->setText(tr("In Error: -"));
                    }

                    break;

                case 3:
                    Outside[i]->setPixmap(QPixmap("://images/Outside_Green_S.png"));
                    LabelOutside_Heading->setText(tr("Application"));
                    for(int j=0;j<4;j++){
                        LabelOutside_Sub[j] = new clickLabel(i+1,this,ui->widget_overview_AppStatus);
                    }
                    break;

                case 4:
                    LabelOutside_Heading->setText(tr("CPU Mode"));
                    for(int j=0;j<4;j++){
                        LabelOutside_Sub[j] = new clickLabel(i+1,this,ui->widget_overview_CPUMode);
                    }
                    LabelOutside_Sub[1]->setFont(normal);
                    if(this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode == CPU_MODE_RUN){
                        Outside[i]->setPixmap(QPixmap("://images/Outside_Green_S.png"));
                    } else {
                        Outside[i]->setPixmap(QPixmap("://images/Outside_Orange_S.png"));
                    }
                    ((this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode < this->list_CpuModes.count()) && (this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode >= 0)) ? LabelOutside_Sub[1]->setText(this->list_CpuModes.at(this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode)) : LabelOutside_Sub[1]->setText(QString::number(this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode));
                    break;

                default:
                    break;
            }

            LabelOutside_Heading->show();

            for(int j=0; j<4; j++){
                LabelOutside_Sub[j]->setStyleSheet("QLabel{color: white;}");
                LabelOutside_Sub[j]->setGeometry(55,55+(j*16),100,20);
                LabelOutside_Sub[j]->show();
            }

            Outside[i]->setAlignment(Qt::AlignCenter);
            Outside[i]->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
            Outside[i]->setGraphicsEffect(Effect);
            Outside[i]->setGeometry(20,20,140,142);
            Outside[i]->show();
        }

        // Center png
        QGraphicsDropShadowEffect* CenterEffect = new QGraphicsDropShadowEffect();
        CenterEffect->setBlurRadius(30);
        CenterEffect->setColor(QColor(120,120,120,255));
        CenterEffect->setOffset(2,2);
        Center->setPixmap(QPixmap("://images/Center_Orange_S.png"));
        Center->setAlignment(Qt::AlignCenter);
        Center->setStyleSheet("background-color: rgba(255, 255, 255, 0);");
        Center->setGraphicsEffect(CenterEffect);
        Center->setGeometry(20,20,140,142);
        Center->show();

        // Center text
        QLabel *LabelCenter_Heading = new QLabel(ui->widget_overview_Center);
        LabelCenter_Heading->setFont(title);
        LabelCenter_Heading->setAlignment(Qt::AlignCenter);
        LabelCenter_Heading->setStyleSheet("QLabel{color: white;}");
        LabelCenter_Heading->setGeometry(37,60,100,60);
        LabelCenter_Heading->setText(tr("System\nDump"));
        LabelCenter_Heading->show();


    return 0;
}

int MainWindow::add_AxisToUi(int x, int index){
    // First three rows + resize background
    QLabel *AxisName = new QLabel(ui->widget_Axis);
    QLabel *AxisDrive = new QLabel(ui->widget_Axis);
    QLabel *AxisAddress = new QLabel(ui->widget_Axis);

    if(index<4){
        ui->text_Axis_Name->setGeometry(ui->text_Axis_Name->x(),ui->text_Axis_Name->y(),(AXIS_LABEL_WIDTH+(index+1)*(10+AXIS_LABEL_WIDTH)+2),ui->text_Axis_Name->height());
        ui->text_Axis_Drive->setGeometry(ui->text_Axis_Drive->x(),ui->text_Axis_Drive->y(),(AXIS_LABEL_WIDTH+(index+1)*(10+AXIS_LABEL_WIDTH)+2),ui->text_Axis_Drive->height());
        ui->text_Axis_Address->setGeometry(ui->text_Axis_Address->x(),ui->text_Axis_Address->y(),(AXIS_LABEL_WIDTH+(index+1)*(10+AXIS_LABEL_WIDTH)+2),ui->text_Axis_Address->height());
    } else {
        ui->text_Axis_Name->setGeometry(ui->text_Axis_Name->x(),ui->text_Axis_Name->y(),700,ui->text_Axis_Name->height());
        ui->text_Axis_Drive->setGeometry(ui->text_Axis_Drive->x(),ui->text_Axis_Drive->y(),700,ui->text_Axis_Drive->height());
        ui->text_Axis_Address->setGeometry(ui->text_Axis_Address->x(),ui->text_Axis_Address->y(),700,ui->text_Axis_Address->height());
    }

    // Axis Name
    AxisName->setGeometry(x,ui->text_Axis_Name->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT_SMALL);
    AxisName->setAlignment(Qt::AlignHCenter);
    if(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Name.length() <= AXIS_TEXT_LENGTH){
        AxisName->setText(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Name);
    } else {
        QStringRef subAxisName(&this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Name, 0, AXIS_TEXT_LENGTH);
        AxisName->setText(subAxisName.toString()+QString(".."));
    }
    AxisName->show();

    // Axis Drive
    AxisDrive->setGeometry(x,ui->text_Axis_Drive->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT_SMALL);
    AxisDrive->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    if(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Drive.length() <= AXIS_TEXT_LENGTH){
        AxisDrive->setText(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Drive);
    } else {
        QStringRef subAxisDrive(&this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Drive,0,AXIS_TEXT_LENGTH);
        AxisDrive->setText(subAxisDrive.toString()+QString(".."));
    }
    AxisDrive->show();

    // Axis Address
    AxisAddress->setGeometry(x,ui->text_Axis_Address->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT_SMALL);
    AxisAddress->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    if(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Adress.length() <= AXIS_TEXT_LENGTH){
        AxisDrive->setText(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Adress);
    } else {
        QStringRef subAxisAddress(&this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Adress,0,AXIS_TEXT_LENGTH);
        AxisAddress->setText(subAxisAddress.toString()+QString(".."));
    }
    AxisAddress->show();

    // Homing
    QLabel *Homing = new QLabel(ui->widget_Axis);
    Homing->setGeometry(x,ui->text_Axis_Homing->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT_BIG);
    Homing->setStyleSheet(this->Styles.axisTopStyle);
    Homing->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Homing==true) ? Homing->setPixmap(QPixmap("://images/Axis_HomingOk.png")) : Homing->setPixmap(QPixmap("://images/Axis_HomingNOk.png"));
    Homing->show();

    // Controller
    QLabel *Controller = new QLabel(ui->widget_Axis);
    Controller->setGeometry(x,ui->text_Axis_Controller->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT_BIG);
    Controller->setStyleSheet(this->Styles.axisOrangeStyle);
    Controller->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Controller==true) ? Controller->setPixmap(QPixmap("://images/Axis_ControllerOn.png")) : Controller->setPixmap(QPixmap("://images/Axis_ControllerOff.png"));
    Controller->show();

    // Simulation
    QLabel *Simulation = new QLabel(ui->widget_Axis);
    Simulation->setGeometry(x,ui->text_Axis_Simulation->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT_BIG);
    Simulation->setStyleSheet(this->Styles.axisOrangeStyle);
    Simulation->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Simulation==true) ? Simulation->setPixmap(QPixmap("://images/Axis_SimulationOn.png")) : Simulation->setPixmap(QPixmap("://images/Axis_SimulationOff.png"));
    Simulation->show();

    // Movement Status
    QLabel *MovemenetStatus = new QLabel(ui->widget_Axis);
    MovemenetStatus->setGeometry(x,ui->text_Axis_Movement->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT_BIG);
    MovemenetStatus->setStyleSheet(this->Styles.axisWhiteStyle);
    MovemenetStatus->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].MovementStatus != 0) ? MovemenetStatus->setPixmap(QPixmap("://images/Axis_MovementOn.png")) : MovemenetStatus->setPixmap(QPixmap("://images/Axis_MovementOff.png"));
    MovemenetStatus->show();

    // Position
    QLabel *Position = new QLabel(ui->widget_Axis);
    Position->setGeometry(x,ui->text_Axis_Position->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT);
    Position->setStyleSheet(this->Styles.axisWhiteStyle);
    Position->setAlignment(Qt::AlignCenter);
    Position->setText(QString::number(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].ActPosition));
    Position->show();

    // Speed
    QLabel *Speed = new QLabel(ui->widget_Axis);
    Speed->setGeometry(x,ui->text_Axis_Speed->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT);
    Speed->setStyleSheet(this->Styles.axisWhiteStyle);
    Speed->setAlignment(Qt::AlignCenter);
    Speed->setText(QString::number(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].ActSpeed));
    Speed->show();

    // Enable
    QLabel *Enable = new QLabel(ui->widget_Axis);
    Enable->setGeometry(x,ui->text_Axis_Enable->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT);
    Enable->setStyleSheet(this->Styles.axisOrangeStyle);
    Enable->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Enable != 0) ? Enable->setPixmap(QPixmap("://images/Axis_On.png")) : Enable->setPixmap(QPixmap("://images/Axis_Off.png"));
    Enable->show();

    // Trigger 1
    QLabel *Trigger1 = new QLabel(ui->widget_Axis);
    Trigger1->setGeometry(x,ui->text_Axis_Trigger1->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT);
    Trigger1->setStyleSheet(this->Styles.axisOrangeStyle);
    Trigger1->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Trigger1 != 0) ? Trigger1->setPixmap(QPixmap("://images/Axis_On.png")) : Trigger1->setPixmap(QPixmap("://images/Axis_Off.png"));
    Trigger1->show();

    // Trigger 2
    QLabel *Trigger2 = new QLabel(ui->widget_Axis);
    Trigger2->setGeometry(x,ui->text_Axis_Trigger2->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT);
    Trigger2->setStyleSheet(this->Styles.axisOrangeStyle);
    Trigger2->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Trigger2 != 0) ? Trigger2->setPixmap(QPixmap("://images/Axis_On.png")) : Trigger2->setPixmap(QPixmap("://images/Axis_Off.png"));
    Trigger2->show();

    // Positive endswitch
    QLabel *PosEndSw = new QLabel(ui->widget_Axis);
    PosEndSw->setGeometry(x,ui->text_Axis_PosEndSw->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT);
    PosEndSw->setStyleSheet(this->Styles.axisOrangeStyle);
    PosEndSw->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].PosEndSw != 0) ? PosEndSw->setPixmap(QPixmap("://images/Axis_On.png")) : PosEndSw->setPixmap(QPixmap("://images/Axis_Off.png"));
    PosEndSw->show();

    // Negative endswitch
    QLabel *NegEndSw = new QLabel(ui->widget_Axis);
    NegEndSw->setGeometry(x,ui->text_Axis_NegEndSw->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT);
    NegEndSw->setStyleSheet(this->Styles.axisOrangeStyle);
    NegEndSw->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].NegEndSw != 0) ? NegEndSw->setPixmap(QPixmap("://images/Axis_On.png")) : NegEndSw->setPixmap(QPixmap("://images/Axis_Off.png"));
    NegEndSw->show();

    // Reference switch
    QLabel *RefSw = new QLabel(ui->widget_Axis);
    RefSw->setGeometry(x,ui->text_Axis_RefSw->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT);
    RefSw->setStyleSheet(this->Styles.axisOrangeStyle);
    RefSw->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].RefSw != 0) ? RefSw->setPixmap(QPixmap("://images/Axis_On.png")) : RefSw->setPixmap(QPixmap("://images/Axis_Off.png"));
    RefSw->show();

    // Alarm
    clickLabel *Alarm = new clickLabel(index+100,this,ui->widget_Axis);
    Alarm->setGeometry(x,ui->text_Axis_Alarm->y(),AXIS_LABEL_WIDTH,AXIS_LABEL_HEIGHT_BIG);
    Alarm->setStyleSheet(this->Styles.axisBottomStyle);
    Alarm->setAlignment(Qt::AlignCenter);
    (this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].Alarm == true) ? Alarm->setPixmap(QPixmap("://images/Axis_AlarmActive.png")) : Alarm->setPixmap(QPixmap("://images/Axis_AlarmNActive.png"));
    Alarm->show();

    return 0;
}

int MainWindow::add_AxisErrorsToUi(){

    ui->table_AxisError->setRowCount(0);

    int index = -1;

    for(unsigned int i=0;i<this->SysDump.Sections.Motion.vAxis.size();i++){
        if(this->SysDump.Sections.Motion.vAxis[i].Name == ui->combo_AxisError->currentText())
            index = static_cast<int>(i);
    }

    if(index >= 0){
        for(unsigned int j=0;j<this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].vAlarm.size();j++){
            ui->table_AxisError->insertRow(static_cast<int>(j));
            ui->table_AxisError->setRowHeight(static_cast<int>(j),TABLE_ROW_HEIGHT);

            for(int cidx=0;cidx<AXISERR_TABLE_COLUMNS;cidx++){
                QTableWidgetItem* item = new QTableWidgetItem();
                //QLabel *levelLabel = new QLabel;

                switch(cidx){
                    case 0:
                        item->setText(QString::number(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].vAlarm[j].ErrorNr));
                        break;

                    case 1:
                        item->setText(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].vAlarm[j].TimeStamp);
                        break;

                    case 2:
                        item->setText(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].vAlarm[j].TextInfo);
                        break;

                    case 3:
                        item->setText(this->SysDump.Sections.Motion.vAxis[static_cast<unsigned int>(index)].vAlarm[j].ErrorText);
                        break;

                default:
                    break;
                }
                ui->table_AxisError->setItem(static_cast<int>(j),cidx,item);
            }
        }
    }
    return 0;
}


void writeByteToLabel(QLabel *pLabel, long long size){

    if(size >= BYTE_PER_GB){
        pLabel->setText(QString::number(static_cast<qreal>(size)/BYTE_PER_GB,'f',3)+(" GB"));
    } else if(size >= BYTE_PER_MB){
        pLabel->setText(QString::number(static_cast<qreal>(size)/BYTE_PER_MB,'f',2)+(" MB"));
    } else if(size >= BYTE_PER_KB){
        pLabel->setText(QString::number(static_cast<qreal>(size)/BYTE_PER_KB,'f',2)+(" KByte"));
    } else {
        pLabel->setText(QString::number(size)+(" Byte"));
    }

}
