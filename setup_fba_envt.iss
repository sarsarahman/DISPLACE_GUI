; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "Displace"
#define MyAppVersion "1.0"
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
#define QT_DIR "C:\Qt\5.12.0\msvc2017_64"
#define QT_PLUGINS_DIR "C:\Qt\5.12.0\msvc2017_64\plugins"
#define SDK_DIR "install\displacesdk_for_QtCreator\installed\x64-windows"

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
Name: english; MessagesFile: compiler:Default.isl; LicenseFile: LICENSE.txt

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
Source: "..\build-DISPLACE_GUI-Desktop_Qt_5_12_0_MSVC2017_64bit-Release\bin\displacegui.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-DISPLACE_GUI-Desktop_Qt_5_12_0_MSVC2017_64bit-Release\bin\dteditor.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-DISPLACE_GUI-Desktop_Qt_5_12_0_MSVC2017_64bit-Release\bin\tseditor.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-DISPLACE_GUI-Desktop_Qt_5_12_0_MSVC2017_64bit-Release\bin\objeditor.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-DISPLACE_GUI-Desktop_Qt_5_12_0_MSVC2017_64bit-Release\bin\displace.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-DISPLACE_GUI-Desktop_Qt_5_12_0_MSVC2017_64bit-Release\bin\scheduler.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-DISPLACE_GUI-Desktop_Qt_5_12_0_MSVC2017_64bit-Release\bin\QMapControl{#QT_DEBUG}.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-DISPLACE_GUI-Desktop_Qt_5_12_0_MSVC2017_64bit-Release\bin\commons.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-DISPLACE_GUI-Desktop_Qt_5_12_0_MSVC2017_64bit-Release\bin\formats.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\build-DISPLACE_GUI-Desktop_Qt_5_12_0_MSVC2017_64bit-Release\bin\qtcommons.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "scripts\gen_ts.R"; DestDir: "{app}\scripts"; Flags: ignoreversion

Source: "{#SDK_DIR}\bin\gdal203.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\geos.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\geos_c.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\Geographic.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\CGAL-vc140-mt-4.13-I-900.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\CGAL_core-vc140-mt-4.13-I-900.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\mpfr.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\mpir.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\boost_system-vc141-mt-x64-1_68.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "{#SDK_DIR}\bin\boost_thread-vc141-mt-x64-1_68.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\boost_regex-vc141-mt-x64-1_68.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\boost_program_options-vc141-mt-x64-1_68.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\boost_date_time-vc141-mt-x64-1_68.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\msqlitecpp.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\sqlite3.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\expat.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\libpq.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\libmysql.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\libcurl.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\libpng16.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\openjp2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\webp.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\libxml2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\ssleay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\libeay32.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\libiconv.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\lzma.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#SDK_DIR}\bin\libcharset.dll"; DestDir: "{app}"; Flags: ignoreversion

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
