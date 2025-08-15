@echo off
setlocal enabledelayedexpansion

:: ---- CONFIG (usually you don't need to change these) ----
set "ROOT=%~dp0"
set "SRC_DIR=%ROOT%src"
set "INCLUDE_DIR=%ROOT%include"
set "BUILD_DIR=%ROOT%build"
set "OUTPUT_EXE=%BUILD_DIR%\main.exe"
set "CPP_STD=c++20"

:: ---- prepare build dir ----
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"

echo.
echo === Compiling .cpp files under: "%SRC_DIR%" ===

:: go into src so we can produce relative paths (avoids C:\... in object names)
pushd "%SRC_DIR%" || (echo ERROR: cannot change directory to %SRC_DIR% & exit /b 1)

for /R %%F in (*.cpp) do (
    rem full path is %%~fF, local CD == SRC_DIR, remove that prefix to get relative path
    set "full=%%~fF"
    set "rel=!full:%CD%\=!"
    rem remove filename to get directory part (may be empty)
    set "reldir=!rel:%%~nxF=!"
    rem create the matching directory inside build
    if not exist "%BUILD_DIR%\!reldir!" mkdir "%BUILD_DIR%\!reldir!"
    echo Compiling: "%%~fF"
    g++ -std=%CPP_STD% -I"%SRC_DIR%" -I"%INCLUDE_DIR%" -c "%%~fF" -o "%BUILD_DIR%\!reldir!%%~nF.o"
    if errorlevel 1 (
        echo.
        echo COMPILATION FAILED for "%%~fF"
        popd
        exit /b 1
    )
)

popd

echo.
echo === Writing objects list to: "%BUILD_DIR%\objects.txt" ===
set "OBJ_LIST=%BUILD_DIR%\objects.txt"
if exist "%OBJ_LIST%" del "%OBJ_LIST%"

for /R "%BUILD_DIR%" %%O in (*.o) do (
    rem each .o path is quoted on its own line (required for response file)
    echo "%%~fO">>"%OBJ_LIST%"
)

if not exist "%OBJ_LIST%" (
    echo No object files found in "%BUILD_DIR%". Nothing to link.
    exit /b 1
)

echo.
echo === Linking (console app) into: "%OUTPUT_EXE%" ===
pushd "%BUILD_DIR%"
g++ @objects.txt -o "%OUTPUT_EXE%" -mconsole
if errorlevel 1 (
    echo.
    echo LINK FAILED
    popd
    exit /b 1
)
popd

echo.
echo Build succeeded: "%OUTPUT_EXE%"
pause
endlocal