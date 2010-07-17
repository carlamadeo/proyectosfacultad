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
# vcc_rcs_ls -d dname
#
# Purpose:
#          rlog
#
# Arguments:
#            -d dname     = file or directory name of archive
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
set dname ""
set fname ""

#
# Define Usage output
#
proc Usage {} {
   upvar #0 argv0 comstring
   puts "\nUsage: $comstring -d dname\n"
}

#
# Grab command line args
#
for {set i 0} {$i < $argc} {incr i} {
   if {[lindex $argv $i] == "-d"} {
      incr i
      set dname [lindex $argv $i]
   } else {
      puts "\nERROR: Unknown option [lindex $argv $i]\n"
      Usage
      exit 1
   }
}

#
# Exclude bad calls
#
if {($argc < 2) || ($dname == "")} {
   puts "\nError: incorrect or insufficient parameters"
   Usage
   exit 1
}

#
# Now we determine whether we are in file or dir mode
#
if {![file isdirectory $dname]} {
   set fname [file tail $dname]
   set dname [file dirname $dname]
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
if {[catch {cd $dname} errmsg]} {
   puts "Error moving to archive directory:\n$errmsg\n\nThis script must fail"
   exit 1
}   

#
# Now to list the contents of the RCS archive
#
if {$fname == ""} {
   if {[catch {cd RCS} errmsg]} {
      puts "Could not move into archive"
      puts "\nThis script must fail\n"
      exit 1
   } else {
      set outbuf [glob *]
      cd ..
   }
   puts  "FILES IN ARCHIVE $dname:\n"
   foreach i $outbuf {
      if {[catch {exec rlog -h $i} errmsg]} {
         puts "\nERROR: rlog failed on file $i"
         puts "\nThis script must fail\n"
         exit 1
      } else {
	puts "$errmsg"
      }
   }
} else {
   if {[catch {exec rlog -h $fname} errmsg]} {
      puts "\nERROR: rlog failed on file $i"
      puts "\nThis script must fail\n"
      exit 1
   } else {
	puts "$errmsg"
   }
}

exit 0
