
//////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = %W% %Y% %D%
//      StP/SE
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//
//////////////////////////////////////////////////////////////////////////////

string
get_dfe_editor(string editor)

{
    string cur_editor;

    if (editor == "dydfe" || editor == "gsdfe")
	return editor;
    
    cur_editor = editor_get_msgd_name();
    if (cur_editor == "dydfe" || cur_editor == "gsdfe")
	return cur_editor;

    cur_editor = toolinfo_variable("dfe_editor");

    if (cur_editor == NULL || cur_editor == "")
	return "dydfe";
    return cur_editor;
} 




void
DfeNavigateSend(string editor, string command)
{
    editor = get_dfe_editor(editor);

    editor_send_msg(editor, command, CH_FIRST);
}

 
void
DfeNavigateSendQrl(string editor, string command)
{
    editor = get_dfe_editor(editor);
    command = "EditorQrlEvalNoSave " + command;
    editor_send_msg(editor, command, CH_FIRST);
}
 

