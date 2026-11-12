@echo off
REM Compila la interfaz grafica con MinGW (gcc). Ejecutar desde esta carpeta.
gcc -Wall -O2 -std=gnu99 -I..\1-con-openspec\src gui_win32.c ..\1-con-openspec\src\costos.c -o costos_gui.exe -mwindows
if errorlevel 1 (echo Error al compilar) else (echo Listo: costos_gui.exe)
