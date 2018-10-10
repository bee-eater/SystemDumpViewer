; -- BrdhgDumpViewer.iss --
#define MainBinaryName  "BR.SystemdumpViewer.exe"
#define BaseName        "Systemdump Viewer"
#define SetupBaseName   "Setup_SystemdumpViewer_"
#define RelPath         "..\..\build\release\"
#define PureFiles       "..\01_prgfiles"
#define DLLS            "..\..\zz_dlls\5.6.1"
#define AppVersion      GetFileVersion(RelPath + MainBinaryName)
#define AVF1            Copy(AppVersion, 1, Pos(".", AppVersion) - 1) + "_" + Copy(AppVersion, Pos(".", AppVersion) + 1)
#define AVF2            Copy(AVF1,       1, Pos(".", AVF1      ) - 1) + "_" + Copy(AVF1      , Pos(".", AVF1      ) + 1)
#define AppVersionFile  Copy(AVF2,       1, Pos(".", AVF2      ) - 1) + "_" + Copy(AVF2      , Pos(".", AVF2      ) + 1)

[Setup]
AppName=Systemdump Viewer
AppId=29567720-C543-11E3-9C1A-0800200C9A66
AppVerName={#BaseName}
AppVersion={#AppVersion}
VersionInfoVersion={#AppVersion}
DefaultDirName={pf}\{#BaseName}
DefaultGroupName=B&R Automation\{#BaseName}
DisableDirPage=auto
DisableProgramGroupPage=auto
UninstallDisplayIcon={app}\{#MainBinaryName}
Compression=lzma2
SolidCompression=yes
OutputDir="../../zz_setup"
OutputBaseFilename={#SetupBaseName + AppVersionFile}

[Types]
Name: "full"; Description: "Full installation"
Name: "compact"; Description: "Compact installation"
Name: "custom"; Description: "Custom installation"; Flags: IsCustom;

[Components]
Name: Core; Description: "Core components"; Types: full compact custom;
Name: Lang; Description: "Additional languages"; Types: full;
Name: Them; Description: "Additional color themes"; Types: full;

[Files]
Source: "{#RelPath}BR.SystemdumpViewer.exe"; DestDir: "{app}"; Components: Core;
; ----- Language files -----
Source: "{#PureFiles}\lang\lang_en.qm"; DestDir: "{app}\lang"; Flags: ignoreversion; Components: Core;
Source: "{#PureFiles}\lang\lang_de.qm"; DestDir: "{app}\lang"; Flags: ignoreversion; Components: Lang; 
Source: "{#PureFiles}\lang\lang_fr.qm"; DestDir: "{app}\lang"; Flags: ignoreversion; Components: Lang;  
Source: "{#PureFiles}\lang\lang_ru.qm"; DestDir: "{app}\lang"; Flags: ignoreversion; Components: Lang; 
Source: "{#PureFiles}\lang\lang_pl.qm"; DestDir: "{app}\lang"; Flags: ignoreversion; Components: Lang; 
; ----- Themes -----
Source: "{#PureFiles}\themes\FusionLightOrange.sdtm"; DestDir: "{app}\themes"; Components: Core;
Source: "{#PureFiles}\themes\*.*"; DestDir: "{app}\themes"; Components: Them; Flags: ignoreversion recursesubdirs;
; ----- CSS -----
Source: "{#PureFiles}\css\*.*"; DestDir: "{app}\css"; Flags: ignoreversion; Components: Core;
; ----- Help -----
Source: "{#PureFiles}\help\*.*"; DestDir: "{app}\help"; Flags: ignoreversion; Components: Core;
; ----- 7z and config -----
Source: "{#PureFiles}\7za.exe"; DestDir: "{app}"; Flags: ignoreversion; Components: Core;
Source: "{#PureFiles}\system.cfg"; DestDir: "{app}"; Flags: ignoreversion onlyifdoesntexist; Components: Core;
Source: "{#PureFiles}\whatsnew.txt"; DestDir: "{app}"; Flags: ignoreversion; Components: Core;
; ----- DLL Files -----
Source: "{#DLLS}\*.*"; DestDir: "{app}"; Components: Core; Flags: ignoreversion recursesubdirs; 

[Icons]
Name: "{commondesktop}\{#BaseName}"; Filename: "{app}\{#MainBinaryName}"; WorkingDir: "{app}" 
Name: "{group}\{#BaseName}"; Filename: "{app}\{#MainBinaryName}"
Name: "{group}\Uninstall"; Filename: "{app}\unins000.exe"

[Run]
Filename: "{app}\{#MainBinaryName}"; Description: "{cm:LaunchProgram,{#MainBinaryName}}"; Flags: nowait postinstall

[code]

// Uninstall before Install Code 
// ---------------------------------------------------------------------------------------------------------------------------------------------
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
// ---------------------------------------------------------------------------------------------------------------------------------------------