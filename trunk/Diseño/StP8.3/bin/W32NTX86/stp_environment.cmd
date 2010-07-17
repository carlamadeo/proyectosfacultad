@echo off
echo Environment variables that can affect StP
echo -----------------------------------------
if NOT x%LM_LICENSE_FILE% == x echo LM_LICENSE_FILE=%LM_LICENSE_FILE%
if x%LM_LICENSE_FILE% == x echo LM_LICENSE_FILE not defined
if NOT x%SYBASE% == x echo SYBASE=%SYBASE%
if x%SYBASE% == x echo SYBASE not defined
if NOT x%DSQUERY% == x echo DSQUERY=%DSQUERY%
if x%DSQUERY% == x echo DSQUERY not defined
if NOT x%ToolInfo% == x echo ToolInfo=%ToolInfo%
if x%ToolInfo% == x echo ToolInfo not defined
echo Use of the following environment variables is discouraged:
if NOT x%IDE_PRODUCT% == x echo IDE_PRODUCT=%IDE_PRODUCT%
if x%IDE_PRODUCT% == x echo IDE_PRODUCT not defined
if NOT x%IDE_PROJDIR% == x echo IDE_PROJDIR=%IDE_PROJDIR%
if x%IDE_PROJDIR% == x echo IDE_PROJDIR not defined
if NOT x%IDE_SYSTEM% == x echo IDE_SYSTEM=%IDE_SYSTEM%
if x%IDE_SYSTEM% == x echo IDE_SYSTEM not defined
if NOT x%IDE_MSGD_HOST% == x echo IDE_MSGD_HOST=%IDE_MSGD_HOST%
if x%IDE_MSGD_HOST% == x echo IDE_MSGD_HOST not defined
if NOT x%IDE_MSGD_PORT% == x echo IDE_MSGD_PORT=%IDE_MSGD_PORT%
if x%IDE_MSGD_PORT% == x echo IDE_MSGD_PORT not defined
if NOT x%IDE_MSGD_SILENT% == x echo IDE_MSGD_SILENT=%IDE_MSGD_SILENT%
if x%IDE_MSGD_SILENT% == x echo IDE_MSGD_SILENT not defined
pause