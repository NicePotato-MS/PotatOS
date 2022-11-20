@echo off


:choice
set /P c=Would you like to wipe emulator data? [Y/N]
if /I "%c%" EQU "Y" goto :yes
if /I "%c%" EQU "N" goto :no
echo Invalid option, please use Y/N
goto :choice

:yes
if exist qemu rmdir /s /q qemu
mkdir qemu
qemu-img create qemu/HDD.img 5G

:no
if exist binaries rmdir /s /q binaries
if exist images rmdir /s /q images
mkdir binaries
mkdir binaries\installer
mkdir images

echo START OF NASM OUTPUT
echo --------------------

echo --------------------
echo END OF NASM OUTPUT

:choice2
set /P c=Would you like to continue? [Y/N]
if /I "%c%" EQU "Y" goto :yes2
if /I "%c%" EQU "N" goto :no2
echo Invalid option, please use Y/N
goto :choice2

:yes2

:no2