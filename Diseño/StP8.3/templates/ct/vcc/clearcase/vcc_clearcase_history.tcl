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
# vcc_clearcase_history -v VOB_tag -w view_tag -f fname [[-g] [-nop] | [-m[long|short|fmt:<fmt>]] [-eve] ] [-min] [-nco] [-sin <date-time>] [-use <login-name>] [-bra <branch-type-selector>] [-o lfile]
#
# Purpose:
#          Display Clearcase history of file 
#
# Arguments:
#            -v VOB_tag   = VOB_tag of the VOB in which we are creating the element
#            -w view_tag  = view extended pathname == view_tag + pathname into view 
#	     -f fname     = name of file to examine
#            -g graphical = switch to make response graphical, default is non-graphical
#            -c comment   = comment for operation
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
set comment ""
set fname ""
set graphical ""
set since ""
set branch ""
set user ""
set tcomstr ""
#Note: the two vars below are used to track whether we are running in
#graphical mode.  They should be modified with care!
set comstr ""
set gcomstr ""
set VOB_tag ""
set view_tag ""

#
# Define Usage output
#
proc Usage {} {
   upvar #0 lfile fhand
   upvar #0 argv0 comstring
   puts $fhand "\nUsage: $comstring -v VOB_tag -w view_tag -f fname \[\[-g\] \[-nop\] \| \[-m\[long\|short\|fmt:\<fmt\>\]\] \[-eve\] \] \[-min\] \[-nco\] \[-sin \<date-time\>\] \[-use \<login-name\>\] \[-bra \<branch-type-selector\>\] \[-o logfile\]\n"
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
   } elseif {[lindex $argv $i] == "-g"} {
      if {$comstr != ""} {
         puts $lfile "\nError: Mixed graphical and non-graphical arguments."
         Usage
         exit 1
      } else {
         set gcomstr [format " -g %s" $gcomstr]
      }
   } elseif {[lindex $argv $i] == "-s"} {
      incr i
      set tcomstr [format "-sin %s %s" [lindex $argv $i] $tcomstr]
   } elseif {[lindex $argv $i] == "-b"} {
      incr i
      set tcomstr [format "-bra %s %s" [lindex $argv $i] $tcomstr]
   } elseif {[lindex $argv $i] == "-u"} {
      incr i
      set tcomstr [format "-use %s %s" [lindex $argv $i] $tcomstr]
   } elseif {[lindex $argv $i] == "-nop"} {
      if {$comstr != ""} { 
         puts $lfile "\nError: Mixed graphical and non-graphical arguments."
         Usage
         exit 1
      } else {
         set gcomstr [format "%s %s" $gcomstr "-nop"]
      }
   } elseif {[lindex $argv $i] == "-m"} {
      incr i
      set tstr [lindex $argv $i]
      if {$gcomstr != ""} {
         puts $lfile "\nError: Mixed graphical and non-graphical arguments."
         Usage
         exit 1
      } else {
         if {$tstr == "long"} {
            if {([string first "-s" $comstr]==0)|([string first "-fmt" $comstr]==0)} {
               puts $lfile "\nError: multiple format specifications on command line"
               Usage
               exit 1
            } else {
               set comstr [format " -%s %s" $tstr $comstr]
            }
         } elseif {$tstr == "short"} {
            if {([string first "-l" $comstr]!=-1)|([string first "-fmt" $comstr]==0)} {
               puts $lfile "\nError: multiple format specifications on command line"
               Usage
               exit 1
            } else {
               set comstr [format "-%s %s" $tstr $comstr]
            }
         } elseif {[string first fmt $tstr] != -1} {
            if {([string first "-s" $comstr]==0)|([string first "-l" $comstr]==0)} {
               puts $lfile "\nError: multiple format specifications on command line"
               Usage
               exit 1
            } else {
               set comstr [format "-fmt %s %s" [string range $tstr [expr [string first : $tstr] + 1] end] $comstr] 
            }
         } else {
            puts $lfile "\nError: unrecognized format(-m) parameter"
            Usage
            exit 1
         }
      }
   } elseif {[lindex $argv $i] == "-eve"} {
      if {$gcomstr != ""} {
         puts $lfile "\nError: Mixed graphical and non-graphical arguments."
         Usage
         exit 1
      } else {
         set comstr [format "%s -eventid" $comstr]
      }
   } elseif {[lindex $argv $i] == "-o"} {
      incr i
      set holder [lindex $argv $i]
   } elseif {[lindex $argv $i] == "-min"} {
      set tcomstr [format " -min %s" $tcomstr]
   } elseif {[lindex $argv $i] == "-nco"} {
      set tcomstr [format " -nco %s" $tcomstr]
   } else {
      puts "\nERROR: Unknown option [lindex $argv $i]\n"
      Usage
      exit 1
   }
}

#
# Exclude bad calls
#
if {($argc < 6) || ($VOB_tag == "") || ($view_tag == "") || ($fname == "")} {
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
   puts $lfile "\nCould not change directories into view"
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
      puts $lfile "\nCannot read file: $fname"
      puts $lfile "\nThis script must fail"
      exit 1
   }
}
    
#
# Make lshistory call
#
if {$gcomstr == ""} {
   set tstr [string trim [format "%s %s %s" $comstr $tcomstr $fname] \ ]
   if {[catch "exec cleartool lshistory $tstr" errmsg]} {
      puts $lfile "lshistory failed with error:"
      puts $lfile "$errmsg"
      puts $lfile "\nThis script must fail"
      exit 1
   } else {
      puts $lfile "\nHISTORY OUTPUT:\n===============\n$errmsg"
   }
} else {
   set tstr [string trim [format "%s %s" $tcomstr $fname] \ ]
   if {[catch "exec cleartool lshistory $tstr" errmsg]} {
      puts $lfile "lshistory failed with error:\n$errmsg\n\nThis script must fail"
      exit 1
   } else {
      set tstr [string trim [format "%s %s %s" $gcomstr $tcomstr $fname] \ ]
      if {[catch "exec cleartool lshistory $tstr &" errmsg]} {
         puts $lfile "Background launch failed"
      }
   }
}

puts $lfile "\nElement history listing successful\n"

exit 0



