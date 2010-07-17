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
# vcc_clearcase_ls -v VOB_tag -w view_tag [-f fname] [-r|-d] [-l|-s] [-vob|-vie] [-nxn] [-vis] 
#
# Purpose:
#          List ClearCase objects
#
# Arguments:
#            -v VOB_tag   = VOB_tag of the VOB in which we are creating the element
#            -w view_tag  = view extended pathname == view_tag + pathname into view 
#	     -f fname     = name of file to examine
#            -r           = recurse subtrees
#            -d           = list information of the directory itself, rather than its contents
#            -l           = long listing
#            -s           = short listing
#            -nxn         = List simple pathnames (instead of version extended names)
#            -vob         = restricts the listing to objects in VOB only
#            -vie         = restricts the listing to view objects only
#            -vis         = restricts the listing to objects visible to the standard dir command
#            -o lfile     = Alternate logfile (stdout is default)
#
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
set fname ""
set VOB_tag ""
set view_tag ""
set depth ""
set frmt ""
set vlevel ""
set comstr ""

#
# Define Usage output
#
proc Usage {} {
   upvar #0 lfile fhand
   upvar #0 argv0 comstring
   puts $fhand "\nUsage: $comstring -v VOB_tag -w view_tag -f fname \[-r\|-d\] \[-l\|-s\] \[-vob\|-vie\] \[-nxn\] \[-vis\] \n"
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
   } elseif {[lindex $argv $i] == "-o"} {
      incr i
      set holder [lindex $argv $i]
   } elseif {[lindex $argv $i] == "-r"} {
      if {$depth != ""} { 
         puts $lfile "\nError: Incompatible arguments."
         Usage
         exit 1
      } else {
         set depth -r
      }
   } elseif {[lindex $argv $i] == "-d"} {
      if {$depth != ""} { 
         puts $lfile "\nError: Incompatible arguments."
         Usage
         exit 1
      } else {
         set depth -d
      }
   } elseif {[lindex $argv $i] == "-s"} {
      if {$frmt != ""} { 
         puts $lfile "\nError: Incompatible arguments."
         Usage
         exit 1
      } else {
         set frmt -s
      }
   } elseif {[lindex $argv $i] == "-l"} {
      if {$frmt != ""} { 
         puts $lfile "\nError: Incompatible arguments."
         Usage
         exit 1
      } else {
         set frmt -l
      }
   } elseif {[lindex $argv $i] == "-vob"} {
      if {$vlevel != ""} { 
         puts $lfile "\nError: Incompatible arguments."
         Usage
         exit 1
      } else {
         set vlevel -vob
      }
   } elseif {[lindex $argv $i] == "-vie"} {
      if {$vlevel != ""} { 
         puts $lfile "\nError: Incompatible arguments."
         Usage
         exit 1
      } else {
         set vlevel -vie
      }
   } elseif {[lindex $argv $i] == "-nxn"} {
      set comstr [format "%s %s" -nxn $comstr]
   } elseif {[lindex $argv $i] == "-vis"} {
      set comstr [format "%s %s" $comstr -vis]
   } else {
      puts "\nERROR: Unknown option [lindex $argv $i]\n"
      Usage
      exit 1
   }
}

#
# Exclude bad calls
#
if {($argc < 4) || ($VOB_tag == "") || ($view_tag == "")} {
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
      puts "Defaulting back to standard output"
      set lfile stdout
   }
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
# Check to see if the VOB is mounted and the view accessable
#
set systemview [file nativename M:/${view_tag}/${VOB_tag}]
if {![file isdirectory $systemview] } {
   # Try to mount the VOB
   if { [catch {exec cleartool mount $VOB_tag} errmsg] } {
      if {![regexp {.*(already mounted.)$} $errmsg]} {
         puts $lfile "\nMount Failed.\n"
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
# Check for file, removing directory if necessary
#
if {![file readable [file nativename ./$fname]]} {
   set fname [file tail $fname]
   if {![file readable $fname]} {
      puts $lfile "Cannot see file in view: $fname"
      puts $lfile "This script must fail"
      exit 1
   }
}
    
#
# Make ls call
#
# ECR 3488 if {[catch "exec cleartool ls $depth $frmt $vlevel $comstr $fname" errmsg]} {
if {[catch {exec cleartool ls $depth $frmt $vlevel $comstr $fname} errmsg]} {
   puts $lfile "ls failed with error:\n$errmsg\n\nThis script must fail"
   exit 1
} else {
   puts $lfile "\nLISTING OUTPUT:\n===============\n$errmsg"
}

puts $lfile "\nElement listing successful\n"

exit 0



