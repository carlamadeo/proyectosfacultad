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
# vcc_clearcase_create -v VOB_tag -w view_tag -f fname -t eltype [-p policy] [-c comment] [-o lfile] 
#
# Purpose:
#          Create an initial version of a system-dump 
#
# Arguments:
#            -v VOB_tag  = VOB_tag of the VOB in which we are creating the element
#            -w view_tag = view extended pathname == view_tag + pathname into view 
#	     -f fname    = Full pathname of file to create
#            -t eltype   = Element type to be created
#            -p policy   = Policy determining whether view-private files are saved, default is "keep"
#            -c comment  = comment for operation
#            -o lfile    = Alternate logfile (stdout is default)
#
# Returns:
#          0 for Successful completion
#          1 Otherwise
#

#
# Initialize variables
#
set lfile stdout
set holder stdout
set policy ""
set comment ""
set fname ""
set VOB_tag ""
set view_tag ""
set ftype  ""

#
# Define Usage output
#
proc Usage {} {
   upvar #0 lfile fhand
   upvar #0 argv0 comstring
   puts $fhand "\nUsage: $comstring -v VOB_tag -w view_tag -f fname  -t eltype \[-p policy\] \[-c comment\] \[-o logfile\]\n"
}

#
# Grab command line args
#
for {set i 0} {$i < $argc} {incr i} {
   if {[lindex $argv $i] == "-v"} {
      incr i
      set VOB_tag [lindex $argv $i]
      if {[string index $VOB_tag 0] != "\\"} {set VOB_tag [format "\\%s" $VOB_tag]}
    } elseif {[lindex $argv $i] == "-w"} {
      incr i
      set view_tag [lindex $argv $i]
   } elseif {[lindex $argv $i] == "-f"} {
      incr i
      set fname [lindex $argv $i]
   } elseif {[lindex $argv $i] == "-p"} {
      incr i
      set policy [format "-%s" [lindex $argv $i] ]
   } elseif {[lindex $argv $i] == "-c"} {
      incr i
      set comment [lindex $argv $i] 
   } elseif {[lindex $argv $i] == "-t"} {
      incr i
      set ftype [lindex $argv $i]
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
# If an alternate logfile was entered, then attempt to open it.
#
if {$holder != "stdout"} {
   set lfile UNINITIALIZED
   catch { set lfile [open $holder a] } errmsg
   if {$lfile == "UNINITIALIZED"} {
      puts "Opening logfile $holder failed"
      puts "Defaulting back to standard output"
      set lfile stdout
   }
}

#
# Exclude bad calls
#
if {($argc < 8) || ($ftype == "") || ($VOB_tag == "") || ($view_tag == "") || ($fname == "")} {
   puts $lfile "\nError: incorrect or insufficient parameters"
   Usage
   exit 1
}

#
# If necessary, separate the view_tag from the extended path
#
set tval [string first \\ $view_tag]
if {$tval == 0} {
   set view_tag [string range $view_tag 1 end]
   set tval [string first \\ $view_tag]
}
if {$tval != -1} {
   set dname [string range $view_tag [expr $tval + 1] end]
   set view_tag [string range $view_tag 0 [expr $tval - 1]]   
} else {
   set dname -1
}

#
# Check entered value
#
if {![file readable $fname]} {
   puts $lfile "Cannot read file: $fname"
   puts $lfile "This script must fail"
   exit 1
}

#
# Confirm policy setting
#
if {$policy == ""} {
   set policy "-keep"
}

#
# Check to see if the VOB is mounted and the view accessable
#
set systemview [file nativename M:/${view_tag}/${VOB_tag}]
if {![file isdirectory $systemview] } {
   # Try to mount the VOB
   if { [catch {exec cleartool mount $VOB_tag} errmsg] } {
      if {![regexp {.*(already mounted.)$} $errmsg]} {
         puts $lfile "\nMounting VOB Failed.\n"
         puts $lfile "Error Message is: $errmsg\n"
         exit 1
      }
   }
   # Try to start the view 
   if { [catch {exec cleartool startview $view_tag} errmsg] } {
      puts $lfile "\nView Setting Failed.\n"
      puts $lfile "Error Message is: $errmsg\n"
      exit 1
   }
}

#
# Now the view should be mounted, so go there
#
if {[catch {cd [file nativename M:/${view_tag}/${VOB_tag}]} errmsg]} {
   puts $lfile "Could not change directories into view"
   puts $lfile "\nThis script must fail\n"
   exit 1
}

#
# Now we must check out the current directory
#
if { [catch {exec cleartool checkout -nc [file nativename [pwd]]} errmsg] } {
   if {![regexp {.*(already checked out).*} $errmsg]} {
      puts $lfile "\nCheckout Failed.\n"
      puts $lfile "Error Message is: $errmsg\n"
      exit 1
   }
}

#
# Test subdir and move there if possible
#
if {$dname != -1} {
   if {![file isdir $dname]} {
      puts $lfile "\nThe entered view subdirectory, $dname, is invalid"
      puts $lfile "\nThis script must fail\n"
      exit 1
   } else {
      if {[catch {cd $dname} errmsg]} {
         puts $lfile "\nCannot change to view subdir $dname."
         puts $lfile "\nThis script must fail\n"
         exit 1
      }
   }
}

#
# In order to use the auto-checkin feature, copy file to view
#
if {[catch {file copy -force $fname [file nativename [pwd]]} errmsg]} {
   puts $lfile "\nFile is Read-Only in view directory."
   puts $lfile "\nThis should indicate that this file is already created"
   exit 1
}
      
#
# Create Appropriate element type for this checkin
#
if {$comment == ""} {
   if {[catch {exec cleartool mkelem -eltype  $ftype -ci -nc [file tail $fname]} errmsg]} {
      puts $lfile "mkelem failed with error:\n$errmsg\n\nThis script must fail"
      exit 1
   }
} else {
   if {[catch {exec cleartool mkelem -eltype $ftype -ci -c [subst "$comment"] [file tail $fname]} errmsg]} {
      puts $lfile "mkelem failed with error:\n$errmsg\n\nThis script must fail"
      exit 1
   }
}

puts $lfile "\nElement creation successful\n"

exit 0
