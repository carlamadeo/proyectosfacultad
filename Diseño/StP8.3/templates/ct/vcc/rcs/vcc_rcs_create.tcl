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
# vcc_rcs_create -f fname [-d dname] [-p policy] [-c comment] [-o lfile] 
#
# Purpose:
#          Create an RCS backup of a system dump
#
# Arguments:
#            -f fname     = FQN of file to be checked in (source location and 
#                           filename)
#            [-d dname]   = directory in which to create the revision control
#            [-p policy]  = policy of create, l for checkout locked working version, 
#                           u for check out unlocked working version, default is to 
#                           delete the working version
#            [-c comment] = option comment for element creation
#            [-o lfile]   = Alternate logfile, default is stdout
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
set lfile stdout
set holder stdout
set fname ""
set dname ""
set policy ""
set comment ""
set same 0

#
# Define Usage output
#
proc Usage {} {
   upvar #0 lfile fhand
   upvar #0 argv0 comstring
   puts $fhand "\nUsage: $comstring -f fname \[-d dname\] \[-p policy\] \[-o logfile\] \[-c comment\]\n"
}

#
# Grab command line args
#
for {set i 0} {$i < $argc} {incr i} {
   if {[lindex $argv $i] == "-d"} {
      incr i
      set dname [lindex $argv $i]
   } elseif {[lindex $argv $i] == "-f"} {
      incr i
      set fname [lindex $argv $i]
   } elseif {[lindex $argv $i] == "-p"} {
      incr i
      if {[regexp -- "^(-).*" [lindex $argv $i]]} {
         puts "\nERROR: Bad policy value [lindex $argv $i]"
         Usage
         exit 1
      }
      set policy [format " -%s " [lindex $argv $i]]
   } elseif {[lindex $argv $i] == "-c"} {
      incr i
      set comment [lindex $argv $i]
   } elseif {[lindex $argv $i] == "-o"} {
      incr i
      set holder [lindex $argv $i]
   } else {
      puts "\nERROR: Unknown option [lindex $argv $i]\n"
      Usage
      exit 1
   }
}

#
# Set dname to default, if necessary
#
if {$dname == ""} {
   set dname [file dirname $fname]
   set same 1
}

#
# Exclude bad calls
#
if {($argc < 2) || ($fname == "")} {
   puts $lfile "\nError: incorrect or insufficient parameters"
   Usage
   exit 1
}

#
# If an alternate logfile was entered, then attempt to open it.
#
if {$holder != "stdout"} {
   set lfile UNINITIALIZED
   catch { set lfile [open $holder a] } errmsg
   if {$lfile == "UNINITIALIZED"} {
      puts "Opening logfile $holder failed"
      set lfile stdout
   }
}

#
# Check entered value
#
if {![file readable $fname]} {
   puts $lfile "\nCannot read file: $fname"
   puts $lfile "\nThis script must fail"
   exit 1
}

#
# Test dir
#
if {![file isdir $dname]} {
   puts $lfile "\nThe entered directory, $dname, is invalid"
   puts $lfile "\nThis script must fail\n"
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
# Check for existing RCS directory
#
if {![file isdir $dname/RCS]} {
   if {[catch {file mkdir [file nativename $dname/RCS]} errmsg]} {
      puts $lfile "\nError creating archive directory:\n$errmsg"
      puts $lfile "\nThis script must fail\n"
      exit 1
   }
}

#
# Move to backup dir
#
if {[catch {cd $dname} errmsg]} {
   puts $lfile "\nError changing to backup directory $dname:\n$errmsg"
   puts $lfile "\nThis script must fail\n"
   exit 1
}

#
# Confirm that file is not already under CM control
#
if {![catch {exec rlog [file tail $fname]} errmsg]} {
   puts $lfile "\nError: File [file tail $fname] is already under RCS control"
   puts $lfile "\nThis script must fail\n"
   exit 1
}

#
# Now to move the file into place
#
if {!$same} {
   if {[catch {file rename -force $fname $dname} errmsg]} {
      puts $lfile "\nWarning: file move error:\n$errmsg"
      puts $lfile "\nThis script must fail\n"
      exit 1
   }
}

#
# Now to run initial RCS command
#
if {[catch {set inpipe [open "| rcs -i -q -I [file tail $fname]" w]} errmsg]} {
   puts $lfile "Error initializing RCS file:\n$errmsg\n\nThis script must fail"
   exit 1
} else {
   if {$comment != ""} {
      puts $inpipe $comment
   }
   puts $inpipe ".\n"
   flush $inpipe
   catch close $inpipe
   puts $lfile "\nSuccess initializing the RCS file"
}

#
# To insure that the rcs call completes :-(yuck!)
#
after 2000

#
# Now to check in the first version
#
set comc [format { %s %s } $policy [file tail $fname]]
if {[catch "exec ci -q -I -f $comc" errmsg]} {
   puts $lfile "Error checking in initial version:\n$errmsg\n\nThis script must fail"
   exit 1
} else {
   puts $lfile "\nSuccess checking in initial version\n"
}

#
# Now, if policy directed us retain a working copy, 
# we move this working file into place
#
if {$policy != ""} {
   if {!$same} {
      if {[catch {file rename [file tail $fname] [file dirname $fname]} errmsg]} {
         puts $lfile "Error moving file:\n$errmsg\n\nThis script must fail"
      }
   }
}

puts $lfile "\nElement creation successful\n"
exit 0
