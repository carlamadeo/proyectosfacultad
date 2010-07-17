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
# vcc_rcs_checkin -f fname [-d dname] [-r rev] [-l [rev] | -u [rev]] [-c comment] [-o lfile] 
#
# Purpose:
#          Checkin an RCS backup of a system dump
#
# Arguments:
#            -f fname     = FQN of file to be checked in (source location and 
#                           filename)
#            [-d dname]   = directory to check the file into
#            [-r rev]     = Specifies both the revision number to be checked in and
#            [-l [rev] |    whether or not a working version is checked back out,
#             -u [rev]]     and whether it is locked or not.
#                           Note: -r without a [rev] is not allowed.
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
set comment ""
set locking ""
set revision ""
set revnum ""
set same 0

#
# Define Usage output
#
proc Usage {} {
   upvar #0 lfile fhand
   upvar #0 argv0 comstring
   puts $fhand "\nUsage: $comstring -f fname \[-d dname\] \[-r rev\] \[-l \[rev\] \| -u \[rev\]\] \[-o logfile\] \[-c comment\]\n"
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
   } elseif {[lindex $argv $i] == "-r"} {
      if {[regexp -- "^(-).*" [lindex $argv [expr $i + 1]]]} {
         puts "\nERROR: -r flag requires a revision to follow"
         Usage
         exit 1
      } else {
         incr i
         #Exclude the case of -r being the last argument
         if {[lindex $argv $i] != ""} {
            set revision [format " -r%s " [lindex $argv $i]]
         } else {
            puts "\nERROR: -r flag requires a revision to follow"
            Usage
            exit 1
         }
      }
   } elseif {[lindex $argv $i] == "-l"} {
      if {$locking != "" } {
         puts "\nERROR: multiple locking policies used on command line"
         Usage
         exit 1
      } else {
         if {![regexp -- "^(-).*" [lindex $argv [expr $i + 1]]]} {
            incr i
            set locking [format " -l%s " [lindex $argv $i]]
         } else {
            set locking " -l "
         }
      }
   } elseif {[lindex $argv $i] == "-u"} {
      if {$locking != "" } {
         puts "\nERROR: multiple locking policies used on command line"
         Usage
         exit 1
      } else {
         if {![regexp -- "^(-).*" [lindex $argv [expr $i + 1]]]} {
            incr i
            set locking [format " -u%s " [lindex $argv $i]]
         } else {
            set locking " -u "
         }
      }
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
# Check for existing RCS directory, exit if it is not found
#
if {![file isdir $dname/RCS]} {
   puts $lfile "\nWARNING: No RCS directory found.  Element never created."
   if {[catch {file mkdir [file nativename $dname/RCS]} errmsg]} {
      puts $lfile "\nmkdir error:\n\n$errmsg"
   }
}

#
# Now to move the file into place
#
if {!$same} {
   if {[catch {file rename $fname $dname} errmsg]} {
      puts $lfile "\nfile move error:\n$errmsg"
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
# Now to check in this version
#
if {[catch {set inpipe [open "| ci -q -I -f $locking $revision [file tail $fname]" w]} errmsg]} {
   puts $lfile "Error checking in file:\n$errmsg\n\nThis script must fail"
} else {
   if {$comment != ""} {
      puts $inpipe $comment
   }
   puts $inpipe ".\n"
   flush $inpipe
   catch close $inpipe
   puts $lfile "\nSuccess checking in the RCS file"
}

#
# Now, if policy directed us retain a working copy, 
# we move this working file into place
#
if {!$same} {
   if {$policy != ""} {
      if {[catch {file rename [file tail $fname] [file dirname $fname]} errmsg]} {
         puts $lfile "\nfile move error:\n$errmsg"
         puts $lfile "\nThis script must fail\n"
      }
   }
}

puts $lfile "\nElement creation successful\n"
exit 0
