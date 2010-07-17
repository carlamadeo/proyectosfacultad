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
# vcc_continuus_create -s servername -dd datadir -db dbasename -pr projname
#                      -pv projver -f filename -t eltype [-c comment] [-o lfile]
#
# Purpose:
#          Create an initial version of a system-dump 
#          If the indicated project is not checked out then this
#          script will check it out (remains to be determined how I will 
#          know this)
#
# Arguments:
#            -s servername = Name of server
#            -dd datadir   = Name of data directory of database
#            -db dbasename = Name of database 
#            -pr projname  = project to create in, plus potential subpath
#            -pv projver   = project version number
#            -f filename   = FQN of file to create in system
#            -t eltype     = The file type of the new element
#            -c comment    = comment for action
#            -o lfile      = Alternate logfile (stdout is default)
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
   puts $fhand [append tstr "\nUsage: $comstring -s servername -dd datadir " \
                            "-db dbasename -pr projname -pv projver " \
                            "-f filename -t eltype " \[-c comment\] " \
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
# DEBUG    puts $fhand "Stopper: success"
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
set datadir ""
set dbasename ""
set password ""
set projname ""
set projver ""
set filename ""
set eltype ""
set comment ""
set version ""

#
# Grab command line args
#
for {set i 0} {$i < $argc} {incr i} {
   if {[lindex $argv $i] == "-s"} {
      incr i
      set servername [lindex $argv $i]
# DEBUG      puts $lfile "Setting servername to $servername"
   } elseif {[lindex $argv $i] == "-dd"} {
      incr i
      set datadir [lindex $argv $i]
# DEBUG      puts $lfile "Setting datadir to $datadir"
   } elseif {[lindex $argv $i] == "-db"} {
      incr i
      set dbasename [lindex $argv $i]
# DEBUG      puts $lfile "Setting dbasename to $dbasename"
   } elseif {[lindex $argv $i] == "-pr"} {
      incr i
      set projname [lindex $argv $i]
# DEBUG      puts $lfile "Setting projname to $projname"
   } elseif {[lindex $argv $i] == "-pv"} {
      incr i
      set projver [lindex $argv $i]
# DEBUG      puts $lfile "Setting projver to $projver"
   } elseif {[lindex $argv $i] == "-f"} {
      incr i
      set filename [lindex $argv $i]
# DEBUG      puts $lfile "Setting filename to $filename"
   } elseif {[lindex $argv $i] == "-t"} {
      incr i
      set eltype [lindex $argv $i]
# DEBUG      puts $lfile "Setting eltype to $eltype"
   } elseif {[lindex $argv $i] == "-c"} {
      incr i
      set comment [lindex $argv $i]
# DEBUG      puts $lfile "Setting comment to $comment"
   } elseif {[lindex $argv $i] == "-o"} {
      incr i
      set holder [lindex $argv $i]
# DEBUG      puts $lfile "Setting logfile to $holder"
   } else {
      puts $lfile "\nERROR: Unknown option [lindex $argv $i]\n"
      Usage
      exit 1
   }
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
# If an alternate logfile was entered, then attempt to open it.
#
if {$holder != "stdout"} {
# DEBUG   puts $lfile "Attempting to open alternate logfile"
   set lfile UNINITIALIZED
   catch { set lfile [open $holder a] } errmsg
   if {$lfile == "UNINITIALIZED"} {
      set lfile stdout
      puts $lfile "Error opening logfile $holder, defaulting to stdout"
   } else {
# DEBUG      puts $lfile "Success opening alternate logfile $holder"
   }
} else {
# DEBUG   puts $lfile "Using standard output"
}

#
# Exclude bad calls
#
if {($argc < 14) || ($servername == "") || ($datadir == "") || \
    ($dbasename == "") || ($filename == "") || ($projname == "") || \
    ($projver == "") || ($eltype == "") || ($password == "")} {
   puts $lfile "\nError: incorrect or insufficient parameters"
   Usage
   exit 1
} else {
# DEBUG   puts $lfile "Argument check passed"
}

#
# Confirm that source file exists
#
if {![file exist $filename]} {
   puts $lfile "\nError: Source File\n$filename\ndoes not exist.\n"
   puts $lfile "\nThis script must fail\n"
   exit 1
} else {
#DEBUG   puts $lfile "File $filename is confirmed to exist."
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
#DEBUG    puts $lfile "Start successful: Setting CCM_ADDR to $env(CCM_ADDR)"
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
# NOTE: $projname now contains both the entire project-version string
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
      if {[catch {exec ccm sync -p ${projname}-${projver}} errmsg2]} {
         puts $lfile "Error syncing a defined project:\n${errmsg2}"
         puts $lfile "\nThis script must fail\n"
         Stopper
         exit 1
      } else {
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
# Move new file into place
#
if {[catch {file copy $filename [pwd]} errmsg]} {
   puts $lfile "Error copying file into work area:"
   puts $lfile "$errmsg"
   puts $lfile "\nThis script must fail\n"
   Stopper
   exit 1
} else {
#DEBUG   puts $lfile "New file [file tail $filename] is in place"
}

#
# Now to create the new element
#
if {[catch {exec ccm create -t $eltype -c $comment [file tail $filename]} \
                                                                     errmsg]} {
   puts $lfile "Error creating the new element"
   puts $lfile "$errmsg"
   puts $lfile "\nThis script must fail\n"
   Stopper
   exit 1
} else {
#DEBUG   puts $lfile "Success creating the new element"
#DEBUG   puts $lfile "$errmsg"   
}

#
# System Shut-Down
#
Stopper

puts $lfile "\nElement Creation successful\n"

exit 0
