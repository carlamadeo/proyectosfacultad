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
#	python "Start <application>" "import StP; StP.Start('<application-name>', '<params>')"
#	python "Run <application>" "import StP; StP.Run('<application-name>', '<params>')"
#
# Custommenus:
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


import vcs, etgui, etguiimpl
#from FileDialog import ShowFileDialog
import os

def Start(cmd, params=''):
	# vcs.is_reachable(cmd) - cmd is in PATH (eg. sniff)
	# os.path.exists(cmd)   - cmd is NOT in PATH (eg. C:/sniff/bin/sniff.exe)
	if vcs.is_reachable(cmd) or os.path.exists(cmd):
		etgui.gSystem.RunProgAsynch(cmd + ' ' + params)
	else:
		print "Error: Executable for %s not found !!!" % cmd

def Run(cmd, params=''):
	if vcs.is_reachable(cmd) or os.path.exists(cmd):
		vcs.system(cmd + ' ' + params)
	else:
		print "Error: Executable for %s not found !!!" % cmd


