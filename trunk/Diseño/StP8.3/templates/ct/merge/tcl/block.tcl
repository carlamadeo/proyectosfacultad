

#----------------------------------------
#        SCANNED BLOCK INFORMATION
#----------------------------------------

#
# Scanned block information is stored in the array 'cgmBlock'.
# This array contains information about the blocks as well as 
# an index stack for processing these blocks.
#
#
# BLOCK TYPES: There are three block types:
#
#	File block:	Block representing the whole file
#
#	Regular block:	Block enclosed by comment tags, containing only other blocks
#
#	Line block:	Block containing code lines only (and no other blocks)
#
#
#
# Please note the fact, that one tag may start more than one block.
# In this case, as a rule all information is stored for every block
# started by this tag. Exceptions of this rule are mentioned in the following
# descriptions (see 'isFirst' and comment 'Only if isFirst == '1'')
#
#  cgmBlock(currentStack)	List of indices for the actual blocks in the stack.
#				For nested blocks: The most inner block (which is
#				the actual block when processing the file) has index
#				'0' (see 'currentBlock')
#	
#  cgmBlock(counter)		Highest used index.
#
#  cgmBlock(i,name)		Name of block with index 'i'. 'i' starts with '1'.
#				Normally, this is the name of the block type as 
#				defined in the config info.
#				Special names:
#
#				_ROOT_		A root block.
#				(empty string)	A block containing only normal
#						text lines (and no other blocks)
#
#  cgmBlock(i,id)		Codegen ID of this block.
#				Empty, if no ID is given in the start tag of the block.
#
#
#  cgmBlock(i,isFirst)		'1'	This block is the first of a series of blocks
#					started by one single comment tag
#				'0'	Otherwise
#
#  cgmBlock(i,manualTagPart)	(Only if isFirst == '1')
#				The manual changeable part of the tag starting this block.
#				This is the text extracted by the file types 'generalOverridePattern'.
#
#
#  cgmBlock(i,children)		Indices of the children of this block. These indices are ordered
#				according the order of blocks in the scanned file.
#
#  cgmBlock(i,endAllowSim)	Like blockType.endedBy.allowSim (or its default value)
#
#  cgmBlock(i,endTypePattern)	Like blockType.endedBy.typePattern (or its default value)
#
#  cgmBlock(i,endIDPattern)	Like blockType.endedBy.idPattern (or its default value)
#
#
#  cgmBlock(i,hasStartLine)	'1', if 'endLine' exists. '0' else.
#
#  cgmBlock(i,startLine)	(Only if hasStartLine == '1')
#				The whole (comment tag) line starting this block.
#
#  cgmBlock(i,hasEndLine)	'1', if 'endLine' exists. '0' else.
#
#  cgmBlock(i,endLine)		(Only if hasEndLine == '1')
#				The whole (comment tag) line ending this block.
#
#  cgmBlock(i,lines)		The code lines contained in this block.
#				
#  cgmBlock(i,strategy)		The actual merge strategy for this block.
#				This strategy is calculated using the block type's
#				default strategy.
#				For the first block only, this strategy is overriden by
#				the override strategy given in the comment tag starting
#				this block (if present).
#				
#  cgmBlock(i,isMerged)		'0'	This block was not yet used for merging
#  				'1'	This block was used for merging already
#
#
# Depending on the block type, the block contains the following attributes:
#
# File block:
#
#  cgmBlock(i,name)		"_ROOT_"
#  cgmBlock(i,id)		""
#  cgmBlock(i,isMerged)
#  cgmBlock(i,strategy)
#  cgmBlock(i,isFirst)		"0"	Because the root block has no starting
#					comment tag
#  cgmBlock(i,hasStartLine)	"0"
#  cgmBlock(i,hasEndLine)	"0"
#
#
# Regular block:
#
#  cgmBlock(i,name)
#  cgmBlock(i,id)
#  cgmBlock(i,isMerged)
#  cgmBlock(i,strategy)
#  cgmBlock(i,isFirst)
#  cgmBlock(i,startLine)	(optional)
#  cgmBlock(i,manualTagPart)	(optional)
#  cgmBlock(i,endTypePattern)
#  cgmBlock(i,endIDPattern)
#  cgmBlock(i,endAllowSim)
#  cgmBlock(i,endLine)		(optional)
#  cgmBlock(i,hasStartLine)
#  cgmBlock(i,hasEndLine)
#
#
# Line block:
#
#  cgmBlock(i,name)		""
#  cgmBlock(i,id)		""
#  cgmBlock(i,isMerged)
#  cgmBlock(i,lines)
#  cgmBlock(i,endTypePattern)	""
#  cgmBlock(i,hasStartLine)	"0"
#  cgmBlock(i,hasEndLine)	"0"
#



proc currentBlock { } {

    # Returns index of the current block

    global cgmBlock
    return [lindex $cgmBlock(currentStack) 0]
}



proc openBlock { { isRoot 0 } } {
    # isRoot	opening root block
    #		A root block is a top-level block not contained in any other block

    # Opens a new block.
    # If 'isRoot' is '0', the new block is connected as a child of the current block


    global cgmBlock

    incr cgmBlock(counter)

    if { $isRoot } {
        if { $cgmBlock(currentStack) != "" } {
            error "MERGE ERROR in openBlock - block stack not empty when opening new root block"
        }
    } else {
        lappend cgmBlock([currentBlock],children) $cgmBlock(counter)
    }

    lvarpush cgmBlock(currentStack) $cgmBlock(counter)

    return $cgmBlock(counter)

}


proc closeBlock { } {

    global cgmBlock

    lvarpop cgmBlock(currentStack)

}



proc forceCloseAllBlocks { } {

    global cgmBlock

    set cgmBlock(currentStack) ""

}


proc findSubBlockById { blockIndex subBlockId } {

    # Finds that subblock of block with index 'blockIndex', which
    # contains ID 'subBlockId'. Line Blocks (name == "") are ignored.
    #
    # Returns the index of the found block or '0', if no block is found.

    global cgmBlock


    foreach locId $cgmBlock($blockIndex,children) {
        if { $cgmBlock($locId,id) == $subBlockId && $cgmBlock($locId,name) != "" } {
            return $locId
        }
    }
    return 0
}



proc initBlock { } {
    global cgmBlock

    catch [ unset cgmBlock ]

    set cgmBlock(counter) 0
    set cgmBlock(currentStack) ""

}



proc dumpBlock { outfiledesc index { indentCount  0 } } {

    global cgmBlock

    # Index 0 not allowed
    if { $index == 0 } {
        return
    }


    if { $indentCount == 0 } {
        puts $outfiledesc "=============================================================="
    }

    set indent [replicate " " $indentCount]

    if { $cgmBlock($index,name) == "" } {
        puts $outfiledesc "${indent}-----BLOCK $index $cgmBlock($index,name) ID:$cgmBlock($index,id)-----------------------"
	foreach line $cgmBlock($index,lines) {
            puts $outfiledesc "${indent}$line"
        }
    }

    if { $cgmBlock($index,name) != "" } {
        if { $cgmBlock($index,name) != "_ROOT_" } {
            puts $outfiledesc "${indent}-----BLOCK $index $cgmBlock($index,name) $cgmBlock($index,strategy) $cgmBlock($index,isFirst)  ID:$cgmBlock($index,id)-----------------------"
        } else {
            puts $outfiledesc "${indent}-----BLOCK $index $cgmBlock($index,name) $cgmBlock($index,strategy) $cgmBlock($index,isFirst)  ID:$cgmBlock($index,id)-----------------------"
        }

        if { $cgmBlock($index,hasStartLine) } {
            puts $outfiledesc "${indent}$cgmBlock($index,startLine)"
        }

	foreach childIndex $cgmBlock($index,children) {
            dumpBlock $outfiledesc $childIndex [expr $indentCount + 3]
        }

        if { $cgmBlock($index,hasEndLine) } {
            puts $outfiledesc "${indent}$cgmBlock($index,endLine)"
        }
    }
    puts $outfiledesc "${indent}----- $index -------------------------------------------------------------"

    if { $indentCount == 0 } {
        puts $outfiledesc "=============================================================="
    }

}




#------------------------------------------------------------------------------------

