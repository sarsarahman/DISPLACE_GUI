; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Displace"
#define MyAppVersion "0.9.4"
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
Source: build\{#Build}\displacegui.exe; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\dtreeeditor.exe; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\tsereditor.exe; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\objeditor.exe; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\displace.exe; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\scheduler.exe; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\qmapcontrol{#QT_DEBUG}1.dll; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\displacecommons.dll; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\formats.dll; DestDir: {app}; Flags: ignoreversion
Source: build\{#Build}\qtcommons.dll; DestDir: {app}; Flags: ignoreversion

Source: scripts\gen_ts.R; DestDir: {app}\scripts; Flags: ignoreversion

Source: install\extra\bin\libgcc_s_seh-1.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\libgdal-1.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\libgeos_c-1.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\libgeos-3-4-2.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\libproj-0.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\libstdc++-6.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\libwinpthread-1.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\libCGAL.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\libgmp-10.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\lib\libboost_system-mgw49-mt-1_57.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\lib\libboost_thread-mgw49-mt-1_57.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\lib\libboost_regex-mgw49-mt-1_57.dll; DestDir: {app}; Flags: ignoreversion

Source: install\extra\bin\Qt5Core{#QT_DEBUG}.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\Qt5Concurrent{#QT_DEBUG}.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\Qt5Gui{#QT_DEBUG}.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\Qt5Network{#QT_DEBUG}.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\Qt5OpenGL{#QT_DEBUG}.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\Qt5PrintSupport{#QT_DEBUG}.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\Qt5Sql{#QT_DEBUG}.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\Qt5Widgets{#QT_DEBUG}.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\Qt5Xml{#QT_DEBUG}.dll; DestDir: {app}; Flags: ignoreversion
Source: install\extra\bin\zlib1.dll; DestDir: {app}; Flags: ignoreversion

Source: install\extra\bin\platforms\qminimal{#QT_DEBUG}.dll; DestDir: {app}\platforms; Flags: ignoreversion
Source: install\extra\bin\platforms\qwindows{#QT_DEBUG}.dll; DestDir: {app}\platforms; Flags: ignoreversion
Source: install\extra\bin\sqldrivers\qsqlite{#QT_DEBUG}.dll; DestDir: {app}\sqldrivers; Flags: ignoreversion

[Icons]
Name: {group}\{#MyAppName}; Filename: {app}\displacegui.exe; Tasks: ; Languages: 
Name: {commondesktop}\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: desktopicon
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\{#MyAppName}; Filename: {app}\{#MyAppExeName}; Tasks: quicklaunchicon
Name: {group}\Decision Tree Editor; Filename: {app}\dtreeeditor.exe; Tasks: ; Languages: 
Name: {group}\Time Series Editor; Filename: {app}\tsereditor.exe

[Run]
;Filename: {tmp}\vcredist_x86.exe; Parameters: /q; WorkingDir: {tmp}
Filename: {app}\{#MyAppExeName}; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent
