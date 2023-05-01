
call vendor\cmake\bin\cmake.exe -S . -B build -G "Visual Studio 16 2019"
@echo off
if not "%1" == "0" (
	PAUSE
)
