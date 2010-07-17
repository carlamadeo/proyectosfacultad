
START %ATAHOME%\doc\bin\msgpad.exe

copy %STPDOCUMENTS%\ata\ataconfg_stp_tp %STPDOCUMENTS%\ata\ataconfg
START /WAIT %ATAHOME%\doc\bin\rtf_builder.exe -config %STPDOCUMENTS%\ata\ataconfg

copy %STPDOCUMENTS%\ata\ataconfg_stp %STPDOCUMENTS%\ata\ataconfg
start /WAIT %ATAHOME%\doc\bin\rtf_builder.exe -config %STPDOCUMENTS%\ata\ataconfg

copy %STPDOCUMENTS%\ata\ataconfg_stp_app %STPDOCUMENTS%\ata\ataconfg
START /WAIT %ATAHOME%\doc\bin\rtf_builder.exe -config %STPDOCUMENTS%\ata\ataconfg
