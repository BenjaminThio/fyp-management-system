@echo off
mkdir build 2>nul

echo Compiling all .cpp files in src\ to build\ .o files...

for %%F in (src\*.cpp) do (
    echo Compiling %%F ...
    g++ -std=c++17 -Iinclude -c %%F -o build\%%~nF.o
)

echo Done.