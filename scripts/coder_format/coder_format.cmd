@echo off
rem $Id$

rem Define Coder Format shell invocation script path.
set coderFormatPath=c:\program files\coder_format
rem Define location of Drupal's file.inc.
set fileInc=c:\program files\coder_format\file.inc


:: ----- You should not need to edit anything below. ----- ::

set oldpwd=%CD%
cd /d "%coderFormatPath%"

rem Simple file extension check, won't work with directories containing a dot.
if '%1'=='--undo' goto undo-directory
if '%~x1'=='' goto directory
goto file

:undo-directory
rem Undo source code formattings performed by --batch-replace
rem Process directory.
if "%~2"=="" goto :EOF
start "coder_format" /D "%coderFormatPath%" /B /WAIT php coder_format.php --undo "%~2" --file-inc "%fileInc%"
goto processed

:directory
rem Recursively format all source code files in a directory.
rem Process directory.
if "%~1"=="" goto :EOF
start "coder_format" /D "%coderFormatPath%" /B /WAIT php coder_format.php --batch-replace "%~1" --file-inc "%fileInc%"
goto processed

:file
rem Format a single source code file.
rem Define source and target files by command line arguments.
set sourcefile=%~1
if not '%~2'=='' (
	set targetfile=%~2
) else (
	set targetfile=%~1
)
rem Process file.
if "%sourcefile%"=="" goto :EOF
if "%targetfile%"=="" goto :EOF
start "coder_format" /D "%coderFormatPath%" /B /WAIT php coder_format.php "%sourcefile%" "%targetfile%" --file-inc "%fileInc%"


:processed
rem Jump back to original working directory.
cd /d "%oldpwd%"

