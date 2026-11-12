# Para Windows con MinGW (make o mingw32-make). Tambien puede usarse build.bat
CC     = gcc
CFLAGS = -Wall -O2 -std=gnu99 -I../1-con-openspec/src

costos_gui.exe: gui_win32.c ../1-con-openspec/src/costos.c ../1-con-openspec/src/costos.h
	$(CC) $(CFLAGS) gui_win32.c ../1-con-openspec/src/costos.c -o $@ -mwindows

clean:
	rm -f costos_gui.exe
