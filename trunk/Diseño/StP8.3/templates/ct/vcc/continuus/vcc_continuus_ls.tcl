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
# vcc_continuus_ls -s servername -db dbasename -dd datadir -pr projname 
#                  -pv projver [-f filename] [-l | -f frmt]
#                 [-m] [-o lfile] 
#
# Purpose:
#          List the contents of a directory object version a work area.
#          (The above sentence was taken from the Continuus Command
#           Reference, CR-P-041-010, page 120)
#
# Arguments:
#            -s servername    = Name of server
#            -dd datadir      = Name of data directory of database
#            -db dbasename    = Name of database 
#            -pr projname     = Name of project, plus potential subpath 
#            -pv projver      = Current version of project
#            -f filename      = Name of file (defaults to directory listing)
#            -l | -f frmt     = Return either a long or specifically 
#                               formatted listing
#            -m               = List files not under CCM control also
#            -o lfile         = Alternate logfile (stdout is default)
#
# Returns:
#          0 for Successful completion
#          1 Otherwise
#

#
# Define Usage output
#
proc Usage {} {
   upvar #0 lfile fhand
   upvar #0 argv0 comstring
   set tstr ""
   puts $fhand [append tstr "\nUsage: $comstring -s servername -db dbasename " \
                            "-dd datadir -pr projname -pv projver " \
                            "-f filename " \[-m\] \[-l \| -f format_string\] " \
                            "\[-o lfile\]\n"]
}

#
# Define procedure to handle server shut-down
#
proc Stopper {} {
   upvar #0 lfile fhand
   if { [catch {exec ccm stop} errmsg]} {
      puts $lfile "Error messages from the stop command:\n"
      puts $lfile "$errmsg"
      return 1
   } else {
#DEBUG      puts $fhand "Stopper: success"
      return 0
   }
}

#
# Initialize variables
#
global done 0
set lfile stdout
set holder stdout
set servername ""
set dbasename ""
set datadir ""
set projname ""
set projver ""
set filename ""
set password ""
set xtrargs ""
set frmt ""

#
# Grab command line args
#
for {set i 0} {$i < $argc} {incr i} {
   if {[lindex $argv $i] == "-s"} {
      incr i
      set servername [lindex $argv $i]
#DEBUG      puts $lfile "Setting servername to $servername"
   } elseif {[lindex $argv $i] == "-db"} {
      incr i
      set dbasename [lindex $argv $i]
#DEBUG      puts $lfile "Setting dbasename to $dbasename"
   } elseif {[lindex $argv $i] == "-dd"} {
      incr i
      set datadir [lindex $argv $i]
#DEBUG      puts $lfile "Setting datadir to $datadir"
   } elseif {[lindex $argv $i] == "-pr"} {
      incr i
      set projname [lindex $argv $i]
#DEBUG      puts $lfile "Setting projname to $projname"
   } elseif {[lindex $argv $i] == "-pv"} {
      incr i
      set projver [lindex $argv $i]
#DEBUG      puts $lfile "Setting projver to $projver"
   } elseif {[lindex $argv $i] == "-f"} {
      incr i
      set filename [lindex $argv $i]
#DEBUG      puts $lfile "Setting filename to $filename"
   } elseif {[lindex $argv $i] == "-m"} {
      set xtrargs " -m "
#DEBUG      puts $lfile "Setting xtrargs to $xtrargs"
   } elseif {[lindex $argv $i] == "-l"} {
      if {$frmt != ""} {
         puts $lfile "Error: The -l and -f options are mutually exclusive."
         Usage
         exit
      } else {
         set frmt " -l "
#DEBUG         puts $lfile "Setting frmt to $frmt"
      }
   } elseif {[lindex $argv $i] == "-fm"} {
      incr i
      if {$frmt != ""} {
         puts $lfile "Error: The -l and -f options are mutually exclusive."
         Usage
         exit
      } else {
         set frmt " -f [lindex $argv $i]"
#DEBUG         puts $lfile "Setting frmt to $frmt"
      }
   } elseif {[lindex $argv $i] == "-o"} {
      incr i
      set holder [lindex $argv $i]
#DEBUG      puts $lfile "Setting logfile to $holder"
   } else {
      puts $lfile "\nERROR: Unknown option [lindex $argv $i]\n"
      Usage
      exit 1
   }
}

#
# If an alternate logfile was entered, then attempt to open it.
#
if {$holder != "stdout"} {
#DEBUG   puts $lfile "Attempting to open alternate logfile"
   set lfile UNINITIALIZED
   catch { set lfile [open $holder a] } errmsg
   if {$lfile == "UNINITIALIZED"} {
      set lfile stdout
      puts $lfile "Error opening logfile $holder, defaulting to stdout"
   } else {
#DEBUG      puts $lfile "Success opening alternate logfile $holder"
   }
} else {
#DEBUG   puts $lfile "Using standard output"
}

# Get value of CCM_PASSWORD.  If it doesn't exist, error.
if {[info exists env(CCM_PASSWORD)]} {
    set password $env(CCM_PASSWORD)
# DEBUG    puts $lfile "Setting password to $password"
} else {
    puts $lfile "\nERROR: CCM_PASSWORD environment variable not set\n"
    exit 1
}

#
# Exclude bad calls
#
if {($argc < 10) || ($servername == "") || ($dbasename == "") || \
    ($datadir == "") || ($projname == "") || \
    ($projver == "") || ($password == "")} {
   puts $lfile "\nError: incorrect or insufficient parameters"
   Usage
   exit 1
} else {
#DEBUG   puts $lfile "Argument check passed"
}

#
# Spawn the database start-up command, grabbing setting returned
#
if {[catch {exec ccm start -m -q -nogui -pw $password \
      -d \\\\${servername}\\${datadir}\\${dbasename}} errmsg]} {
   puts $lfile "Error in start command:"
   puts $lfile "$errmsg"
   puts $lfile "This command must fail"
   exit 1
} else {
   set env(CCM_ADDR) $errmsg
#DEBUG   puts $lfile "Start successful: Setting CCM_ADDR to $env(CCM_ADDR)"
}

#
# If necessary, separate the project name from the subpath
#
set tval [string first \\ $projname]
if {$tval == 0} {
   set projname [string range $projname 1 end]
   set tval [string first \\ $projname]
}
if {$tval != -1} {
   set dname [string range $projname [expr $tval + 1] end]
   set projname [string range $projname 0 [expr $tval - 1]]   
} else {
   set dname -1
}
#DEBUGputs $lfile "After parsing project name for subdirectories:"
#DEBUGputs $lfile "projname came out as $projname"
#DEBUGputs $lfile "dname came out as $dname"

#
# Determine the work_area of the project-version
#
if { [catch {exec ccm attr -show wa_path -p ${projname}-${projver}} errmsg]} {
   puts $lfile "Project-Version ${projname}-${projver} does not exist\n"
   puts $lfile "\nThis script must fail\n"
   Stopper
   exit 1
} else {
   if {[file isdirectory $errmsg]} {
      set tstr ""
#DEBUG      puts $lfile [append tstr "Changing to Work Area directory of project " \
                          "$projname-$projver"]
      cd $errmsg/$projname
#DEBUG      puts $lfile "We are now in [pwd]"
   } else {
#DEBUG      puts $lfile "Work Area not found, syncing this project..."
      if {[catch {exec ccm sync -p ${projname}-${projver}} errmsg2]} {
         puts $lfile "Error syncing a defined project:\n${errmsg2}"
         puts $lfile "\nThis script must fail\n"
         Stopper
         exit 1
      } else {
#DEBUG         puts $lfile "Success syncing a defined project:\n${errmsg2}"
#DEBUG         puts $lfile "Changing to Work Area directory of project"
         cd $errmsg/$projname
#DEBUG         puts $lfile "We are now in [pwd]"
      }
   }
}

#
# Now we traverse to the proper subdirectory
#
if {$dname != -1} {
#DEBUG   puts "Subdir case.  Changing dirs..."
   cd $dname
#DEBUG   puts "We are now in directory [pwd]"
}

#
# Confirm that source file exists
#
if {$filename != ""} {
   set filename [file tail $filename]
   if {![file exist $filename]} {
      puts $lfile "\nError: Source File\n$filename\nis not found in project.\n"
      puts $lfile "\nThis script must fail\n"
      Stopper
      exit 1
   } else {
#DEBUG      puts $lfile "File $filename is confirmed to exist in project."
   }
} else {
   set filename "."
}

#
# Now to ls the new element
#
if {[catch {eval exec ccm ls $frmt $xtrargs $filename} errmsg]} {
   puts $lfile "Error listing element"
   puts $lfile "$errmsg"
   puts $lfile "\nThis script must fail\n"
   Stopper
   exit 1
} else {
#DEBUG   puts $lfile "Success listing element"
   puts $lfile "LISTING FILES IN $projname-$projver:\n"
   puts $lfile "$errmsg"
}

#
# System Shut-Down
#
Stopper

puts $lfile "\nElement Listing successful\n"

exit 0
