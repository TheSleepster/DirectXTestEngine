@echo off

REM 64-bit MSVC building
REM
REM THE SYNTAX FOR DISABLING A WARNING IS  on MSVC -wd<error-number> EX: -wd4201
REM -O2 for release builds on MSVC

Set MSVCCommonCompilerFlags=-MD -GR- -EHa- -Zi -W4 -wd4189 -wd4100
set MSVCCommonLinkerFlags= d3d11.lib kernel32.lib user32.lib shell32.lib gdi32.lib winmm.lib msvcrt.lib

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl %MSVCCommonCompilerFlags% \DirectX\code\*.cpp /I%MSVCCommonIncludePaths% /link %MSVCCommonLinkerFlags%
popd


REM CLANG 64-bit


REM Set ClangCommonCompilerFlags=-MJ 
REM Set ClangCommonLinkerFlags=-lkernel32 -luser32 -lshell32 -lgdi32 -lwinmm 
REM
REM IF NOT EXIST ..\build mkdir ..\build
REM pushd ..\build
REM clang++ \DirectX\code\*.cpp -MJ%ClangCommonCompierFlags% %ClangCommonLinkerFlags%
REM popd

@echo ====================
@echo Compilation Finished
@echo ====================
