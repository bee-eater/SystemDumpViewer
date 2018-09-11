@echo off
echo.Welcome, %USERNAME%
echo.What would you like to do?
echo.
echo. 1 - Copy release files
echo.
echo. 0 - Exit
echo.
choice /n /c "01" /m "Please choose: "
echo.

if "%errorlevel%"=="1" (
    echo.Ending program...
)

if "%errorlevel%"=="2" (
	xcopy /s 01_prgfiles ..\build\release /Y
) 

echo.
echo.Done!
echo.
pause