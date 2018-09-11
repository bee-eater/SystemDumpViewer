; -- BrdhgDumpViewer.iss --
#define MainBinaryName  "BR.SystemdumpViewer.exe"
#define BaseName        "Systemdump Viewer"
#define SetupBaseName   "Setup_SystemdumpViewer_"
#define AppVersion      GetFileVersion(AddBackslash(SourcePath) + MainBinaryName)
#define AVF1            Copy(AppVersion, 1, Pos(".", AppVersion) - 1) + "_" + Copy(AppVersion, Pos(".", AppVersion) + 1)
#define AVF2            Copy(AVF1,       1, Pos(".", AVF1      ) - 1) + "_" + Copy(AVF1      , Pos(".", AVF1      ) + 1)
#define AppVersionFile  Copy(AVF2,       1, Pos(".", AVF2      ) - 1) + "_" + Copy(AVF2      , Pos(".", AVF2      ) + 1)

[Setup]
AppName=Systemdump Viewer
AppId=29567720-C543-11E3-9C1A-0800200C9A66
AppVersion=1.2.42.20209
DefaultDirName={pf}\{#BaseName}
DefaultGroupName=B&R Automation\{#BaseName}
DisableDirPage=auto
DisableProgramGroupPage=auto
UninstallDisplayIcon={app}\{#MainBinaryName}
Compression=lzma2
SolidCompression=yes
OutputDir="./setup"
OutputBaseFilename={#SetupBaseName + AppVersionFile}

[Files]
Source: "BR.SystemdumpViewer.exe"; DestDir: "{app}"
Source: "help\BR.SystemdumpViewer.chm"; DestDir: "{app}\help"; Flags: isreadme
Source: "platforms\qwindows.dll"; DestDir: "{app}\platforms"
Source: "platforms\qoffscreen.dll"; DestDir: "{app}\platforms"
Source: "platforms\qminimal.dll"; DestDir: "{app}\platforms"
Source: "D3DCompiler_43.dll"; DestDir: "{app}"
Source: "icudt51.dll"; DestDir: "{app}"
Source: "icuin51.dll"; DestDir: "{app}"
Source: "icuuc51.dll"; DestDir: "{app}"
Source: "libEGL.dll"; DestDir: "{app}"
Source: "libgcc_s_dw2-1.dll"; DestDir: "{app}"
Source: "libGLESv2.dll"; DestDir: "{app}"
Source: "libstdc++-6.dll"; DestDir: "{app}"
Source: "libwinpthread-1.dll"; DestDir: "{app}"
Source: "Qt5OpenGL.dll"; DestDir: "{app}"
Source: "Qt5Core.dll"; DestDir: "{app}"
Source: "Qt5Gui.dll"; DestDir: "{app}"
Source: "Qt5PrintSupport.dll"; DestDir: "{app}"
Source: "Qt5Svg.dll"; DestDir: "{app}"
Source: "Qt5Widgets.dll"; DestDir: "{app}"
Source: "qwt.dll"; DestDir: "{app}"
Source: "config.ini"; DestDir: "{app}"

[Icons]
Name: "{commondesktop}\{#BaseName}"; Filename: "{app}\{#MainBinaryName}"; WorkingDir: "{app}" 
Name: "{group}\{#BaseName}"; Filename: "{app}\{#MainBinaryName}"
Name: "{group}\Uninstall"; Filename: "{app}\unins000.exe"

[code]
/////////////////////////////////////////////////////////////////////
function GetUninstallString(): String;
var
  sUnInstPath: String;
  sUnInstallString: String;
begin
  sUnInstPath := ExpandConstant('Software\Microsoft\Windows\CurrentVersion\Uninstall\{#emit SetupSetting("AppId")}_is1');
  sUnInstallString := '';
  if not RegQueryStringValue(HKLM, sUnInstPath, 'UninstallString', sUnInstallString) then
    RegQueryStringValue(HKCU, sUnInstPath, 'UninstallString', sUnInstallString);
  Result := sUnInstallString;
end;

/////////////////////////////////////////////////////////////////////
function IsUpgrade(): Boolean;
begin
  Result := (GetUninstallString() <> '');
end;

/////////////////////////////////////////////////////////////////////
function UnInstallOldVersion(): Integer;
var
  sUnInstallString: String;
  iResultCode: Integer;
begin
// Return Values:
// 1 - uninstall string is empty
// 2 - error executing the UnInstallString
// 3 - successfully executed the UnInstallString

  // default return value
  Result := 0;

  // get the uninstall string of the old app
  sUnInstallString := GetUninstallString();
  if sUnInstallString <> '' then begin
    sUnInstallString := RemoveQuotes(sUnInstallString);
    if Exec(sUnInstallString, '/SILENT /NORESTART /SUPPRESSMSGBOXES','', SW_HIDE, ewWaitUntilTerminated, iResultCode) then
      Result := 3
    else
      Result := 2;
  end else
    Result := 1;
end;

/////////////////////////////////////////////////////////////////////
procedure CurStepChanged(CurStep: TSetupStep);
begin
  if (CurStep=ssInstall) then
  begin
    if (IsUpgrade()) then
    begin
      UnInstallOldVersion();
    end;
  end;
end;