# SystemDumpViewer
Viewer for SystemDump.xml files of [B&amp;R](https://www.br-automation.com) PLCs with a few nice features...

![SystemDumpViewer](https://github.com/bee-eater/SystemDumpViewer/blob/master/99_projinfo/Screenshot_211022169.png)

# How to compile
1. Get Qt 5.6.X from here: [Download](http://download.qt.io/official_releases/qt/5.6/5.6.3/qt-opensource-windows-x86-mingw492-5.6.3.exe)
1. Install Qt with mingw492_32
	*Newer Qt versions seem to have this Bug/Problem [Link](https://bugreports.qt.io/browse/QTBUG-57841)
1. Copy the qwt.dll from libs folder to you build/debug or build/release folder

# More info about the project
## Recommended folder structure
To make the .bat scripts and everything run smoothly, the following structure
is recommended:
![FolderStructure](https://github.com/bee-eater/SystemDumpViewer/blob/master/99_projinfo/folder_structure.png)

## Lanugages
Qt supports translation in the code.
If you want text translated, use tr("mytext"), if not use QString("mytext")

Steps with bt_EditLanguages.bat to add translations
1. Option 5 in batch: Update and remove obsolete
2. Edit the desired languages in Linguist (currently german, english, french, russian and polish)
3. Option 4 in batch: Release
4. Option 8 in batch: Copy to debug

Now you should have up to date .qm files and the translation should work in your debug environment

## Program structure 
**A very brief overview :-)**

### systemdump.h
Here is the main systemdump structure defined for the member this->SysDump in the main class
New values have to be added here.

### main_mapXml.cpp
In file main_mapXml.cpp there are the possible entries in the Systemdump.xml defined
as mappings with lowercase texts and enums, so switch cases can be used.
The std::maps and the enums themselves have to be defined in the mainwindow.h (Starting around line 212) aswell.

### main_readXml.cpp
In file main_readXml the values are read from the xml structure with the rapidxml library.
The function get_SystemDumpSections(...) reads all the sections. The error number thrown if
an unknown attribute is found defines the place in the code where to look for easier locating.
See file errorNumbers.txt for (possibly incomplete) list of error Numbers.
The values are written to this->SysDump...

### main_displayValues.cpp
Here the Values are displayed on the visu, using the values from this->SysDump
