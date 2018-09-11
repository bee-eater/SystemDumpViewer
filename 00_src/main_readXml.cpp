#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "stdio.h"
#include "stdlib.h"
#include "iostream"
#include "sstream"
#include "fstream"

#include <QMessageBox>
#include <QDebug>

bool MainWindow::readXML(const char* filename){
    int result=0;
    QString msgStr;
    this->ErrorNr=0;

    // Close old systemdump but remember IP!
    QString rememberedIP = this->ipAddress;
    this->on_actionClose_xml_triggered();
    this->ipAddress = rememberedIP;

    if(QFile(filename).exists()){

        // Save current file in string
        this->currentFileString = QDir::cleanPath(this->pCharToUtf8(filename));

        // Stausbar
        this->statusbar->showMessage(tr("Loading %1 ...").arg(this->currentFileString),DEFAULT_MESSAGE_TIME);

        // Change window title - is changed back on close!
        setWindowTitle(tr("Systemdump Viewer") + " - " + this->currentFileString);

        // Add to recent files list
        recentSetCurrentFile(this->currentFileString);

        std::ifstream file(filename);
        if(file){
            std::stringstream buffer;
            buffer << file.rdbuf();
            file.close();
            std::string content(buffer.str());

            // Check if xml header is given
            std::string xmltest = content.substr(0,5);

            if (xmltest.compare("<?xml")==0){

                xml_document<> doc;
                doc.parse<0>(&content[0]);

                this->init_Maps();

                result = this->get_SystemDumpMainInfo(&doc);

                if (result == 0){
                    result = this->get_SystemDumpSections(&doc);

                    if(result==0){

                        if (this->ErrorNr!=0){
                            msgStr = tr("While loading the xml-file one or more\nattributes could not be read correctly!\n\nError nr.: %1\n\nAttribute: %2").arg(QString::number(this->ErrorNr),this->Error);
                            QMessageBox::information(this,tr("Information"),msgStr);
                        }

                        // Enable menu entries
                        ui->actionClose_xml->setDisabled(false);
                        ui->actionSave_xml_as->setDisabled(false);
                        ui->actionSave_PDF_Report->setDisabled(false);
                        this->startScreenShow(false);
                        this->displayValues();

                    } else {

                        msgStr = tr("An error occured trying to open the xml-file!\n");
                        QMessageBox::warning(this,tr("Warning"),msgStr);
                        return 1;

                    }

                    return 0;

                } else {

                    msgStr = tr("No valid systemdump!");
                    QMessageBox::warning(this,tr("Warning"),msgStr);
                    return 1;

                }
            } else {
                QMessageBox::warning(this,tr("Warning"),tr("No valid .xml file! Please verify that your file is not damaged!"));
                return 1;
            }
        }
        else
        {
            return 1;
        }
    } else {
        // Reset window title
        setWindowTitle(tr("Systemdump Viewer"));
        QMessageBox::warning(this,tr("Warning"),tr("File not found!"));
        return 1;
    }
}

bool MainWindow::get_SystemDumpMainInfo(xml_document<> *doc){

    // XML Content
    xml_node<> *pRoot = doc->first_node();
    xml_attribute<> *pAttribute;
    string tmpString;

    while(pRoot != NULL){
        tmpString = pRoot->name();
        std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

        switch (map_SystemDump[tmpString]){
        case SYSDUMP_DUMP:
            pAttribute = pRoot->first_attribute();

            while(pAttribute != NULL){
                tmpString = pAttribute->name();
                std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                switch (map_SystemDumpAttributes[tmpString]){
                case SYSDUMP_000_GENERATOR:
                    this->SysDump.Generator = QString::fromStdString(pAttribute->value());
                    break;

                case SYSDUMP_000_TIMESTAMP:
                    this->SysDump.Timestamp = QString::fromStdString(pAttribute->value());
                    break;

                case SYSDUMP_000_VERSION:
                    this->SysDump.Version = QString::fromStdString(pAttribute->value());
                    break;

                case SYSDUMP_000_ERR:
                    this->ErrorNr = 20002;
                    this->Error = QString::fromStdString(tmpString);
                    break;
                default:
                    return 1;
                }
                pAttribute = pAttribute->next_attribute();
            }
            break;

        case SYSDUMP_ERR:
            this->ErrorNr = 20001;
            this->Error = QString::fromStdString(tmpString);
            break;
        default:
            return 1;
        }
        pRoot = pRoot->next_sibling();
    }
    return 0;
}

int MainWindow::get_SystemDumpSections(xml_document<> *doc){

    xml_node<> *pRoot = doc->first_node()->first_node();

    xml_node<> *pItem;
    xml_node<> *pSubItem;
    xml_node<> *pSubSubItem;

    xml_attribute<> *pAttribute;
    xml_attribute<> *pAttribute2;

    int cnt = 0, subcnt = 0, index = 0, subindex = 0, result = 0;

    string tmpString, tmpString2;

    while(pRoot != NULL){

        // SWITCH THROUGH SECTIONS
        // -------------------------------------------------------------------------------
        pAttribute = pRoot->first_attribute();
        tmpString = pAttribute->name();
        std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

        if (tmpString == "title"){

            tmpString = pAttribute->value();
            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

            switch (map_Sections[tmpString]) {

            case SECTION_VALUES:

                // SECTION VALUES
                // ---------------------------------------------------------------------------
                pItem = pRoot->first_node()->first_node();
                while (pItem!=NULL) {

                    tmpString = pItem->name();
                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                    switch (map_000_SectionValues[tmpString]) {

                    case VALUE_000_OPERATIONAL:

                        // GET OPERATIONAL VALUES
                        // -------------------------------------------------------------------
                        pSubItem = pItem->first_node();
                        while (pSubItem != NULL) {

                            result = this->get_StringsForIdAndValue(pSubItem,&tmpString,&tmpString2);

                            if (result==0){
                                switch (map_001_OperationalValues[tmpString]) {

                                    case VALUE_001_NODENUMBER:
                                        this->SysDump.Sections.Values.OperationalValues.NodeNumber = QString::fromStdString(tmpString2.c_str());
                                        break;

                                    case VALUE_001_CPUMODE:
                                        this->SysDump.Sections.Values.OperationalValues.CurrentCpuMode = (qint16)atoi(tmpString2.c_str());
                                        break;

                                    case VALUE_001_BATTERYSTATUS:
                                        this->SysDump.Sections.Values.OperationalValues.BatteryStatus = (qint16)atoi(tmpString2.c_str());
                                        break;

                                    case VALUE_001_CPUTEMP:
                                        this->SysDump.Sections.Values.OperationalValues.CPUtemperature = (qint16)atoi(tmpString2.c_str());
                                        break;

                                    case VALUE_001_CPUUSAGE:
                                        this->SysDump.Sections.Values.OperationalValues.CurrentCpuUsage = (qint16)atoi(tmpString2.c_str());
                                        break;

                                    case VALUE_001_TARGETTIME:
                                        this->SysDump.Sections.Values.OperationalValues.TargetTime = QString::fromStdString(tmpString2.c_str());
                                        break;

                                    case VALUE_001_OPHOURS:
                                        this->SysDump.Sections.Values.OperationalValues.OperatingHours = (qint32)atoi(tmpString2.c_str());
                                        break;

                                    case VALUE_001_POWERCYCLES:
                                        this->SysDump.Sections.Values.OperationalValues.PowerOnCycles = (qint32)atoi(tmpString2.c_str());
                                        break;

                                    case VALUE_001_ERR:
                                        this->ErrorNr = 20101;
                                        this->Error = QString::fromStdString(tmpString);
                                        break;
                                    default:
                                        return 2;
                                }
                            }
                            pSubItem = pSubItem->next_sibling();
                        }
                        break;

                    case VALUE_000_TIMESYNC:

                        // GET TIME SYNCHRONISATION
                        // -------------------------------------------------------------------
                        pSubItem = pItem->first_node();
                        while(pSubItem!=NULL){

                            tmpString = pSubItem->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch (map_002_TimeSynchronisation[tmpString]) {
                                case VALUE_002_PARAM:
                                    this->SysDump.Sections.Values.TimeSynchronisation.TimeZone = QString::fromStdString(pSubItem->first_attribute("value")->value());
                                    break;

                                case VALUE_002_SERVER:
                                    pSubSubItem = pSubItem->first_node();
                                    while(pSubSubItem!=NULL){
                                        this->SysDump.Sections.Values.TimeSynchronisation.Server.SNTPserver = QString::fromStdString(pSubSubItem->first_attribute("value")->value());
                                        pSubSubItem = pSubSubItem->next_sibling();
                                    }
                                    break;

                                case VALUE_002_CLIENT:
                                    pSubSubItem = pSubItem->first_node();
                                    while(pSubSubItem!=NULL){

                                        result = this->get_StringsForIdAndValue(pSubSubItem,&tmpString,&tmpString2);

                                        if(result==0){
                                            switch(map_003_TimeSynchClient[tmpString]){
                                                case VALUE_003_CLIENT:
                                                    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPclient = QString::fromStdString(tmpString2.c_str());
                                                    break;

                                                case VALUE_003_SERVER1:
                                                    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver1 = QString::fromStdString(tmpString2.c_str());
                                                    break;

                                                case VALUE_003_SERVER2:
                                                    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver2 = QString::fromStdString(tmpString2.c_str());
                                                    break;

                                                case VALUE_003_SERVER3:
                                                    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver3 = QString::fromStdString(tmpString2.c_str());
                                                    break;

                                                case VALUE_003_SERVER4:
                                                    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPserver4 = QString::fromStdString(tmpString2.c_str());
                                                    break;

                                                case VALUE_003_SNTP_SYNC_INT:
                                                    this->SysDump.Sections.Values.TimeSynchronisation.Client.SNTPsynchInterval = (qint16)atoi(tmpString2.c_str());
                                                    break;

                                                case VALUE_003_RTC_SYNC_INT:
                                                    this->SysDump.Sections.Values.TimeSynchronisation.Client.RTCsynchInterval = (qint16)atoi(tmpString2.c_str());
                                                    break;

                                                case VALUE_003_ERR:
                                                    this->ErrorNr = 20103;
                                                    this->Error = QString::fromStdString(tmpString);
                                                    break;
                                                default:
                                                    return 3;
                                            }
                                        }
                                        pSubSubItem = pSubSubItem->next_sibling();
                                    }
                                    break;

                                case VALUE_002_ERR:
                                    this->ErrorNr = 20102;
                                    this->Error = QString::fromStdString(tmpString);
                                    break;
                                default:
                                    return 4;
                            }
                            pSubItem = pSubItem->next_sibling();
                        }
                        break;

                    case VALUE_000_SOFTWARE:

                        // GET SOFTWARE
                        // -------------------------------------------------------------------
                        pSubItem = pItem->first_node();
                        while(pSubItem!=NULL){

                            result = this->get_StringsForIdAndValue(pSubItem,&tmpString,&tmpString2);

                            if (result==0){
                                switch (map_004_Software[tmpString]) {
                                case VALUE_004_AR:
                                    this->SysDump.Sections.Values.SoftwareVersions.AR = QString::fromStdString(tmpString2.c_str());
                                    break;

                                case VALUE_004_VC:
                                    this->SysDump.Sections.Values.SoftwareVersions.VC = QString::fromStdString(tmpString2.c_str());
                                    break;

                                case VALUE_004_MC:
                                    this->SysDump.Sections.Values.SoftwareVersions.MC = QString::fromStdString(tmpString2.c_str());
                                    break;

                                case VALUE_004_CNC:
                                    this->SysDump.Sections.Values.SoftwareVersions.CNC = QString::fromStdString(tmpString2.c_str());
                                    break;

                                case VALUE_004_ERR:
                                    this->ErrorNr = 20104;
                                    this->Error = QString::fromStdString(tmpString);
                                    break;
                                default:
                                    return 5;
                                }
                            }
                            pSubItem = pSubItem->next_sibling();
                        }
                        break;

                    case VALUE_000_CPUCONF:

                        // GET CPU CONFIGURATION
                        // -------------------------------------------------------------------
                        pSubItem = pItem->first_node();
                        while(pSubItem!=NULL){

                            tmpString = pSubItem->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch (map_005_CpuConfiguration[tmpString]) {
                            case VALUE_005_PARAM:

                                result = this->get_StringsForIdAndValue(pSubItem,&tmpString,&tmpString2);

                                if (result == 0){
                                    switch(map_008_CpuConfigurationParam[tmpString]){
                                    case VALUE_008_HOSTNAME:
                                        this->SysDump.Sections.Values.CpuConfiguration.Hostname = QString::fromStdString(tmpString2.c_str());
                                        break;

                                    case VALUE_008_DEFDOMAIN:
                                        this->SysDump.Sections.Values.CpuConfiguration.DefaultDomain = QString::fromStdString(tmpString2.c_str());
                                        break;

                                    case VALUE_008_CPUMODESWITCH:
                                        this->SysDump.Sections.Values.CpuConfiguration.CPUModeSwitch = QString::fromStdString(tmpString2.c_str());
                                        break;

                                    case VALUE_008_PROFILING:
                                        this->SysDump.Sections.Values.CpuConfiguration.Profiling = (bool)atoi(tmpString2.c_str());
                                        break;

                                    case VALUE_008_FTP:
                                        this->SysDump.Sections.Values.CpuConfiguration.FTP = (bool)atoi(tmpString2.c_str());
                                        break;

                                    case VALUE_008_USBREMOTEINSTALL:
                                        this->SysDump.Sections.Values.CpuConfiguration.USBremoteInstall = (bool)atoi(tmpString2.c_str());
                                        break;

                                    case VALUE_008_CFREMOTEINSTALL:
                                        this->SysDump.Sections.Values.CpuConfiguration.CFremoteInstall = (bool)atoi(tmpString2.c_str());
                                        break;

                                    case VALUE_008_ERR:
                                        this->ErrorNr = 20105;
                                        this->Error = QString::fromStdString(tmpString);
                                        break;
                                    default:
                                        return 6;
                                    }
                                }
                                break;

                            case VALUE_005_REBOOT:
                                pSubSubItem = pSubItem->first_node();
                                while(pSubSubItem!=0){

                                    result = this->get_StringsForIdAndValue(pSubSubItem,&tmpString,&tmpString2);

                                    if (result == 0){
                                        switch(map_006_CpuConfigurationReboot[tmpString]){
                                        case VALUE_006_RESET:
                                            this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterReset = (qint16)atoi(tmpString2.c_str());
                                            break;

                                        case VALUE_006_PFAIL:
                                            this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterPowerFail = (qint16)atoi(tmpString2.c_str());
                                            break;

                                        case VALUE_006_CHANGECF:
                                            this->SysDump.Sections.Values.CpuConfiguration.RebootMode.AfterCFChange = (qint16)atoi(tmpString2.c_str());
                                            break;

                                        case VALUE_006_ERR:
                                            this->ErrorNr = 20106;
                                            this->Error = QString::fromStdString(tmpString);
                                            break;
                                        default:
                                            return 7;
                                        }
                                    }
                                    pSubSubItem = pSubSubItem->next_sibling();
                                }
                                break;

                            case VALUE_005_PRESERVE:
                                pSubSubItem = pSubItem->first_node();
                                while(pSubSubItem!=0){

                                    result = this->get_StringsForIdAndValue(pSubSubItem,&tmpString,&tmpString2);

                                    if (result == 0){
                                        switch(map_007_CpuConfigurationPreserve[tmpString]){
                                        case VALUE_007_CHANGECF:
                                            this->SysDump.Sections.Values.CpuConfiguration.PreservePVMemory.AfterCFChange = (qint16)atoi(tmpString2.c_str());
                                            break;

                                        case VALUE_007_ERR:
                                            this->ErrorNr = 20107;
                                            this->Error = QString::fromStdString(tmpString);
                                            break;
                                        default:
                                            return 8;
                                        }
                                    }
                                    pSubSubItem = pSubSubItem->next_sibling();
                                }
                                break;

                            case VALUE_005_ERR:
                                this->ErrorNr = 20108;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 8;
                            }
                            pSubItem = pSubItem->next_sibling();
                        }
                        break;

                    case VALUE_000_APPINFO:

                        // GET APPLICATION INFO
                        // -------------------------------------------------------------------
                        pSubItem = pItem->first_node();
                        while(pSubItem!=NULL){

                            result = this->get_StringsForIdAndValue(pSubItem,&tmpString,&tmpString2);

                            if (result == 0){
                                switch (map_009_ApplicationInfo[tmpString]) {
                                case VALUE_009_APPLINK:
                                    this->SysDump.Sections.Values.ApplicationInfo.ApplicationLink = QString::fromStdString(tmpString2.c_str());
                                    break;

                                case VALUE_009_APPSTATUS:
                                    this->SysDump.Sections.Values.ApplicationInfo.ApplicationStatus = (qint16)atoi(tmpString2.c_str());
                                    break;

                                case VALUE_009_ERR:
                                    this->ErrorNr = 20109;
                                    this->Error = QString::fromStdString(tmpString);
                                    break;
                                default:
                                    return 9;
                                }
                            }
                            pSubItem = pSubItem->next_sibling();
                        }

                        break;

                    case VALUE_000_ERR:
                        this->ErrorNr = 20110;
                        this->Error = QString::fromStdString(tmpString);
                        break;
                    default:
                        return 10;
                    }
                    pItem = pItem->next_sibling();
                }
                break;

            case SECTION_ETHERNET:

                // SECTION ETHERNET
                // ---------------------------------------------------------------------------
                pItem = pRoot->first_node()->first_node();
                while (pItem!=NULL) {

                    tmpString = pItem->name();
                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                    switch(map_010_Ethernet[tmpString]){
                    case ETHERNET_PARAM:
                        pSubItem = pItem->first_node();
                        while (pSubItem!=NULL){

                            result = this->get_StringsForIdAndValue(pSubItem,&tmpString,&tmpString2);

                            if (result == 0){
                                switch(map_011_EthernetPar[tmpString]){
                                case ETHERNET_000_DEFGATE:
                                    this->SysDump.Sections.Ethernet.DefaultGateway = QString::fromStdString(tmpString2.c_str());
                                    break;

                                case ETHERNET_000_ERR:
                                    this->ErrorNr = 20201;
                                    this->Error = QString::fromStdString(tmpString);
                                    break;
                                default:
                                    return 11;
                                }
                            }
                            pSubItem = pSubItem->next_sibling();
                        }
                        break;

                    case ETHERNET_INTERFACE:
                        cnt++;
                        index = cnt-1;
                        this->SysDump.Sections.Ethernet.vInterface.push_back(sInterface());

                        pSubItem = pItem->first_node();
                        while(pSubItem!=NULL){
                            this->SysDump.Sections.Ethernet.vInterface[index].InterfaceID = pItem->first_attribute("id",0,false)->value();

                            result = this->get_StringsForIdAndValue(pSubItem,&tmpString,&tmpString2);

                            if (result == 0){
                                switch(map_012_EthernetIf[tmpString]){
                                case ETHERNET_001_IPADRESS:
                                    this->SysDump.Sections.Ethernet.vInterface[index].IpAddress = QString::fromStdString(tmpString2.c_str());
                                    break;

                                case ETHERNET_001_SUBNET:
                                    this->SysDump.Sections.Ethernet.vInterface[index].SubnetMask = QString::fromStdString(tmpString2.c_str());
                                    break;

                                case ETHERNET_001_IPCONF:
                                    this->SysDump.Sections.Ethernet.vInterface[index].IpConfig = (qint16)atoi(tmpString2.c_str());
                                    break;

                                case ETHERNET_001_SNMPCONF:
                                    this->SysDump.Sections.Ethernet.vInterface[index].SNMPConfig = (qint16)atoi(tmpString2.c_str());
                                    break;

                                case ETHERNET_001_ETHREMOTEINSTALL:
                                    this->SysDump.Sections.Ethernet.vInterface[index].EthernetRemoteInstall = (bool)atoi(tmpString2.c_str());
                                    break;

                                case ETHERNET_001_ERR:
                                    this->ErrorNr = 20202;
                                    this->Error = QString::fromStdString(tmpString);
                                    break;
                                default:
                                    return 12;
                                }
                            }
                            pSubItem = pSubItem->next_sibling();
                        }
                        break;

                    case ETHERNET_ERR:
                        this->ErrorNr = 20203;
                        this->Error = QString::fromStdString(tmpString);
                        break;
                    default:
                        return 13;
                    }
                    pItem = pItem->next_sibling();
                }
                break;

            case SECTION_MEMORY:

                cnt=0;

                // SECTION MEMORY
                // ---------------------------------------------------------------------------
                pItem = pRoot->first_node()->first_node();
                while (pItem!=NULL) {

                    tmpString = pItem->name();
                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                    switch(map_013_Memory[tmpString]){
                    case MEMORY_CF:
                        pAttribute = pItem->first_attribute();
                        while(pAttribute != NULL){

                            tmpString = pAttribute->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch (map_029_CfAttributes[tmpString]){
                            case MEMORY_002_NUMOFPARTITIONS:
                                this->SysDump.Sections.Memory.CompactFlash.NumberOfPartitions = (qint16)atoi(pAttribute->value());
                                break;

                            case MEMORY_002_SIZE:
                                this->SysDump.Sections.Memory.CompactFlash.Size = (qint32)atoi(pAttribute->value());
                                break;

                            case MEMORY_002_NUMBEROFSECTORS:
                                this->SysDump.Sections.Memory.CompactFlash.NumberOfSectors = (qint32)atoi(pAttribute->value());
                                break;

                            case MEMORY_002_SIZEPERSECTOR:
                                this->SysDump.Sections.Memory.CompactFlash.SizePerSector = (qint32)atoi(pAttribute->value());
                                break;

                            case MEMORY_002_ERR:
                                this->ErrorNr = 20301;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 14;
                            }
                            pAttribute = pAttribute->next_attribute();
                        }

                        pSubItem = pItem->first_node();
                        while(pSubItem!=NULL){
                            cnt++;
                            index = cnt-1;
                            //result = this->add_MemoryCfPartition(cnt);
                            this->SysDump.Sections.Memory.CompactFlash.vPartition.push_back(sPartition());

                            pAttribute = pSubItem->first_attribute();
                            while(pAttribute!=NULL){

                                tmpString = pAttribute->name();
                                std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                switch(map_030_PartitionAttributes[tmpString]){
                                case MEMORY_003_AVAILABLE:
                                    this->SysDump.Sections.Memory.CompactFlash.vPartition[index].available = (qint32)atoi(pAttribute->value());
                                    break;

                                case MEMORY_003_DESCRIPTION:
                                    this->SysDump.Sections.Memory.CompactFlash.vPartition[index].description = QString::fromStdString(pAttribute->value());
                                    break;

                                case MEMORY_003_ID:
                                    this->SysDump.Sections.Memory.CompactFlash.vPartition[index].id = QString::fromStdString(pAttribute->value());
                                    break;

                                case MEMORY_003_SIZE:
                                    this->SysDump.Sections.Memory.CompactFlash.vPartition[index].size = (qint32)atoi(pAttribute->value());
                                    break;

                                case MEMORY_003_USED:
                                    this->SysDump.Sections.Memory.CompactFlash.vPartition[index].used = (qint32)atoi(pAttribute->value());
                                    break;

                                case MEMORY_003_ERR:
                                    this->ErrorNr = 20302;
                                    this->Error = QString::fromStdString(tmpString);
                                    break;
                                default:
                                    return 15;
                                }
                                pAttribute = pAttribute->next_attribute();
                            }

                        pSubItem = pSubItem->next_sibling();
                        }
                        break;

                    case MEMORY_DRAM:
                        pAttribute = pItem->first_attribute();
                        while(pAttribute != NULL){

                            tmpString = pAttribute->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_031_DRAMAttributes[tmpString]){
                            case MEMORY_004_AVAILABLE:
                                this->SysDump.Sections.Memory.DRAM.available = (qint32)atoi(pAttribute->value());
                                break;

                            case MEMORY_004_LARGESTBLOCK:
                                this->SysDump.Sections.Memory.DRAM.LargestAvailableBlock = (qint32)atoi(pAttribute->value());
                                break;

                            case MEMORY_004_SIZE:
                                this->SysDump.Sections.Memory.DRAM.size = (qint32)atoi(pAttribute->value());
                                break;

                            case MEMORY_004_USED:
                                this->SysDump.Sections.Memory.DRAM.used = (qint32)atoi(pAttribute->value());
                                break;

                            case MEMORY_004_ERR:
                                this->ErrorNr = 20303;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 16;
                            }
                            pAttribute = pAttribute->next_attribute();
                        }
                        break;

                    case MEMORY_SRAM:
                        // SRAM Attributes
                        pAttribute = pItem->first_attribute();
                        while(pAttribute != NULL){

                            tmpString = pAttribute->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_032_SRAMAttributes[tmpString]){
                            case MEMORY_005_SIZE:
                                this->SysDump.Sections.Memory.SRAM.size = (qint32)atoi(pAttribute->value());
                                break;

                            case MEMORY_005_ERR:
                                this->ErrorNr = 20304;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 17;
                            }
                            pAttribute = pAttribute->next_attribute();
                        }

                        pSubItem = pItem->first_node();
                        while(pSubItem != NULL){

                            tmpString = pSubItem->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_015_MemorySRAM[tmpString]){
                            case MEMORY_001_NOTCONFIGURED:
                                pAttribute = pSubItem->first_attribute();
                                while(pAttribute != NULL){

                                    tmpString = pAttribute->name();
                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                    switch(map_035_NotConfigured[tmpString]){
                                    case MEMORY_008_SIZE:
                                        this->SysDump.Sections.Memory.SRAM.notConfigured = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_008_ERR:
                                        this->ErrorNr = 20305;
                                        this->Error = QString::fromStdString(tmpString);
                                        break;
                                    default:
                                        return 18;
                                    }
                                    pAttribute = pAttribute->next_attribute();
                                }
                                break;

                            case MEMORY_001_USERRAM:
                                pAttribute = pSubItem->first_attribute();
                                while(pAttribute != NULL){

                                    tmpString = pAttribute->name();
                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                    switch(map_033_USERRAMAttributes[tmpString]){
                                    case MEMORY_006_AVAILABLE:
                                        this->SysDump.Sections.Memory.SRAM.USERRAM.available = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_006_DEVICE:
                                        this->SysDump.Sections.Memory.SRAM.USERRAM.device = QString::fromStdString(pAttribute->value());
                                        break;

                                    case MEMORY_006_LARGESTBLOCK:
                                        this->SysDump.Sections.Memory.SRAM.USERRAM.LargestAvailableBlock = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_006_SIZE:
                                        this->SysDump.Sections.Memory.SRAM.USERRAM.size = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_006_USED:
                                        this->SysDump.Sections.Memory.SRAM.USERRAM.used = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_006_ERR:
                                        this->ErrorNr = 20306;
                                        this->Error = QString::fromStdString(tmpString);
                                        break;
                                    default:
                                        return 19;
                                    }
                                    pAttribute = pAttribute->next_attribute();
                                }
                                break;

                            case MEMORY_001_REMMEM:
                                pAttribute = pItem->first_attribute();
                                while(pAttribute != NULL){

                                    tmpString = pAttribute->name();
                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                    switch(map_034_REMMEMAttributes[tmpString]){
                                    case MEMORY_007_AVAILABLE:
                                        this->SysDump.Sections.Memory.SRAM.REMMEM.available = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_007_DEVICE:
                                        this->SysDump.Sections.Memory.SRAM.REMMEM.device = QString::fromStdString(pAttribute->value());
                                        break;

                                    case MEMORY_007_LARGESTBLOCK:
                                        this->SysDump.Sections.Memory.SRAM.REMMEM.LargestAvailableBlock = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_007_PERMANENT:
                                        this->SysDump.Sections.Memory.SRAM.REMMEM.permanent = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_007_REMGLOBAL:
                                        this->SysDump.Sections.Memory.SRAM.REMMEM.remGlobal = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_007_REMLOCAL:
                                        this->SysDump.Sections.Memory.SRAM.REMMEM.remLocal = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_007_SIZE:
                                        this->SysDump.Sections.Memory.SRAM.REMMEM.size = (qint32)atoi(pAttribute->value());
                                        break;

                                    case MEMORY_007_ERR:
                                        this->ErrorNr = 20307;
                                        this->Error = QString::fromStdString(tmpString);
                                        break;
                                    default:
                                        return 20;
                                    }
                                    pAttribute = pAttribute->next_attribute();
                                }
                                break;

                            case MEMORY_001_ERR:
                                this->ErrorNr = 20308;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 21;
                            }
                            pSubItem = pSubItem->next_sibling();
                        }
                        break;

                    case MEMORY_ERR:
                        this->ErrorNr = 20309;
                        this->Error = QString::fromStdString(tmpString);
                        break;
                    default:
                        return 22;
                    }
                    pItem = pItem->next_sibling();
                }
                break;

            case SECTION_TIMING:

                cnt = 0;

                // SECTION TIMING
                // ---------------------------------------------------------------------------
                pItem = pRoot->first_node()->first_node();
                while(pItem != NULL){

                    tmpString = pItem->name();
                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                    switch(map_016_Timing[tmpString]){
                    case TIMING_TIMERCONF:
                        pAttribute = pItem->first_attribute();
                        while(pAttribute != NULL){

                            tmpString = pAttribute->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_036_TimingTimerConfiguration[tmpString]){
                            case TIMING_001_IDLETASKCLASS:
                                this->SysDump.Sections.Timing.TimerConfig.IdleTaskClass = (qint16)atoi(pAttribute->value());
                                break;

                            case TIMING_001_MULTIPLEVALUE:
                                // there only can be one attribute, multiple or dividing ...
                                ui->text_TimerConf_mvostc->setText(tr("Multiple value of system timer cycle:"));
                                this->SysDump.Sections.Timing.TimerConfig.MultipleValueOfSTC = (qint16)atoi(pAttribute->value());
                                break;

                            case TIMING_001_DIVIDINGVALUE:
                                // ... using the same var and changing the label
                                ui->text_TimerConf_mvostc->setText(tr("Dividing value of system timer cycle:"));
                                this->SysDump.Sections.Timing.TimerConfig.MultipleValueOfSTC = (qint16)atoi(pAttribute->value());
                                break;

                            case TIMING_001_SYSTICK:
                                this->SysDump.Sections.Timing.TimerConfig.systemTick = (qint16)atoi(pAttribute->value());
                                break;

                            case TIMING_001_SYSTIMER:
                                this->SysDump.Sections.Timing.TimerConfig.SystemTimer = QString::fromStdString(pAttribute->value());
                                break;

                            case TIMING_001_TCIDLETIME:
                                this->SysDump.Sections.Timing.TimerConfig.TaskClassIdleTime = (qint16)atoi(pAttribute->value());
                                break;

                            case TIMING_001_ERR:
                                this->ErrorNr = 20401;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 23;
                            }
                            pAttribute = pAttribute->next_attribute();
                        }
                        break;

                    case TIMING_TASKCLASSCONF:
                        pSubItem = pItem->first_node();
                        while(pSubItem != NULL){

                            tmpString = pSubItem->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_017_TimingTaskClass[tmpString]){
                            case TIMING_000_TASKCLASS:
                                cnt++;
                                index = cnt-1;
                                this->SysDump.Sections.Timing.vTaskClass.push_back(sTaskClass());

                                pAttribute = pSubItem->first_attribute();
                                while(pAttribute!=NULL){

                                    tmpString = pAttribute->name();
                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                    switch(map_037_TimingTaskClassConfiguration[tmpString]){
                                    case TIMING_002_CYCLETIME:
                                        this->SysDump.Sections.Timing.vTaskClass[index].CycleTime = (qint32)atoi(pAttribute->value());
                                        break;

                                    case TIMING_002_CYCLIC:
                                        this->SysDump.Sections.Timing.vTaskClass[index].Cyclic = (qint16)atoi(pAttribute->value());
                                        break;

                                    case TIMING_002_INPUTDELAY:
                                        this->SysDump.Sections.Timing.vTaskClass[index].InputDelay = QString::fromStdString(pAttribute->value());
                                        break;

                                    case TIMING_002_OUTPUTDELAY:
                                        this->SysDump.Sections.Timing.vTaskClass[index].OutputDelay = QString::fromStdString(pAttribute->value());
                                        break;

                                    case TIMING_002_STACK:
                                        this->SysDump.Sections.Timing.vTaskClass[index].Stack = (qint16)atoi(pAttribute->value());
                                        break;

                                    case TIMING_002_TOLERANCE:
                                        this->SysDump.Sections.Timing.vTaskClass[index].Tolerance = (qint32)atoi(pAttribute->value());
                                        break;

                                    case TIMING_002_ERR:
                                        this->ErrorNr = 20402;
                                        this->Error = QString::fromStdString(tmpString);
                                        break;
                                    default:
                                        return 24;
                                    }
                                    pAttribute = pAttribute->next_attribute();
                                }
                                break;

                            case TIMING_000_ERR:
                                this->ErrorNr = 20403;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 25;
                            }
                            pSubItem = pSubItem->next_sibling();
                        }
                        break;

                    case TIMING_ERR:
                        this->ErrorNr = 20404;
                        this->Error = QString::fromStdString(tmpString);
                        break;
                    default:
                        return 26;
                    }
                    pItem = pItem->next_sibling();
                }
                break;

            case SECTION_CPUUSAGE:

                cnt = 0;

                // SECTION CPU USAGE
                // -------------------------------------------------------------------
                pItem = pRoot->first_node()->first_node();
                while(pItem != NULL){

                    tmpString = pItem->name();
                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                    switch(map_018_CpuUsage[tmpString]){
                    case CPUUSAGE_INTERVAL:
                        cnt++;
                        index = cnt-1;
                        this->SysDump.Sections.CpuUsage.vZoomInterval.push_back(sZoomInterval());

                        pAttribute = pItem->first_attribute();
                        while(pAttribute!=NULL){

                            tmpString = pAttribute->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_038_CpuUsageInterval[tmpString]){
                            case CPUUSAGE_000_AVERAGE:
                                this->SysDump.Sections.CpuUsage.vZoomInterval[index].average = (qint16)atoi(pAttribute->value());
                                break;

                            case CPUUSAGE_000_DESCRIPTION:
                                this->SysDump.Sections.CpuUsage.vZoomInterval[index].description = QString::fromStdString(pAttribute->value());
                                break;

                            case CPUUSAGE_000_ID:
                                this->SysDump.Sections.CpuUsage.vZoomInterval[index].id = (qint16)atoi(pAttribute->value());
                                break;

                            case CPUUSAGE_000_MAX:
                                this->SysDump.Sections.CpuUsage.vZoomInterval[index].maximum = (qint16)atoi(pAttribute->value());
                                break;

                            case CPUUSAGE_000_ERR:
                                this->ErrorNr = 20501;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 27;
                            }
                            pAttribute = pAttribute->next_attribute();
                        }

                        // Get the Values
                        pSubItem = pItem->first_node();
                        while(pSubItem != NULL){
                            tmpString = pSubItem->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_038_CpuUsageValues[tmpString]){
                                case CPUUSAGE_001_AVERAGE:
                                    pAttribute = pSubItem->first_attribute();
                                    tmpString = pAttribute->name();
                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                    if(tmpString=="values"){
                                        this->get_CpuUsageValues(&this->SysDump.Sections.CpuUsage.vZoomInterval[index].values.vAverage,pAttribute->value());
                                    }
                                    break;

                                case CPUUSAGE_001_MAXIMUM:
                                    pAttribute = pSubItem->first_attribute();
                                    tmpString = pAttribute->name();
                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                    if(tmpString=="values"){
                                        this->get_CpuUsageValues(&this->SysDump.Sections.CpuUsage.vZoomInterval[index].values.vMaximum,pAttribute->value());
                                    }
                                    break;

                                case CPUUSAGE_001_ERR:
                                    this->ErrorNr = 20503;
                                    this->Error = QString::fromStdString(tmpString);
                                    break;

                                default:
                                    return 27;

                            }
                            pSubItem = pSubItem->next_sibling();
                        }
                        break;

                    case CPUUSAGE_ERR:
                        this->ErrorNr = 20502;
                        this->Error = QString::fromStdString(tmpString);
                        break;
                    default:
                        return 28;
                    }
                    pItem = pItem->next_sibling();
                }

                break;

            case SECTION_SOFTWARE:

                cnt = 0;

                // SECTION SOFTWARE
                // -------------------------------------------------------------------
                pItem = pRoot->first_node()->first_node();
                while(pItem != NULL){

                    tmpString = pItem->name();
                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                    switch(map_019_Software[tmpString]){
                    case SOFTWARE_APPLICATIONMODULE:
                        cnt ++;
                        subcnt = 0;
                        index = cnt-1;
                        this->SysDump.Sections.Software.vAppModule.push_back(sAppModule());

                        this->SysDump.Sections.Software.vAppModule[index].id = pItem->first_attribute("Id")->value();
                        pSubItem = pItem->first_node();
                        while(pSubItem != NULL){

                            tmpString = pSubItem->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_020_SoftwareAppModule[tmpString]){
                            case SOFTWARE_000_MODULE:
                                subcnt++;
                                subindex = subcnt-1;
                                this->SysDump.Sections.Software.vAppModule[index].vModules.push_back(sSoftwModule());

                                pAttribute = pSubItem->first_attribute();
                                while(pAttribute != NULL){

                                    tmpString = pAttribute->name();
                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                    switch(map_039_ModuleAttributes[tmpString]){
                                    case SOFTWARE_001_ADRESS:
                                        this->SysDump.Sections.Software.vAppModule[index].vModules[subindex].address = QString::fromStdString(pAttribute->value());
                                        break;

                                    case SOFTWARE_001_ID:
                                        this->SysDump.Sections.Software.vAppModule[index].vModules[subindex].id = (qint16)atoi(pAttribute->value());
                                        break;

                                    case SOFTWARE_001_MEMORY:
                                        this->SysDump.Sections.Software.vAppModule[index].vModules[subindex].memory = QString::fromStdString(pAttribute->value());
                                        break;

                                    case SOFTWARE_001_NAME:
                                        this->SysDump.Sections.Software.vAppModule[index].vModules[subindex].name = QString::fromStdString(pAttribute->value());
                                        break;

                                    case SOFTWARE_001_SIZE:
                                        this->SysDump.Sections.Software.vAppModule[index].vModules[subindex].size = (qint32)atoi(pAttribute->value());
                                        break;

                                    case SOFTWARE_001_TIMESTAMP:
                                        this->SysDump.Sections.Software.vAppModule[index].vModules[subindex].timestamp = QString::fromStdString(pAttribute->value());
                                        break;

                                    case SOFTWARE_001_TYPE:
                                        this->SysDump.Sections.Software.vAppModule[index].vModules[subindex].type = (qint16)atoi(pAttribute->value());
                                        break;

                                    case SOFTWARE_001_VERSION:
                                        this->SysDump.Sections.Software.vAppModule[index].vModules[subindex].version = QString::fromStdString(pAttribute->value());
                                        break;

                                    case SOFTWARE_001_ERR:
                                        this->ErrorNr = 20601;
                                        this->Error = QString::fromStdString(tmpString);
                                        break;
                                    default:
                                        return 29;
                                    }
                                    pAttribute = pAttribute->next_attribute();
                                }

                                break;
                            case SOFTWARE_000_ERR:
                                this->ErrorNr = 20602;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 30;
                            }
                            pSubItem = pSubItem->next_sibling();
                        }

                        break;

                    case SOFTWARE_MODULE:
                        // Older Targets dont have Application Modules->Place all Modules in last AppMod!!
                        //subindex = sizeof(this->SysDump.Sections.Software.pAppModule)/sizeof(this->SysDump.Sections.Software.pAppModule[0])-1;
                        cnt++;
                        index = cnt-1;
                        if (this->SysDump.Sections.Software.vAppModule.size() == 0){
                            this->SysDump.Sections.Software.vAppModule.push_back(sAppModule());
                            this->SysDump.Sections.Software.vAppModule[0].id = tr("<no module ...>");
                        }
                        this->SysDump.Sections.Software.vAppModule[0].vModules.push_back(sSoftwModule());

                        pAttribute = pItem->first_attribute();
                        while(pAttribute != NULL){

                            tmpString = pAttribute->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_039_ModuleAttributes[tmpString]){
                            case SOFTWARE_001_ADRESS:
                                this->SysDump.Sections.Software.vAppModule[0].vModules[index].address = QString::fromStdString(pAttribute->value());
                                break;

                            case SOFTWARE_001_ID:
                                this->SysDump.Sections.Software.vAppModule[0].vModules[index].id = (qint16)atoi(pAttribute->value());
                                break;

                            case SOFTWARE_001_MEMORY:
                                this->SysDump.Sections.Software.vAppModule[0].vModules[index].memory = QString::fromStdString(pAttribute->value());
                                break;

                            case SOFTWARE_001_NAME:
                                this->SysDump.Sections.Software.vAppModule[0].vModules[index].name = QString::fromStdString(pAttribute->value());
                                break;

                            case SOFTWARE_001_SIZE:
                                this->SysDump.Sections.Software.vAppModule[0].vModules[index].size = (qint32)atoi(pAttribute->value());
                                break;

                            case SOFTWARE_001_TIMESTAMP:
                                this->SysDump.Sections.Software.vAppModule[0].vModules[index].timestamp = QString::fromStdString(pAttribute->value());
                                break;

                            case SOFTWARE_001_TYPE:
                                this->SysDump.Sections.Software.vAppModule[0].vModules[index].type = (qint16)atoi(pAttribute->value());
                                break;

                            case SOFTWARE_001_VERSION:
                                this->SysDump.Sections.Software.vAppModule[0].vModules[index].version = QString::fromStdString(pAttribute->value());
                                break;

                            case SOFTWARE_001_ERR:
                                this->ErrorNr = 20604;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 29;
                            }
                            pAttribute = pAttribute->next_attribute();
                        }

                        break;

                    case SOFTWARE_ERR:
                        this->ErrorNr = 20603;
                        this->Error = QString::fromStdString(tmpString);
                        break;
                    default:
                        return 31;
                    }
                    pItem = pItem->next_sibling();
                }
                break;

            case SECTION_HARDWARE:

                cnt = 0;

                // SECTION HARDWARE
                // -------------------------------------------------------------------
                pItem = pRoot->first_node()->first_node();
                while(pItem != NULL){

                    tmpString = pItem->name();
                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                    switch(map_021_Hardware[tmpString]){
                    case HARDWARE_NODE:

                        // Create node and get attributes
                        cnt++;
                        subcnt = 0;
                        index = cnt-1;
                        this->SysDump.Sections.Hardware.vNode.push_back(sNode());

                        // ATTRIBUTES FROM NODE
                        // -------------------------------------------------------
                        pAttribute = pItem->first_attribute();
                        while(pAttribute!=NULL){

                            tmpString = pAttribute->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_023_HardwareNodeAttributes[tmpString]){
                            case HARDWARE_001_CONFIGURED:
                                this->SysDump.Sections.Hardware.vNode[index].Configured = QString::fromStdString(pAttribute->value());
                                break;

                            case HARDWARE_001_IOINFO:
                                this->SysDump.Sections.Hardware.vNode[index].IOinfo = (qint16)atoi(pAttribute->value());
                                break;

                            case HARDWARE_001_ID:
                                this->SysDump.Sections.Hardware.vNode[index].ID = (qint16)atoi(pAttribute->value());
                                break;

                            case HARDWARE_001_STATUS:
                                this->SysDump.Sections.Hardware.vNode[index].Status = (qint16)atoi(pAttribute->value());
                                break;

                            case HARDWARE_001_ERR:
                                this->ErrorNr = 20701;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 32;
                            }
                            pAttribute = pAttribute->next_attribute();
                        }

                        // SUTITEMS FROM NODE
                        // -------------------------------------------------------
                        pSubItem = pItem->first_node();
                        while(pSubItem != NULL){

                            tmpString = pSubItem->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_022_HardwareNode[tmpString]){
                            case HARDWARE_000_MODULESTATUS:
                                pAttribute = pSubItem->first_attribute();
                                while(pAttribute != NULL){

                                    tmpString = pAttribute->name();
                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                    switch(map_024_HardwareModuleStatusAttributes[tmpString]){
                                    case HARDWARE_002_CONFIGURED:
                                        this->SysDump.Sections.Hardware.vNode[index].ModuleStatus.Configured = QString::fromStdString(pAttribute->value());
                                        break;

                                    case HARDWARE_002_MODULEOK:
                                        this->SysDump.Sections.Hardware.vNode[index].ModuleStatus.ModuleOk = (qint16)atoi(pAttribute->value());
                                        break;

                                    case HARDWARE_002_PLUGGED:
                                        this->SysDump.Sections.Hardware.vNode[index].ModuleStatus.Plugged = QString::fromStdString(pAttribute->value());
                                        break;

                                    case HARDWARE_002_SUPERVISED:
                                        this->SysDump.Sections.Hardware.vNode[index].ModuleStatus.Supervised = (qint16)atoi(pAttribute->value());
                                        break;

                                    case HARDWARE_002_ERR:
                                        this->ErrorNr = 20702;
                                        this->Error = QString::fromStdString(tmpString);
                                        break;
                                    default:
                                        return 33;
                                    }
                                    pAttribute = pAttribute->next_attribute();
                                }
                                break;

                            case HARDWARE_000_IOINFORMATION:
                                pAttribute = pSubItem->first_attribute();
                                while(pAttribute != NULL){

                                    tmpString = pAttribute->name();
                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                    switch(map_025_HardwareIOInfoAttributes[tmpString]){
                                    case HARDWARE_003_EQUIPMENTID:
                                        this->SysDump.Sections.Hardware.vNode[index].IOInformation.EquipmentID = QString::fromStdString(pAttribute->value());
                                        break;

                                    case HARDWARE_003_FWVERSION:
                                        this->SysDump.Sections.Hardware.vNode[index].IOInformation.FwVersion = QString::fromStdString(pAttribute->value());
                                        break;

                                    case HARDWARE_003_HWREVISION:
                                        this->SysDump.Sections.Hardware.vNode[index].IOInformation.HardwareRevision = QString::fromStdString(pAttribute->value());
                                        break;

                                    case HARDWARE_003_HWVARIANT:
                                        this->SysDump.Sections.Hardware.vNode[index].IOInformation.HardwareVariant = QString::fromStdString(pAttribute->value());
                                        break;

                                    case HARDWARE_003_MODPATH:
                                        this->SysDump.Sections.Hardware.vNode[index].IOInformation.ModulePath = QString::fromStdString(pAttribute->value());
                                        break;

                                    case HARDWARE_003_SERIALNR:
                                        this->SysDump.Sections.Hardware.vNode[index].IOInformation.Serialnumber = QString::fromStdString(pAttribute->value());
                                        break;

                                    case HARDWARE_003_ERR:
                                        this->ErrorNr = 20703;
                                        this->Error = QString::fromStdString(tmpString);
                                        break;
                                    default:
                                        return 34;
                                    }
                                    pAttribute = pAttribute->next_attribute();
                                }
                                break;

                            case HARDWARE_000_IOINFO:
                                pSubSubItem = pSubItem->first_node();
                                while(pSubSubItem != NULL){

                                    tmpString = pSubSubItem->name();
                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                    switch(map_026_HardwareNodeIoInfo[tmpString]){
                                    case HARDWARE_004_CHANNEL:
                                        subcnt++;
                                        subindex = subcnt-1;
                                        this->SysDump.Sections.Hardware.vNode[index].IOChannels.vChannel.push_back(sChannel());

                                        pAttribute = pSubSubItem->first_attribute();
                                        while(pAttribute != NULL){

                                            tmpString = pAttribute->name();
                                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                            switch(map_027_HardwareChannelAttributes[tmpString]){
                                            case HARDWARE_005_DIAGNOSE:
                                                this->SysDump.Sections.Hardware.vNode[index].IOChannels.vChannel[subindex].Diagnose = QString::fromStdString(pAttribute->value());
                                                break;

                                            case HARDWARE_005_FORCESTATUS:
                                                this->SysDump.Sections.Hardware.vNode[index].IOChannels.vChannel[subindex].ForceStatus = QString::fromStdString(pAttribute->value());
                                                break;

                                            case HARDWARE_005_FORCEVALUE:
                                                this->SysDump.Sections.Hardware.vNode[index].IOChannels.vChannel[subindex].ForceValue = QString::fromStdString(pAttribute->value());
                                                break;

                                            case HARDWARE_005_IECTYPE:
                                                this->SysDump.Sections.Hardware.vNode[index].IOChannels.vChannel[subindex].IECType = QString::fromStdString(pAttribute->value());
                                                break;

                                            case HARDWARE_005_IECVALUE:
                                                this->SysDump.Sections.Hardware.vNode[index].IOChannels.vChannel[subindex].IECValue = QString::fromStdString(pAttribute->value());
                                                break;

                                            case HARDWARE_005_ID:
                                                this->SysDump.Sections.Hardware.vNode[index].IOChannels.vChannel[subindex].ID = (qint16)atoi(pAttribute->value());
                                                break;

                                            case HARDWARE_005_NAME:
                                                this->SysDump.Sections.Hardware.vNode[index].IOChannels.vChannel[subindex].Name = QString::fromStdString(pAttribute->value());
                                                break;

                                            case HARDWARE_005_PHYSICALVALUE:
                                                this->SysDump.Sections.Hardware.vNode[index].IOChannels.vChannel[subindex].PhysicalValue = QString::fromStdString(pAttribute->value());
                                                break;

                                            case HARDWARE_005_TYPE:
                                                this->SysDump.Sections.Hardware.vNode[index].IOChannels.vChannel[subindex].Type = (qint16)atoi(pAttribute->value());
                                                break;

                                            case HARDWARE_005_ERR:
                                                this->ErrorNr = 20704;
                                                this->Error = QString::fromStdString(tmpString);
                                                break;
                                            default:
                                                return 35;
                                            }
                                            pAttribute = pAttribute->next_attribute();
                                        }

                                        break;

                                    case HARDWARE_004_ERR:
                                        this->ErrorNr = 20705;
                                        this->Error = QString::fromStdString(tmpString);
                                        break;
                                    default:
                                        return 36;
                                    }
                                    pSubSubItem = pSubSubItem->next_sibling();
                                }
                                break;

                            case HARDWARE_000_ERR:
                                this->ErrorNr = 20706;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 37;
                            }
                            pSubItem = pSubItem->next_sibling();
                        }

                        break;

                    case HARDWARE_ERR:
                        this->ErrorNr = 20707;
                        this->Error = QString::fromStdString(tmpString);
                        break;
                    default:
                        return 38;
                    }
                    pItem = pItem->next_sibling();
                }
                break;

            case SECTION_MOTION:

                cnt = 0;

                // SECTION Motion
                // -------------------------------------------------------------------
                pItem = pRoot->first_node();
                while(pItem != NULL){

                    tmpString = pItem->name();
                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                    switch(map_040_Motion[tmpString]){
                    case MOTION_000_MOTION:
                        pAttribute = pItem->first_attribute();
                        while(pAttribute != NULL){

                            tmpString = pAttribute->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_041_MotionAttributes[tmpString]){
                            case MOTION_004_AVAILABLE:
                                this->SysDump.Sections.Motion.Available = (bool)atoi(pAttribute->value());

                                if(this->SysDump.Sections.Motion.Available==true){
                                    pSubItem = pItem->first_node();
                                    while(pSubItem != NULL){

                                        tmpString = pSubItem->name();
                                        std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                        switch(map_042_MotionNodes[tmpString]){
                                        case MOTION_AXIS:
                                            cnt++;
                                            subcnt = 0;
                                            index = cnt-1;
                                            this->SysDump.Sections.Motion.vAxis.push_back(sAxis());

                                            pAttribute2 = pSubItem->first_attribute();
                                            while(pAttribute2 != NULL){

                                                tmpString = pAttribute2->name();
                                                std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                                switch(map_045_MotionAxisAttribute[tmpString]){
                                                case MOTION_001_ACTPOS:
                                                    this->SysDump.Sections.Motion.vAxis[index].ActPosition = (qint32)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_ACTSPEED:
                                                    this->SysDump.Sections.Motion.vAxis[index].ActSpeed = (qint32)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_ADRESS:
                                                    this->SysDump.Sections.Motion.vAxis[index].Adress = QString::fromStdString(pAttribute2->value());
                                                    break;

                                                case MOTION_001_ALARM:
                                                    this->SysDump.Sections.Motion.vAxis[index].Alarm = (bool)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_CONTROLLER:
                                                    this->SysDump.Sections.Motion.vAxis[index].Controller = (bool)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_DRIVE:
                                                    this->SysDump.Sections.Motion.vAxis[index].Drive = QString::fromStdString(pAttribute2->value());
                                                    break;

                                                case MOTION_001_ENABLE:
                                                    this->SysDump.Sections.Motion.vAxis[index].Enable = (bool)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_HOMING:
                                                    this->SysDump.Sections.Motion.vAxis[index].Homing = (bool)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_ID:
                                                    this->SysDump.Sections.Motion.vAxis[index].ID = QString::fromStdString(pAttribute2->value());
                                                    break;

                                                case MOTION_001_MOVEMENTSTATUS:
                                                    this->SysDump.Sections.Motion.vAxis[index].MovementStatus = (qint16)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_NAME:
                                                    this->SysDump.Sections.Motion.vAxis[index].Name = QString::fromStdString(pAttribute2->value());
                                                    break;

                                                case MOTION_001_NEGENDSW:
                                                    this->SysDump.Sections.Motion.vAxis[index].NegEndSw = (bool)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_POSENDSW:
                                                    this->SysDump.Sections.Motion.vAxis[index].PosEndSw = (bool)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_REFSW:
                                                    this->SysDump.Sections.Motion.vAxis[index].RefSw = (bool)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_SIMULATION:
                                                    this->SysDump.Sections.Motion.vAxis[index].Simulation = (bool)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_TRIGGER1:
                                                    this->SysDump.Sections.Motion.vAxis[index].Trigger1 = (bool)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_TRIGGER2:
                                                    this->SysDump.Sections.Motion.vAxis[index].Trigger2 = (bool)atoi(pAttribute2->value());
                                                    break;

                                                case MOTION_001_ERR:
                                                    this->ErrorNr = 20803;
                                                    this->Error = QString::fromStdString(tmpString);
                                                    break;
                                                default:
                                                    return 42;
                                                }
                                                pAttribute2 = pAttribute2->next_attribute();
                                            }

                                            break;

                                        case MOTION_ALARM:

                                            if(this->SysDump.Sections.Motion.vAxis.size() > 0){
                                                pSubSubItem = pSubItem->first_node();
                                                while(pSubSubItem != NULL){

                                                    tmpString = pSubSubItem->name();
                                                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                                    switch(map_043_MotionAlarm[tmpString]){
                                                    case MOTION_003_ERRORENTRY:
                                                        subcnt++;
                                                        subindex = subcnt-1;
                                                        this->SysDump.Sections.Motion.vAxis[index].vAlarm.push_back(sAlarm());

                                                        pAttribute2 = pSubSubItem->first_attribute();
                                                        while(pAttribute2 != NULL){

                                                            tmpString = pAttribute2->name();
                                                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                                            switch(map_046_MotionAlarmAttributes[tmpString]){
                                                            case MOTION_002_ERRORNO:
                                                                this->SysDump.Sections.Motion.vAxis[index].vAlarm[subindex].ErrorNr = (qint16)atoi(pAttribute2->value());
                                                                break;

                                                            case MOTION_002_ERRORTEXT:
                                                                this->SysDump.Sections.Motion.vAxis[index].vAlarm[subindex].ErrorText = QString::fromStdString(pAttribute2->value());
                                                                break;

                                                            case MOTION_002_ID:
                                                                this->SysDump.Sections.Motion.vAxis[index].vAlarm[subindex].ID = (qint16)atoi(pAttribute2->value());
                                                                break;

                                                            case MOTION_002_PARID:
                                                                this->SysDump.Sections.Motion.vAxis[index].vAlarm[subindex].ParID = (qint16)atoi(pAttribute2->value());
                                                                break;

                                                            case MOTION_002_STATUS:
                                                                this->SysDump.Sections.Motion.vAxis[index].vAlarm[subindex].Status = (qint16)atoi(pAttribute2->value());
                                                                break;

                                                            case MOTION_002_TEXTINFO:
                                                                this->SysDump.Sections.Motion.vAxis[index].vAlarm[subindex].TextInfo = QString::fromStdString(pAttribute2->value());
                                                                break;

                                                            case MOTION_002_TEXTPARID:
                                                                this->SysDump.Sections.Motion.vAxis[index].vAlarm[subindex].TextParID = (qint16)atoi(pAttribute2->value());
                                                                break;

                                                            case MOTION_002_TIMESTAMP:
                                                                this->SysDump.Sections.Motion.vAxis[index].vAlarm[subindex].TimeStamp = QString::fromStdString(pAttribute2->value());
                                                                break;

                                                            case MOTION_002_ERR:
                                                                this->ErrorNr = 20805;
                                                                this->Error = QString::fromStdString(tmpString);
                                                                break;
                                                            default:
                                                                return 43;
                                                            }
                                                            pAttribute2 = pAttribute2->next_attribute();
                                                        }

                                                        break;

                                                    case MOTION_003_ERR:
                                                        this->ErrorNr = 20806;
                                                        this->Error = QString::fromStdString(tmpString);
                                                        break;
                                                    default:
                                                        return 44;
                                                    }
                                                    pSubSubItem = pSubSubItem->next_sibling();
                                                }
                                            } else {
                                                // Axisindex error
                                            }

                                            break;

                                        case MOTION_ERR:
                                            this->ErrorNr = 20804;
                                            this->Error = QString::fromStdString(tmpString);
                                            break;
                                        default:
                                            return 41;
                                        }
                                        pSubItem = pSubItem->next_sibling();
                                    }


                                }
                                break;

                            case MOTION_004_ERR:
                                this->ErrorNr = 20802;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 40;
                            }
                            pAttribute = pAttribute->next_attribute();
                        }
                        break;

                    case MOTION_000_ERR:
                        this->ErrorNr = 20801;
                        this->Error = QString::fromStdString(tmpString);
                        break;
                    default:
                        return 39;
                    }
                    pItem = pItem->next_sibling();
                }
                break;

            case SECTION_LOGGER:

                cnt = 0;

                // SECTION Logger
                // -------------------------------------------------------------------
                pItem = pRoot->first_node()->first_node();
                while(pItem != NULL){

                    tmpString = pItem->name();
                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                    switch(map_050_Logger[tmpString]){
                    case LOGGER_MODULE:
                        cnt++;
                        subcnt = 0;
                        index = cnt-1;
                        this->SysDump.Sections.Logger.vModule.push_back(sLogModule());

                        // Attributes
                        pAttribute = pItem->first_attribute();
                        while(pAttribute != NULL){

                            tmpString = pAttribute->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_051_LoggerModuleAttributes[tmpString]){
                            case LOGGER_000_ADRESS:
                                this->SysDump.Sections.Logger.vModule[index].Adress = QString::fromStdString(pAttribute->value());
                                break;

                            case LOGGER_000_ID:
                                this->SysDump.Sections.Logger.vModule[index].ID = (qint16)atoi(pAttribute->value());
                                break;

                            case LOGGER_000_NAME:
                                this->SysDump.Sections.Logger.vModule[index].Name = QString::fromStdString(pAttribute->value());
                                break;

                            case LOGGER_000_VERSION:
                                this->SysDump.Sections.Logger.vModule[index].Version = QString::fromStdString(pAttribute->value());
                                break;

                            case LOGGER_000_ERR:
                                this->ErrorNr = 20902;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 46;
                            }
                            pAttribute = pAttribute->next_attribute();
                        }

                        // Entries
                        pSubItem = pItem->first_node()->first_node();
                        while(pSubItem != NULL){

                            tmpString = pSubItem->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_052_LoggerModuleEntry[tmpString]){
                            case LOGGER_001_ENTRY:

                                subcnt++;
                                subindex = subcnt-1;

                                // "OLD" Logger -> v1.00.0
                                if(this->SysDump.Sections.Logger.vModule[index].Version==QString("1.00.0")){

                                    this->SysDump.Sections.Logger.vModule[index].vEntry.push_back(sLogModuleEntry());

                                    pAttribute = pSubItem->first_attribute();
                                    while(pAttribute != NULL){

                                        tmpString = pAttribute->name();
                                        std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                        switch(map_053_LoggerModuleEntryAttributes[tmpString]){
                                        case LOGGER_002_ASCIIDATA:
                                            this->SysDump.Sections.Logger.vModule[index].vEntry[subindex].ASCIIData = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_002_BINARYDATA:
                                            this->SysDump.Sections.Logger.vModule[index].vEntry[subindex].BinaryData = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_002_ERRDESCRIPTION:
                                            this->SysDump.Sections.Logger.vModule[index].vEntry[subindex].ErrorDescription = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_002_EVENTID:
                                        case LOGGER_002_ERRORNR:
                                            this->SysDump.Sections.Logger.vModule[index].vEntry[subindex].ErrorNr = (qint32)atoi(pAttribute->value());
                                            break;

                                        case LOGGER_002_OBJECTID:
                                            // Nothing yet
                                            break;

                                        case LOGGER_002_ID:
                                            this->SysDump.Sections.Logger.vModule[index].vEntry[subindex].ID = (qint32)atoi(pAttribute->value());
                                            break;

                                        case LOGGER_002_LEVEL:
                                            this->SysDump.Sections.Logger.vModule[index].vEntry[subindex].Level = (qint16)atoi(pAttribute->value());
                                            break;

                                        case LOGGER_002_OSTASK:
                                            this->SysDump.Sections.Logger.vModule[index].vEntry[subindex].OSTask = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_002_TIMESTAMP:
                                            this->SysDump.Sections.Logger.vModule[index].vEntry[subindex].TimeStamp = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_002_ERR:
                                            this->ErrorNr = 20904;
                                            this->Error = QString::fromStdString(tmpString);
                                            break;
                                        default:
                                            return 48;
                                        }

                                        pAttribute = pAttribute->next_attribute();
                                    }

                                    this->SysDump.Sections.Logger.vModule[index].vEntry[subindex].Logger = this->SysDump.Sections.Logger.vModule[index].Name;

                                // END OF LOGGER v1.00.0
                                } else {

                                    // LOGGER OTHER THAN V1.00.0 -> ("V1.02.0") or other
                                    this->SysDump.Sections.Logger.vModule[index].vEntryV2.push_back(sLogModuleEntryV2());

                                    pAttribute = pSubItem->first_attribute();
                                    while(pAttribute != NULL){

                                        tmpString = pAttribute->name();
                                        std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                                        switch(map_054_LoggerModuleEntryAttributesV2[tmpString]){

                                        case LOGGER_202_ASCIIDATA:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].ASCIIData = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_202_BINARYDATA:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].BinaryData = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_202_DESCRIPTION:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].Description = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_202_ENTEREDBY:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].EnteredBy = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_202_EVENTID:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].EventID = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_202_FACILITY:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].Facility = (qint32)atoi(pAttribute->value());
                                            break;

                                        case LOGGER_202_ID:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].ID = (qint32)atoi(pAttribute->value());
                                            break;

                                        case LOGGER_202_INFO:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].Info = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_202_ORIGINRECORDID:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].OriginRecID = (qint32)atoi(pAttribute->value());
                                            break;

                                        case LOGGER_202_SEVERITY:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].Severity = (qint32)atoi(pAttribute->value());
                                            break;

                                        case LOGGER_202_TIMESTAMP:
                                            this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].TimeStamp = QString::fromStdString(pAttribute->value());
                                            break;

                                        case LOGGER_202_ERR:
                                            this->ErrorNr = 20905;
                                            this->Error = QString::fromStdString(tmpString);
                                            break;
                                        default:
                                            return 51;
                                        }

                                        pAttribute = pAttribute->next_attribute();
                                    }

                                    this->SysDump.Sections.Logger.vModule[index].vEntryV2[subindex].Logger = this->SysDump.Sections.Logger.vModule[index].Name;

                                } // END OF LOGGER V1.02.0 AND LATER

                                break;

                            case LOGGER_001_ERR:
                                this->ErrorNr = 20903;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 47;
                            }

                            pSubItem = pSubItem->next_sibling();
                        } // WHILE LOGGER MODULE
                        break;

                    case LOGGER_ERR:
                        this->ErrorNr = 20901;
                        this->Error = QString::fromStdString(tmpString);
                        break;
                    default:
                        return 45;
                    }
                    pItem = pItem->next_sibling();
                }
                break;

            case SECTION_PROFILER:

                cnt = 0;

                // SECTION Logger
                // -------------------------------------------------------------------
                pItem = pRoot->first_node()->first_node();
                while(pItem != NULL){

                    tmpString = pItem->name();
                    std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                    switch(map_060_Profiler[tmpString]){
                    case PROFILER_MODULE:
                        cnt++;
                        index = cnt-1;
                        this->SysDump.Sections.Profiler.vModule.push_back(sProfModule());

                        pAttribute = pItem->first_attribute();
                        while(pAttribute != NULL){

                            tmpString = pAttribute->name();
                            std::transform(tmpString.begin(), tmpString.end(), tmpString.begin(), ::tolower);

                            switch(map_061_ProfilerAttributes[tmpString]){
                            case PROFILER_000_ID:
                                this->SysDump.Sections.Profiler.vModule[index].ID = (qint16)atoi(pAttribute->value());
                                break;

                            case PROFILER_000_NAME:
                                this->SysDump.Sections.Profiler.vModule[index].Name = QString::fromStdString(pAttribute->value());
                                break;

                            case PROFILER_000_SIZE:
                                this->SysDump.Sections.Profiler.vModule[index].Size = (qint32)atoi(pAttribute->value());
                                break;

                            case PROFILER_000_TIMESTAMP:
                                this->SysDump.Sections.Profiler.vModule[index].TimeStamp = QString::fromStdString(pAttribute->value());
                                break;

                            case PROFILER_000_ERR:
                                this->ErrorNr = 21002;
                                this->Error = QString::fromStdString(tmpString);
                                break;
                            default:
                                return 50;
                            }
                            pAttribute = pAttribute->next_attribute();
                        }

                        break;
                    case PROFILER_ERR:
                        this->ErrorNr = 21001;
                        this->Error = QString::fromStdString(tmpString);
                        break;
                    default:
                        return 49;
                    }
                    pItem = pItem->next_sibling();
                }
                break;

            case SECTION_ADDITIONAL:

                // SECTION Additional
                // -------------------------------------------------------------------
                // Additional Dump Data -> Nothing to do...
                break;

            case SECTION_ERR:
                break;
            default:
                return 2;
            }
        } // endif (Attribute=="title")
        pRoot = pRoot->next_sibling();
    }

    // ------------------------------------------------------------------------------------------------
    // FINISHED PARSING XML NODES

    return 0;
}

int MainWindow::get_StringsForIdAndValue(xml_node<> *pItem, string * const &tmpString, string * const &tmpString2){

    xml_attribute<> *pAttribute, *pAttribute2;

    pAttribute = pItem->first_attribute();
    *tmpString = pAttribute->name();
    std::transform(tmpString->begin(), tmpString->end(), tmpString->begin(), ::tolower);

    if(*tmpString=="value" || *tmpString=="id"){

        pAttribute2 = pAttribute->next_attribute();

        *tmpString = pAttribute2->name();
        std::transform(tmpString->begin(), tmpString->end(), tmpString->begin(), ::tolower);

        if(*tmpString=="value" || *tmpString=="id"){

            (*tmpString == "value") ? *tmpString = pAttribute->value() : *tmpString2 = pAttribute->value() ;
            (*tmpString == "id") ? *tmpString = pAttribute2->value() : *tmpString2 = pAttribute2->value();
            std::transform(tmpString->begin(), tmpString->end(), tmpString->begin(), ::tolower);

            return 0;

        } else {

            return 1;
        }
    } else {

        return 1;
    }

}

int MainWindow::get_CpuUsageValues(vector<int> *values, char *cValues){
    std::string str(cValues);
    std::stringstream ss(str);
    int i;

    while (ss >> i)
    {
        values->push_back(i);

        if (ss.peek() == ',')
            ss.ignore();
    }
    return 0;
}
