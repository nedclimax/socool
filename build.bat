@echo off
setlocal enabledelayedexpansion
setlocal enableextensions

if not exist bin mkdir bin
if not exist tools mkdir tools
:: fetch tools
if not exist tools\sokol-shdc.exe (
	curl -fSsL -o tools\sokol-shdc.exe https://raw.githubusercontent.com/floooh/sokol-tools-bin/master/bin/win32/sokol-shdc.exe
)

:: build shaders
if not exist shaders\bin mkdir shaders\bin
pushd shaders\bin
for /r %%f in (..\*.glsl) do (
	..\..\tools\sokol-shdc -i %%f -o %%f.h -l hlsl5 -b
)
xcopy /Y ..\*.h .
del ..\*.h
popd

set CFLAGS=/Zi /FC /TC /W4 /WX /GR- /wd4100

pushd bin
cl %CFLAGS% /Fesktest /I.. /I..\include ..\src\*.c
popd