#
# Scan one file and analyze it according the config information.
#
#


#
# Determine file type  -> myFileType
#

#
# Actual regexp list: Regexp's, which are currently of interest when scanning the file
#
#	- myFileType.generalPatterns (if exists)
#
#	else:
#
#	- all patterns given by  
#		myFileType.possibleBlocks.startedBy.typePattern
#
#	and
#
#	- of all active blocks:
#		CGMblockType.endedBy.typePattern
#


# Algorithm:
#
# 1. Set up initial actual regexp list
#
# 2. Scan for next start/end pattern
#
# START PATTERN:
#
# S1. Get the one (or all) block types for this pattern
#
# S2. open this block
#
#	allocate block contents on stack
#
#	add the block's end pattern to the actual regexp list
#
#	add some references to the block contents that it can be
#	accessed by the block ID.
#
#
# END PATTERN:
#
# E1. Check, whether the ended block is open (if not -> ERROR)
#
# E2. close this block
#
#	block level -= 1
#
#	remove the block's end pattern from the actual regexp list
#
#






proc getFileType { fname } {

    # Gets file type out of the file name

    global cgmFileType
    foreach elem $cgmFileType(_index) {
        if { [regexp [keylget cgmFileType($elem) fileNamePattern] $fname] } {
            return [keylget cgmFileType($elem) name]
        }
    }
    # ERROR
    error "MERGE ERROR in getFileType: no file type for $fname"
}


proc getBlockType { line { isBOF 0 } } {

    # Returns a list of the started blocks
    # Empty list: No block is started

    # isBOF	1	We are at the beginning of the file.
    #			In this case all blocks with empty typePattern are started.
    #
    #		0	Else. Normal processing is done here.
    #

    # Checks, if 'line' is a comment tag, which starts a block.
    # As one comment line can start more than one block, a list of 
    # started blocks is returned.


    # NOTE: This procedure must  n o t  be called for end tag lines.
    #       Therefore the line must before be checked, if it is an end tag.
    #       (Reason: Otherwise the warning (see later) would be printed for end tags,
    #        if they match the general pattern).

    global fileType
    global cgmFileType

    set locBlockType ""
    set locGeneralPattern ""
    set isGenPatternMatches 0

    if { $isBOF } {

        # We are at the beginning of the file.
        # Match all blocks with empty startedBy::typePattern
        foreach blockname [keylget cgmFileType($fileType) possibleBlocks] {
            if { [cgmBlockType.start.typePattern $blockname] == "" } {

                 # If already found some block types started by this line:
                 # Further blocks may only be started, if their 'cgmBlockType.start.allowSim' == TRUE
                 if { $locBlockType == "" || [cgmBlockType.start.allowSim $blockname] } {

                    lappend locBlockType $blockname
                }
            }
        }

    } else {


        # We are inside the file.
        # Do normal processing: Match $line against the typePatterns.

        # At first look, if the line matches the general pattern (if exists).

        catch {
            set locGeneralPattern [keylget cgmFileType($fileType) generalPattern]
            }

        # 'isGenPatternMatches' will be 1, if generalPattern is provided and matches the line
        if { $locGeneralPattern != "" && [regexp $locGeneralPattern $line] } {
            set isGenPatternMatches 1
        }

        # If generalPattern matched or if it is not provided: $line is a potential start tag
        if { $isGenPatternMatches ||  $locGeneralPattern == "" } {

            foreach blockname [keylget cgmFileType($fileType) possibleBlocks] {
                 set locPat [cgmBlockType.start.typePattern $blockname]
                 if { $locPat != "" && [regexp $locPat $line] } {

                     # If already found some block types started by this line:
                     # Further blocks may only be started, if their 'cgmBlockType.start.allowSim' == TRUE
                     if { $locBlockType == "" || [cgmBlockType.start.allowSim $blockname] } {

                         # Line matches typePattern: Line is really a start tag.
                         # Add block name to result.
                         lappend locBlockType $blockname
                     }
                 }
            }
        }


        # PRECONDITION (see comments for this procedure):
        #
        #     $line must not be an end tag
        #
        #
        # Print warning, if general pattern matched, but no block type was found.
        # In this case, '$line' matches the generalPattern, but does not start a block,
        # which should not happen or may be caused by an error in the configuration definition.
        if { $isGenPatternMatches && $locBlockType == "" } {
            puts stderr "MERGE WARNING - General pattern matched, but no block type matched for the following code line:"
            puts stderr "                $line"
            puts stderr "MERGE WARNING - Tag line is ignored"
            puts stderr ""
        }
    }

    return $locBlockType
}


proc getBlockEnd { line  { isFirst 1 } { isEOF 0 } } {

    # isFirst	1	First call of this procedure for this line.
    #			The current end tag is allowed to match against $line in every case.
    #
    #		0	This procedure was called already for this line
    #			The current end tag is allowed to match against $line
    #			only if 'cgmBlockType.end.allowSim' == TRUE.
    #
    #
    # isEOF	1	We are at the end of the file.
    #			In this case an open block with empty endedBy::typePattern is closed.
    #
    #		0	Else. Normal processing is done here.
    #

    # Checks, if 'line' is a comment tag, which ends the current block.
    # Additionally, if ID patterns are given, it is checked, whether
    # the ID of the end tag matches the ID of the start tag.
    # If not, a warning is printed.

    global cgmBlock

    set locTypePat $cgmBlock([currentBlock],endTypePattern)


    # Do nothing, if current block is the ROOT block
    if { $cgmBlock([currentBlock],name) == "_ROOT_" } {
        return 0
    }


    # If the line $line closed already a block (isFirst == FALSE):
    # Further blocks may only be closed by the same line,
    # if their 'endedBy.allowSim' == TRUE
    if { $isFirst || $cgmBlock([currentBlock],endAllowSim) } {

        if { $isEOF } {

            # We are at the end of the file.
            # If the open block has an empty end tag (and is not the ROOT block):
            # This block is ended on EOF
            if { $locTypePat == "" && $cgmBlock([currentBlock],name) != "_ROOT_" } {
                return 1
            }

        } else {

            # We are inside the file. Normal processing.

            if { $locTypePat != "" && [regexp $locTypePat $line] } {

                # Line is the end tag for the current block


                # If endIDPattern is given:
                # Check, whether ID matches the corresponding start tag
                # If ID does not match: WARNING

                set idMatch ""
                set locIdPat $cgmBlock([currentBlock],endIDPattern)
                if { $locIdPat != "" } {
                    regexp $locIdPat $line wholeMatch idMatch

                    if { $idMatch != $cgmBlock([currentBlock],id) } {
                        puts stderr "MERGE WARNING - ID does not match between start and end tag"
                        puts stderr "MERGE WARNING - start id: $cgmBlock([currentBlock],id)"
                        puts stderr "MERGE WARNING - end   id: $idMatch"
                    }
                }

                return 1
            }
        }
    }

    return 0
}



proc evalBlock { blockType line isFirst { isBOF 0 } } {
    #
    # blockType		type of block which is starting here
    #
    # line		tag comment line starting this block.
    #			Valid only if '$isBOF == 0'
    #
    # isFirst		interesting, if one comment tag starts
    #			more than one block
    #			1	The first of the started blocks
    #			0	else
    #
    # isBOF		1	We are at BOF. Block 'blockType' shall be started
    #				at BOF and is not delimited by comment tags.
    #				Therefore no start line exists.

    # Starts a block.
    # A new block is opened and some evaluated information
    # from the start tag is stored in the block info.

    # NOTE: The tag comment line itself will be stored in the 
    #       first block only (isFirst == 1)
    #       The override options will also be used for
    #       the first block only.

    global cgmBlock
    global fileType

    # open new block - this will be a Regular Block
    openBlock


    # Init some block attributes
    set cgmBlock([currentBlock],hasStartLine) 0
    set cgmBlock([currentBlock],hasEndLine) 0

    # Set block name
    set cgmBlock([currentBlock],name) $blockType

    # Set default strategy
    set cgmBlock([currentBlock],strategy) [cgmBlockType.defaultStrategy $blockType]

    # Some actions only, if a start tag ($line) exists 
    if { ! $isBOF } {

        # scan id and override strategy from comment tag line
        # (override strategy only used for the first block)
        if { $isFirst == 1 } {
            foreach overrideStrat [cgmBlockType.overrideStrategy $blockType] {
                if { [regexp [keylget overrideStrat pattern] $line] } {
                    set cgmBlock([currentBlock],strategy) [keylget overrideStrat strategy]
                    break
                }
            }
        }

        # If block ist the first block started by the comment tag:
        # store tag line and manual part of tag line in block info
        if { $isFirst == 1 } {
            # Store tag line
            set cgmBlock([currentBlock],startLine) $line
            set cgmBlock([currentBlock],hasStartLine) 1

            # Extract and store manual changeable part of the tag line
            set manMatch ""
            set locManPat [cgmFileType.generalOverridePattern $fileType]
            if { $locManPat != "" } {
                regexp $locManPat $line wholeMatch manMatch
            }
            set cgmBlock([currentBlock],manualTagPart) $manMatch
        }

        # Extract ID of current block and store it
        set idMatch ""
        set locIdPat [cgmBlockType.start.idPattern $blockType]
        if { $locIdPat != "" } {
            regexp $locIdPat $line wholeMatch idMatch
        }
        set cgmBlock([currentBlock],id) $idMatch

    } else {

        # No start tag line exists: Set default values

        set cgmBlock([currentBlock],manualTagPart) ""
        set cgmBlock([currentBlock],id) ""
    }


    # set end patterns
    set cgmBlock([currentBlock],endTypePattern) [cgmBlockType.end.typePattern $blockType]
    set cgmBlock([currentBlock],endIDPattern) [cgmBlockType.end.idPattern $blockType]
    set cgmBlock([currentBlock],endAllowSim)  [cgmBlockType.end.allowSim $blockType]


    # Initialize other attributes
    set cgmBlock([currentBlock],isMerged) 0
    set cgmBlock([currentBlock],isFirst) $isFirst
    set cgmBlock([currentBlock],hasEndLine) 0

}



proc evalBlockEnd { line { isFirst 1 } { isEOF 0 } } {
    #
    # line		tag comment line ending this block
    #
    # isFirst	1	First call of this procedure for this line.
    #			Take the end line in this case.
    #
    #		0	This procedure was called already for this line
    #			Do not take the end line in this case.
    #
    #
    # isEOF		1	We are at EOF.
    #				Block is not ended by comment tags.
    #				Therefore no end line exists.

    # A block is ended (by a comment tag).
    # Store some information and close the block

    global cgmBlock

    if { ! $isEOF && $isFirst } {
        # store tag line in block info
        set cgmBlock([currentBlock],endLine) $line
        set cgmBlock([currentBlock],hasEndLine) 1
    }

    # close new block
    closeBlock
}


proc evalFileBlock { } {
    # blockType		type of block which is strating here

    # Returns ID for root block. This ID is used as index for 'cgmBlock'.

    # Starts a block, which represents the whole file itself.
    # As the file block is not enclosed by comment tags,
    # the attributes for this block come from the fileType
    # information instead of the tag information.

    # The file block is also called 'root' block, as it is not 
    # contained in any oether block.

    # NOTE: The tag comment line itself will be stored in the 
    #       first block.
    #       The override options will also be used for
    #       the first block only.

    global cgmBlock
    global fileType

    # open new root block
    openBlock 1

    # Set some block attributes

    # Set block name
    set cgmBlock([currentBlock],name) "_ROOT_"

    # store strategy
    set cgmBlock([currentBlock],strategy) [cgmFileType.strategy $fileType]

    # Some other default values
    set cgmBlock([currentBlock],endTypePattern) ""
    set cgmBlock([currentBlock],isFirst) 0
    set cgmBlock([currentBlock],id) ""
    set cgmBlock([currentBlock],isMerged) "0"
    set cgmBlock([currentBlock],hasStartLine) 0
    set cgmBlock([currentBlock],hasEndLine) 0

    # Return ID (index) of file block
    return [currentBlock]
}



proc evalFileBlockEnd { } {

    # Closes the file block

    global cgmBlock

    # check, whether the stack level of blocks is empty
    if { $cgmBlock([currentBlock],name) != "_ROOT_" } {
        puts "MERGE WARNING - missing end tags in file"
    }

    # close all open blocks (there should be but one block)
    forceCloseAllBlocks
}



proc flushLineBuf {} {

    # Transfer contents of the line buffer to a new block

    # This procedure opens a new block inside the current block,
    # stores the lines and closes the block

    global cgmBlock
    global cgmLineBuf

    # open new block for the lines contained in cgmLineBuf
    openBlock


    # Set some block attributes to default values for line blocks

    # Set block name and some values
    set cgmBlock([currentBlock],name) ""
    set cgmBlock([currentBlock],id) ""
    set cgmBlock([currentBlock],isMerged) "0"
    set cgmBlock([currentBlock],hasStartLine) 0
    set cgmBlock([currentBlock],hasEndLine) 0

    # Store the lines
    set cgmBlock([currentBlock],lines) $cgmLineBuf

    # close new block
    closeBlock

    # Free the line buffer
    set cgmLineBuf ""
}



proc evalNormalLine { line } {
    # line		code line

    # NOTE: Any subsequent normal code lines are collected 
    #       and stored in one block later (see 'flushLineBuf')

    global cgmLineBuf

    lappend cgmLineBuf $line
}



proc scanFile { fname } {

    # Scans one file and stores its contents + block structure and information
    # in cgmBlock.

    # Returns ID for root block. This ID is used as index for 'cgmBlock'.
    # In case of errors when opening this file '0' is returned.

    # fname	Name of the file to be scanned
    #


    set fileBlockID ""

    global fileType
    global cgmFileType

    # Initialize line buffer. The line buffer will contain any subsequent
    # code lines between the comment tags, until the line buffer is
    # flushed into a new block by 'flushLineBuf'.
    global cgmLineBuf
    set cgmLineBuf ""

    # Get file type out of filename
    set fileType [getFileType $fname]

    # open file. Return if error.
    set openError [catch { set fdesc [open $fname r] }]

    if { $openError } {
	# SH BEGIN
	set openError [catch { set fdesc [open $fname w] }]
        if { $openError } {
            error "MERGE ERROR when opening file $fname"
        }
	close $fdesc
        set openError [catch { set fdesc [open $fname r] }]

	# SH END
    }

    # open new block for entire file. Returns ID of the file block
    set fileBlockID [evalFileBlock]


    # Open all blocks, which start at file beginning

    set blockTypeList [getBlockType "" 1]

    if { $blockTypeList != "" } {
        # Start the new block(s)
        set isFirst 1
        foreach blockType $blockTypeList {
            evalBlock $blockType "" $isFirst 1
            set isFirst 0

        }
    }



    # Process file

    while { ![eof $fdesc] } {

        set line  [gets $fdesc]

        if  { [eof $fdesc] } {
            break
        }


        # Check line, if it is an end tag, start tag or a normal line

        if { [getBlockEnd $line]} {


            # ----> The line is a comment tag and ends the current block

            # Evaluate the last buffered lines
            flushLineBuf

            # Finish the current block
            evalBlockEnd $line

            # Now check, if further blocks are closed by the same tag
            # and finish them, too
            while { [getBlockEnd $line 0] } {
                evalBlockEnd $line 0
            }

        } else {

            # Check line, whether it starts one or more blocks
            # (that means, whether the line is a comment tag)
            set blockTypeList [getBlockType $line]

            if { $blockTypeList != "" } {

                # ----> The line starts one or more blocks

                # Evaluate the last buffered lines
                flushLineBuf

                # Start the new block(s)
                set isFirst 1
                foreach blockType $blockTypeList {
                    evalBlock $blockType $line $isFirst
                    set isFirst 0
                }

            } else {

                # ----> The line is a normal code line
    
                # Append the normal lines to the line buffer
                evalNormalLine $line
            } 
        }
    }

    # Evaluate the last buffered lines
    flushLineBuf


    # Close last open block(s?), if it ends at EOF.
    if { [getBlockEnd "" 1 1] } {
        evalBlockEnd "" 1 1

        # Now check, if further blocks are closed by the same tag
        # and finish them, too
        while { [getBlockEnd "" 0 1] } {
            evalBlockEnd "" 0 1
        }
    }


    # Close the file block
    evalFileBlockEnd

    # Close the file
    close $fdesc


    return $fileBlockID

}


