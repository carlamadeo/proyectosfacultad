This file describes the steps needed for SNiFF+ version 4.x.

For older versions of SNiFF+ please follow the instructions in file
README-sniff.txt.

Navigation from StP to SNiFF+
-----------------------------

The Navigation is available in the Class Diagram and the Class Table
of StP/UML. For Class, Attribute and Operation symbols you will find
several SNiFF+ tools in the GoTo Menu.

You need a running SNiFF+ session and a loaded Project for the
navigation.


Navigation from SNiFF+ to StP
-----------------------------

The navigation from SNiFF+ to StP only works for Class Symbols



Follow the given instructions to make both navigations available:

###################################################
###################################################

Common for the UNIX and the Windows integration:
------------------------------------------------
Copy the files of the Sniff4/python directory to <Sniff-Install Directory>/python.

Copy the file StP.sniff from Sniff4/config/tools/integrations to the directory
<Sniff-Install Directory>/config/tools/integrations

Add the following line just after the starting comments
import.file("integrations/StP")
and add the following into the ExtendMenuOrToolBar part
                "<MENU>"     -- sub menu
                    "StP"    -- menu ID
                "</>"
to file <Sniff-Install Directory>/config/tools/integrations/SniffCustomMenus.sniff
The file should look like this snipplet:

-----------8<--------------
--
--
import.file("integrations/StP")


-- custom menu
ExtendMenuOrToolBar (
	Base : "CustomMenu"					-- predefined menu ID (do not modify)
	Items : array (
--		"CustomRunWinScript"			-- menu entry ID
--		"CustomRunJavaApplication"		-- menu entry ID
--		"CustomRunPythonScript"			-- menu entry ID
--		"-"								-- separator
--		"<MENU>"						-- sub menu
--			"CustomPythonDialogSubMenu"	-- menu ID
--		"</>"
                "<MENU>"     -- sub menu
                    "StP"    -- menu ID
                "</>"
        )
)
-----------8<--------------

Add the following ToolInfo variables to your ToolInfo file
    projdir=<StP Project Directory>
    system=<StP System Name>
    ProgrammingEnvironment=SNiFF


Only for the Windows integration, for UNIX look below:
------------------------------------------------------
Edit the file <Sniff-Install Directory>/bin/sniffstart.cmd,
change the last line from:
start sniff.exe %1 %2 %3 %4 %5 %6 %7 %8 %9
to
start sniff.exe -p %SNIFF_PORT% %1 %2 %3 %4 %5 %6 %7 %8 %9

Set the following environment variables
PATH=%PATH%;<StP-Install Directory>\bin\W32NTX86;%SNIFF_DIR4%\bin;%SNIFF_DIR4%\lib
SNIFF_DIR4=<Sniff-Install Directory>
SNIFF_PORT=<some free tcp port number>

'SNIFF_PORT' should get assigned some free port number, usually a number in
the range 1025 ... 65535, please check that the number is not in use, lower
numbers might be used by StP itself, some ports, like 1700 or 6100, might be
in use by FlexLM and Sybase.


Only for the UNIX integration, for Windows look above:
------------------------------------------------------
Edit the file <Sniff-Install Directory>/bin/sniff, and change at the end the lines
    if [ $# = 0 ]; then
		exec $PROG_DIR/bin.$SNIFF_ARCH/$PROG_NAME
    else
		exec $PROG_DIR/bin.$SNIFF_ARCH/$PROG_NAME "$@"
    fi
to
    if [ $# = 0 ]; then
		exec $PROG_DIR/bin.$SNIFF_ARCH/$PROG_NAME -p $SNIFF_PORT
    else
		exec $PROG_DIR/bin.$SNIFF_ARCH/$PROG_NAME -p $SNIFF_PORT "$@"
    fi

Set the following environment variables
PATH=$PATH:<StP-Install Directory>/bin/W32NTX86:$SNIFF_DIR4/bin:$SNIFF_DIR4/lib
SNIFF_DIR4=<Sniff-Install Directory>
SNIFF_PORT=<some free tcp port number>

'SNIFF_PORT' should get assigned some free port number, usually a number in
the range 1025 ... 65535, please check that the number is not in use, lower
numbers might be used by StP itself, some ports, like 1700, 4100 or 8000,
might be in use by FlexLM, Sybase or X-Windows.

###################################################
###################################################
