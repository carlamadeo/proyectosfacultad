//
//      sccsid[] = %W% %Y% %D%
//      StP/OO
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//

int
FileSaveIfModified()
{
    if (gte_is_modified() == True)
	gte_stpem("FileSave");

    /*
     * IN THE GTE, IF THE EDITOR_QRL_EVAL EXPRESSION RETURNS 0,
     * THEN SUCCESS IS REPORTED TO STPEM (THE OPPOSITE IS TRUE
     * FOR THE GDE, FOR SOME REASON)
     */

    return 0;  
}

