#
# Test utility for the merger.
#
# $1 name of the configuration file
# $2 name of the manual changed file
# $3 name of the new generated file
# $4 name of the target file (can be equal to the manual file)
#
# [$5] (optional) Name of checkfile, containing the expected result for the target file
# [$6] (optional) Name of checkdump, containing the expected block dump for the scanned files
#

set mergetcldir [file join $env(CG_DIR) "merge/tcl"]
puts $mergetcldir
source [file join $mergetcldir block.tcl]
source [file join $mergetcldir mergefile.tcl]
source [file join $mergetcldir scanconfig.tcl]
source [file join $mergetcldir scanfile.tcl]



proc getNumOccurrences { fname pattern } {

    # Returns number of lines in 'file' containing 'pattern'

    set openError [catch { set fdesc [open $fname r] }]

    if { $openError } {
        return 0
    }

    set count 0

    while { ![eof $fdesc] } {

        set line [gets $fdesc]

        if  { [eof $fdesc] } {
            break
        }
        if { [regexp $pattern $line] } {
            incr count
        }

    }
    close $fdesc

    return $count
}



proc diffFiles { fname1 fname2 } {

    # Checks, if files differ. Returns '1', if files are different

    global isTestError
    set result 0

    set openError [catch { set fdesc1 [open $fname1 r] }]
    if { $openError } {
        puts "MERGE TEST WARNING - error opening file $fname1"
        return 0
    }
    set openError [catch { set fdesc2 [open $fname2 r] }]
    if { $openError } {
        puts "MERGE TEST WARNING - error opening file $fname2"
        close $fdesc1
        return 0
    }

    while { ! [eof $fdesc1] && ! [eof $fdesc1] } {

        set line1 [gets $fdesc1]
        set line2 [gets $fdesc2]

        if { $line1 != $line2 ||  [eof $fdesc1] != [eof $fdesc2] } {
            puts stderr "MERGE TEST - Diff: $fname1:	$line1"
            puts stderr "MERGE TEST - Diff: $fname2:	$line2"
            set isTestError 1
            set result 1
            break
        }

    }

    if {  [eof $fdesc1] != [eof $fdesc2] } {
            puts stderr "MERGE TEST - Diff: Different file length of $fname1 and $fname2"
            set isTestError 1
            set result 1
    }

    close $fdesc1
    close $fdesc2

    return $result
}



global argv
global argc
global isTestError
set isTestError 0

if { [lindex $argv 3] == "" } {
    puts "Usage:  mergetest.tcl  configfile  manual  generated  target  \[checkfile \[checkdump\]\]"
    exit 1
}

set configfile "[lindex $argv 0]"
set manualfile "[lindex $argv 1]"
set genfile    "[lindex $argv 2]"
set targetfile "[lindex $argv 3]"
set checkfile  "[lindex $argv 4]"
set checkdump  "[lindex $argv 5]"

# Call merger
mergeInit   $configfile
mergeFiles  $manualfile $genfile $targetfile 


# Get occurrences of signal words

set numMAN_MAN    [getNumOccurrences $manualfile _MANUAL_]
set numMAN_TARGET [getNumOccurrences $targetfile _MANUAL_]
set numGEN_GEN    [getNumOccurrences $genfile _GENERATED_]
set numGEN_TARGET [getNumOccurrences $targetfile _GENERATED_]
set numNOTOCCUR_TARGET [getNumOccurrences $targetfile _NOTOCCUR_]


if { $numMAN_MAN != $numMAN_TARGET } {
    puts stderr "MERGE TEST ERROR - '_MANUAL_' mismatch: manual file: $numMAN_MAN, target file: $numMAN_TARGET"
    set isTestError 1
    }

if { $numGEN_GEN != $numGEN_TARGET } {
    puts stderr "MERGE TEST ERROR - '_GENERATED_' mismatch: generated file: $numGEN_GEN, target file: $numGEN_TARGET"
    set isTestError 1
    }

if { $numNOTOCCUR_TARGET != "0" } {
    puts stderr "MERGE TEST ERROR - '_NOTOCCUR_' occured $numNOTOCCUR_TARGET times in target file"
    set isTestError 1
    }


if { $isTestError == "1" } {
    puts stderr ""
}

if { $checkfile != "" } {
    if { [diffFiles $targetfile $checkfile] } {
        puts stderr "MERGE TEST ERROR - Files differ: $targetfile  $checkfile"
        set isTestError 1
    }
}

if { $checkdump != "" } {
    if { [diffFiles $checkdump ${checkdump}.OUT] } {
        puts stderr "MERGE TEST ERROR - Dumpfiles differ: $checkdump  ${checkdump}.OUT"
        set isTestError 1
    }
}


if { $isTestError == "0" } {
    puts stderr ""
    puts stderr "MERGE TEST - OK, no error ocurred."
}
