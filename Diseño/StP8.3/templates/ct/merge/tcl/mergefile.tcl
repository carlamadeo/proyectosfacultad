#
# Two-way merge of generated files
#
# To start, call:
#
#	'proc mergeInit configName'
#
# and then for every file:
#
#	'proc mergeFiles fnameGen fnameMan fnameTarget'
#
#
#
# Global variables:
#
#
# Configuration information:
#
#    global cgmBlockType	Block type info
#    global cgmFileType		File types	
#    global cgmRemoveStrategy	Remove strategies
#    global cgmInsertStrategy	Insert strategies
#    global cgmStrategy		Merge strategies
#
# Other variables used during scanning and merging:
#
#    global cgmBlock		Contains the code blocks created by scanning the input files
#    global cgmLineBuf		Line buffer containing subsequent code lines between the comment tags
#    global fileType		Type of the file being merged
#    global targetFileDesc	File descriptor for target file (output file)
#





proc mergePrintLine { line } {
    # Prints the line 'line' to the target file

    global targetFileDesc

    puts $targetFileDesc $line
}




proc printBlock { blockIndex } {

    # Prints the complete contents of the block
    #
    # Can be called for all block types

    #
    # blockIndex	Index of the block to be printed
    #

    global cgmBlock

    if { $cgmBlock($blockIndex,name) == "" } {

        # Print Line Block

	foreach line $cgmBlock($blockIndex,lines) {
            mergePrintLine $line
        }


    } else {

        # Print Regular Block or File Block

        # Print block normally

        # Print start tag line, if exists
        if { $cgmBlock($blockIndex,hasStartLine) } {
            mergePrintLine $cgmBlock($blockIndex,startLine)
        }

        # Print all contained blocks
	foreach childIndex $cgmBlock($blockIndex,children) {
            printBlock $childIndex
        }

        # Print end tag line, if exists
        if { $cgmBlock($blockIndex,hasEndLine) } {
            mergePrintLine $cgmBlock($blockIndex,endLine)
        }
    }
}




proc printRemovedBlock { blockIndex pLocRemoveStrategy { isTopLevelOfRemove 1 } } {

    # Prints the complete contents of the block
    # This block is removed. Print block according the removal
    # configuration of the current fileType (i.e. print under comments)
    #
    #
    # This procedure has not to be called, if the removed block shall not be printed.
    #
    # Can be called for all block types

    #
    # blockIndex	Index of the block to be printed
    #
    #
    # locRemoveStrategy (per reference)
    # 			Keyed list containing the remove strategy
    #                   (as defined in the configuration file)
    #
    # isTopLevelOfRemove 
    # 			'1'	block is the outmost block which is removed
    # 			'0'	block is contained in another removed block
    #

    global cgmBlock
    upvar $pLocRemoveStrategy locRemoveStrategy

    if { $cgmBlock($blockIndex,name) == "" } {

        # Print Line Block


        # Line Block was removed in the new generated source.
        # Print Line Block according the removal configuration

        # Substitute all lines of the Line Block by the
        # 'removedLineSubstitute' pattern.

	foreach line $cgmBlock($blockIndex,lines) {
            regsub [removeStrategy.removedLineSubstitute locRemoveStrategy 0] \
                   $line \
                   [removeStrategy.removedLineSubstitute locRemoveStrategy 1] \
                   subLine
            mergePrintLine $subLine
        }
    }

    if { $cgmBlock($blockIndex,name) != "" } {

        # Print Regular Block or File Block


        # Block was removed in the new generated source.
        # Print Block according the removal configuration


        # Print start tag line, if exists

        if { $cgmBlock($blockIndex,hasStartLine) } {

            # Print 'removedPreLines1' before the block itself and before the tag line.

            # Print them only if this block is the outmost removed block
            # (and thus not scoped by another removed block).
            # This is to avoid nested comments for nested removed blocks.

            if { $isTopLevelOfRemove } {
	        foreach preLine [removeStrategy.removedPreLines1 locRemoveStrategy] {
                    mergePrintLine $preLine
                }
            }

            # Substitute start tag line of the Line Block by the
            # 'removedStartPatternSubstitute' pattern.
            # This is to deactivate the pattern for subsequent merges.

            regsub [removeStrategy.removedStartPatternSubstitute locRemoveStrategy 0] \
                   $cgmBlock($blockIndex,startLine) \
                   [removeStrategy.removedStartPatternSubstitute locRemoveStrategy 1] \
                   subLine
            mergePrintLine $subLine

            # Print 'removedPreLines2' after the tag line.

            if { $isTopLevelOfRemove } {
	        foreach preLine [removeStrategy.removedPreLines2 locRemoveStrategy] {
                    mergePrintLine $preLine
                }
            }
        }



        # Print all contained blocks

        foreach childIndex $cgmBlock($blockIndex,children) {
            printRemovedBlock $childIndex locRemoveStrategy 0
        }



        # Print end tag line, if exists

        if { $cgmBlock($blockIndex,hasEndLine) } {

            # Print 'removedPostLines1' after the block itself, but before the tag line.
            if { $isTopLevelOfRemove } {
	        foreach preLine [removeStrategy.removedPostLines1 locRemoveStrategy] {
                    mergePrintLine $preLine
                }
            }

            # Substitute end tag line of the Line Block by the
            # 'removedEndPatternSubstitute' pattern.
            # This is to deactivate the pattern for subsequent merges.

            regsub [removeStrategy.removedEndPatternSubstitute locRemoveStrategy 0] \
                   $cgmBlock($blockIndex,endLine) \
                   [removeStrategy.removedEndPatternSubstitute locRemoveStrategy 1] \
                   subLine
            mergePrintLine $subLine

            # Print 'removedPostLines2' after the ent tag line.
            if { $isTopLevelOfRemove } {
	        foreach preLine [removeStrategy.removedPostLines2 locRemoveStrategy] {
                    mergePrintLine $preLine
                }
            }
        }
    }
}




proc insertBlock { blockIndex parentMaster } {

    # The block is inserted in the slave file.
    # The block is printed according the insertStrategies defined for this block.
    #
    # Allowed block types: Regular Block, File Block

    # blockIndex	Index of inserted block
    #
    # parentMaster	Merge master of parent block ('M' or 'G').
    #

    global cgmBlock
    global fileType

    # Get insert strategy for this block. Default comes from fileType, it can be
    # overriden by 'blockType.overrideInsertStrategy'.

    set locInsertStrategy [cgmBlockType.overrideInsertStrategy $cgmBlock($blockIndex,name)]

    if { $locInsertStrategy == "" } {
        set locInsertStrategy [cgmFileType.insertStrategy $fileType]
    }


    # Evaluate insert strategy
    set locInsertLocation [cgmInsertStrategy.location $locInsertStrategy]

    if { $locInsertLocation == "Bottom" } {

        # Currently, the only choice

        # Insert new block at the bottom, that is, at the current location
        #
        # (NOTE: Other possibilities would be: Insert at top or before/after defined
        #        blocks.
        #        To get this, the following would be nessecary:
        #
        #		- blocks containing only one line (a start tag) must be possible
        #		- the merge result must be produced in the same block
        #			structure as the scanned input files instead of directly
        #			writing to the output file.
        #  NOTE END)

        # Print 'before lines'
        for { set i [cgmInsertStrategy.linesBefore $locInsertStrategy] } { $i > 0 } { incr i -1 } {
            mergePrintLine ""
        }

        # Print complete inserted block
        printBlock $blockIndex

        # Print 'after lines'
        for { set i [cgmInsertStrategy.linesAfter $locInsertStrategy] } { $i > 0 } { incr i -1 } {
            mergePrintLine ""
        }


    } else {

        error "MERGE ERROR - invalid value for insertStrategy: $locInsertLocation"
    }

}


proc removeBlock { blockIndex parentMaster } {

    # The block is removed.
    # This procedure may print this block under comments or so
    #
    # Allowed block types: Regular Block, File Block

    # blockIndex	Index of removed block
    #
    # parentMaster	Merge master of parent block ('M' or 'G').
    #

    global cgmBlock
    global fileType


    # Get remove strategy for this block.
    #
    # NOTE: If present, the block's overrideRemoveStrategy overrides the cmgFileType's
    #       removeStrategy.

    set locRemoveStrategy [cgmBlockType.overrideRemoveStrategy $cgmBlock($blockIndex,name)]

    if { $locRemoveStrategy == "" } {
        set locRemoveStrategy [cgmFileType.removeStrategy $fileType]
    }

    # Get whole value of the remove strategy named by 'locRemoveStrategy' (tuning(?))
    set locRemoveStrategyValue [cgmRemoveStrategy.value $locRemoveStrategy]

    # Default: If the subsequent 'keepRemoved' rules don't save
    # the removed code, do remove it.
    set doPrint 0

    # Evaluate the 'keepRemoved' rules
    set locKeepRemoved [keylget locRemoveStrategyValue keepRemoved]

    if { $locKeepRemoved == "Always" } {
        set doPrint 1

    } elseif { $locKeepRemoved == "Never" } {
        set doPrint 0

    } elseif { $locKeepRemoved == "ManualMaster" } {
        if { $parentMaster == "M" } {
            set doPrint 1
        } else {
            set doPrint 0
        }

    } elseif { $locKeepRemoved == "GeneratedMaster" } {
        if { $parentMaster == "G" } {
            set doPrint 1
        } else {
            set doPrint 0
        }
    }


    if { $doPrint } {
        printRemovedBlock $blockIndex locRemoveStrategyValue
    }

}







proc mergeRegularBlocks { blockIndexGen blockIndexMan } {

    # Merge two blocks.
    #
    # To be called for Root Blocks or Regular Blocks.

    global cgmBlock
    global fileType


    # General merge strategy:
    #
    # The  m a s t e r  block controls the contents of the result.
    #
    #		That means: The contained lines and the order of
    #		the contained blocks are controlled by the master block.
    #
    # The  g e n e r a t e d  block controls the existence of contained blocks.
    #
    #		That means: Whether a new block is created or a block 
    #		is removed is always controlled by the generated block.
    #


    # Special case: No manual file block exists (block index == 0)
    #
    # Copy the whole generated block to the target file

    if { $blockIndexMan == 0 } {
        printBlock $blockIndexGen
        return
    }

    if { $blockIndexGen == 0 } {
        printBlock $blockIndexMan
        puts stderr "MERGE WARNING - No generated file exists"
        return
    }


    # Determine merge strategy of the two blocks
    #
    # NOTE: The merge strategy is determined by the manual file.
    # This is because in normal cases the merge strategy is determined
    # by the block type (which is the same between generated and manual file)
    # or the strategy is overridden in the manual file.
    # In both cases the manual file is relevant.

    set locStrategy $cgmBlock($blockIndexMan,strategy)

    set master [cgmStrategy.master $locStrategy]

    # Now master contains 'M' or 'G' (manual or generated file is
    #                                 master in this block)

    if { $master == "M" } {
        set blockIndexMaster $blockIndexMan
        set blockIndexSlave  $blockIndexGen
    } else {
        set blockIndexMaster $blockIndexGen
        set blockIndexSlave  $blockIndexMan
    }


    #
    # Start merging the block
    #

    #
    # Merge the first line of the block
    #

    # The first line (the comment tag) is always taken from the
    # generated file with only the manual part overridden by the
    # manual file

    if { $cgmBlock($blockIndexGen,hasStartLine) } {
        # We have a starting comment tag

        # Substitute the manual part ot the tag
        regsub [cgmFileType.generalOverridePattern $fileType] \
               $cgmBlock($blockIndexGen,startLine) \
               $cgmBlock($blockIndexMan,manualTagPart) \
               newTag

        # Print new tag line
        mergePrintLine $newTag
    }


    #
    # Merge the contained blocks (child blocks)
    #

    foreach blockIndexMasterChild $cgmBlock($blockIndexMaster,children) {

        # Merge block with index 'blockIndexMasterChild' in the master block
        # Action depends on block type:


        if { $cgmBlock($blockIndexMasterChild,name) == "" } {


            # Child block is a Line Block


            # Merge is done by just printing the whole master block
            # and thusly overwriting the slave block

            printBlock $blockIndexMasterChild

            # Mark child block (master and slave) as merged
            set cgmBlock($blockIndexMasterChild,isMerged) 1

        } else {


            # Child block is a Regular Block


            # Find the corresponding block in the slave block
            set blockIndexSlaveChild [findSubBlockById $blockIndexSlave $cgmBlock($blockIndexMasterChild,id)]


            if { $blockIndexSlaveChild == 0 } {

                # Block not found in slave block. Action depends on master:

                if { $master == "G" } {

                    # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                    #
                    #  MERGE -  INSERTED GENERATED MASTER BLOCK
                    #
                    # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

                    # If master = 'G':
                    # Print complete master block as it is.

                    # Print complete block
                    printBlock $blockIndexMasterChild

                    # Mark child block as merged
                    set cgmBlock($blockIndexMasterChild,isMerged) 1

                } else {

                    # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                    #
                    #  MERGE -  DELETED MANUAL MASTER BLOCK
                    #
                    # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

                    # If master = 'M':
                    # Block is not printed (de facto it is removed)

                    removeBlock $blockIndexMasterChild $master
                }

            } else {

                # Block found in slave block.

                # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                #
                #  MERGE -  MERGE MASTER BLOCK with SLAVE BLOCK
                #
                # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

                # Check, if this block was not merged already
                if { $cgmBlock($blockIndexSlaveChild,isMerged) != "0" } {
                    error "MERGE ERROR - block ID $cgmBlock($blockIndexMasterChild,id) occured twice in master block."
                }

                # Security check: Check, that both block types are equal
                if { $cgmBlock($blockIndexMasterChild,name) != $cgmBlock($blockIndexSlaveChild,name) } {
                    puts stderr "MERGE ERROR - different block types matched on pattern."
                    puts stderr "              Check type and ID patterns for the following pattern(s):"
                    puts stderr "              $cgmBlock($blockIndexMasterChild,name)   $cgmBlock($blockIndexSlaveChild,name)"
                    error "MERGE ERROR - different block types matched on pattern."
                }

                # Merge recursively. Note the order of passed arguments to 'mergeRegularBlocks'
                # (the index of the generated block is always passed before that of the manual block)
                if { $master == "G" } {
                    mergeRegularBlocks $blockIndexMasterChild $blockIndexSlaveChild
                } else {
                    mergeRegularBlocks $blockIndexSlaveChild $blockIndexMasterChild
                }

                # Mark child block (master and slave) as merged
                set cgmBlock($blockIndexMasterChild,isMerged) 1
                set cgmBlock($blockIndexSlaveChild,isMerged) 1
            }

        }

    }


    #
    # Now check, whether some blocks are still not merged from the slave block
    # Note, that Line Blocks of the slave are  n o t  marked as merged
    # and they are not treated here (as they are to be removed).

    # Note: There is different behaviour, if the slave is the generated or the manual block

    foreach blockIndexSlaveChild $cgmBlock($blockIndexSlave,children) {
        if { $cgmBlock($blockIndexSlaveChild,isMerged) == 0 && $cgmBlock($blockIndexSlaveChild,name) != "" } {

            if { $master == "G" } {

                # Slave is the manual block

                # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                #
                #  MERGE -  DELETED MANUAL SLAVE BLOCKS
                #
                # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

                # If manual slave blocks are found unmerged, they will be removed
                # (or set under comment or so)

                removeBlock $blockIndexSlaveChild $master

            } else {

                # Slave is the generated block

                # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
                #
                #  MERGE -  INSERTED GENERATED SLAVE BLOCKS
                #
                # - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

                # If generated slave blocks are found unmerged, they will be inserted
                # now (that is, at the end of the current block)

                insertBlock $blockIndexSlaveChild $master
            }
        }
    }


    #
    # Merge the last line of the block
    #

    # The first line (the comment tag) is always taken from the
    # generated file.
    # Note: The root block does not contain an end line

    if { $cgmBlock($blockIndexGen,hasEndLine) } {

        # Print new tag line
        mergePrintLine $cgmBlock($blockIndexGen,endLine)
    }




}





proc mergeFileBlocks { blockIndexGen blockIndexMan } {

    mergeRegularBlocks $blockIndexGen $blockIndexMan 

}



proc mergeInit { configName  } {

    # Scan configuration information
    #
    scanConfig $configName
}



proc mergeFiles { fnameMan fnameGen fnameTarget { fnameDump "" } } {

    # fnameDump	 will dump the scanned blocks for testing reasons to this file

    global fileType
    set fileType ""

    # Global initializations of block array
    #
    initBlock

    # Scan files
    set blockIndexGen [scanFile $fnameGen]
    set blockIndexMan [scanFile $fnameMan]

    if { $fnameDump != "" } {

        set fdescDump [open $fnameDump w]
        # Dump blocks
        puts $fdescDump ""
        puts $fdescDump "DUMP OF GENERATED FILE:"
        dumpBlock $fdescDump $blockIndexGen
        puts $fdescDump ""
        puts $fdescDump "DUMP OF MANUAL FILE:"
        dumpBlock $fdescDump $blockIndexMan
        close $fdescDump
    }

    # open target file
    global targetFileDesc
    set openError [catch {set targetFileDesc [open $fnameTarget w] }]

    if { $openError } {
        error "MERGE ERROR when opening file $fnameTarget"
    }

    # Merge files
    mergeFileBlocks $blockIndexGen $blockIndexMan

    # close target file
    close $targetFileDesc
}

