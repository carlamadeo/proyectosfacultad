# Microsoft Developer Studio Project File - Name="resource DLL uml" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=resource DLL uml - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "uml.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "uml.mak" CFG="resource DLL uml - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "resource DLL uml - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
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
# Begin Target

# Name "resource DLL uml - Win32 Release"
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

SOURCE=.\activity_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\actor_collaboration.cur
# End Source File
# Begin Source File

SOURCE=.\actor_collaboration.ico
# End Source File
# Begin Source File

SOURCE=.\actor_collaboration_l.cur
# End Source File
# Begin Source File

SOURCE=.\actor_sequence.cur
# End Source File
# Begin Source File

SOURCE=.\actor_sequence.ico
# End Source File
# Begin Source File

SOURCE=.\actor_sequence_l.cur
# End Source File
# Begin Source File

SOURCE=.\actor_usecase.cur
# End Source File
# Begin Source File

SOURCE=.\actor_usecase.ico
# End Source File
# Begin Source File

SOURCE=.\actor_usecase_l.cur
# End Source File
# Begin Source File

SOURCE=.\association.ico
# End Source File
# Begin Source File

SOURCE=.\aggregation.ico
# End Source File
# Begin Source File

SOURCE=.\associationclasslink.ico
# End Source File
# Begin Source File

SOURCE=.\associationor.ico
# End Source File
# Begin Source File

SOURCE=.\asynchronous_message.cur
# End Source File
# Begin Source File

SOURCE=.\asynchronous_message.ico
# End Source File
# Begin Source File

SOURCE=.\asynchronous_message_l.cur
# End Source File
# Begin Source File

SOURCE=.\asynchronousmessage.ico
# End Source File
# Begin Source File

SOURCE=.\attribute.cur
# End Source File
# Begin Source File

SOURCE=.\attribute.ico
# End Source File
# Begin Source File

SOURCE=.\attribute_l.cur
# End Source File
# Begin Source File

SOURCE=.\balking_message.cur
# End Source File
# Begin Source File

SOURCE=.\balking_message.ico
# End Source File
# Begin Source File

SOURCE=.\balking_message_l.cur
# End Source File
# Begin Source File

SOURCE=.\balkingmessage.ico
# End Source File
# Begin Source File

SOURCE=.\binary_component.cur
# End Source File
# Begin Source File

SOURCE=.\binary_component.ico
# End Source File
# Begin Source File

SOURCE=.\binary_component_l.cur
# End Source File
# Begin Source File

SOURCE=.\class.cur
# End Source File
# Begin Source File

SOURCE=.\class.ico
# End Source File
# Begin Source File

SOURCE=.\class_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\class_l.cur
# End Source File
# Begin Source File

SOURCE=.\class_tables.ico
# End Source File
# Begin Source File

SOURCE=.\classes.ico
# End Source File
# Begin Source File

SOURCE=.\collaboration.cur
# End Source File
# Begin Source File

SOURCE=.\collaboration.ico
# End Source File
# Begin Source File

SOURCE=.\collaboration_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\collaboration_l.cur
# End Source File
# Begin Source File

SOURCE=.\collaborationrole.ico
# End Source File
# Begin Source File

SOURCE=.\communicates.ico
# End Source File
# Begin Source File

SOURCE=.\component_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\composite_object.cur
# End Source File
# Begin Source File

SOURCE=.\composite_object.ico
# End Source File
# Begin Source File

SOURCE=.\composite_object_l.cur
# End Source File
# Begin Source File

SOURCE=.\composite_state.cur
# End Source File
# Begin Source File

SOURCE=.\composite_state.ico
# End Source File
# Begin Source File

SOURCE=.\composite_state_l.cur
# End Source File
# Begin Source File

SOURCE=.\concurrent_subregion.cur
# End Source File
# Begin Source File

SOURCE=.\concurrent_subregion.ico
# End Source File
# Begin Source File

SOURCE=.\concurrent_subregion_l.cur
# End Source File
# Begin Source File

SOURCE=.\constraint.cur
# End Source File
# Begin Source File

SOURCE=.\constraint.ico
# End Source File
# Begin Source File

SOURCE=.\constraint_l.cur
# End Source File
# Begin Source File

SOURCE=.\decision.cur
# End Source File
# Begin Source File

SOURCE=.\decision.ico
# End Source File
# Begin Source File

SOURCE=.\decision_l.cur
# End Source File
# Begin Source File

SOURCE=.\deep_history.cur
# End Source File
# Begin Source File

SOURCE=.\deep_history.ico
# End Source File
# Begin Source File

SOURCE=.\deep_history_l.cur
# End Source File
# Begin Source File

SOURCE=.\deephistory.ico
# End Source File
# Begin Source File

SOURCE=.\dependency.ico
# End Source File
# Begin Source File

SOURCE=.\deployment.cur
# End Source File
# Begin Source File

SOURCE=.\deployment_component.ico
# End Source File
# Begin Source File

SOURCE=.\deployment_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\deployment_l.cur
# End Source File
# Begin Source File

SOURCE=.\deploys.ico
# End Source File
# Begin Source File

SOURCE=.\ellipsis.ico
# End Source File
# Begin Source File

SOURCE=.\end_time.cur
# End Source File
# Begin Source File

SOURCE=.\end_time.ico
# End Source File
# Begin Source File

SOURCE=.\end_time_l.cur
# End Source File
# Begin Source File

SOURCE=.\executable_component.cur
# End Source File
# Begin Source File

SOURCE=.\executable_component.ico
# End Source File
# Begin Source File

SOURCE=.\executable_component_l.cur
# End Source File
# Begin Source File

SOURCE=.\extends.ico
# End Source File
# Begin Source File

SOURCE=.\extension_point.cur
# End Source File
# Begin Source File

SOURCE=.\extension_point.ico
# End Source File
# Begin Source File

SOURCE=.\extension_point_l.cur
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

SOURCE=.\foreign_class.cur
# End Source File
# Begin Source File

SOURCE=.\foreign_class.ico
# End Source File
# Begin Source File

SOURCE=.\foreign_instantiated_class.ico
# End Source File
# Begin Source File

SOURCE=.\foreign_parameterized_class.ico
# End Source File
# Begin Source File

SOURCE=.\generalization.ico
# End Source File
# Begin Source File

SOURCE=.\generalization_tree.ico
# End Source File
# Begin Source File

SOURCE=.\history.cur
# End Source File
# Begin Source File

SOURCE=.\history.ico
# End Source File
# Begin Source File

SOURCE=.\history_l.cur
# End Source File
# Begin Source File

SOURCE=.\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\implements.ico
# End Source File
# Begin Source File

SOURCE=.\inherits.ico
# End Source File
# Begin Source File

SOURCE=.\inheritance_tree.ico
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

SOURCE=.\inputlink.ico
# End Source File
# Begin Source File

SOURCE=.\insert_attribute.ico
# End Source File
# Begin Source File

SOURCE=.\insert_operation.ico
# End Source File
# Begin Source File

SOURCE=.\instantiated_class.cur
# End Source File
# Begin Source File

SOURCE=.\instantiated_class.ico
# End Source File
# Begin Source File

SOURCE=.\instantiated_class_l.cur
# End Source File
# Begin Source File

SOURCE=.\interface.cur
# End Source File
# Begin Source File

SOURCE=.\interface.ico
# End Source File
# Begin Source File

SOURCE=.\interface_l.cur
# End Source File
# Begin Source File

SOURCE=.\iscomponentof.ico
# End Source File
# Begin Source File

SOURCE=.\junction_point.ico
# End Source File
# Begin Source File

SOURCE=.\mergecontrol.cur
# End Source File
# Begin Source File

SOURCE=.\mergecontrol.ico
# End Source File
# Begin Source File

SOURCE=.\mergecontrol_l.cur
# End Source File
# Begin Source File

SOURCE=.\mergecontrollink.ico
# End Source File
# Begin Source File

SOURCE=.\multi_object.cur
# End Source File
# Begin Source File

SOURCE=.\multi_object.ico
# End Source File
# Begin Source File

SOURCE=.\multi_object_l.cur
# End Source File
# Begin Source File

SOURCE=.\naryassociationrole.ico
# End Source File
# Begin Source File

SOURCE=.\note.cur
# End Source File
# Begin Source File

SOURCE=.\note_l.cur
# End Source File
# Begin Source File

SOURCE=.\notelink.ico
# End Source File
# Begin Source File

SOURCE=.\object.cur
# End Source File
# Begin Source File

SOURCE=.\object.ico
# End Source File
# Begin Source File

SOURCE=.\object_collaboration.cur
# End Source File
# Begin Source File

SOURCE=.\object_collaboration_l.cur
# End Source File
# Begin Source File

SOURCE=.\object_l.cur
# End Source File
# Begin Source File

SOURCE=.\object_sequence.cur
# End Source File
# Begin Source File

SOURCE=.\object_sequence.ico
# End Source File
# Begin Source File

SOURCE=.\object_sequence_l.cur
# End Source File
# Begin Source File

SOURCE=.\objectassociation.ico
# End Source File
# Begin Source File

SOURCE=.\objectinstantiation.ico
# End Source File
# Begin Source File

SOURCE=.\objectlink.ico
# End Source File
# Begin Source File

SOURCE=.\operation.cur
# End Source File
# Begin Source File

SOURCE=.\operation.ico
# End Source File
# Begin Source File

SOURCE=.\operation_l.cur
# End Source File
# Begin Source File

SOURCE=.\outputlink.ico
# End Source File
# Begin Source File

SOURCE=.\package.cur
# End Source File
# Begin Source File

SOURCE=.\package.ico
# End Source File
# Begin Source File

SOURCE=.\package_l.cur
# End Source File
# Begin Source File

SOURCE=.\packages.ico
# End Source File
# Begin Source File

SOURCE=.\parameterized_class.cur
# End Source File
# Begin Source File

SOURCE=.\parameterized_class.ico
# End Source File
# Begin Source File

SOURCE=.\parameterized_class_l.cur
# End Source File
# Begin Source File

SOURCE=.\passive_object.cur
# End Source File
# Begin Source File

SOURCE=.\passive_object.ico
# End Source File
# Begin Source File

SOURCE=.\passive_object_l.cur
# End Source File
# Begin Source File

SOURCE=.\refines.ico
# End Source File
# Begin Source File

SOURCE=.\requirements_tables.ico
# End Source File
# Begin Source File

SOURCE=.\return_message.cur
# End Source File
# Begin Source File

SOURCE=.\return_message.ico
# End Source File
# Begin Source File

SOURCE=.\return_message_l.cur
# End Source File
# Begin Source File

SOURCE=.\returnmessage.ico
# End Source File
# Begin Source File

SOURCE=.\sequence_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\simple_message.cur
# End Source File
# Begin Source File

SOURCE=.\simple_message.ico
# End Source File
# Begin Source File

SOURCE=.\simple_message_l.cur
# End Source File
# Begin Source File

SOURCE=.\simplemessage.ico
# End Source File
# Begin Source File

SOURCE=.\source_component.cur
# End Source File
# Begin Source File

SOURCE=.\source_component.ico
# End Source File
# Begin Source File

SOURCE=.\source_component_l.cur
# End Source File
# Begin Source File

SOURCE=.\splitcontrol.cur
# End Source File
# Begin Source File

SOURCE=.\splitcontrol.ico
# End Source File
# Begin Source File

SOURCE=.\splitcontrol_l.cur
# End Source File
# Begin Source File

SOURCE=.\splitcontrollink.ico
# End Source File
# Begin Source File

SOURCE=.\start_time.cur
# End Source File
# Begin Source File

SOURCE=.\start_time.ico
# End Source File
# Begin Source File

SOURCE=.\start_time_l.cur
# End Source File
# Begin Source File

SOURCE=.\state.cur
# End Source File
# Begin Source File

SOURCE=.\state.ico
# End Source File
# Begin Source File

SOURCE=.\state_activity.ico
# End Source File
# Begin Source File

SOURCE=.\state_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\state_l.cur
# End Source File
# Begin Source File

SOURCE=.\state_machine.cur
# End Source File
# Begin Source File

SOURCE=.\state_machine.ico
# End Source File
# Begin Source File

SOURCE=.\state_machine_a.ico
# End Source File
# Begin Source File

SOURCE=.\state_machine_l.cur
# End Source File
# Begin Source File

SOURCE=.\state_machine_s.ico
# End Source File
# Begin Source File

SOURCE=.\state_tables.ico
# End Source File
# Begin Source File

SOURCE=.\stateambassadorscopelink.ico
# End Source File
# Begin Source File

SOURCE=.\statetransitionlink.ico
# End Source File
# Begin Source File

SOURCE=.\stereotype.cur
# End Source File
# Begin Source File

SOURCE=.\stereotype.ico
# End Source File
# Begin Source File

SOURCE=.\stereotype_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\stereotype_l.cur
# End Source File
# Begin Source File

SOURCE=.\stereotypeinheritance.ico
# End Source File
# Begin Source File

SOURCE=".\StP-UML.ico"
# End Source File
# Begin Source File

SOURCE=.\swimlane.cur
# End Source File
# Begin Source File

SOURCE=.\swimlane.ico
# End Source File
# Begin Source File

SOURCE=.\swimlane_l.cur
# End Source File
# Begin Source File

SOURCE=.\synchronous_message.cur
# End Source File
# Begin Source File

SOURCE=.\synchronous_message.ico
# End Source File
# Begin Source File

SOURCE=.\synchronous_message_l.cur
# End Source File
# Begin Source File

SOURCE=.\synchronousmessage.ico
# End Source File
# Begin Source File

SOURCE=.\test_type_tables.ico
# End Source File
# Begin Source File

SOURCE=.\this_system.cur
# End Source File
# Begin Source File

SOURCE=.\this_system.ico
# End Source File
# Begin Source File

SOURCE=.\this_system_l.cur
# End Source File
# Begin Source File

SOURCE=.\timeout_message.cur
# End Source File
# Begin Source File

SOURCE=.\timeout_message.ico
# End Source File
# Begin Source File

SOURCE=.\timeout_message_l.cur
# End Source File
# Begin Source File

SOURCE=.\timeoutmessage.ico
# End Source File
# Begin Source File

SOURCE=.\type.ico
# End Source File
# Begin Source File

SOURCE=.\type_collaboration.cur
# End Source File
# Begin Source File

SOURCE=.\type_collaboration_l.cur
# End Source File
# Begin Source File

SOURCE=.\typeelementlink.ico
# End Source File
# Begin Source File

SOURCE=.\uml.rc
# End Source File
# Begin Source File

SOURCE=.\use_case.cur
# End Source File
# Begin Source File

SOURCE=.\use_case.ico
# End Source File
# Begin Source File

SOURCE=.\use_case_diagrams.ico
# End Source File
# Begin Source File

SOURCE=.\use_case_l.cur
# End Source File
# Begin Source File

SOURCE=.\use_cases.ico
# End Source File
# Begin Source File

SOURCE=.\uses.ico
# End Source File
# Begin Source File

SOURCE=.\val_of_attribute.cur
# End Source File
# Begin Source File

SOURCE=.\val_of_attribute_l.cur
# End Source File
# Begin Source File

SOURCE=.\value_of_attribute.ico
# End Source File
# End Group
# End Target
# End Project
