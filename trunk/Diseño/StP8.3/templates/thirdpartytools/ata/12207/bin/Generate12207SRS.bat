

START %ATAHOME%\doc\bin\msgpad.exe

copy %STPDOCUMENTS%\ata\ataconfg_srs_tp %STPDOCUMENTS%\ata\ataconfg
START /WAIT %ATAHOME%\doc\bin\rtf_builder.exe -config %STPDOCUMENTS%\ata\ataconfg

copy %STPDOCUMENTS%\ata\ataconfg_srs %STPDOCUMENTS%\ata\ataconfg
START /WAIT %ATAHOME%\doc\bin\rtf_builder.exe -config %STPDOCUMENTS%\ata\ataconfg

copy %STPDOCUMENTS%\ata\ataconfg_srs_app %STPDOCUMENTS%\ata\ataconfg
START /WAIT %ATAHOME%\doc\bin\rtf_builder.exe -config %STPDOCUMENTS%\ata\ataconfg


