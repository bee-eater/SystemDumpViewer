@echo off
echo.Welcome, %USERNAME%
echo.What would you like to do now %USERNAME%?
echo.
echo. 1 - Edit german and english
echo. 2 - Edit german, english, french, russian and polish
echo. 3 - Edit all languages
echo.
echo. 4 - Release files
echo. 
echo. 5 - Update and remove obsolete
echo. 6 - Update
echo.
echo. 7 - Copy languages to release directory
echo. 8 - Copy languages to debug directory
echo.
echo. 0 - Exit
echo.
choice /n /c "012345678" /m "Please choose: "
echo.

:again

set dling=C:\Dev\Qt\v5.12.6\5.12.6\mingw73_32\bin\

if "%errorlevel%"=="1" (
    echo.Ending program...
	goto :end
)

if "%errorlevel%"=="2" (
	%dling%\linguist 00_src/lang/lang_de.ts 00_src/lang/lang_en.ts
) 

if "%errorlevel%"=="3" (
	%dling%\linguist 00_src/lang/lang_de.ts 00_src/lang/lang_en.ts 00_src/lang/lang_fr.ts 00_src/lang/lang_ru.ts 00_src/lang/lang_pl.ts
)

if "%errorlevel%"=="4" (
	%dling%\linguist 00_src/lang/lang_de.ts 00_src/lang/lang_en.ts 00_src/lang/lang_fr.ts 00_src/lang/lang_ru.ts 00_src/lang/lang_pl.ts
)

if "%errorlevel%"=="5" (
	%dling%\lrelease 00_src/BR.SystemdumpViewer.pro
)

if "%errorlevel%"=="6" (
	%dling%\lupdate -noobsolete 00_src/BR.SystemdumpViewer.pro
)

if "%errorlevel%"=="7" (
	%dling%\lupdate 00_src/BR.SystemdumpViewer.pro
)

if "%errorlevel%"=="8" (
	xcopy /e 00_src\lang\*.qm ..\build\release\lang\ /Y
    xcopy /e 00_src\lang\*.qm 01_prgfiles\lang\ /Y
)

if "%errorlevel%"=="9" (
	xcopy /e 00_src\lang\*.qm ..\build\debug\lang\ /Y
)

echo.
echo.Done!
echo.
pause 
cls
echo.What would you like to do now %USERNAME%?
echo.
echo. 1 - Edit german and english
echo. 2 - Edit german, english, french, russian and polish
echo. 3 - Edit all languages
echo.
echo. 4 - Release files
echo. 
echo. 5 - Update and remove obsolete
echo. 6 - Update
echo.
echo. 7 - Copy languages to release directory
echo. 8 - Copy languages to debug directory
echo.
echo. 0 - Exit
echo.
choice /n /c "012345678" /m "Please choose: "
echo.
goto :again

:end