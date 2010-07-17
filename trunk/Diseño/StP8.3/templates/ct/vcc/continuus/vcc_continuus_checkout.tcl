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
# vcc_continuus_checkout -s servername -dd datadir -db dbasename -pr projname 
#                        -pv projver -f filename [-r filever] [-c comment] 
#                        [-o lfile] 
#
# Purpose:
#          Check a system-dump out of CCMS 
#
# Arguments:
#            -s servername = Name of server
#            -dd datadir   = Name of data directory of database
#            -db dbasename = Name of database 
#            -pr projname  = project to create in, plus potential subpath
#            -pr projver   = current version of project
#            -f filename   = Name of file to checkout
#            -c comment    = comment for action
#            -o lfile      = Alternate logfile (stdout is default)
#            -r filever    = Version of file to be checked out
#
# Note: The filename parameter is the fully qualified name of the
#       result of this script.  It is the name of the file to 
#       checkout appended to the path into which it should be 
#       placed.  This path can be null, i.e. the filename can just
#       be the name.  In this case the file is checked out into the
#       work area, and not copied elsewhere.
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
               "-db dbasename -pr projname -pv projver -f filename " \
               "\[-r filever] \[-c comment\] \[-o lfile\]\n"]
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
set datadir ""
set dbasename ""
set password ""
set projname ""
set projver ""
set filename ""
set filever ""
set comment ""

#
# Grab command line args
#
for {set i 0} {$i < $argc} {incr i} {
   if {[lindex $argv $i] == "-s"} {
      incr i
      set servername [lindex $argv $i]
#DEBUG      puts $lfile "Setting servername to $servername"
    } elseif {[lindex $argv $i] == "-dd"} {
      incr i
      set datadir [lindex $argv $i]
#DEBUG      puts $lfile "Setting datadir to $datadir"
    } elseif {[lindex $argv $i] == "-db"} {
      incr i
      set dbasename [lindex $argv $i]
#DEBUG      puts $lfile "Setting dbasename to $dbasename"
   } elseif {[lindex $argv $i] == "-pr"} {
      incr i
      set projname [lindex $argv $i]
#DEBUG      puts $lfile "Setting projname to $projname"
   } elseif {[lindex $argv $i] == "-pv"} {
      incr i
      set projver [lindex $argv $i]
#DEBUG      puts $lfile "Setting projname to $projname"
   } elseif {[lindex $argv $i] == "-r"} {
      incr i
      set filever [lindex $argv $i]
#DEBUG      puts $lfile "Setting filever to $filever"
   } elseif {[lindex $argv $i] == "-f"} {
      incr i
      set filename [lindex $argv $i]
#DEBUG      puts $lfile "Setting filename to $filename"
   } elseif {[lindex $argv $i] == "-c"} {
      incr i
      set comment [lindex $argv $i]
#DEBUG      puts $lfile "Setting comment to $comment"
   } elseif {[lindex $argv $i] == "-o"} {
      incr i
      set holder [lindex $argv $i]
#DEBUG      puts $lfile "Setting holder to $holder"
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
if {($argc < 12) || ($servername == "") || ($datadir == "") || \
    ($dbasename == "") || ($filename == "") || ($projname == "") || \
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
#DEBUG      puts $lfile "We are now in the work_area: [pwd]"
   } else {
      if {[catch {exec ccm sync -p ${projname}-${projver}} errmsg2]} {
         puts $lfile "Error syncing a defined project:\n${errmsg2}"
         puts $lfile "\nThis script must fail\n"
         exit 1
      } else {
#DEBUG         puts $lfile "Changing to Work Area directory of project"
         cd $errmsg/$projname
#DEBUG         puts $lfile "We are now in [pwd]"
      }
   }
}

#
# Separate eventual location from filename under CCMS control
#
set fdir [file dirname $filename]
set filename [file tail $filename]
#DEBUGputs $lfile "File directory is $fdir"
#DEBUGputs $lfile "File name is $filename"
if {![file isdirectory $fdir]} {
   puts $lfile "\nError: Destination directory:\n   $fdir\ndoes not exist.\n"
   puts $lfile "\nThis script must fail\n"
   Stopper
   exit 1
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
if {![file exist $filename]} {
   puts $lfile "\nError: Source File:\n   $filename\nis not found in project."
   puts $lfile "\nThis script must fail\n"
   Stopper
   exit 1
} else {
#DEBUG   puts $lfile "File [file tail $filename] is confirmed in work_area"
}

#
# If $filever is set, execute the ccm use command
#
if {$filever != ""} {
    if {[catch {exec ccm use $filename-$filever} errmsg]} {
        puts $lfile "Error executing ccm use for $filename-$filever"
        puts $lfile "$errmsg"
        puts $lfile "This script must fail"
        Stopper
        exit 1
    } else {
#DEBUG        puts $lfile "ccm use successful for $filename-$filever"
    }
}

#
# Now to checkout the element
#
if {$comment == ""} {
   if {[catch {exec ccm checkout $filename} errmsg]} {
      puts $lfile "Error checking out the new element"
      puts $lfile "$errmsg"
      puts $lfile "This script must fail"
      Stopper
      exit 1
   } else {
#DEBUG      puts $lfile "Checkout successful.\nResponse from command:\n$errmsg"
   }
} else {
   if {[catch {exec ccm checkout -c $comment $filename} errmsg]} {
      puts $lfile "Error checking out the new element"
      puts $lfile "$errmsg"
      puts $lfile "This script must fail"
      Stopper
      exit 1
   } else {
      puts $lfile "Checkout successful.\nResponse from command:\n$errmsg"
   }
}

#
# Now to copy this file to the desired directory
#
if {$fdir != "."} {
   if {[catch {file copy -force $filename $fdir} errmsg]} {
      puts $lfile "Error moving file($filename) into place($fdir):\n$errmsg"
   } else {
#DEBUG      puts $lfile "File copy successful.\nNew version is now in:$fdir"
   }
} else {
#DEBUG   puts $lfile "No file copy required.\nNew version is now in work_area:[pwd]"
}

#
# System Shut-Down
#
Stopper

puts $lfile "\nElement Checkout successful\n"

exit 0
