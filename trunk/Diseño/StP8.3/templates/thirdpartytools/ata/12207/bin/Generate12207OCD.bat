
START %ATAHOME%\doc\bin\msgpad.exe

copy %STPDOCUMENTS%\ata\ataconfg_ocd_tp %STPDOCUMENTS%\ata\ataconfg
START /WAIT %ATAHOME%\doc\bin\rtf_builder.exe -config %STPDOCUMENTS%\ata\ataconfg

copy %STPDOCUMENTS%\ata\ataconfg_ocd %STPDOCUMENTS%\ata\ataconfg
start /WAIT %ATAHOME%\doc\bin\rtf_builder.exe -config %STPDOCUMENTS%\ata\ataconfg

copy %STPDOCUMENTS%\ata\ataconfg_ocd_app %STPDOCUMENTS%\ata\ataconfg
START /WAIT %ATAHOME%\doc\bin\rtf_builder.exe -config %STPDOCUMENTS%\ata\ataconfg
