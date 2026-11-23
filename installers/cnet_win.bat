@echo off
setlocal EnableExtensions EnableDelayedExpansion
chcp 65001 >nul

powershell -NoProfile -Command "exit([int](-not ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)))"
if errorlevel 1 (
    echo Requesting administrator privileges...
    powershell -NoProfile -ExecutionPolicy Bypass -Command "Start-Process -FilePath '%~f0' -Verb RunAs"
    exit /b
)

for %%I in ("%~dp0..") do set "BASE_DIR=%%~fI"

set "HEADERS_DIR=%BASE_DIR%\headers"
set "SRC_FILE=%BASE_DIR%\src\cnet.c"

set "INSTALL_ROOT=%ProgramFiles%\CNET"
set "INSTALL_LIB=%INSTALL_ROOT%\lib"
set "INSTALL_INC=%INSTALL_ROOT%\include"

echo CNET installer: repository = %BASE_DIR%

if not exist "%HEADERS_DIR%\cnet.h" goto :bad_layout
if not exist "%SRC_FILE%" goto :bad_layout

where gcc >nul 2>&1
if errorlevel 1 (
    echo GCC not found. Install MinGW-w64 and add it to PATH.
    goto :fail
)

set "GCC_TARGET="
for /f "delims=" %%A in ('gcc -dumpmachine 2^>nul') do set "GCC_TARGET=%%A"

set "NPCAP_RUNTIME=%SystemRoot%\System32\Npcap"
if not exist "%NPCAP_RUNTIME%\wpcap.dll" (
    echo Npcap runtime not found. Install Npcap with "WinPcap API-compatible Mode".
    goto :fail
)
echo Npcap runtime: found

if defined NPCAP_SDK if exist "%NPCAP_SDK%\Include\pcap\pcap.h" goto :sdk_ready
if defined NPCAP_SDK if exist "%NPCAP_SDK%\Include\pcap.h" goto :sdk_ready

set "PF86=%ProgramFiles(x86)%"
set "NPCAP_SDK="
for %%P in (
    "%ProgramFiles%\Npcap SDK"
    "%PF86%\Npcap SDK"
    "C:\npcap-sdk"
    "C:\Npcap SDK"
    "%USERPROFILE%\Desktop\npcap-sdk"
    "%USERPROFILE%\Downloads\npcap-sdk"
    "%BASE_DIR%\npcap-sdk"
    "%BASE_DIR%\..\npcap-sdk"
) do (
    if not defined NPCAP_SDK if exist "%%~P\Include\pcap\pcap.h" set "NPCAP_SDK=%%~P"
    if not defined NPCAP_SDK if exist "%%~P\Include\pcap.h" set "NPCAP_SDK=%%~P"
)

if not defined NPCAP_SDK (
    echo Npcap SDK not found in the usual places; scanning %SystemDrive% ...
    for /f "delims=" %%F in ('dir /s /b "%SystemDrive%\pcap.h" 2^>nul ^| findstr /i /c:"\Include\pcap\pcap.h" /c:"\Include\pcap.h"') do (
        if not defined NPCAP_SDK (
            set "P=%%~dpF"
            if /i "!P:~-5!"=="pcap\" set "P=!P:~0,-5!"
            pushd "!P!.."
            set "NPCAP_SDK=!CD!"
            popd
        )
    )
)

if not defined NPCAP_SDK (
    echo Npcap SDK not found. Download and extract it, or set the NPCAP_SDK variable.
    goto :fail
)

:sdk_ready
echo Npcap SDK: %NPCAP_SDK%
if not exist "%NPCAP_SDK%\Include\pcap\pcap.h" (
    echo Warning: the SDK has no Include\pcap\pcap.h; cnet.h includes the pcap/pcap.h header.
)

set "NPCAP_LIB="
if defined GCC_TARGET (
    echo !GCC_TARGET! | findstr /i "x86_64 amd64" >nul
    if not errorlevel 1 if exist "%NPCAP_SDK%\Lib\x64\wpcap.lib" set "NPCAP_LIB=%NPCAP_SDK%\Lib\x64"
)
if not defined NPCAP_LIB if exist "%NPCAP_SDK%\Lib\wpcap.lib" set "NPCAP_LIB=%NPCAP_SDK%\Lib"

if not defined NPCAP_LIB (
    echo Npcap import libraries not found under "%NPCAP_SDK%\Lib".
    goto :fail
)
echo Npcap libraries: %NPCAP_LIB%

if not exist "%INSTALL_LIB%" mkdir "%INSTALL_LIB%"
if not exist "%INSTALL_INC%" mkdir "%INSTALL_INC%"

xcopy /E /I /Y "%HEADERS_DIR%" "%INSTALL_INC%" >nul
if errorlevel 1 (
    echo Failed to copy the headers.
    goto :fail
)
echo Headers installed: %INSTALL_INC%

gcc -shared -static-libgcc -O2 -DNDEBUG -I"%HEADERS_DIR%" -I"%NPCAP_SDK%\Include" "%SRC_FILE%" "%NPCAP_LIB%\wpcap.lib" "%NPCAP_LIB%\Packet.lib" -liphlpapi -lws2_32 -o "%INSTALL_LIB%\cnet.dll"
if errorlevel 1 (
    echo Compilation failed.
    goto :fail
)
echo Library installed: %INSTALL_LIB%\cnet.dll

echo Updating the system PATH ...
powershell -NoProfile -ExecutionPolicy Bypass -Command "$dirs=@('%INSTALL_LIB%','%NPCAP_RUNTIME%'); $k=[Microsoft.Win32.Registry]::LocalMachine.OpenSubKey('SYSTEM\CurrentControlSet\Control\Session Manager\Environment',$true); $p=[string]$k.GetValue('Path','',[Microsoft.Win32.RegistryValueOptions]::DoNotExpandEnvironmentNames); $kind=$k.GetValueKind('Path'); $parts=@($p -split ';' | Where-Object {$_ -ne ''}); $add=@(); foreach($d in $dirs){ if((Test-Path -LiteralPath $d) -and ($parts -notcontains $d)){ $add+=$d } }; if($add.Count -eq 0){ exit 2 }; $k.SetValue('Path', (($parts+$add) -join ';'), $kind); exit 0"

if errorlevel 3 goto :path_fail
if errorlevel 2 (
    echo System PATH: already up to date
    goto :done
)
if errorlevel 1 goto :path_fail
echo System PATH: updated

:done
echo.
echo CNET installation completed successfully.
echo   include: %INSTALL_INC%
echo   library: %INSTALL_LIB%\cnet.dll
echo Open a new terminal so the updated PATH takes effect.
pause
exit /b 0

:bad_layout
echo Repository layout not found.
echo Expected: "%HEADERS_DIR%\cnet.h" and "%SRC_FILE%"
goto :fail

:path_fail
echo Failed to update the system PATH. Add "%INSTALL_LIB%" manually.

:fail
pause
exit /b 1
