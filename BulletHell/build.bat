@echo off

cd /d %~dp0

gcc src\*.c -Iinclude -IC:\raylib\raylib\src -LC:\raylib\raylib\src -lraylib -lopengl32 -lgdi32 -lwinmm -lm -o BulletHell.exe

pause
