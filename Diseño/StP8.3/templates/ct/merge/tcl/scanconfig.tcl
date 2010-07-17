#
# Scan the config file and store the contens
#
# Additional utilities for accessing the config information
#

#---------------------------------------------------------------
#
# Procedures helping to scan the config file
# and store the information in global variables
#

proc strategy { listVal } {
    global cgmStrategy

    set locList $listVal

    set name [keylget locList name]

    set cgmStrategy($name) $listVal

    lappend cgmStrategy(_index) $name

}


proc removeStrategy { listVal } {
    global cgmRemoveStrategy

    set locList $listVal

    set name [keylget locList name]

    set cgmRemoveStrategy($name) $listVal

    lappend cgmRemoveStrategy(_index) $name

}


proc insertStrategy { listVal } {
    global cgmInsertStrategy

    set locList $listVal

    set name [keylget locList name]

    set cgmInsertStrategy($name) $listVal

    lappend cgmInsertStrategy(_index) $name

}


proc fileType { listVal } {
    global cgmFileType

    set locList $listVal

    set name [keylget locList name]

    set cgmFileType($name) $listVal
    lappend cgmFileType(_index) $name

}


proc blockType { listVal } {
    global cgmBlockType

    set locList $listVal

    set name [keylget locList name]

    set cgmBlockType($name) $listVal
    lappend cgmBlockType(_index) $name
}



global cgmActualConfigFileName
set cgmActualConfigFileName ""

proc scanConfig { fname } {

    global cgmActualConfigFileName
    # Tuning: Do not scan config file if it is scanned already

    if { $cgmActualConfigFileName != $fname } {

        source $fname
        set cgmActualConfigFileName $fname
    }
}





#---------------------------------------------------------------
#
# Access utilities for config information
# plus providing default values (where applicable)
#
#---------------------------------------------------------------

proc cgmBlockType.start.typePattern { blockname } {
    global cgmBlockType
    return [keylget cgmBlockType($blockname) startedBy.typePattern]
}

proc cgmBlockType.start.idPattern { blockname } {
    global cgmBlockType
    set locIdPat ""
    catch { set locIdPat [keylget cgmBlockType($blockname) startedBy.idPattern] }
    return $locIdPat
}

proc cgmBlockType.start.allowSim { blockname } {
    global cgmBlockType
    set loc "0"
    catch { set loc [keylget cgmBlockType($blockname) startedBy.allowSim] }
    return $loc
}

proc cgmBlockType.end.typePattern { blockname } {
    global cgmBlockType
    return [keylget cgmBlockType($blockname) endedBy.typePattern]
}

proc cgmBlockType.end.idPattern { blockname } {
    global cgmBlockType
    set locIdPat ""
    catch { set locIdPat [keylget cgmBlockType($blockname) endedBy.idPattern] }
    return $locIdPat
}

proc cgmBlockType.end.allowSim { blockname } {
    global cgmBlockType
    set loc "0"
    catch { set loc [keylget cgmBlockType($blockname) endedBy.allowSim] }
    return $loc
}

proc cgmBlockType.defaultStrategy { blockname } {
    global cgmBlockType
    return [keylget cgmBlockType($blockname) defaultStrategy]
}

proc cgmBlockType.overrideStrategy { blockname } {
    global cgmBlockType
    set loc ""
    catch { set loc [keylget cgmBlockType($blockname) startedBy.overrideStrategy] }
    return $loc
}

proc cgmBlockType.overrideRemoveStrategy { blockname } {
    global cgmBlockType
    set loc ""
    catch { set loc [keylget cgmBlockType($blockname) overrideRemoveStrategy] }
    return $loc
}

proc cgmBlockType.overrideInsertStrategy { blockname } {
    global cgmBlockType
    set loc ""
    catch { set loc [keylget cgmBlockType($blockname) overrideInsertStrategy] }
    return $loc
}


#---------------------------------------------------------------

proc cgmFileType.strategy { fileType } {
    global cgmFileType
    return [keylget cgmFileType($fileType) strategy]
}

proc cgmFileType.generalOverridePattern { fileType } {
    global cgmFileType
    set loc ""
    catch { set loc [keylget cgmFileType($fileType) generalOverridePattern] }
    return $loc
}

proc cgmFileType.removeStrategy { fileType } {
    global cgmFileType
    set loc [keylget  cgmFileType($fileType) removeStrategy]
    return $loc
}

proc cgmFileType.insertStrategy { fileType } {
    global cgmFileType
    set loc [keylget  cgmFileType($fileType) insertStrategy]
    return $loc
}


#---------------------------------------------------------------

proc cgmInsertStrategy.location { insertStrategy } {
    global cgmInsertStrategy
    return [keylget cgmInsertStrategy($insertStrategy) location]
}

proc cgmInsertStrategy.linesBefore { insertStrategy } {
    global cgmInsertStrategy
    set loc "0"
    catch { set loc [keylget cgmInsertStrategy($insertStrategy) linesBefore] }
    return $loc
}

proc cgmInsertStrategy.linesAfter { insertStrategy } {
    global cgmInsertStrategy
    set loc "0"
    catch { set loc [keylget cgmInsertStrategy($insertStrategy) linesAfter] }
    return $loc
}

#---------------------------------------------------------------

proc cgmRemoveStrategy.value { removeStrategy } {
    global cgmRemoveStrategy
    set loc ""
    catch { set loc $cgmRemoveStrategy($removeStrategy) }
    return $loc
}


proc removeStrategy.removedPreLines1 { pRemoveStrategyValue } {
    upvar $pRemoveStrategyValue removeStrategyValue
    set loc ""
    catch { set loc [keylget removeStrategyValue removedPreLines1] }
    return $loc
}

proc removeStrategy.removedPreLines2 { pRemoveStrategyValue } {
    upvar $pRemoveStrategyValue removeStrategyValue
    set loc ""
    catch { set loc [keylget removeStrategyValue removedPreLines2] }
    return $loc
}

proc removeStrategy.removedPostLines1 { pRemoveStrategyValue } {
    upvar $pRemoveStrategyValue removeStrategyValue
    set loc ""
    catch { set loc [keylget removeStrategyValue removedPostLines1] }
    return $loc
}

proc removeStrategy.removedPostLines2 { pRemoveStrategyValue } {
    upvar $pRemoveStrategyValue removeStrategyValue
    set loc ""
    catch { set loc [keylget removeStrategyValue removedPostLines2] }
    return $loc
}

proc removeStrategy.removedStartPatternSubstitute { pRemoveStrategyValue index } {
    upvar $pRemoveStrategyValue removeStrategyValue
    set loc ""
    catch { set loc [lindex [keylget removeStrategyValue removedStartPatternSubstitute] $index] }
    return $loc
}

proc removeStrategy.removedEndPatternSubstitute { pRemoveStrategyValue index } {
    upvar $pRemoveStrategyValue removeStrategyValue
    set loc ""
    catch { set loc [lindex [keylget removeStrategyValue removedEndPatternSubstitute] $index] }
    return $loc
}

proc removeStrategy.removedLineSubstitute { pRemoveStrategyValue index } {
    upvar $pRemoveStrategyValue removeStrategyValue
    set loc ""
    catch { set loc [lindex [keylget removeStrategyValue removedLineSubstitute] $index] }
    return $loc
}


#---------------------------------------------------------------

proc cgmStrategy.master { strategy } {
    global cgmStrategy
    return [keylget cgmStrategy($strategy) master]
}

