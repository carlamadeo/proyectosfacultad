# Microsoft Developer Studio Project File - Name="resource DLL se" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=resource DLL se - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "se.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "se.mak" CFG="resource DLL se - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "resource DLL se - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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
# PROP Ignore_Export_Lib 0
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
# Begin Special Build Tool
OutDir=.\Release
SOURCE="$(InputPath)"
PreLink_Cmds=nm $(OutDir)/*.obj | egrep ' T ' | awk 'BEGIN { print "EXPORTS"} {              printf("\t"); printf($3); printf("\n")}' > $(OutDir)/exports.def
# End Special Build Tool
# Begin Target

# Name "resource DLL se - Win32 Release"
# Begin Group "Source"

# PROP Default_Filter ".c;.cpp;.cxx"
# Begin Source File

SOURCE=..\..\ct\win32res\DllMain.C
# End Source File
# End Group
# Begin Group "Include"

# PROP Default_Filter ".h;.hpp;.hxx"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\action_logic_tables.ico
# End Source File
# Begin Source File

SOURCE=.\all_control_spec_tables.ico
# End Source File
# Begin Source File

SOURCE=.\anchor.cur
# End Source File
# Begin Source File

SOURCE=.\anchor.ico
# End Source File
# Begin Source File

SOURCE=.\anchor_l.cur
# End Source File
# Begin Source File

SOURCE=.\code_block.cur
# End Source File
# Begin Source File

SOURCE=.\code_block.ico
# End Source File
# Begin Source File

SOURCE=.\code_block_l.cur
# End Source File
# Begin Source File

SOURCE=.\source_comment.cur
# End Source File
# Begin Source File

SOURCE=.\source_comment.ico
# End Source File
# Begin Source File

SOURCE=.\source_comment_l.cur
# End Source File
# Begin Source File

SOURCE=.\cspec_bar.cur
# End Source File
# Begin Source File

SOURCE=.\cspec_bar.ico
# End Source File
# Begin Source File

SOURCE=.\cspec_bar_l.cur
# End Source File
# Begin Source File

SOURCE=.\data_flow_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\data_structure_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\decision_tables.ico
# End Source File
# Begin Source File

SOURCE=.\demarco_cspec_bar.ico
# End Source File
# Begin Source File

SOURCE=.\demarco_external.cur
# End Source File
# Begin Source File

SOURCE=.\demarco_external.ico
# End Source File
# Begin Source File

SOURCE=.\demarco_external_l.cur
# End Source File
# Begin Source File

SOURCE=.\demarco_external_store.ico
# End Source File
# Begin Source File

SOURCE=.\demarco_offpage_external.ico
# End Source File
# Begin Source File

SOURCE=.\demarco_offpage_process.ico
# End Source File
# Begin Source File

SOURCE=.\demarco_process.cur
# End Source File
# Begin Source File

SOURCE=.\demarco_process.ico
# End Source File
# Begin Source File

SOURCE=.\demarco_process_l.cur
# End Source File
# Begin Source File

SOURCE=.\demarco_store.cur
# End Source File
# Begin Source File

SOURCE=.\demarco_store.ico
# End Source File
# Begin Source File

SOURCE=.\demarco_store_l.cur
# End Source File
# Begin Source File

SOURCE=.\do_while_statement.cur
# End Source File
# Begin Source File

SOURCE=.\do_while_statement.ico
# End Source File
# Begin Source File

SOURCE=.\do_while_statement_l.cur
# End Source File
# Begin Source File

SOURCE=.\directory_objects.ico
# End Source File
# Begin Source File

SOURCE=.\enumeration.cur
# End Source File
# Begin Source File

SOURCE=.\enumeration.ico
# End Source File
# Begin Source File

SOURCE=.\enumeration_l.cur
# End Source File
# Begin Source File

SOURCE=.\event_action.cur
# End Source File
# Begin Source File

SOURCE=.\event_action.ico
# End Source File
# Begin Source File

SOURCE=.\event_action_l.cur
# End Source File
# Begin Source File

SOURCE=.\event_logic_tables.ico
# End Source File
# Begin Source File

SOURCE=.\file_objects.ico
# End Source File
# Begin Source File

SOURCE=.\flow_chart_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\final_state.cur
# End Source File
# Begin Source File

SOURCE=.\final_state.ico
# End Source File
# Begin Source File

SOURCE=.\final_state_l.cur
# End Source File
# Begin Source File

SOURCE=.\for_statement.cur
# End Source File
# Begin Source File

SOURCE=.\for_statement.ico
# End Source File
# Begin Source File

SOURCE=.\for_statement_l.cur
# End Source File
# Begin Source File

SOURCE=.\gane_cspec_bar.ico
# End Source File
# Begin Source File

SOURCE=.\gane_external.cur
# End Source File
# Begin Source File

SOURCE=.\gane_external.ico
# End Source File
# Begin Source File

SOURCE=.\gane_external_l.cur
# End Source File
# Begin Source File

SOURCE=.\gane_external_store.ico
# End Source File
# Begin Source File

SOURCE=.\gane_offpage_external.ico
# End Source File
# Begin Source File

SOURCE=.\gane_offpage_process.ico
# End Source File
# Begin Source File

SOURCE=.\gane_process.cur
# End Source File
# Begin Source File

SOURCE=.\gane_process.ico
# End Source File
# Begin Source File

SOURCE=.\gane_process_l.cur
# End Source File
# Begin Source File

SOURCE=.\gane_store.cur
# End Source File
# Begin Source File

SOURCE=.\gane_store.ico
# End Source File
# Begin Source File

SOURCE=.\gane_store_l.cur
# End Source File
# Begin Source File

SOURCE=.\global_data.cur
# End Source File
# Begin Source File

SOURCE=.\global_data.ico
# End Source File
# Begin Source File

SOURCE=.\global_data_l.cur
# End Source File
# Begin Source File

SOURCE=.\if_else_statement.cur
# End Source File
# Begin Source File

SOURCE=.\if_else_statement.ico
# End Source File
# Begin Source File

SOURCE=.\if_else_statement_l.cur
# End Source File
# Begin Source File

SOURCE=.\if_statement.cur
# End Source File
# Begin Source File

SOURCE=.\if_statement.ico
# End Source File
# Begin Source File

SOURCE=.\if_statement_l.cur
# End Source File
# Begin Source File

SOURCE=.\initial_state.cur
# End Source File
# Begin Source File

SOURCE=.\initial_state.ico
# End Source File
# Begin Source File

SOURCE=.\initial_state_l.cur
# End Source File
# Begin Source File

SOURCE=".\input-output_data.cur"
# End Source File
# Begin Source File

SOURCE=".\input-output_data.ico"
# End Source File
# Begin Source File

SOURCE=".\input-output_data_l.cur"
# End Source File
# Begin Source File

SOURCE=.\input_data.cur
# End Source File
# Begin Source File

SOURCE=.\input_data.ico
# End Source File
# Begin Source File

SOURCE=.\input_data_l.cur
# End Source File
# Begin Source File

SOURCE=.\input_flag.cur
# End Source File
# Begin Source File

SOURCE=.\input_flag.ico
# End Source File
# Begin Source File

SOURCE=.\input_flag_l.cur
# End Source File
# Begin Source File

SOURCE=.\iteration.cur
# End Source File
# Begin Source File

SOURCE=.\iteration.ico
# End Source File
# Begin Source File

SOURCE=.\iteration_l.cur
# End Source File
# Begin Source File

SOURCE=.\library_call.cur
# End Source File
# Begin Source File

SOURCE=.\library_call.ico
# End Source File
# Begin Source File

SOURCE=.\library_call_l.cur
# End Source File
# Begin Source File

SOURCE=.\library_module.cur
# End Source File
# Begin Source File

SOURCE=.\library_module.ico
# End Source File
# Begin Source File

SOURCE=.\library_module_l.cur
# End Source File
# Begin Source File

SOURCE=.\macro.cur
# End Source File
# Begin Source File

SOURCE=.\macro.ico
# End Source File
# Begin Source File

SOURCE=.\macro_l.cur
# End Source File
# Begin Source File

SOURCE=.\module.cur
# End Source File
# Begin Source File

SOURCE=.\module.ico
# End Source File
# Begin Source File

SOURCE=.\module_l.cur
# End Source File
# Begin Source File

SOURCE=.\note.cur
# End Source File
# Begin Source File

SOURCE=.\note_l.cur
# End Source File
# Begin Source File

SOURCE=.\offpage_connector.cur
# End Source File
# Begin Source File

SOURCE=.\offpage_connector.ico
# End Source File
# Begin Source File

SOURCE=.\offpage_connector_l.cur
# End Source File
# Begin Source File

SOURCE=.\offpage_external.cur
# End Source File
# Begin Source File

SOURCE=.\offpage_external.ico
# End Source File
# Begin Source File

SOURCE=.\offpage_external_l.cur
# End Source File
# Begin Source File

SOURCE=.\offpage_process.cur
# End Source File
# Begin Source File

SOURCE=.\offpage_process.ico
# End Source File
# Begin Source File

SOURCE=.\offpage_process_l.cur
# End Source File
# Begin Source File

SOURCE=.\offpage_store.cur
# End Source File
# Begin Source File

SOURCE=.\offpage_store_l.cur
# End Source File
# Begin Source File

SOURCE=.\output_data.cur
# End Source File
# Begin Source File

SOURCE=.\output_data.ico
# End Source File
# Begin Source File

SOURCE=.\output_data_l.cur
# End Source File
# Begin Source File

SOURCE=.\output_flag.cur
# End Source File
# Begin Source File

SOURCE=.\output_flag.ico
# End Source File
# Begin Source File

SOURCE=.\output_flag_l.cur
# End Source File
# Begin Source File

SOURCE=.\pointer_call.cur
# End Source File
# Begin Source File

SOURCE=.\pointer_call.ico
# End Source File
# Begin Source File

SOURCE=.\pointer_call_l.cur
# End Source File
# Begin Source File

SOURCE=.\process_activation_matrix_tables.ico
# End Source File
# Begin Source File

SOURCE=.\process_activation_tables.ico
# End Source File
# Begin Source File

SOURCE=.\requirements_tables.ico
# End Source File
# Begin Source File

SOURCE=.\se.rc
# End Source File
# Begin Source File

SOURCE=.\selection.cur
# End Source File
# Begin Source File

SOURCE=.\selection.ico
# End Source File
# Begin Source File

SOURCE=.\selection_l.cur
# End Source File
# Begin Source File

SOURCE=.\sceselection.cur
# End Source File
# Begin Source File

SOURCE=.\sceselection.ico
# End Source File
# Begin Source File

SOURCE=.\sceselection_l.cur
# End Source File
# Begin Source File

SOURCE=.\sequence.cur
# End Source File
# Begin Source File

SOURCE=.\sequence.ico
# End Source File
# Begin Source File

SOURCE=.\sequence_l.cur
# End Source File
# Begin Source File

SOURCE=.\state.cur
# End Source File
# Begin Source File

SOURCE=.\state.ico
# End Source File
# Begin Source File

SOURCE=.\state_event_matrix_tables.ico
# End Source File
# Begin Source File

SOURCE=.\state_l.cur
# End Source File
# Begin Source File

SOURCE=.\state_transition_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\state_transition_tables.ico
# End Source File
# Begin Source File

SOURCE=".\StP-SE.ico"
# End Source File
# Begin Source File

SOURCE=.\structure_chart_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\switch_statement.cur
# End Source File
# Begin Source File

SOURCE=.\switch_statement.ico
# End Source File
# Begin Source File

SOURCE=.\switch_statement_l.cur
# End Source File
# Begin Source File

SOURCE=.\terminal.cur
# End Source File
# Begin Source File

SOURCE=.\terminal.ico
# End Source File
# Begin Source File

SOURCE=.\terminal_l.cur
# End Source File
# Begin Source File

SOURCE=.\typedef.cur
# End Source File
# Begin Source File

SOURCE=.\typedef.ico
# End Source File
# Begin Source File

SOURCE=.\typedef_l.cur
# End Source File
# Begin Source File

SOURCE=.\user_call.cur
# End Source File
# Begin Source File

SOURCE=.\user_call.ico
# End Source File
# Begin Source File

SOURCE=.\user_call_l.cur
# End Source File
# Begin Source File

SOURCE=.\void_terminal.cur
# End Source File
# Begin Source File

SOURCE=.\void_terminal.ico
# End Source File
# Begin Source File

SOURCE=.\void_terminal_l.cur
# End Source File
# Begin Source File

SOURCE=.\while_statement.cur
# End Source File
# Begin Source File

SOURCE=.\while_statement.ico
# End Source File
# Begin Source File

SOURCE=.\dataflow.ico
# End Source File
# Begin Source File

SOURCE=.\controlflow.ico
# End Source File
# Begin Source File

SOURCE=.\commentlink.ico
# End Source File
# Begin Source File

SOURCE=.\modulecall.ico
# End Source File
# Begin Source File

SOURCE=.\defaultstyle.ico
# End Source File
# Begin Source File

SOURCE=.\offpageconnection.ico
# End Source File
# Begin Source File

SOURCE=.\seflowlink.ico
# End Source File
# Begin Source File

SOURCE=.\sesourcecommentlink.ico
# End Source File
# Begin Source File

SOURCE=.\component.ico
# End Source File
# Begin Source File

SOURCE=.\transition.ico
# End Source File
# Begin Source File

SOURCE=.\while_statement_l.cur
# End Source File
# End Group
# End Target
# End Project
