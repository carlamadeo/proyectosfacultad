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
# vcc_rcs_checkout -f fname [-d dname] [-r [rev]] [-l [rev]] [-fo [rev]] [-o lfile] 
#
# Purpose:
#          Check out from RCS a system dump
#
# Arguments:
#            -f fname     = FQN of file to be checked out (evantual source location
#                           and filename)
#            [-d dname]   = location of RCS directory
#            [-r [rev]]   = retrieves the latest revision whose number is less 
#                           than or equal to [dev], or the latest by default
#            [-l [rev]]   = same as -r except that the revision is locked
#            [-fo [rev]]  = same as -r, forcing the over-writing of working files
#            [-o lfile]   = Alternate logfile, default is stdout
#
# Returns:
#          0 for Successful completion
#          1 Otherwise
#
# Note: Spaces between flags and values are _required_
#
# Note: While multiple uses of the [rev] causes co itself emit a warning, 
#       this script will use the first [rev] encountered on the command line 
#       and ignore the rest
#

#
# Initialize variables
#
set lfile stdout
set holder stdout
set fname ""
set dname ""
set revnum ""
set revision ""
set locking ""
set force ""
set same 0

#
# Define Usage output
#
proc Usage {} {
   upvar #0 lfile fhand
   upvar #0 argv0 comstring
   puts $fhand "\nUsage: $comstring -f fname \[-d dname\] \[-l \[rev\]\] \[-r \[dev\]\] \[-fo \[rev\]\] \[-o logfile\]\n"
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
   } elseif {[lindex $argv $i] == "-l"} {
      if {![regexp -- "^(-).*" [lindex $argv [expr $i + 1]]]} {
         incr i
         if {$revnum == ""} {
            set revnum [lindex $argv $i]
            set locking [format " -l%s " $revnum]
         } else {
            set locking " -l "
         }
      } else {
         set locking " -l "
      }
   } elseif {[lindex $argv $i] == "-fo"} {
      if {![regexp -- "^(-).*" [lindex $argv [expr $i + 1]]]} {
         incr i
         if {$revnum == ""} {
            set revnum [lindex $argv $i]
            set force [format " -f%s " $revnum]
         } else {
            set force " -f "
         }
      } else {
         set force " -f "
      }
   } elseif {[lindex $argv $i] == "-r"} {
      if {![regexp -- "^(-).*" [lindex $argv [expr $i + 1]]]} {
         incr i
         if {$revnum == ""} {
            set revnum [lindex $argv $i]
            set revision [format " -r%s " $revnum]
         } else {
            set revision " -r "
         }
      } else {
         set revision " -r "
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
   exec co -q %s %s %s %s
} $revision $locking $force [file tail $fname]]
if {[catch $comc errmsg] != 0} {
   puts $lfile "Error checking out RCS file:\n$errmsg\n\nThis script must fail"
   exit 1
}

#
# Now to move the file into place
#
if {!$same} {
   if {[catch {file rename [file tail $fname] [file dirname $fname]} errmsg]} {
      puts $lfile "\nfile move error:\n$errmsg"
      puts $lfile "\nThis script must fail\n"
      exit 1
   } else {
      puts $lfile "\nFile move successful"
   }
}

puts $lfile "\nElement checkout successful\n"
exit 0
