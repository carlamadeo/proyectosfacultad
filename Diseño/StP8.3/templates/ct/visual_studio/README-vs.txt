Navigation to Microsoft VisualStudio
------------------------------------
   
Set the StP ToolInfo variable 
  
source_editor=MSDev
   
to make the navigation to VisualStudio available.

The Navigation is available in the Class Diagram and the Class Table 
of StP/UML. For Class, Attribute and Operation symbols the navigation
command "Source Code" will load the appropriate file in VisualStudio.

If your model has been reverse engineered, StP will know where the
necessary source code file is located, otherwise StP will ask for the
location of the source code file.
(see also chapter 10 of "Generating and Reengineering Code")


Navigation to StP
-----------------

Navigating from VisualStudio requires that the program knows the StP
product (usually UML), the project directory and system and the 
location of StP.

The product is set via
        The .IDEproduct file is set to 'uml'
        The IDE_PRODUCT environment variable is set to 'uml'

The project directory is set via
        The toolinfo variable projdir is set to the appropriate directory
        The IDE_PROJDIR environment variable is set to the appropriate directory

The system is set via
        The toolinfo variable system is set to the appropriate system name
        The IDE_SYSTEM environment variable is set to the appropriate system name

The environment variable settings take precedence over the other methods.

The location of StP should be set by adding 
<StP-Install Directory>/bin/W32ntx86
to the operating system's PATH variable.


Create a new command in VisualStudio by using the Tools/Customize
menu command. For example:

Create a new command "Navigate to StP/UML Class Diagram".
In the command specification use:

Command:   navtostp
Arguments: uclassd UmlClass $(CurText)

Optionally check Option 'Close window on exiting'.

This allows you to navigate to StP based on the currently selected string 
(e.g. a highlighted class name) in the VisualStudio code editor.

Create additional navigations by using different editor types and
UML AppTypes.

navtostp has 3 arguments:
<editor> <AppType> <label>

<editor> can be:
uclassd (Class Diagram)
uclasst (Class Table)
ustated (State Diagram)

<AppType> can be:
UmlClass
UmlAttribute 
UmlOperation
UmlStateMachine

<label> should always be $(CurText), this is subsituted by VisualStudio
according to the current selection.

