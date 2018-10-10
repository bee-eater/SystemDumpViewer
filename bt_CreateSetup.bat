@echo off
echo.SystemdumpViewer Setup Builder v1.0
echo.---------------------------------------------
echo.
echo.Building setup, please be patient...
"..\zz_inno\ISCC.exe" .\50_inno\BR.SystemdumpViewer.iss >> NUL
echo.
echo.Done!
echo.
pause