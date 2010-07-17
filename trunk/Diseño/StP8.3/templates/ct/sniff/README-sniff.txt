Navigation to SNiFF+
--------------------

Set the StP ToolInfo variable 

ProgrammingEnvironment=SNiFF

to make the navigation to SNiFF+ available.
The Navigation is available in the Class Diagram
and the Class Table of StP/UML. For Class, Attribute
and Operation symbols you will find several SNiFF+
tools in the GoTo Menu.

You need a running SNiFF+ session and a loaded Project
for the navigation.

Environments:

SNiFF_PROJECT, defines the SNiFF+ project you want to use
By default SNiFF+ will look in all opened projects for
the specified symbol.

SNiFF_SESSION_ID, defines an Id for the SNiFF+ session


Navigation to StP
-----------------

The navigation to StP only works for Class Symbols

Navigating from SNiFF+ requires that the program knows the StP
product (usually UML), the project directory and system and
the location of StP.

The product is set via
        The .IDEproduct file is set to 'uml'
        The IDE_PRODUCT environment variable is set to 'uml'

The project directory is set via
        The toolinfo variable projdir is set to the appropriate directory
        The IDE_PROJDIR environment variable is set to the appropriate directory

The system is set via
        The toolinfo variable system is set to the appropriate system name
        The IDE_SYSTEM environment variable is set to the appropriate system name

The environment variable settings take presidence over the other methods.

The location of StP should be set by adding
<StP-Install Directory>/bin/W32ntx86
to the operating systems PATH variable.


Follow the given instructions to make the navigation available
for the following SNiFF+ versions:

###################################################
###################################################

SNiFF+ 4.x:

Please follow the instructions as written in file README-sniff4.txt.

###################################################
###################################################

SNiFF+ 3.x higher or equal to 3.2:

Copy file StP.py to <SNiFF+ Dir>/lib/python/Sniff

Edit SiteMenus.sniff and add the following lines:

###################################################
#            Software through Pictures            #
###################################################

^ProjectEditor
> StP/UML
python "StP Desktop" "import StP; StP.Start('stp')"


^Editor
> StP/UML
python "StP Desktop" "import StP; StP.Start('stp')"
-
python "Class Diagram for Class '%k'" "import StP;StP.Run('navtostp', 'uclassd UmlClass %k')"
python "Class Table for Class '%k'" "import StP;StP.Run('navtostp', 'uclasst UmlClass %k')"
python "State Diagram for Class '%k'" "import StP;StP.Run('navtostp', 'ustated UmlStateMachine %k')"

###################################################
###################################################

SNiFF+ Versions older than 3.2

Edit the SiteMenus.sniff and add the following lines:

###################################################
#            Software through Pictures            #
###################################################

^Editor
> StP/UML
shell "Class Diagram for Class '%k'" "navtostp uclassd UmlClass %k"
shell "Class Table for Class '%k'" "navtostp uclasst UmlClass %k"
shell "State Diagram for Class '%k'" "navtostp ustated UmlStateMachine %k"
