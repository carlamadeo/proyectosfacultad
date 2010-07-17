#
# Author: Guenter Weitersberger
# Copyright (c) 2000 Wind River / TakeFive, Salzburg
#
# StP.py = Application.py
#
# Functions:
#	Start(cmd, params='')
#		starts an application (cmd)
#		parameters optional - space is set between command and parameters
#		SNiFF+ is available during the execution
#
#	Run(cmd, params='')
#		starts an application (cmd)
#		parameters optional - space is set between command and parameters
#		SNiFF+ is not available during the execution
#
#
# Usage:
#	called from Stp custom menu: StP.Start('<application-name>', '<params>')"
#	called from Stp custom menu: StP.Run('<application-name>', '<params>')"
#
# Custom menus:
#	see $SNIFF_DIR4/config/tools/integrations/StP.sniff
# Old custom menus:
#	^ProjectEditor
#	> StP/UML
#	python "StP Desktop" "import StP; StP.Start('stp')"
#
#
#	^Editor
#	> StP/UML
#	python "StP Desktop" "import StP; StP.Start('stp')"
#	-
#	python "Class Diagram for Class '%k'" "import StP;StP.Run('navtostp', 'uclassd UmlClass %k')"
#	python "Class Table for Class '%k'" "import StP;StP.Run('navtostp', 'uclasst UmlClass %k')"
#	python "State Diagram for Class '%k'" "import StP;StP.Run('navtostp', 'ustated UmlStateMachine %k')"


#from FileDialog import ShowFileDialog
import os, string
from cm import System
from cm.Constants import TRUE, FALSE
from sniffaccess import SniffAccess
import CMContext

# test == TRUE - print debug info to log window
# test == FALSE - no debug info

Test = FALSE

def Start(cmd, params=''):
	# System.is_reachable(cmd) - cmd is in PATH (eg. sniff)
	# os.path.exists(cmd)   - cmd is NOT in PATH (eg. C:/sniff/bin/sniff.exe)
	if System.is_reachable(cmd) or os.path.exists(cmd):
		cmdList = [cmd]
		if params != '':
			cmdList = cmdList + string.split(params)
		if Test:
			CMContext.LogInfo("Stp::Start(" + string.join(cmdList) + ")")
		CMContext.RunCmdDetached(cmdList)
	else:
		CMContext.LogError("Error: Executable for " + cmd + " not found !!!")

def Run(cmd, params='', classname=''):
	if System.is_reachable(cmd) or os.path.exists(cmd):
		cmdList = [cmd]
		if params != '':
			cmdList = cmdList + string.split(params)
		if classname != '':
			sa = SniffAccess.Sniffaccess ()
			theResult = ""
			if len (sa.SA_GetClassFromName (classname)):  # returns 0 or 1
				cmdList = cmdList + [classname]
			else:
				CMContext.LogWarning("The selected string: '" + classname + "' is not a class name")
				return
		if Test:
			CMContext.LogInfo("Stp::Run(" + string.join(cmdList) + ")")
		System.execute(cmdList)
	else:
		CMContext.LogError("Error: Executable for " + cmd + " not found !!!")


