@Echo off

REM Check run bat at the installer directory
if EXIST gen_win32_release.bat (
    REM Passed
) else (
    echo Please run it at the installer directory.
    GOTO END
)

REM Store current directory
SETLOCAL
SET CURR_DIR=%CD%

REM Set project variables
SET APP_NAME=USB-Sensor_Desktop
SET APP_EXE_NAME=%APP_NAME%.exe
SET RELEASE_DIR=D:\Temp\_QtBuild\build-USB-Sensor_Desktop-Desktop_Qt_5_11_3_MinGW_32bit-Release
SET INSTALLER_DIR=%RELEASE_DIR%\installer_win32
SET DATA_DIR="%INSTALLER_DIR%\packages\com.dogratian.app\data"
SET BIN_DIR="%DATA_DIR%\bin"
SET PROTABLE_DIR=%INSTALLER_DIR%\%APP_NAME%_Protable

REM Set tools location
SET QT_DIR=C:\Qt\5.11.3\mingw53_32
SET IFW_LOC=C:\Qt\Tools\QtInstallerFramework\3.0\bin

REM Get top app directory
cd ..
SET APP_DIR=%CD%
cd %CURR_DIR%

REM add QT to PATH
SET PATH=%QT_DIR%\bin;%PATH%

REM Check release dir
if NOT EXIST "%RELEASE_DIR%" (
    echo "%RELEASE_DIR%" not found.
    GOTO END
)
if NOT EXIST "%RELEASE_DIR%/release/%APP_EXE_NAME%" (
    echo "%RELEASE_DIR%/release/%APP_EXE_NAME%" not found.
    goto END
)

REM Prepare installer dir at RELEASE_DIR
if EXIST "%INSTALLER_DIR%" (
    rmdir /s /q "%INSTALLER_DIR%
)
mkdir "%INSTALLER_DIR%"

REM Prepare installer directory
xcopy config "%INSTALLER_DIR%\config" /e /h /i /q
xcopy packages "%INSTALLER_DIR%\packages" /e /h /i /q
if EXIST "%INSTALLER_DIR%" (
    rmdir /s /q "%BIN_DIR%
)
mkdir "%BIN_DIR%"

REM Copy exe to bin directory
copy "%RELEASE_DIR%\release\*.exe" "%BIN_DIR%"
copy "%APP_DIR%\asset\app_icon.ico" "%DATA_DIR%"

REM Use deploy tools to prepare all library
cd /d "%BIN_DIR%"
"%QT_DIR%\bin\windeployqt.exe" %APP_EXE_NAME% --release --qmldir "%APP_DIR%/qml"

REM Add extra lib
copy "%QT_DIR%\bin\libwinpthread-1.dll" .
copy "%QT_DIR%\bin\libstdc++-6.dll" .
copy "%QT_DIR%\bin\libgcc_s_dw2-1.dll" .

if "%~1"=="-s" goto END
REM Gen installer package
cd /d "%INSTALLER_DIR%"
echo Generating installer package...
"%IFW_LOC%\binarycreator.exe" --offline-only -t "%IFW_LOC%\installerbase.exe" -p packages -c config\config.xml "%APP_NAME%_installer_win32"
echo Please find the installer exe at "%INSTALLER_DIR%".

REM Copy protable files
xcopy %BIN_DIR% %PROTABLE_DIR%\ /y /e /q
echo Please find the protable version at "%PROTABLE_DIR%".

:END
cd /d "%CURR_DIR%"
