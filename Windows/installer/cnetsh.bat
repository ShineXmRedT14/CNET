@echo off
setlocal enabledelayedexpansion
chcp 65001 >nul

echo Checking for administator rules...
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo Please, run with administator rules.
    powershell -Command "Start-Process -FilePath '%comspec%' -ArgumentList '/c \"custom_title_placeholder & cd /d \"%~dp0\" & \"%~f0\"\"' -Verb RunAs"
    exit /b
)

set "PROJECT_ROOT=%~dp0.."
cd /d "%PROJECT_ROOT%"

if "%DLL_NAME%"=="" set "DLL_NAME=cnet.dll"
if "%SOURCE_FILES%"=="" set "SOURCE_FILES=src\cnet.c"
if "%INCLUDE_DIR%"=="" set "INCLUDE_DIR=headers"
if "%OUTPUT_DIR%"=="" set "OUTPUT_DIR=bin"

echo Checking for Npcap driver...
sc query npcap >nul 2>nul

if %errorlevel% equ 0 (
    echo Npcap driver is already installed.
    sc query npcap
) else (
    echo Npcap driver not found. Installing...
    set "NPCAP_URL=https://npcap.com/dist/npcap-sdk-1.17.zip"
    set "NPCAP_TEMP=%TEMP%\npcap_installer.exe"

    echo Downloads Npcap...
    powershell -Command "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri '!NPCAP_URL!' -OutFile '!NPCAP_TEMP!'"

    if exist "!NPCAP_TEMP!" (
        start /wait "" "!NPCAP_TEMP!" /S
        del /f /q "!NPCAP_TEMP!"

        sc query npcap >nul 2>nul
        if !errorlevel! equ 0 (
            echo Npcap successfully installed.
        ) else (
            echo Installing is ended, but driver not runs.
        )
    ) else (
        echo Can not to download Npcap.
    )
)

echo Checking for GCC (MinGW-w64)...
where gcc >nul 2>nul
if %errorlevel% equ 0 (
    echo GCC found.
    goto :compile
)

echo GCC not found. Installing...

set "MINGW_URL=https://github.com/brechtsanders/winlibs_mingw/releases/download/16.2.0posix-23.1.3-14.0.0-ucrt-r3/winlibs-i686-posix-dwarf-gcc-16.2.0-llvm-23.1.3-mingw-w64ucrt-14.0.0-r3.zip"
set "ZIP_FILE=%TEMP%\mingw64.zip"
set "INSTALL_DIR=C:\mingw64"

if not exist "%INSTALL_DIR%" mkdir "%INSTALL_DIR%"

echo Downloads GCC...
powershell -Command "[Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; Invoke-WebRequest -Uri '%MINGW_URL%' -OutFile '%ZIP_FILE%'"

powershell -Command "Expand-Archive -Path '%ZIP_FILE%' -DestinationPath '%INSTALL_DIR%' -Force" 2>nul
if %errorlevel% neq 0 (
    powershell -Command "Add-Type -A 'System.IO.Compression.FileSystem'; [IO.Compression.ZipFile]::ExtractToDirectory('%ZIP_FILE%', '%INSTALL_DIR%')"
)

if exist "%ZIP_FILE%" del /f /q "%ZIP_FILE%"

if exist "%INSTALL_DIR%\mingw64\bin" (
    set "PATH=%INSTALL_DIR%\mingw64\bin;%PATH%"
) else (
    set "PATH=%INSTALL_DIR%\bin;%PATH%"
)

where gcc >nul 2>nul
if %errorlevel% neq 0 (
    echo Can not to install. Install it's by yourself.
    pause
    exit /b
)
echo MinGW-w64 successfully installed.

:compile
echo.
echo Compile and move CNET library...
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

if not exist "%SOURCE_FILES%" (
    echo File not found: %SOURCE_FILES%
    goto :end
)

gcc -shared -o "%OUTPUT_DIR%\%DLL_NAME%" %SOURCE_FILES% -I"%INCLUDE_DIR%" -Wl,--out-implib,"%OUTPUT_DIR%\%DLL_NAME%.a"

if %errorlevel% equ 0 (
    echo Successfully compiled...

    echo copies .h files int "%OUTPUT_DIR%" path...
    set "copied_count=0"
    if exist "%INCLUDE_DIR%" (
        for %%F in ("%INCLUDE_DIR%\*.h") do (
            copy /Y "%%F" "%OUTPUT_DIR%\" >nul
            set /a copied_count+=1
        )
    )

    for %%I in ("%OUTPUT_DIR%") do set "ABS_OUTPUT_DIR=%%~fI"

    echo.
    echo Global register in Windows...

    setx C_INCLUDE_PATH "!ABS_OUTPUT_DIR!" /M >nul
    setx LIBRARY_PATH "!ABS_OUTPUT_DIR!" /M >nul

    powershell -Command "$oldPath = [Environment]::GetEnvironmentVariable('Path', 'Machine'); if (-not $oldPath.Contains('!ABS_OUTPUT_DIR!')) { [Environment]::SetEnvironmentVariable('Path', $oldPath + ';!ABS_OUTPUT_DIR!', 'Machine') }"

    echo Successfully installed CNET library.
) else (
    echo Can not to successfully install CNET library.
)

:end
pause
endlocal