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
# vcc_clearcase_checkout -v VOB_tag -w view_tag -f fname [-s state] [-r version | -b branch] [-c comment] [-o lfile] 
#
# Purpose:
#          Check out a file 
#
# Arguments:
#            -v VOB_tag    = VOB_tag of the VOB in which we are creating the element
#            -w view_tag   = view extended pathname == view_tag + pathname into view 
#	     -f fname      = Full pathname of destination == filename + where it will go
#            [-s state]    = State of checkout.  May be "reserved" or "unreserved", default is unreserved
#            [-r version | = Version to be checked out (for unreserved checkouts), default is newest version
#             -b branch]   = branch-pname = Branch from which to checkout specified version (or newest version by default)
#            [-c comment]  = comment for operation, default is no comment (-nc)
#            [-o lfile]    = Alternate logfile, default is stdout
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
set comment ""
set state "-unreserved"
set version ""
set branch ""
set VOB_tag ""
set view_tag ""
set fname ""

#
# Define Usage output
#
proc Usage {} {
   upvar #0 lfile fhand
   upvar #0 argv0 comstring
   puts $fhand "\nUsage: $comstring -v VOB_tag -w view_tag -f fname \[-s state\]  \[-r revision | -b branch \] \[-c comment\] \[-o logfile\]\n"
}

#
# Grab command line args
#
puts $lfile "\n"
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
   } elseif {[lindex $argv $i] == "-c"} {
      incr i
      set comment [lindex $argv $i] 
   } elseif {[lindex $argv $i] == "-s"} {
      incr i
      set state [format "-%s" [lindex $argv $i]]
   } elseif {[lindex $argv $i] == "-r"} {
      incr i
      if {$version != ""} {puts "\nBoth version and branch cannot be set.\n"; Usage; exit} else {set version [lindex $argv $i]}
   } elseif {[lindex $argv $i] == "-b"} {
      incr i
      if {$version != ""} {puts "\nBoth version and branch cannot be set.\n"; Usage; exit} else {set version [format "-bra %s" [lindex $argv $i]]}
   } elseif {[lindex $argv $i] == "-o"} {
      incr i
      set holder [lindex $argv $i]
   } else {
      puts "\nERROR: Unknown option [lindex $argv $i]\n"
      puts "This script must fail\n"
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
if {($argc < 6) || ($VOB_tag == "") || ($view_tag == "") || ($fname == "")} {
   $lfile "\nError: incorrect or insufficient parameters"
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
if {![file isdirectory [file dirname $fname]]} {
   puts $lfile "Cannot find desired target directory: [file dirname $fname]"
   puts $lfile "This script must fail"
   exit 1
}

#
# Check to see if the VOB is mounted and the view accessable
#
set systemview [file nativename M:/${view_tag}/${VOB_tag}]
if {![file isdirectory $systemview ] } {
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
      puts $lfile "\nThe entered directory, $dname, is invalid"
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
# Now we reset the version into the appropriate format
#
if {$version != ""} {
   #
   # Here we have to deal with TCL and back-slashes
   #
   set outstr ""
   set curpos [string first \\ $version]
   while {$curpos != -1} {
      append outstr [string range $version 0 $curpos] \\
      set version [string range $version [expr $curpos + 1] end]
      set curpos [string first \\ $version]
   }
   append outstr $version
   set version $outstr
   #
   if {![regexp {(-bra).*} $version]} {
      set version [format "-ver %s@@%s" [file tail $fname] $outstr]
   } else {
      set version [format "%s %s" $version [file tail $fname]]
   }
} else {
   #Note: version holds at least the filename, for no other reason than to allow
   #the command line below to be used consistently
   set version [file tail $fname]
}


#
# Finally, check this element out...
#
if {$comment == ""} {
   if {[catch "exec cleartool checkout $state -nc $version" errmsg]} {
      if {![regexp {.*(is different from version selected by view).*} $errmsg]} {
         puts $lfile "checkout failed with error:"
         puts $lfile "$errmsg"
         puts $lfile "\nThis script must fail"
         exit 1
      }
   }
} else {
   if {[catch "exec cleartool checkout $state -c \"[subst "$comment"]\" $version" errmsg]} {
      if {![regexp {.*(is different from version selected by view).*} $errmsg]} {
         puts $lfile "checkout failed with error:"
         puts $lfile "$errmsg"
         puts $lfile "\nThis script must fail"
         exit 1
      }
   }
}

#
# copy dump into position
#
if {[file dirname $fname] != [pwd]} {
   if {[catch {file copy -force [file tail $fname] [file dirname $fname]} errmsg]} {
      puts $lfile "File copy failed with error:\n$errmsg\n\nThis script must fail"
      exit 1
   }
} else {
      puts "\nElement name without directory specified\nElement checked out in view"
}
puts $lfile "\nElement checkout successful\n"

exit 0
