#-----------------------------------------------------------------------------
# Software Through Pictures
# Copyright (c) 1996-1999 Aonix.  All Rights Reserved.
#
# This source program is the trade secret and confidential property
# of Aonix, and is protected under the law of copyright
# as an unpublished work.  Use and disclosure of this program is restricted
# by license agreement.
#-----------------------------------------------------------------------------
#
# vcc_rcs_rlog -f fname [additional args]
#
# Purpose:
#          rlog
#
# Arguments:
#            -f fname     = name of file to be reported on
#
# Returns:
#          0 for Successful completion
#          1 Otherwise
#
# Note: Spaces between flags and values are _required_
#


#
# Initialize variables
#
set fname ""
set rargs ""

#
# Define Usage output
#
proc Usage {} {
   upvar #0 argv0 comstring
   puts "\nUsage: $comstring -f fname \[-u \[rev\]\]\n"
}

#
# Grab command line args
#
for {set i 0} {$i < $argc} {incr i} {
   if {[lindex $argv $i] == "-f"} {
      incr i
      set fname [lindex $argv $i]
   } elseif {[lindex $argv $i] == "-u"} {
      if {![regexp -- "^(-).*" [lindex $argv [expr $i + 1]]]} {
         incr i
         set revision [format " -u%s " $[lindex $argv $i]]
      } else {
         set locking " -u "
      }
   } else {
      set rargs [format {%s %s} $rargs [lindex $argv $i]]
   }
}

#
# Exclude bad calls
#
if {($argc < 2) || ($fname == "")} {
   puts "\nError: incorrect or insufficient parameters"
   Usage
   exit 1
}

#
# If necessary, set environment to indicate _some_ username
#
if {[catch {set name $env(LOGNAME)} errmsg]} {
   puts "Resetting LOGNAME anonymous"
   set env(LOGNAME) anonymous
}

#
# Now to move to backup dir
#
if {[catch {cd [file nativename [file dirname $fname]]} errmsg]} {
   puts "Error moving to RCS file:\n$errmsg\n\nThis script must fail"
   exit 1
}   

#
# Now to run RCS command
#
set comc [format {
   exec rlog %s %s
} $rargs [file tail $fname]]

if {[catch $comc errmsg] != 0} {
   puts "Error analyzing RCS file:\n$errmsg\n\nThis script must fail"
   exit 1
} else {
   puts  "\nRLOG OUTPUT:\n============$errmsg"
}

puts "\nElement analysis successful\n"
exit 0
