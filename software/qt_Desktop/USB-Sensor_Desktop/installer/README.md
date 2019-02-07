# QT installer

## Win32
1. Build the release version.
2. Open a DOS prompt and change to this directory.
3. Run the gen_win32_release.bat to generate the installer exe file.

If you are not using the suggested build directory, you need to set RELEASE_DIR to fit your situation.
```
SET RELEASE_DIR=D:\Temp\_QtBuild\build-FakeILock_Desktop-Desktop_Qt_5_11_3_MinGW_32bit-Release
```


Also you may need to change the QT location.
```
REM Set tools location
SET QT_DIR=C:\Qt\5.11.3\mingw53_32
SET IFW_LOC=C:\Qt\Tools\QtInstallerFramework\3.0\bin
```

