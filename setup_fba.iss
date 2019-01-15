; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Displace"
#define MyAppVersion "0.9.17"
#define MyAppPublisher "Displace Project"
#define MyAppURL "http://www.displace-project.org"
#define MyAppExeName "displacegui.exe"

; to debug:
;#define Build "debug"
;#define QT_DEBUG "d"

; Download install/vc_redist.x64.exe from
; https://www.microsoft.com/it-it/download/details.aspx?id=48145

#define Build "release"
#define QT_DEBUG ""
;#define QT_DIR "C:\Qt\5.11.1\msvc2017_64"
#define QT_DIR "C:\Qt\5.10.0\msvc2017_64"
;#define QT_PLUGINS_DIR "C:\Qt\5.11.1\msvc2017_64\plugins"
#define QT_PLUGINS_DIR "C:\Qt\5.10.0\msvc2017_64\plugins"
#define SDK_DIR "install\msvc"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{4E085CFA-BEB3-4D99-88BF-DA0EBCBB3FC9}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=c:\{#MyAppName}
DefaultGroupName={#MyAppName}
OutputDir=install
OutputBaseFilename={#MyAppName}-{#MyAppVersion}
Compression=lzma
SolidCompression=true

[Languages]
Name: english; MessagesFile: compiler:Default.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
;Source: "cmake-build-{#Build}\bin\displacegui.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "cmake-build-{#Build}\bin\dteditor.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "cmake-build-{#Build}\bin\tseditor.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "cmake-build-{#Build}\bin\objeditor.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "cmake-build-{#Build}\bin\displace.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "cmake-build-{#Build}\bin\scheduler.exe"; DestDir: "{app}"; Flags: ignoreversion
;Source: "cmake-build-{#Build}\bin\qmapcontrol{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "cmake-build-{#Build}\bin\commons.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "cmake-build-{#Build}\bin\formats.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "cmake-build-{#Build}\bin\qtcommons.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-displace-Desktop_Qt_5_10_0_MSVC2017_64bit-Release\displacegui.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-displace-Desktop_Qt_5_10_0_MSVC2017_64bit-Release\dtreeeditor.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-displace-Desktop_Qt_5_10_0_MSVC2017_64bit-Release\tsereditor.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-displace-Desktop_Qt_5_10_0_MSVC2017_64bit-Release\objeditor.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-displace-Desktop_Qt_5_10_0_MSVC2017_64bit-Release\displace.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-displace-Desktop_Qt_5_10_0_MSVC2017_64bit-Release\scheduler.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-displace-Desktop_Qt_5_10_0_MSVC2017_64bit-Release\qmapcontrol{#QT_DEBUG}1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-displace-Desktop_Qt_5_10_0_MSVC2017_64bit-Release\displacecommons.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-displace-Desktop_Qt_5_10_0_MSVC2017_64bit-Release\formats.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-displace-Desktop_Qt_5_10_0_MSVC2017_64bit-Release\qtcommons.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "scripts\gen_ts.R"; DestDir: "{app}\scripts"; Flags: ignoreversion

Source: "{#SDK_DIR}\lib\gdal111.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\lib\geos.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\lib\geos_c.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\lib\CGAL-vc140-mt-4.9.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\lib\CGAL_core-vc140-mt-4.9.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\lib\gmp.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\lib\mpir.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\lib\boost_system-vc140-mt-1_63.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "{#SDK_DIR}\lib\boost_thread-vc140-mt-1_63.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\lib\boost_regex-vc140-mt-1_63.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\lib\boost_program_options-vc140-mt-1_63.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "{#QT_DIR}\bin\Qt5Core{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_DIR}\bin\Qt5Concurrent{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_DIR}\bin\Qt5Gui{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_DIR}\bin\Qt5Network{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_DIR}\bin\Qt5OpenGL{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_DIR}\bin\Qt5PrintSupport{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_DIR}\bin\Qt5Sql{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_DIR}\bin\Qt5Widgets{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#QT_DIR}\bin\Qt5Xml{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: {#QT_DIR}\bin\zlib1.dll; DestDir: {app}; Flags: ignoreversion

Source: "{#QT_PLUGINS_DIR}\platforms\qminimal{#QT_DEBUG}.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#QT_PLUGINS_DIR}\platforms\qwindows{#QT_DEBUG}.dll"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#QT_PLUGINS_DIR}\sqldrivers\qsqlite{#QT_DEBUG}.dll"; DestDir: "{app}\sqldrivers"; Flags: ignoreversion
;Source: "install\vcredist_x64.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall
Source: "install\vc_redist.x64.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\displacegui.exe; Tasks: ; Languages: 
Name: {commondesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: quicklaunchicon
Name: {group}\Decision Tree Editor; Filename: {app}\dteditor.exe; Tasks: ; Languages: 
Name: {group}\Time Series Editor; Filename: {app}\tseditor.exe

[Run]
;Filename: {tmp}\vcredist_x64.exe; Parameters: /quiet; WorkingDir: {tmp}
Filename: {tmp}\vc_redist.x64.exe; Parameters: /quiet; WorkingDir: {tmp}
Filename: {app}\{#MyAppExeName}; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent
