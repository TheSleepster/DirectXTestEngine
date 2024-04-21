@echo off

REM THE SYNTAX FOR DISABLING A WARNING IS -wd<error-number> EX: -wd4201
REM -O2 for release builds

Set CommonCompilerFlags=-MD -GR- -EHa- -Zi -W4 -wd4189 -wd4100
set CommonLinkerFlags= d3d11.lib kernel32.lib user32.lib shell32.lib gdi32.lib winmm.lib msvcrt.lib

REM cl %CommonCompilerFlags% \DirectX\code\*.cpp /I%CommonIncludePaths% /MD /link %CommonLinkerFlags% 

REM 64-bit building

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl %CommonCompilerFlags% \DirectX\code\*.cpp /I%CommonIncludePaths% /link %CommonLinkerFlags%
popd

@echo ====================
@echo Compilation Finished
@echo ====================
