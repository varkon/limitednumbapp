set NDDRIV=P:
set ORIGDIR= %CD%

subst %NDDRIV% /d
subst %NDDRIV% %ORIGDIR%
"%ProgramFiles(x86)%\Microsoft Visual Studio 11.0\Common7\IDE\WDExpress.exe"
rem "%ProgramFiles(x86)%\Microsoft Visual Studio 11.0\Common7\IDE\devenv"
