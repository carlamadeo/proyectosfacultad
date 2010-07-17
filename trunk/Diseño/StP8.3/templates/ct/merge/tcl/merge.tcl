# Main script for the merge utility
#
# $1 name of the configuration file
# $2 name of the manual changed file
# $3 name of the new generated file
# $4 name of the target file (can be equal to the manual file)
# [$5] (optional}  dumpfile	causes a dump of the scanned block structure to be written on this file
#

global env
global argv
global argc
source $env(MERGE_PATH)/tcl/scanconfig.tcl
source $env(MERGE_PATH)/tcl/block.tcl
source $env(MERGE_PATH)/tcl/scanfile.tcl
source $env(MERGE_PATH)/tcl/mergefile.tcl

###############################################################################
proc getFileName {file} {
	set fileList [file split $file]
	return [lindex $fileList [ expr [llength $fileList] - 1]]
}

###############################################################################
proc getDirName {file} {
	set dirName [file dirname $file];
	if { $dirName == "" } {	return "";}
	set delim "/"
	return $dirName$delim;
}

###############################################################################
proc fileDiff { fileName } {

	set manFileName  $fileName;
	set genFileName  "[getDirName $fileName]__[getFileName $fileName]";

	if { ![file exists $manFileName] } { 
		return 0 
	}

	if { [file size $manFileName] != [file size $genFileName] } {
		return 0;
	}

	set genFile [open $genFileName];
	set manFile [open $manFileName];
	while {[eof $genFile] != 1} {
		if { [gets $genFile] != [gets $manFile] } {
			close $genFile
			close $manFile
			return 0;
		}
	}
	close $genFile
	close $manFile
	return 1;
}


###############################################################################
set fileName ""
set mergeConfigFile ""

if { ![file exists "_acdGeneratedFiles.mrg"] } {
	puts "cannot open file _acdGeneratedFiles.mrg"
	return 
}

set file [open "_acdGeneratedFiles.mrg"]
if {[eof $file] != 1} {
	set mergeConfigFile "$env(MERGE_PATH)/MergeConfig/[gets $file]"
}

mergeInit "$mergeConfigFile"

while {[eof $file] != 1} {

	set fileName [gets $file]
	set fileName [string trim $fileName]

	if {$fileName == ""} continue

	set delim "_"
	set genFileName [getDirName $fileName]
	set genFileName $genFileName$delim
	set genFileName $genFileName[getFileName $fileName]

	set newFileName [getDirName $fileName]
	set newFileName $newFileName$delim$delim
	set newFileName $newFileName[getFileName $fileName]

	mergeFiles "$fileName" "$genFileName" "$newFileName"

	if { [fileDiff $fileName] != 1 } {
		puts "Merge $fileName"
		file delete $fileName
		file rename $newFileName $fileName
	} else {
		file delete $newFileName
	}

	catch { file delete $genFileName }
}

close $file
catch {file delete "_acdGeneratedFiles.mrg"}
