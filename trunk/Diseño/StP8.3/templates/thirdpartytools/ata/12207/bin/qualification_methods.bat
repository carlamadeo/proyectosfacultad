echo off

REM  $1 -projdir
REM  $2 -s 
REM  $3 -t 
REM  $4 -o  
REM  qrl script  support/qrl/GenerateDataTypeReport.qrl

SET PROJDIR=%1%
SHIFT
SET SYSTEM=%1%
SHIFT
SET TARGET=%1%
SHIFT
SET OUTPUTFILE=%1%
SHIFT
SET LEVEL=%1%


qrp.exe -projdir %PROJDIR% -s %SYSTEM% -t %TARGET% -o %OUTPUTFILE% -x level %LEVEL%  support/qrl/GenerateQualificationReport.qrl