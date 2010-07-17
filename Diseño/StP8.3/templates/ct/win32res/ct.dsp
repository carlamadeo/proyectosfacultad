# Microsoft Developer Studio Project File - Name="resource DLL ct" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=resource DLL ct - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ct.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ct.mak" CFG="resource DLL ct - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "resource DLL ct - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 1
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /TP /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "NDEBUG" /D "WIN32DLL" /D "_WINDOWS" /D "WIN32" /FD /TP /c
# SUBTRACT CPP /Fr /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386 /out:"..\icons.dll"
# SUBTRACT LINK32 /pdb:none
# Begin Target

# Name "resource DLL ct - Win32 Release"
# Begin Group "Source"

# PROP Default_Filter ".c;.cpp;.cxx"
# Begin Source File

SOURCE=.\DllMain.C
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter ".h;.hpp;.hxx"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\align.ico
# End Source File
# Begin Source File

SOURCE=.\arc.cur
# End Source File
# Begin Source File

SOURCE=.\arc.ico
# End Source File
# Begin Source File

SOURCE=.\arc_l.cur
# End Source File
# Begin Source File

SOURCE=.\arrow_left.ico
# End Source File
# Begin Source File

SOURCE=.\arrow_right.ico
# End Source File
# Begin Source File

SOURCE=.\bad.ico
# End Source File
# Begin Source File

SOURCE=.\blue_shade.bmp
# End Source File
# Begin Source File

SOURCE=.\check.ico
# End Source File
# Begin Source File

SOURCE=.\check_semantics.ico
# End Source File
# Begin Source File

SOURCE=.\check_syntax.ico
# End Source File
# Begin Source File

SOURCE=.\class_categories.ico
# End Source File
# Begin Source File

SOURCE=.\classes.ico
# End Source File
# Begin Source File

SOURCE=.\closed_folder.ico
# End Source File
# Begin Source File

SOURCE=.\comment.cur
# End Source File
# Begin Source File

SOURCE=.\comment.ico
# End Source File
# Begin Source File

SOURCE=.\comment_l.cur
# End Source File
# Begin Source File

SOURCE=.\copy.cur
# End Source File
# Begin Source File

SOURCE=.\copy.ico
# End Source File
# Begin Source File

SOURCE=.\copy_diagram.ico
# End Source File
# Begin Source File

SOURCE=.\corner.cur
# End Source File
# Begin Source File

SOURCE=.\cover_pa.ico
# End Source File
# Begin Source File

SOURCE=.\ct.rc
# End Source File
# Begin Source File

SOURCE=.\current_cell.ico
# End Source File
# Begin Source File

SOURCE=.\cut.ico
# End Source File
# Begin Source File

SOURCE=.\delete.ico
# End Source File
# Begin Source File

SOURCE=.\delete_row.ico
# End Source File
# Begin Source File

SOURCE=.\diag_annot.ico
# End Source File
# Begin Source File

SOURCE=.\diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\directory_objects.ico
# End Source File
# Begin Source File

SOURCE=.\edit_diagram.ico
# End Source File
# Begin Source File

SOURCE=.\edit_notedesc.ico
# End Source File
# Begin Source File

SOURCE=.\entities.ico
# End Source File
# Begin Source File

SOURCE=.\execute_query.ico
# End Source File
# Begin Source File

SOURCE=.\external_classes.ico
# End Source File
# Begin Source File

SOURCE=.\file_objects.ico
# End Source File
# Begin Source File

SOURCE=.\find.ico
# End Source File
# Begin Source File

SOURCE=.\fit_to_label.ico
# End Source File
# Begin Source File

SOURCE=.\folder.ico
# End Source File
# Begin Source File

SOURCE=.\format_selection.ico
# End Source File
# Begin Source File

SOURCE=.\frame_32.ico
# End Source File
# Begin Source File

SOURCE=.\grid.ico
# End Source File
# Begin Source File

SOURCE=".\hide-show.ico"
# End Source File
# Begin Source File

SOURCE=.\ileaf_32.ico
# End Source File
# Begin Source File

SOURCE=.\insert_row.ico
# End Source File
# Begin Source File

SOURCE=.\message_log.ico
# End Source File
# Begin Source File

SOURCE=.\microsoft_word_files_16.ico
# End Source File
# Begin Source File

SOURCE=.\model_elements.ico
# End Source File
# Begin Source File

SOURCE=.\new_diagram.ico
# End Source File
# Begin Source File

SOURCE=.\new_file.ico
# End Source File
# Begin Source File

SOURCE=.\new_model.ico
# End Source File
# Begin Source File

SOURCE=.\new_table.ico
# End Source File
# Begin Source File

SOURCE=.\note.ico
# End Source File
# Begin Source File

SOURCE=.\note_item.ico
# End Source File
# Begin Source File

SOURCE=.\object_annot.ico
# End Source File
# Begin Source File

SOURCE=.\omt_subsystems.ico
# End Source File
# Begin Source File

SOURCE=.\open_file.ico
# End Source File
# Begin Source File

SOURCE=.\open_folder.ico
# End Source File
# Begin Source File

SOURCE=.\open_model.ico
# End Source File
# Begin Source File

SOURCE=.\open_report.ico
# End Source File
# Begin Source File

SOURCE=.\ortho_draw.ico
# End Source File
# Begin Source File

SOURCE=.\panner.ico
# End Source File
# Begin Source File

SOURCE=.\parent_dir.ico
# End Source File
# Begin Source File

SOURCE=.\paste.ico
# End Source File
# Begin Source File

SOURCE=.\print.ico
# End Source File
# Begin Source File

SOURCE=.\print_preview.ico
# End Source File
# Begin Source File

SOURCE=.\properties.ico
# End Source File
# Begin Source File

SOURCE=.\redo.ico
# End Source File
# Begin Source File

SOURCE=.\refresh.ico
# End Source File
# Begin Source File

SOURCE=.\replace.ico
# End Source File
# Begin Source File

SOURCE=.\requirements_tables.ico
# End Source File
# Begin Source File

SOURCE=.\right_pointer.cur
# End Source File
# Begin Source File

SOURCE=.\rme_32.ico
# End Source File
# Begin Source File

SOURCE=.\rme_cover_page.ico
# End Source File
# Begin Source File

SOURCE=.\rme_diagram.ico
# End Source File
# Begin Source File

SOURCE=.\rme_diagram_set.ico
# End Source File
# Begin Source File

SOURCE=.\rme_object_set.ico
# End Source File
# Begin Source File

SOURCE=.\rme_table.ico
# End Source File
# Begin Source File

SOURCE=.\rme_table_set.ico
# End Source File
# Begin Source File

SOURCE=.\rme_text_set.ico
# End Source File
# Begin Source File

SOURCE=.\save_all.ico
# End Source File
# Begin Source File

SOURCE=.\save_file.ico
# End Source File
# Begin Source File

SOURCE=.\script_manager.ico
# End Source File
# Begin Source File

SOURCE=.\select.ico
# End Source File
# Begin Source File

SOURCE=.\showdesktop.ico
# End Source File
# Begin Source File

SOURCE=.\size_4way.cur
# End Source File
# Begin Source File

SOURCE=.\size_hor.cur
# End Source File
# Begin Source File

SOURCE=.\size_vert.cur
# End Source File
# Begin Source File

SOURCE=.\spline.cur
# End Source File
# Begin Source File

SOURCE=.\spline.ico
# End Source File
# Begin Source File

SOURCE=.\spline_l.cur
# End Source File
# Begin Source File

SOURCE=.\straight_draw.ico
# End Source File
# Begin Source File

SOURCE=.\table_annot.ico
# End Source File
# Begin Source File

SOURCE=.\tables.ico
# End Source File
# Begin Source File

SOURCE=.\undo.ico
# End Source File
# Begin Source File

SOURCE=.\use_cases.ico
# End Source File
# Begin Source File

SOURCE=.\whats_this.ico
# End Source File
# Begin Source File

SOURCE=.\yellow_shade.bmp
# End Source File
# Begin Source File

SOURCE=.\zoom_in.ico
# End Source File
# Begin Source File

SOURCE=.\zoom_out.ico
# End Source File
# Begin Source File

SOURCE=.\zoom_to_fit.ico
# End Source File
# End Group
# End Target
# End Project
