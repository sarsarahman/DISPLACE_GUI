; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Displace"
#define MyAppVersion "0.6.2"
#define MyAppPublisher "Displace Project"
#define MyAppURL "http://www.displace-project.org"
#define MyAppExeName "displacegui.exe"

; to debug:
;#define Build "debug"
;#define QT_DEBUG "d"
#define Build "release"
#define QT_DEBUG ""
#define MINGW_DIR "C:\Qt\qt-5.3.2-x64-mingw491r1-seh-opengl\mingw64\bin"
#define QT_DIR "C:\Qt\qt-5.3.2-x64-mingw491r1-seh-opengl\qt-5.3.2-x64-mingw491r1-seh-opengl\bin"
#define QT_PLUGINS_DIR "C:\Qt\qt-5.3.2-x64-mingw491r1-seh-opengl\qt-5.3.2-x64-mingw491r1-seh-opengl\plugins"

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
DefaultDirName={pf}\{#MyAppName}
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
Source: build\{#Build}\displacegui.exe; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\displace.exe; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\qmapcontrol1.dll; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\displacecommons.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\usr\local\bin\libgdal-1.dll; DestDir: {app}; Flags: ignoreversion
;NOTE: Don't use "Flags: ignoreversion" on any shared system files
;Source: C:\lib\vcredist_x86.exe; DestDir: {tmp}
Source: {#MINGW_DIR}\libwinpthread-1.dll; DestDir: {app}
Source: {#QT_DIR}\icuin53.dll; DestDir: {app}
Source: {#QT_DIR}\icuuc53.dll; DestDir: {app}
Source: {#QT_DIR}\icudt53.dll; DestDir: {app}
Source: {#MINGW_DIR}\libgcc_s_seh-1.dll; DestDir: {app}
Source: {#QT_DIR}\libstdc++-6.dll; DestDir: {app}
;ource: C:\mingw\bin\libiconv-2.dll; DestDir: {app}
;Source: C:\MinGW\OpenSSL-Win32\libeay32.dll; DestDir: {app}
;Source: C:\MinGW\OpenSSL-Win32\libssl32.dll; DestDir: {app}
Source: {#QT_DIR}\Qt5Core{#QT_DEBUG}.dll; DestDir: {app}
Source: {#QT_DIR}\Qt5Gui{#QT_DEBUG}.dll; DestDir: {app}
Source: {#QT_DIR}\Qt5Widgets{#QT_DEBUG}.dll; DestDir: {app}
Source: {#QT_DIR}\Qt5Network{#QT_DEBUG}.dll; DestDir: {app}
Source: {#QT_DIR}\Qt5Sql{#QT_DEBUG}.dll; DestDir: {app}
Source: {#QT_DIR}\Qt5Svg{#QT_DEBUG}.dll; DestDir: {app}
Source: {#QT_DIR}\Qt5Xml{#QT_DEBUG}.dll; DestDir: {app}
Source: {#QT_DIR}\Qt5PrintSupport{#QT_DEBUG}.dll; DestDir: {app}
Source: {#QT_PLUGINS_DIR}\platforms\qwindows{#QT_DEBUG}.dll; DestDir: {app}\platforms
Source: {#QT_PLUGINS_DIR}\sqldrivers\qsqlite{#QT_DEBUG}.dll; DestDir: {app}\sqldrivers
;Source: {#MINGW_DIR}\..\x86_64-w64-mingw32\lib\zlib1.dll; DestDir: {app}
Source: install\extra\usr\local\bin\zlib1.dll; DestDir: {app}

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\{#MyAppExeName}
Name: {commondesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: quicklaunchicon

[Run]
;Filename: {tmp}\vcredist_x86.exe; Parameters: /q; WorkingDir: {tmp}
Filename: {app}\{#MyAppExeName}; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent
