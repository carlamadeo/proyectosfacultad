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
# vcc_rcs_uncheckout -f fname [-d dname] [-u [rev]] [-o lfile] 
#
# Purpose:
#          UN-Check out from RCS, a system dump
#
# Arguments:
#            -f fname     = FQN, name of file to be unchecked out
#            [-d dname]   = location of RCS archive directory
#            [-u [rev]]   = the optional revision to be unchecked out
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
set revision ""
set same 0

#
# Define Usage output
#
proc Usage {} {
   upvar #0 lfile fhand
   upvar #0 argv0 comstring
   puts $fhand "\nUsage: $comstring -f fname -d dname \[-u \[rev\]\] \[-o logfile\]\n"
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
   } elseif {[lindex $argv $i] == "-u"} {
      if {![regexp -- "^(-).*" [lindex $argv [expr $i + 1]]]} {
         incr i
         set revision [format " -u%s " [lindex $argv $i]]
      } else {
         set locking " -u "
      }
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
# Set default argument
#
if {$revision == ""} {
   set revision " -u "
}

#
# If necessary, set environment to indicate _some_ username
#
if {[catch {set name $env(LOGNAME)} errmsg]} {
   puts "Resetting LOGNAME anonymous"
   set env(LOGNAME) anonymous
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
# Test dir
#
if {![file isdir $dname]} {
   puts $lfile "\nThe entered archive directory, $dname, is invalid"
   puts $lfile "\nThis script must fail\n"
   exit 1
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
# Now to run RCS command
#
set comc [format {
   exec rcs -q %s %s
} $revision [file tail $fname]]

if {[catch $comc errmsg] != 0} {
   puts $lfile "Error un-checking out RCS file:\n$errmsg\n\nThis script must fail"
   exit 1
}


puts $lfile "\nElement uncheckout successful\n"
exit 0
