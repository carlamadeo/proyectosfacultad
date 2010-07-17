
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved

#include "qrl/include/general.inc"

// set to 1 to turn on debugging messages
int ps_debug = 1;
boolean shared_generalization_flag = False;  // ECR 2653

void
ps_err(string msg)
{
    if (ps_debug)
        print_error(msg);
    return;
}


// ECR 6997 begin
list
parse_funcs_list(string funcs)
{
    list result;
    string curr_char;
    string curr_func;
    int pos = 0;
    int last_pos = 0;
    int length = string_length(funcs);
    int parens_counter = 0;

    result = list_create("string", 0);

    while (pos < length)
    {
        curr_char = string_extract(funcs, pos, 1);
        if (curr_char == "(")
        {
            parens_counter++;
        } 
        else if (curr_char == ")") 
        {
            parens_counter--;
            if (parens_counter < 0)
            {
                // closing paren without matching opening paren, return empty list
                list_clear(result);
                return NULL;
            }
        }
        else if (curr_char == ",")
        {
            if (parens_counter == 0)
            {
                // comma outside function parameters is function separator
                curr_func = string_extract(funcs, last_pos, pos - last_pos);
                curr_func = string_strip(curr_func, "B", " ");
                list_append(result, curr_func);
                last_pos = pos + 1; // skip comma
            }
        }

        pos++;
    }

    if (parens_counter != 0)
    {
        // error in parenthesis matching, return empty list
        list_clear(result);
        return NULL;
    }

    curr_func = string_extract(funcs, last_pos, pos - last_pos);
    curr_func = string_strip(curr_func, "B", " ");
    if (curr_func != "")
        list_append(result, curr_func);

    return result;
}
// ECR 6997 end


void
ps_dump()

{
    int i, j;
    _ps_propsheet_tp propsheet;
    _ps_field_tp field;

    for (i=0; i<list_count(PsL); i++)
    {
        propsheet = list_get(PsL, i);
        ps_err("PropSheet: " + propsheet.name);
        for (j=0; j<list_count(propsheet.propsheet); j++)
        {
            field = list_get(propsheet.propsheet, j);
            ps_err("fieldname: " + field.fieldname + " fieldtype: " + field.fieldtype);
        }
    }
}



// Fieldtypes
const int ps_StringAnt = 1;
const int ps_TrueFalse = 2;
const int ps_StringList = 3;
const int ps_Choice = 4;
// special FieldTypes
const int ps_Class = 5;
const int ps_Role = 6;
const int ps_SeqCollab = 7;
// ECR 150
const int ps_StringChoice = 8;
// ECR 3786
const int ps_ActState = 9;

// Other values
const int ps_Node = 1;
const int ps_Link = 2;
const int ps_Cntx = 3;

// ECR 3789 begin
// AG 991217-01 Add support for editing the NoteDescription with
//           	an external editor directly from the property sheet
			
// ECR 6880 Property sheets from table editors
// moved to gde_qrl_std.inc
// const int ps_NoteDescriptionExternal = 99;
// const boolean ps_NoteDescription_cb_debug = False;
// ECR 6880 end
string tf;
// ECR 3789 end
// Routines for creating field records

// Begin add ECR 5050
const int ps_ChooseAction = 100;
const int ps_ChooseGuard = 101;
const int ps_ChooseInEvent = 102;
const int ps_ChooseOutEvent = 103;
// End add ECR 5050

// private structures.  Please respect that

struct _ps_propsheet_tp

{
    string name;
    string first_field;
    int node_link_cntx; // ps_Node, ps_Link, ps_Cntx
    list which_syms;
    list propsheet;
    int maps_to;       // ps_Node, ps_Link, ps_Cntx
};

struct _ps_field_tp
{
    string fieldname;  // defined name in the rules file
    int fieldtype;     // values above
    string note_name;  // the note
    string item_name;  // the item
    list choicel;      // ordered list of possibilities for ps_Choice
                        // ECR 3789 begin
                        // The following member
                        // has been introduced to support the note description
                        // but has been named so it can be used by
                        // further enhancements
    string generic_string_value;// ECR 3789 end
    boolean modified;  /* sometimes used to determine if a field has
                        * been modified */
};

// private data. Please respect that
list PsL = list_create("_ps_propsheet_tp", 0);

struct _ps_class_assoc_tp
{
    int invertDirection; // ECR 6586
    link fromrole;
    link torole;
    string fromclassname;
    string toclassname;
};
_ps_class_assoc_tp CurAssoc;


void
ps_init(string s1, string s2, int nlc, list which_syms, int maps_to)

{
    _ps_propsheet_tp ps;

    ps.name = s1;
    ps.first_field = s2;
    ps.node_link_cntx = nlc;
    ps.propsheet = list_create("_ps_field_tp", 0);
    ps.which_syms = which_syms;
    ps.maps_to = maps_to;
    list_append(PsL, ps);

    //if (ps.first_field != "")
    //    ui_text_set_readonly(to_ui_prop(ps.name, ps.first_field), True);

    return;
}


void
_ps_update(_ps_propsheet_tp newps)

{
    int i;
    _ps_propsheet_tp ps;

    for (i=0; i<list_count(PsL); i++)
    {
        ps = list_get(PsL, i);
        if (ps.name == newps.name)
        {
            list_set(PsL, i, newps);
            return;
        }
    }
    return;
}

_ps_propsheet_tp
_ps_find(string psname)

{
    int i;
    _ps_propsheet_tp ps;

    for (i=0; i<list_count(PsL); i++)
    {
       ps = list_get(PsL, i);
       if (psname == ps.name)
          return ps;
    }
    return NULL;
}



void
ps_append_field(string psname, string fieldname, string note_name, string item_name, int type, list choicel)

{
    _ps_propsheet_tp ps, pstemp;
    _ps_field_tp field;
    int i;

    field.fieldname = fieldname;
    field.fieldtype = type;
    field.item_name = item_name;
    field.note_name = note_name;
    field.choicel = choicel;
    // ECR 3789 begin
	field.generic_string_value =""; // Init newly introduced member
    // ECR 3789 end
    field.modified = False;

    ps = _ps_find(psname);

    if (ps == NULL)
    {
        ps_err("ps_append_field(): call ps_init first");
        return;
    }

    list_append(ps.propsheet, field);

    _ps_update(ps);

    return;
}


void
_ps_activate(_ps_propsheet_tp ps, boolean t_or_f)

{
    int i;
    _ps_field_tp field;

    if (ps.first_field != "")
        ui_activate(to_ui_prop(ps.name, ps.first_field), t_or_f);
    for (i=0; i < list_count(ps.propsheet); i++)
    {
        field = list_get(ps.propsheet, i);
        ui_activate(to_ui_prop(ps.name, field.fieldname), t_or_f);
    }
//ECR 4540 begin
    if(current_editor_apptype() != "UmlStereotypeDiagram")
    {
        ui_activate(to_ui_prop(ps.name, "ChooseStereotype"), t_or_f);
    }
//ECR 4540 end

	// ECR5304
	// The OK, Rest and Apply buttons must be disabled when
	// the whole property sheet is disabled
	ui_activate(to_ui_prop(ps.name, "OK"), t_or_f);
	ui_activate(to_ui_prop(ps.name, "Reset"), t_or_f);
	ui_activate(to_ui_prop(ps.name, "Apply"), t_or_f);
    return;
}



boolean _ps_ApplyBusy = False;
gde_symbol _ps_LastFill = NULL;

void
ps_reset_last_fill()
{
    _ps_LastFill = NULL;

    return;
}



// function to call to fill a property sheet. Single parameter
// is the name of the property sheet in the rules file.

// ECR 5183 need some mechanism to prevent ps_ValueChanged_cb to get active
boolean disable_ValueChanged_check = False;

void
ps_fill(string psname)

{
    _ps_propsheet_tp ps;
    _ps_field_tp field;
    string label, s;
    gde_symbol sym;
    string str;
    list strl;
    item itm;
    list l;
    node n;
    link lk;
    cntx ctx;
    // ECR 3789 begin
    note nt; // Need a note object
    // ECR 3789 end
    int i, j, val, objid;

    // ECR 4192 begin
    int otherValue = 0;
    // ECR 4192 end
    // ECR 5439 need objid of other end of association roles
    int fromRoleObjId = 0;
    boolean default_modAgg = False;

    // ECR 157
    item from_navi, to_navi;
    boolean default_mod = False;

    // ECR 6335
	// This is a list of all the objects that
	// are being edited by this property sheet
	//
	list lock_check = list_create( "int", 0 );
	file_lock lock_check_file_lock;
    // End ECR 6335

    if (_ps_ApplyBusy)
        return;

    ps = _ps_find(psname);
    if (ps == NULL)
        return;

    // fill the readonly name property
    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() == "gte") {
        if(gte_get_hsect(gte_get_current_cell_row()) != gte_lookup_symbol(list_get(ps.which_syms, 0))
            || gte_get_vsect(gte_get_current_cell_col()) != gte_lookup_symbol(list_get(ps.which_syms, 1))
            || list_count(gte_cell_mapped_oms_nodes(1, 1, gte_get_current_cell_row(), gte_get_current_cell_col())) == 0) {
            _ps_activate(ps, False);
            return;
        }
    }
    else
    // ECR 6880 end
    if ((list_count(gde_selected_symbols()) != 1) ||
        (list_count(gde_symbols_find_with_types(gde_selected_symbols(), ps.which_syms)) == 0))
    {
        _ps_activate(ps, False);
        return;
    }


    // ECR 5304 _ps_activate(ps, True);  

    // the editor sends too many action events, so this guy ends up
    // being called more then needed.  The only problem here is that
    // there are rare cases where one might want to refill the same
    // thing twice, in which case this mechanism makes that difficult.
    // It is set so double click will do a fill no matter what.
    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() == "gte") {
        if(_ps_LastFill != NULL
            && _ps_LastFill.cur_row == gte_get_current_cell_row()
            && _ps_LastFill.cur_col == gte_get_current_cell_col()
            && _ps_LastFill.label == gte_get_current_cell_label()) {
            _ps_activate(ps, True);
            return;
        }
        _ps_LastFill.cur_row = gte_get_current_cell_row();
        _ps_LastFill.cur_col = gte_get_current_cell_col();
        _ps_LastFill.label = gte_get_current_cell_label();
    }
    else {
        sym = list_get(gde_selected_symbols(), 0);
        if (_ps_LastFill == sym) {
            _ps_activate(ps, True);
            return;
        }
        _ps_LastFill = sym;
    }
    // ECR 6880 end

    ui_footer_set_value(to_ui_prop(ps.name, "Status"), "Filling property sheet ...");

    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() == "gte") {
        node gte_node = list_get(gte_cell_mapped_oms_nodes(1, 1, _ps_LastFill.cur_row, _ps_LastFill.cur_col), 0);
        // message("mapped (fill): " + gte_cell_mapped_oms_nodes(1, 1, gte_get_current_cell_row(), gte_get_current_cell_col()));
        objid = gte_node.id;
        label = _ps_temp_label = _ps_LastFill.label;
    }
    else {
        objid = _ps_symbol_objid_and_label(ps, sym, True);
        label = _ps_temp_label;
    }
    // ECR 6880 end

    if (objid == -1)
    {
        _ps_activate(ps, False);

        ps_reset_last_fill();
        return;
    }

    // ECR 5304 start
    // ECR 6880
    // if (label == "" && gde_symbol_is_node(sym))
    if (label == "" && (PropSheet__editor_type() == "gte" || gde_symbol_is_node(sym)))
    {
        if (PropSheet__editor() == "ustated")  // ECR 7171
        {                                      // ECR 7171
        // ECR 5374 begin
	    string nodetype = gde_nt_name(gde_node_nt(to_gde_node(sym)));
        if (nodetype != UML_DEEP_HISTORY_SYMBOL_TYPE && 
            nodetype != UML_SHALLOW_HISTORY_SYMBOL_TYPE)
        // ECR 5374 end
        {
            _ps_activate(ps, False);
            ps_reset_last_fill();
            return;
        }
        } // ECR 7171
    }
    // ECR 5304 end
    
    _ps_activate(ps, True);  // ECR 5304 

    if (ps.first_field != "")
        ui_text_set_value(to_ui_prop(ps.name, ps.first_field), label);

    ui_busy(to_ui_owner(to_ui_prop(ps.name, "Status")), True);

    // ECR 4880 begin
    ui_text_set_readonly(to_ui_prop(ps.name, "ObjectName"), True);
    // ECR 4880 end

    // ECR 5439
    disable_ValueChanged_check = True;
    // now fill the rest of the prop sheet.
    for (i=0; i < list_count(ps.propsheet); i++)
    {
        field = list_get(ps.propsheet, i);

        // ECR 6335
		// lock_check: Do this again and again, because objid changes for
		//			 association while in this loop.
		// 			 So that we than have a list of all objects that are
		//           being annotated by this property sheet.
		//           In most cases this will be a one-element list (UseCases,
		//           ActionStates etc.), but Associations have 3 elements
		//
		if( list_count( lock_check ) == list_find( lock_check, 0 , objid ) )
			list_append( lock_check, objid );
        // End ECR 6335
        
        if (field.fieldtype == ps_StringAnt)
        {
            // normal string annotation.  Get the value, fill the field
            // add note name to query?
            itm = find_by_query("item[${field.item_name} && obj_id = ${objid}]");
            if (itm != NULL && str_has_val(itm.value))
                ui_text_set_value(to_ui_prop(ps.name, field.fieldname), itm.value);
            else
                ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
        }
        else if (field.fieldtype == ps_StringList)
        {
            str = "";
            // a item that is of cardinality 0..n
            // added  && note[${field.note_name}] for ECR 5837
            strl = list_select("item[${field.item_name} && obj_id = ${objid} && note[${field.note_name}]]");
            for (j=0; j<list_count(strl); j++)
            {
                itm = list_get(strl, j);
                if (str_has_val(itm.value))
                    str += itm.value + "\n";
            }
            ui_text_set_value(to_ui_prop(ps.name, field.fieldname), str);
        }
        else if (field.fieldtype == ps_TrueFalse)
        {
            itm = find_by_query("item[${field.item_name} && obj_id = ${objid}]");
            if (itm != NULL)
            {
                if (itm.value == "True")
                    ui_choice_set_value(to_ui_prop(ps.name, field.fieldname), 1);
                else
                    ui_choice_set_value(to_ui_prop(ps.name, field.fieldname), 0);
            }
	    else
                ui_choice_set_value(to_ui_prop(ps.name, field.fieldname), 0);
        }
        else if (field.fieldtype == ps_Choice)
        {
            itm = find_by_query("item[${field.item_name} && obj_id = ${objid}]");
            if (itm == NULL || str_no_val(itm.value))
// ECR4270, removed next 3 lines, default blank visibility is always first value
//                if (field.fieldname == "InheritanceVisibility")
//                    val = 2;
//                else
                    val = 0;
            else
            {
                val = list_find(field.choicel, 0, itm.value);
                if (val == list_count(field.choicel))
                {
                    ps_err("Attempt to set invalid value `" + itm.value + "' for " + field.fieldname + ".");
                    val = 0;
                }
            }
    	    // ECR 4192 begin
    	    if( field.fieldname == "ToAggregationType")
            {
                // ECR 5439
                // check only when the To- field is filled, since only the the From-
                // field has been filled already (see uclassd.ps.qrl)
                // write out a different error when there are no roles!
                if(fromRoleObjId == objid) {
                    print_error("A reflexive association needs role names to distinguish both ends");
                }
                else {
                    otherValue =  ui_value(to_ui_prop(ps.name,"FromAggregationType"));

                    if( val > 0 && otherValue > 0) {
                        print_error("Only one end of an association can be of type aggregation or composition");
                        val = 0; // unset choice
                        default_modAgg = True;
                    }
                }
            }
// ECR 5439 do not check here, because the other side has not been updated yet
//            else if ( field.fieldname == "FromAggregationType")
//            {
//                otherValue =  ui_value(to_ui_prop(ps.name,"ToAggregationType"));
//
//                if( val > 0 && otherValue > 0) {
//                    print_error("Only one end of an association can be of type aggregation or composition");
//                    val = 0; // unset choice
//                }
//            }
    	    // ECR 4192 end

            ui_choice_set_value(to_ui_prop(ps.name, field.fieldname), val);
        }
        // ECR 150
        else if (field.fieldtype == ps_StringChoice)
        {
            itm = find_by_query("item[${field.item_name} && obj_id = ${objid}]");
            if (itm == NULL || str_no_val(itm.value))
                val = 0;
            else
            {
                val = list_find(field.choicel, 0, itm.value);
                if (val == list_count(field.choicel))
                    val = 0;
            }
            ui_choice_set_value(to_ui_prop(ps.name, field.fieldname), val);
        }
        // ECR 150 end
        else if (field.fieldtype == ps_SeqCollab)
        {
            if (field.fieldname == "NameComponent")
            {
                s = uml_message_MessageName(gde_selected_mapsymbol());
                if (str_has_val(s))
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
            else if (field.fieldname == "PredecessorGuard")
            {
                s = uml_message_PredecessorGuard(gde_selected_mapsymbol());
                if (str_has_val(s))
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
            else if (field.fieldname == "MessageArgs")
            {
                s = uml_message_MessageArgs(gde_selected_mapsymbol());
                if (str_has_val(s))
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
            else if (field.fieldname == "ReturnType")
            {
                s = uml_message_ReturnType(gde_selected_mapsymbol());
                if (str_has_val(s))
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
            else if (field.fieldname == "MessageRecurrence")
            {
                s = uml_message_Recurrence(gde_selected_mapsymbol());
                // The above should be fixed, rather then this code
                if (str_has_val(s))
                {
		    if (string_extract(s, 0, 1) == "[") {
                        s = string_extract(s, 1, string_length(s) - 1);
			// check for the end only if the first one was
			// '[', so iteration is handled properly
			// now check for the end, just to be nice
			if (string_extract(s, string_length(s) - 1, 1) == "]")
			    s = string_extract(s, 0, string_length(s) - 1);
		    }
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
                }
                if (str_has_val(s))
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
            else if (field.fieldname == "MessageSequence")
            {
                s = uml_message_SequenceVal(gde_selected_mapsymbol());
                if (str_has_val(s))
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
        }
        // ECR 3786
        else if (field.fieldtype == ps_ActState)
        {
            if (field.fieldname == "TransitionName")
            {
                s = uml_transition_event(gde_selected_mapsymbol());
                if (str_has_val(s)) {
                    j = string_find(s, 0, ":");
                    if(j != 0 && j < string_find(s, 0, "("))
                        s = string_extract(s, 0, j);
                    else
                        s = "";
                }
                if (str_has_val(s))
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
            else if (field.fieldname == "TransitionEvent")
            {
                s = uml_transition_event(gde_selected_mapsymbol());
                if (str_has_val(s)) {
                    j = string_find(s, 0, ":");
                    //ECR 6332 begin
                    //if(j < string_find(s, 0, "(") && string_length(s) > j+1)
                    if (j < string_find(s, 0, "("))
                    //ECR 6332
                        s = string_extract(s, j + 1, string_length(s) - j - 1);
                }
                if (str_has_val(s))
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
            else if (field.fieldname == "TransitionGuard")
            {
                s = uml_transition_guard(gde_selected_mapsymbol());
                if (str_has_val(s))
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
            else if (field.fieldname == "TransitionAction")
            {
                s = uml_transition_action(gde_selected_mapsymbol());
                if (str_has_val(s)) {
                    // ECR 6997 begin
                    //strl = string_to_list(s, ",");
                    //for(j = 0; j < list_count(strl); ++ j) {
                    //    list_set(strl, j, string_strip(list_get(strl, j), "B", " "));
                    //}
                    strl = parse_funcs_list(s);
                    // ECR 6997 end
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), list_to_string(strl, "\n"));
                }
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
            else if (field.fieldname == "TransitionSend")
            {
                s = uml_transition_send_event(gde_selected_mapsymbol());
                if (str_has_val(s)) {
                    // ECR 6997 begin
                    //strl = string_to_list(s, ",");
                    //for(j = 0; j < list_count(strl); ++ j) {
                    //    list_set(strl, j, string_strip(list_get(strl, j), "B", " "));
                    //}
                    strl = parse_funcs_list(s);
                    // ECR 6997 end
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), list_to_string(strl, "\n"));
                }
                else
                    ui_text_set_value(to_ui_prop(ps.name, field.fieldname), "");
            }
        }
        // ECR 3786 end
        else if (field.fieldtype == ps_Role)
        {
            s = "";
            if (field.fieldname == "FromRole")
            {
                if (str_has_val(CurAssoc.fromrole.name ))
                   s = CurAssoc.fromrole.name;
            }
            else
            {
                if (str_has_val(CurAssoc.torole.name ))
                   s = CurAssoc.torole.name;
            }
            ui_text_set_value(to_ui_prop(ps.name, field.fieldname), s);
            //ECR 3855 begin change
    	    // ui_text_set_readonly(to_ui_prop(ps.name, field.fieldname), True);
            //ECR 3855 end change
        }
        // ECR 3789 begin
        // 991217-01 Added filling for the external note description
        else if( field.fieldtype == ps_NoteDescriptionExternal )
        {
            // Implementation for external editing of a note description
            // Note that no actual GUI element is used, but only the info
            // from the note is stored internally to be retrieved later

            nt = find_by_query( "note[${field.note_name} && obj_id = ${objid}]" );
            field.generic_string_value ="";
            if( nt!= NULL && str_has_val( nt.desc ) )
            {
                    field.generic_string_value = nt.desc;
            }
            list_set( ps.propsheet,i, field );
            _ps_update( ps );
        }
        // ECR 3789 end
        else if (field.fieldtype == ps_Class)
        {
            // special association readonly field
            if (field.fieldname == "FromClass")
                ui_text_set_value(to_ui_prop(ps.name, field.fieldname), CurAssoc.fromclassname);
            else
                ui_text_set_value(to_ui_prop(ps.name, field.fieldname), CurAssoc.toclassname);
            // ECR 4371, class names should be read-only here
            ui_text_set_readonly(to_ui_prop(ps.name, field.fieldname), True);
        }
        //else
        //    ps_err("Implement fill for type " + field.fieldtype + ".");

        // since there are annotations from three different objects, we rely on the order that
        // the property sheet was defined in here.
        if (ps.name == "UmlClassLinkAssociation_ps")
        {
            if (field.fieldname == "TaggedValues") {
                objid = CurAssoc.fromrole.id;
                // ECR 5439
                fromRoleObjId = objid;
            }
            if (field.fieldname == "FromOrdered")
                objid = CurAssoc.torole.id;
            // Begin Add ECR 157
            if( field.fieldname == "FromRoleNavigability" || field.fieldname == "ToRoleNavigability" )
            {
                from_navi = find_by_query( "item[UmlRoleNavigability && obj_id==${CurAssoc.fromrole.id}]" );
                to_navi = find_by_query( "item[UmlRoleNavigability && obj_id==${CurAssoc.torole.id}]" );
                // the following combinations mean bi-directional so we set both sides to true
                if( from_navi == NULL && to_navi == NULL )
                {
                    ui_choice_set_value(to_ui_prop(ps.name, field.fieldname), 1);
                    default_mod = True;
                }
                else if( to_navi != NULL && from_navi == NULL && to_navi.value == "False" )
                {
                    ui_choice_set_value(to_ui_prop(ps.name, field.fieldname), 1);
                    default_mod = True;
                }
                else if( from_navi != NULL && from_navi.value == "False" && to_navi == NULL )
                {
                    ui_choice_set_value(to_ui_prop(ps.name, field.fieldname), 1);
                    default_mod = True;
                }
                else if( from_navi != NULL && to_navi != NULL && from_navi.value == "False" && to_navi.value == "False" )
                {
                    ui_choice_set_value(to_ui_prop(ps.name, field.fieldname), 1);
                    default_mod = True;
                }
            }
            // End ECR 157
        }
    }

    // now go through and reset everything to unmodified
    ps = _ps_find(psname);
    for (i=0; i < list_count(ps.propsheet); i++)
    {
        field = list_get(ps.propsheet, i);
        // Begin change ECR 157
        // don't overwrite intentionally changed Navigability fields
        // ECR 5439 I do not understand the if, so I replaced it ...
        if( field.modified == False )
            continue;
        if( ( field.fieldname == "FromRoleNavigability" || field.fieldname == "ToRoleNavigability" ) && default_mod == True )
            continue;
        if( field.fieldname == "ToAggregationType" && default_modAgg == True )
            continue;
//        if( ( !default_mod && field.modified ) ||
//            ( default_mod && ( field.fieldname != "FromRoleNavigability" &&
//              field.fieldname != "ToRoleNavigability" ) ) )
        field.modified = False;
        // End change ECR 157
        list_set( ps.propsheet, i, field );
        _ps_update(ps);
    }

    // ECR 6335
	// Add this point we have all the objects that are edited by 
	// this property sheet in the list
	//
	for( i = 0; i< list_count( lock_check ); i++ )
	{
		// We do two queries, because we do not know if it is a node
		// or a link just from the id.
		//
		if( 0 )
		{
            n = find_by_query( "node[id="+list_get(lock_check,i)+"]" );
            lk = find_by_query( "link[id="+list_get(lock_check,i)+"]" );
    		ps_err( "Checking lock for "+to_string( list_get( lock_check, i ) ));
    		if( n )
    			ps_err( n.name );
    		if( lk )
    			ps_err( lk.name );
		}
		// Now let's see if we have a file lock.
		//
		lock_check_file_lock = find_by_query( "file_lock[file[ObjectAnnotation & contains_notes[obj_id="+list_get(lock_check,i)+"]]]" );
		if( NULL!= lock_check_file_lock )
		{
			if( ! editor_confirm( "This object's annotation is locked by "+lock_check_file_lock.user+".\nAny changes in the property sheet would not be saved, continue ?","OK","Cancel" ))
			{
			    ui_owner_dismiss( to_ui_owner(to_ui_prop(ps.name, "Status")) );
		    }
			break;
		}
	}
    // end ECR 6335
    
    ui_footer_set_value(to_ui_prop(ps.name, "Status"), "");
    ui_busy(to_ui_owner(to_ui_prop(ps.name, "Status")), False);

    // ECR 5439
    disable_ValueChanged_check = False;
    return;
}



boolean
ps_AnyModifiedFromNth(list fields, int n)
{
    int i;
    _ps_field_tp field;
// ECR 2653 begin
    if (shared_generalization_flag == True)
        return True;
// ECR 2653 end

    for(i = n; i < list_count(fields); i++)
    {
        field = list_get(fields, i);
        if (field.modified)
            return True;
        if (field.fieldtype == ps_SeqCollab || field.fieldtype == ps_ActState)
            return True;
    }
    return False;
}

// ECR 2653 begin
int
ps_ApplyHierarchy_cb(ui_event_tp event, ui_prop prop, int id, string value)
{
    gde_arc arc, tmparc;
    gde_symbol symbol;
    int i, j;
    list symbols, arcs, syms, tmpsyms;
    list endn, fromn, inarcs, outarcs;
    
    symbols = gde_selected_symbols();
    if (list_count(symbols) != 1)
    {
        // This should never happen here ...
        ps_err("Focus no longer on an object. Reselect an object first.");
        _ps_activate(ps, False);
        return 0;
    }
    
    symbol = list_get(symbols, 0);
    if (gde_symbol_is_arc(symbol) &&
       (list_count(gde_arc_links(to_gde_arc(symbol))) != 1))
    {
        arc = to_gde_arc(symbol);
        if (gde_lt_name(gde_arc_lt(arc)) != "Generalization")
            return 0;

        syms = list_create("gde_symbol", 0);

        endn = list_create("gde_node", 0);
        fromn = list_create("gde_node", 0);
        
        gde_arc_get_real_tonode(arc, False, endn, NULL);
        gde_arc_get_real_fromnode(arc, False, fromn, NULL);
        
        if (list_count(endn) == 1)
        {
            // single end node
            for (i = 0; i < list_count(fromn); i++)
            {
                outarcs = gde_node_outarcs(list_get(fromn, i));
                for (j = 0; j < list_count(outarcs); j++)
                {
                    tmparc = list_get(outarcs, j);
                    if (list_get(endn, 0) == gde_arc_get_real_tonode(tmparc, False, NULL, NULL))
                        list_append(syms, to_gde_symbol(tmparc));
                }
            }
        }

        if (list_count(fromn) == 1)
        {
            // single start node
            for (i = 0; i < list_count(endn); i++)
            {
                inarcs = gde_node_inarcs(list_get(endn, i));
                for (j = 0; j < list_count(inarcs); j++)
                {
                    tmparc = list_get(inarcs, j);
                    if (list_get(fromn, 0) == gde_arc_get_real_fromnode(tmparc, False, NULL, NULL))
                        list_append(syms, to_gde_symbol(tmparc));
                }
            }
        }

        shared_generalization_flag = True;

        for (i = 0; i < list_count(syms); i++)
        {
            tmpsyms = list_create("gde_symbol", 0);
            list_append(tmpsyms, list_get(syms, i));

            gde_select_symbols_in_list(tmpsyms);
            ps_Apply_cb(event, prop, id, value);
            
            list_clear(tmpsyms);
        }

        shared_generalization_flag = False;

        list_clear(syms);

        // reselect original arc
        tmpsyms = list_create("gde_symbol", 0);
        list_append(tmpsyms, to_gde_symbol(arc));
        gde_select_symbols_in_list(tmpsyms);
        list_clear(tmpsyms);
    }

    return 0;
}
// ECR 2653 end

// Generic callbacks for property sheet management.

int
ps_Apply_cb(ui_event_tp event, ui_prop prop, int id, string value)

{
    int rc, i, j, handle, val, objid;
    ui_owner owner;
    list symbols, l, itml;
    item itm;
    string psname, s, str;
    _ps_propsheet_tp ps;
    _ps_field_tp field;
    string label;
    string new_symbol_label;

    if (ui_button_is_ok(prop))
        rc = 1;
    else
        rc = 0;

    // ECR 157
    boolean set_to_bidirectional = False;

    // get property sheet name
    owner = to_ui_owner(prop);
    psname = ui_owner_name(owner);
    ps = _ps_find(psname);

    // _ps_activate(ps, True);

    // Make sure the property sheet is still valid
    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() != "gte") {
    symbols = gde_selected_symbols();
    if (list_count(symbols) != 1)
    {
        // This should never happen here ...
        ps_err("Focus no longer on an object. Reselect an object first.");
        _ps_activate(ps, False);
        return 0;
    }
    }
    // ECR 6880 end

///////////////////

    // ECR 6880 Property sheets from table editors
    // ECR4824
    // The label for message in the sequence diagram is the only
    // exception in the property sheets: it is called MessageObjectName
    // but not ObjectName
    if (ps.name == "UmlSequenceLink_ps")
            new_symbol_label = ui_text_value(to_ui_prop(ps.name, "MessageObjectName"));
    else
            new_symbol_label = ui_text_value(to_ui_prop(ps.name, "ObjectName"));

    if (new_symbol_label == NULL)
        new_symbol_label = "";

    //if (new_symbol_label != _ps_temp_label) // ECR 2653
    if (new_symbol_label != _ps_temp_label && shared_generalization_flag != True)
    {
        if (PropSheet__editor_type() == "gte") {
	    gte_filltable_string("FillTable { { Hsect { Index Current}" +
				 "{ Vsect { Index Current } { Row { Index Current }" +
				 "{ Cell { Index Current } { Label \"" + 
				 new_symbol_label + "\"}}}}}}");
	    gte_refresh_table();
        }
        else {
            gde_symbol selected_symbol;
            gde_node selected_gde_node;
            gde_arc selected_gde_arc;

        selected_symbol = list_get(symbols, 0);

        //gde_save_state();
        if (gde_symbol_is_node(selected_symbol))
        {
            selected_gde_node = to_gde_node(selected_symbol);
            gde_node_set_label(selected_gde_node, new_symbol_label);
        }
        if (gde_symbol_is_arc(selected_symbol))
        {
            selected_gde_arc = to_gde_arc(selected_symbol);
            gde_arc_set_label(selected_gde_arc, new_symbol_label);
        }
        // action_completed changes objid in mid edit
        //gde_action_completed();
        //_ps_symbol_objid_and_label(ps, selected_symbol, False);
        // JED - in case action_completed() modified the label
        //ui_text_set_value(to_ui_prop(ps.name, "ObjectName"), _ps_temp_label);
    }
    }
    // ECR 6880 end
//////////////////////

	// if there is no change, then just quit
    if (!ps_AnyModifiedFromNth(ps.propsheet, 0))
        return rc;

    // ECR4824
    // Somebody commented the gde_action_completed() call above
    // maybe there was some bug with it, so if there is one, it
    // will be now localized only in the message properties of the
    // sequence diagrams. This call is needed to update the
    // repository so the annotations will be set to the proper
    // object.
    if (ps.name == "UmlSequenceLink_ps" || ps.name == "UmlStateLink_ps")
            gde_action_completed();

    //ECR 6331 begin
    if (ps.name == "UmlStateTransLink_ps")
        gde_action_completed();
    //ECR 6331 end
	
    ui_busy(owner, True);
    _ps_ApplyBusy = True;

    ui_footer_set_value(to_ui_prop(ps.name, "Status"), "Setting properties ...");

    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() == "gte") {
        node gte_node = list_get(gte_cell_mapped_oms_nodes(1, 1, _ps_LastFill.cur_row, _ps_LastFill.cur_col), 0);
        objid = gte_node.id;
    }
    else
        objid = _ps_symbol_objid_and_label(ps, list_get(symbols, 0), False);
    // ECR 6880 end
    label = _ps_temp_label;

	//if (ps.first_field != "") // ECR 2653
	if (ps.first_field != "" && shared_generalization_flag != True)
    {
        ui_text_set_value(to_ui_prop(ps.name, ps.first_field), label);
    }

    // go through the list of fields, and do the right thing with each one
    // ECR 3789 begin
    // Initializing for setting of notedescriptions via
    //					ngoae

    enh_ps_apply_cb_init( );
    // ECR 3789 end

    boolean assoc_roles_created = False; // ECR 7707
    
    // ECR 3855 begin add
    // we need to set the role names before anything else b/c that might create
    // new UmlRoles and UmlAssociation !!!
    if (ps.name == "UmlClassLinkAssociation_ps")
    {
        for (i=0; i<list_count(ps.propsheet); i++)
        {
            field = list_get(ps.propsheet, i);
    	    if (field.fieldtype == ps_Role)
            {
                if (field.fieldname == "FromRole" && field.modified ) {
                    // ECR 6586
                    ps_create_roles(0+CurAssoc.invertDirection, ui_text_value(to_ui_prop(ps.name,"FromRole")), ps.name);
                    assoc_roles_created = True; // ECR 7707
                }
                else if( field.fieldname == "ToRole" && field.modified ) {
                    // ECR 6586
                    ps_create_roles(1-CurAssoc.invertDirection, ui_text_value(to_ui_prop(ps.name,"ToRole")), ps.name);
                    assoc_roles_created = True; // ECR 7707
                }
            }
        }
    }
    // End Add ECR 3855

// ECR 6832 begin

    if (ps.name == "UmlCollaborationCntx_ps")
    {
        for (i = 0; i < list_count(ps.propsheet); i++)
        {
            field = list_get(ps.propsheet, i);
            if (field.fieldtype == ps_SeqCollab)
            {
                if (field.fieldname == "NameComponent")
                {
                    // special handling for Recurrence
                    str = ui_text_value(to_ui_prop(ps.name, "MessageRecurrence"));
                    if (str_has_val(str))
                    {
		                s = string_extract(str, 0, 1);
		                if (s != "[" && s != "*")
			                str = "[" + str;

		                // now check for the end, just to be nice
		                s = string_extract(str, 0, 1);
		                if (s == "[")
		                {
			                s = string_extract(str, string_length(str) - 1, 1);
			                if (s != "]")
			                    str += "]";
		                }

                        if (str == "[]")
                            str = "";
                    }
                    else
                        str = "";

                    uml_set_message_label(gde_selected_mapsymbol(),
                                          ui_text_value(to_ui_prop(ps.name, "PredecessorGuard")),
                                          ui_text_value(to_ui_prop(ps.name, "MessageSequence")),
                                          str,
                                          ui_text_value(to_ui_prop(ps.name, "ReturnType")),
                                          ui_text_value(to_ui_prop(ps.name, "NameComponent")),
                                          ui_text_value(to_ui_prop(ps.name, "MessageArgs")));
                }
            }
        }
    }

    // Reget object id because changing message label above may cause 
    // remapping of message in repository

    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() == "gte") {
        gte_node = list_get(gte_cell_mapped_oms_nodes(1, 1, _ps_LastFill.cur_row, _ps_LastFill.cur_col), 0);
        objid = gte_node.id;
    }
    else
    // ECR 6880 end
        objid = _ps_symbol_objid_and_label(ps, list_get(gde_selected_symbols(), 0), False);
    label = _ps_temp_label;

// ECR 6832 end

    handle = send_start_edit_annot(objid);

    // ECR 6335
	// This is to check if the annotation really has been written
	// We use the file history before and after any data has been stored
	// to disk and see if it changes
	//
	file_hist bef, after;
	list fh=list_create("file_hist", 0 );

	// Get the file histories for this objects annotation and from that
	// list get the last entry, which should be the newest entry 
	//
	fh = list_select( "file_hist[file[ObjectAnnotation & contains_notes[obj_id="+objid+"]]]"  );
	if( fh == NULL || list_count( fh ) == 0 )
		bef = NULL;
	else
		bef = list_get( fh, list_count( fh ) -1 );
    // End ECR 6335
    
    // Begin Add ECR 157
    // special handling for navigability
    if (ps.name == "UmlClassLinkAssociation_ps")
    {
        for (i=0; i<list_count(ps.propsheet); i++)
        {
            field = list_get(ps.propsheet, i);
            if( field.fieldname == "FromRoleNavigability" || field.fieldname == "ToRoleNavigability" )
            {
                if( ( ui_choice_value(to_ui_prop(ps.name, "FromRoleNavigability")) == 0 &&
                      ui_choice_value(to_ui_prop(ps.name, "ToRoleNavigability")) == 0 ) &&
                      ! set_to_bidirectional )
                {
                    print_error( "Non navigable associations are not supported, switching to bidirectional." );
                    set_to_bidirectional = True;
                }
            }
        }
    }
    // End Add ECR 157

    for (i=0; i<list_count(ps.propsheet); i++)
    {
        field = list_get(ps.propsheet, i);

// ECR 2653 begin
        if (shared_generalization_flag)
            field.modified = True;
// ECR 2653 end

        // ECR 7707
        if (ps.name == "UmlClassLinkAssociation_ps" && assoc_roles_created)
            field.modified = True;

        if (field.fieldtype == ps_StringAnt && field.modified)
        {
            // normal string annotation.
            s = ui_text_value(to_ui_prop(ps.name, field.fieldname));
            editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + field.note_name, handle);
            editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + field.item_name, handle);

            if (str_has_val(s))
                editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(s), handle);
            else
                editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, handle);
        }
        else if (field.fieldtype == ps_StringList && field.modified)
        {
            s = ui_text_value(to_ui_prop(ps.name, field.fieldname));
            if (str_has_val(s))
                l = string_to_list(s, "\n");
            else
                l = list_create("string", 0);

            itml = list_select("item[${field.item_name} && obj_id = ${objid}]");

            editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + field.note_name, handle);
            for (j=0; j<list_count(itml); j++)
            {
                itm = list_get(itml, j);

                if (str_has_val(itm.value) && list_find(l, 0, itm.value) == list_count(l))
                {
                    // delete item
                    // Begin ECR 3922
                    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + stpem_string_val( field.item_name + ":" + itm.value ), handle);
                    // End ECR 3922
                    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, handle);
                }
            }

            if (str_has_val(s))
            {
                for (j=0; j<list_count(l); j++)
                {
                    str = list_get(l, j);
                    if (str_has_val(str))
                        // Begin ECR 3922
                        editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + stpem_string_val( field.item_name + ":" + str ), handle);
                        // End ECR 3922
                }
            }

        }
        else if (field.fieldtype == ps_TrueFalse && field.modified)
        {
            val = ui_choice_value(to_ui_prop(ps.name, field.fieldname));

            if (val == 0 ||  val == 1)
            {
                editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + field.note_name, handle);
                editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + field.item_name, handle);
                // ECR 157
                if( ( field.fieldname == "FromRoleNavigability" ||
                      field.fieldname == "ToRoleNavigability" ) && val == 0 )
                    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, handle);
                else if (val == 0)
                // End ECR 157
                    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + False, handle);
                else
                    editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + True, handle);
            }
        }
        // ECR 3856 begin
        else if (field.fieldtype == ps_Choice &&
                (field.fieldname == "FromAggregationType" ||
                 field.fieldname == "ToAggregationType"))
        {
            val = ui_choice_value(to_ui_prop(ps.name, field.fieldname));
            str = list_get(field.choicel, val);
            editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + field.note_name, handle);
            editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + field.item_name, handle);
            editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(str), handle);
        }
        // ECR 3856 end
        else if (field.fieldtype == ps_Choice && field.modified)
        {
            val = ui_choice_value(to_ui_prop(ps.name, field.fieldname));
            // ECR 6849
            // if (val == 0  && field.fieldname != "InheritanceVisibility")
            if (val == 0)
            {
                editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + field.note_name, handle);
                editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + field.item_name, handle);
                editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_DEL, handle);
            }
            else
            {
                str = list_get(field.choicel, val);
                editor_send_msg(NGOAE_CLASS, OAE_CMD_NOTE_SEL + field.note_name, handle);
                editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SEL + field.item_name, handle);
                editor_send_msg(NGOAE_CLASS, OAE_CMD_ITEM_SETVAL + stpem_string_val(str), handle);
            }
        }
        else if (field.fieldtype == ps_SeqCollab)
        {
// ECR 6832 begin
//            if (field.fieldname == "NameComponent")
//            {
//                // special handling for Recurrence
//                str = ui_text_value(to_ui_prop(ps.name, "MessageRecurrence"));
//                if (str_has_val(str))
//                {
//		    s = string_extract(str, 0, 1);
//		    if (s != "[" && s != "*")
//			str = "[" + str;
//
//		    // now check for the end, just to be nice
//		    s = string_extract(str, 0, 1);
//		    if (s == "[")
//		    {
//			s = string_extract(str, string_length(str) - 1, 1);
//			if (s != "]")
//			    str += "]";
//		    }
//
//                    if (str == "[]")
//                        str = "";
//                }
//                else
//                    str = "";
//
//                uml_set_message_label(gde_selected_mapsymbol(),
//                ui_text_value(to_ui_prop(ps.name, "PredecessorGuard")),
//                ui_text_value(to_ui_prop(ps.name, "MessageSequence")),
//                str,
//                ui_text_value(to_ui_prop(ps.name, "ReturnType")),
//                ui_text_value(to_ui_prop(ps.name, "NameComponent")),
//                ui_text_value(to_ui_prop(ps.name, "MessageArgs")));
//            }
// ECR 6832 end
        }
        // ECR 3789 begin
        // Setting the notedescription
        else if( field.fieldtype == ps_NoteDescriptionExternal && field.modified )
        {
            // 991208 AG: Used field.note_name as first argument to temp_file, but
            //			this gives non-unique temp_files, if note_names start with
            //			the same text, eg. KORDOBAEffect KORDOBAShortDesc

            tf = temp_file( to_string(i),
                    to_string( ps_NoteDescriptionExternal ) );

            // 991208 AG: Was write_file, changed to be append_file, otherwise all other
            //			  entries in the file are removed
            append_file( user_set_annot_file,
                                    to_string(objid) +":"+field.note_name+":NULL:NULL:"+tf+"\n" );

            write_file( tf, field.generic_string_value );
            //message(to_string(get_version()));

        }			
        // ECR 3789 end
        //else if (field.fieldtype == ps_Role)
        //{
        //    ps_err("Implement Role name set from psheet");
        //}
        //else
        //    ps_err("Implement apply for type " + field.fieldtype + ".");

        if ( field.modified )
        {
            field.modified = False;
            list_set( ps.propsheet, i, field );
            _ps_update(ps);
        }

        // since there are annotations from three different objects, we rely on the order that
        // the property sheet was defined in here.
        if (ps.name == "UmlClassLinkAssociation_ps")
        {
            if (field.fieldname == "TaggedValues")
            {
                if (!ps_AnyModifiedFromNth(ps.propsheet, i + 1))
                    break;
                // ECR 7174
                // send_oae_save_update_unload(handle);
                send_oae_save_update_unload_sync(handle);
                objid = CurAssoc.fromrole.id;
                // ECR 7174
                // handle = send_start_edit_annot(objid); // ECR 7450
                editor_send_msg(NGOAE_CLASS, OAE_CMD_ANNOT_EDIT + objid, handle); // ECR 7450
            }
            if (field.fieldname == "FromOrdered")
            {
                if (!ps_AnyModifiedFromNth(ps.propsheet, i + 1))
                    break;
                // ECR 7174
                // send_oae_save_update_unload(handle);
                send_oae_save_update_unload_sync(handle);
                objid = CurAssoc.torole.id;
                // ECR 7174
                // handle = send_start_edit_annot(objid); // ECR 7450
                editor_send_msg(NGOAE_CLASS, OAE_CMD_ANNOT_EDIT + objid, handle); // ECR 7450
            }
        }
    }

    // ECR 3789 begin
    enh_ps_apply_cb_post( handle );
    // ECR 3789 end
    send_oae_save_update_unload(handle);

    // ECR 6335
	// Now we loop 5 times through the list. After each loop we
	// do a sleep of 1 sec, so we take 5 sec at most.
	// This might give wrong results if the ngoae takes longer than
	// 5 seconds!
	//
	// We only do this if a TI Var is set. At a later time, we could
	// use this TI Var for specifying the number of retries.
	//
	if( NULL != toolinfo_variable( "SyncPropSheetTimeout" ) )
	{
    	for( i = 5; i >0; i-- )
    	{
    		// Get the latest file_hist entry again!
    		//
    		fh = list_select( "file_hist[file[ObjectAnnotation & contains_notes[obj_id="+objid+"]]]"  );
    		if( fh == NULL || list_count( fh ) == 0 )
    			after = NULL;
    		else
    			after = list_get( fh, list_count( fh ) -1 );
    
    		// Compare times. If we have a newer time, break out of loop
    		//
    		if( bef == NULL && after != NULL )
    			break; 
    
    		if( after != NULL && after.time > bef.time )
    			break;
    
    		//		ps_err( to_string( after.time) +":"+ to_string( bef.time ));
    		sleep( 1 );
    	}
    
    	// If we came here by means of the loop
    	if( i <=0 )
    		ps_err( "Annotations probably were not saved to due a communications problem. Please recheck" );
    }
    // End ECR 6335
    
    // The selected symbol's label can change on Apply action.
    // Refresh it. fix #13487.
    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() == "gte") {
        gte_node = list_get(gte_cell_mapped_oms_nodes(1, 1, _ps_LastFill.cur_row, _ps_LastFill.cur_col), 0);
        objid = gte_node.id;
    }
    else
    // ECR 6880 end
        objid = _ps_symbol_objid_and_label(ps, list_get(symbols, 0), False);
    label = _ps_temp_label;

    //if (ps.first_field != "") // ECR 2653
    if (ps.first_field != "" && shared_generalization_flag != True)
        ui_text_set_value(to_ui_prop(ps.name, ps.first_field), label);

    ui_busy(owner, False);
    _ps_ApplyBusy = False;
    ui_footer_set_value(to_ui_prop(ps.name, "Status"), "Done Setting properties ...");

    return rc;
}



int
ps_Cancel_cb(ui_event_tp event, ui_prop prop, int id, string value)

{
    //ps_err("Cancel CB");
    //ps_Reset_cb(event, prop, id, value);
    _ps_LastFill=NULL; // ECR 4038
    return 0;
}


int
ps_Reset_cb(ui_event_tp event, ui_prop prop, int id, string value)

{
    ui_owner owner;
    string psname;

    owner = to_ui_owner(prop);
    psname = ui_owner_name(owner);
    _ps_LastFill = NULL;

    ps_fill(psname);

    return 1;
}

int
ps_Select_cb(ui_event_tp event, ui_prop prop, int id, string value)

{
    ps_err("Select CB");
    return 0;
}

int
ps_MessageValueChanged_cb(ui_event_tp event, ui_prop prop, int id, string value)
{
    string s;
    ui_owner owner = to_ui_owner(prop);
    string theMessageName = ui_text_value(to_ui_prop(owner, "NameComponent"));
    string theReturnType = ui_text_value(to_ui_prop(owner, "ReturnType"));
    string theMessageArgs = ui_text_value(to_ui_prop(owner, "MessageArgs"));
    string thePredecessorGuard = ui_text_value(to_ui_prop(owner, "PredecessorGuard"));
    string theSequenceValue = ui_text_value(to_ui_prop(owner, "MessageSequence"));

    string str = ui_text_value(to_ui_prop(owner, "MessageRecurrence"));
    if (str_has_val(str))
    {
        s = string_extract(str, 0, 1);
        if (s != "[" && s != "*")
        str = "[" + str;

        // now check for the end, just to be nice
        s = string_extract(str, 0, 1);
        if (s == "[")
        {
            s = string_extract(str, string_length(str) - 1, 1);
            if (s != "]")
               str += "]";
        }

        if (str == "[]")
        str = "";
    }
    else
       str = "";

    string theMessageRecurrence = str;
    string newLabel = "";
    if(thePredecessorGuard!="" && thePredecessorGuard!=NULL)
        newLabel = thePredecessorGuard + "/";
    string theSequenceExpression = "";
    if(theSequenceValue!="" && theSequenceValue!=NULL)
        theSequenceExpression = theSequenceValue;
    if(theMessageRecurrence!="" && theMessageRecurrence!=NULL)
        theSequenceExpression += theMessageRecurrence;
    if(theSequenceExpression!="" && theSequenceExpression!=NULL)
        newLabel += theSequenceExpression + ":";
    if(theReturnType!="" && theReturnType!=NULL)
    newLabel += theReturnType + ":=";
    newLabel += theMessageName + "(" + theMessageArgs + ")";

    if (newLabel == "()")
        newLabel = "";

    ui_text_set_value(to_ui_prop(owner, "MessageObjectName"), newLabel);

    return 0;

}

// ECR 3786
int
ps_TransitionValueChanged_cb(ui_event_tp event, ui_prop prop, int id, string value)
{
    ui_owner owner = to_ui_owner(prop);
    ui_prop p;
    string theTransitionName = "";
    string theTransitionEvent = "";
    string theTransitionParameter = "";
    string theTransitionGuard = ui_text_value(to_ui_prop(owner, "TransitionGuard"));
    string theTransitionAction = ui_text_value(to_ui_prop(owner, "TransitionAction"));
    string theTransitionSend = "";

    if((p = to_ui_prop(owner, "TransitionName")) != NULL)
    theTransitionName = ui_text_value(p);
    if((p = to_ui_prop(owner, "TransitionEvent")) != NULL)
    theTransitionEvent = ui_text_value(p);
    if((p = to_ui_prop(owner, "TransitionParameter")) != NULL)
    theTransitionParameter = ui_text_value(p);
    if((p = to_ui_prop(owner, "TransitionSend")) != NULL)
    theTransitionSend = ui_text_value(p);

    string newLabel = theTransitionName;
    if(newLabel != "")
    newLabel += ":";
    if(theTransitionEvent != "")
    newLabel += theTransitionEvent;
    if(theTransitionParameter != "")
    newLabel += "(" + string_search_and_replace(string_strip(theTransitionParameter, "B", "\n\r "), "\n", ", ") + ")";
    if(theTransitionGuard != "")
    newLabel += "[" + theTransitionGuard + "]";
    if(theTransitionAction != "")
    newLabel += "/" + string_search_and_replace(string_strip(theTransitionAction, "B", "\n\r "), "\n", ",");
    if(theTransitionSend != "") {
        if(theTransitionAction == "")
            newLabel += "/";
        newLabel += "^" + string_search_and_replace(string_strip(theTransitionSend, "B", "\n\r "), "\n", ",");
    }

    ui_text_set_value(to_ui_prop(owner, "ObjectName"), newLabel);

    return 0;

}
// ECR 3786 End

// called to handle an event when a special field gets modified in any way.
// we can keep track of what has been modified in flags, and only write
// things into annotations if they have been changed
int
ps_ValueChanged_cb(ui_event_tp event, ui_prop prop, int id, string value)
{
    ui_owner owner = to_ui_owner(prop);
    string psname = ui_owner_name(owner);
    _ps_propsheet_tp ps = _ps_find(psname);
    _ps_field_tp field;
    int i;

    if (ps == NULL)
        return 0;

    // ECR 150
    int val;
    _ps_field_tp field_choice;
    // ECR 150 end
    // ECR 4192 begin
    int     otherValue = 0;
    // ECR 4192 end

    string fieldname = ui_prop_name(prop);

    for (i=0; i<list_count(ps.propsheet); i++)
    {
        field = list_get(ps.propsheet, i);
        if (field.fieldname == fieldname)
        {
    	    // ECR 150
    	    if (field.fieldtype == ps_StringChoice)
    	    {
                val = ui_choice_value(prop);
                if (val == 0)
                    return 0;
                ++ i;
                if (i >= list_count(ps.propsheet))
                    return 0;
                field_choice = field;
                field = list_get(ps.propsheet, i);
                ui_text_set_value(to_ui_prop(ps.name, field.fieldname), list_get(field_choice.choicel, val));
    	    }
    	    else if (field.fieldtype == ps_StringAnt && i > 0)
    	    {
                field = list_get(ps.propsheet, i - 1);
                if(field.fieldtype == ps_StringChoice)
                    ui_choice_set_value(to_ui_prop(ps.name, field.fieldname), 0);
                field = list_get(ps.propsheet, i);
    	    }
    	    // ECR 150 end

    	    // ECR 4192 begin
            // ECR 5439 do not check while filling the property window
    	    else if( disable_ValueChanged_check != True && field.fieldtype == ps_Choice )
    	    {
                val = ui_choice_value(to_ui_prop(ps.name, field.fieldname));
                if( field.fieldname == "ToAggregationType")
                {
                    otherValue =  ui_value(to_ui_prop(ps.name,"FromAggregationType"));

                    if( val > 0 && otherValue > 0) {
                        print_error("Only one end of an association can be of type aggregation or composition");
                        // unset choice
                        // ECR 5439 reset the other end
                        ui_choice_set_value(to_ui_prop(ps.name, "FromAggregationType"), 0);
                        // we have to reread, because the call above causes a recursive call of this function
                        ps = _ps_find(psname);
                    }
                }
                else if ( field.fieldname == "FromAggregationType")
                {
                    otherValue =  ui_value(to_ui_prop(ps.name,"ToAggregationType"));

                    if( val > 0 && otherValue > 0) {
                        print_error("Only one end of an association can be of type aggregation or composition");
                        // unset choice
                        // ECR 5439 reset the other end
                        ui_choice_set_value(to_ui_prop(ps.name, "ToAggregationType"), 0);
                        // we have to reread, because the call above causes a recursive call of this function
                        ps = _ps_find(psname);
                    }
                }
    	    }
            // ECR 4192 end

            if (!field.modified)
            {
                field.modified = True;
                list_set( ps.propsheet, i, field );
                _ps_update(ps);
            }
            return 0;
        }
    }
    return 0;
}



int
ps_DeSelect_cb(ui_event_tp event, ui_prop prop, int id, string value)

{
    ps_err("DeSelect CB");
    return 0;
}



// misc. helpers

string _ps_temp_label;

int
_ps_symbol_objid_and_label(_ps_propsheet_tp ps, gde_symbol sym, boolean fill)

{
    gde_node gn = NULL;
    gde_cntx gc = NULL;
    gde_link gl = NULL;
    node n;
    // --- START PATCH ECR 6726 ---
    node temp_node; 
    int idx;
    // --- END PATCH ECR 6726 ---
    cntx ctx;
    link lk;
    list l;
    int i;

    _ps_temp_label = "";

    // ECR 5184 begin
    // if (ps.node_link_cntx == ps_Node)
    if (ps.node_link_cntx == ps_Node && gde_symbol_is_node(sym))
    // ECR 5184 begin
    {
        gn = to_gde_node(sym);
        _ps_temp_label = gde_node_label(gn);

        if (ps.maps_to == ps_Node)
            l = gde_node_oms_nodes(gn);
        else if (ps.maps_to == ps_Link)
            l = gde_node_oms_links(gn);
        else
            l = gde_node_oms_cntxs(gn);
    }
    // ECR 5184 begin
    // else if (ps.node_link_cntx == ps_Link)
    else if (ps.node_link_cntx == ps_Link && gde_symbol_is_arc(sym))
    // ECR 5184 end
    {
        gl = list_get(gde_arc_links(to_gde_arc(sym)), 0);
        _ps_temp_label = gde_link_label(gl);

        if (ps.maps_to == ps_Node)
            l = gde_link_oms_nodes(gl);
        else if (ps.maps_to == ps_Link)
            l = gde_link_oms_links(gl); 
        else
            l = gde_link_oms_cntxs(gl);
    }
    // ECR 5184 begin
    // else // ps_Cntx
    else if (ps.node_link_cntx == ps_Cntx && gde_symbol_is_cntx(sym))
    // ECR 5184 end
    {
        gc = to_gde_cntx(sym);
        _ps_temp_label = gde_cntx_label(gc);

        if (ps.maps_to == ps_Node)
            l = gde_cntx_oms_nodes(gc);
        else if (ps.maps_to == ps_Link)
            l = gde_cntx_oms_links(gc);
        else
            l = gde_cntx_oms_cntxs(gc);
    }
    // ECR 5184 begin
    else
        return -1;
    // ECR 5184 end

    if (l == NULL || list_count(l) == 0)
        return -1;

    if (ps.maps_to == ps_Node)
    {
        n = list_get(l, 0);

    // Special case for instantiated class, where
    // we need to use the UmlClass
    if (n.type == "UmlInstantiatedClass")
        n = list_get(l, 1);
    
    // Special case for ObjectInstance
    // --- START PATCH ECR 6726 --- (obj_ps_symbol_ol.repl2)
    if (n.type == "UmlObjectClassScope") {
        idx = 1;
        // n = list_get(l, 1);
        while (idx < list_count(l)) {
            temp_node = list_get(l, idx);
            if (temp_node.type == "UmlObjectInstance") {
                n = temp_node;
                break;
            }
            idx = idx + 1;
        }
    }
    // --- END PATCH ECR 6726 ---


        if (fill && (n.type == "UmlAssociation"))
// ECR 5501 begin
            //_ps_get_assoc_info(n);
            if (_ps_get_assoc_info(n) == -1) 
                return -1;
// ECR 5501 end
        return n.id;
    }
    else if (ps.maps_to == ps_Link)
    {
        lk = list_get(l, 0);
	return lk.id;
    }
    else //(ps.maps_to == ps_Cntx)
    {
        ctx = list_get(l, 0);
        return ctx.id;
    }
}



// ECR 5501 begin
//void
int
// ECR 5501 end
_ps_get_assoc_info(node n)

{
    int colon;
    int comma;
    int len;
    link l;
    list ll;
    
// ECR 5501 begin
    if (n.name == "")
    {
        print_error("Classes which participate in an association or aggregation must be named.");
        return -1;
    }
// ECR 5501 end

    len = string_length(n.name);
    colon = string_find(n.name, 0, ":");

    if (colon == len) // unlabeled link
    {
        comma = string_find(n.name, 0, ",");
        CurAssoc.fromclassname = string_extract(n.name, 0, comma);
    }
    else
    {
        comma = string_find(n.name, colon, ",");
        CurAssoc.fromclassname = string_extract(n.name, colon + 1, comma - colon - 1);
    }
    CurAssoc.toclassname = string_extract(n.name, comma + 1, len - comma - 1);

    CurAssoc.fromrole = NULL;
    CurAssoc.torole = NULL;

    // ECR 7707
    string assoc_signature; assoc_signature = n.sig;

    ll = list_select("link[to_node_id = ${n.id}]");
    l = list_get(ll, 0);
    n = find_by_query("node[id = ${l.from_node_id}]");

    // this arbitrarily picks a direction when the assoc is unnamed
    if (n.name == CurAssoc.fromclassname)
    {
        CurAssoc.fromrole = l;
        if (list_count(ll) == 1) // reflexive association
            CurAssoc.torole = CurAssoc.fromrole;
        else
            CurAssoc.torole = list_get(ll, 1);
    }
    else
    {
        CurAssoc.torole = l;
        if (list_count(ll) == 1) // reflexive association
            CurAssoc.fromrole = CurAssoc.torole;
        else
            CurAssoc.fromrole = list_get(ll, 1);
    }
// ECR 7707 begin
    if (CurAssoc.fromclassname == CurAssoc.toclassname && list_count(ll) > 1)
    {
        // reflexive association with 2 roles
        string ro1, ro2;
        colon = string_find(assoc_signature, 0, ":");
        ro1 = string_extract(assoc_signature, 0, colon);
        ro2 = string_extract(assoc_signature, colon + 1, string_length(assoc_signature) - colon - 1);

        if (CurAssoc.fromrole.name == ro1 && CurAssoc.torole.name == ro2)
        {
            // roles are OK
        }
        else if (CurAssoc.fromrole.name == ro2 && CurAssoc.torole.name == ro1)
        {
            // roles need to be swaped
            link tmp_link;
            tmp_link = CurAssoc.fromrole;
            CurAssoc.fromrole = CurAssoc.torole;
            CurAssoc.torole = tmp_link;
        }
        else
        {
            // something goes wrong
            print_error("Inconsistency in association's roles.");
        }
    }
// ECR 7707 end
    // ECR 6586 - start
    if(gde_node_label(gde_link_fromnode(list_get(gde_arc_links(to_gde_arc(list_get(gde_selected_symbols(), 0))), 0))) != CurAssoc.fromclassname)
        CurAssoc.invertDirection = 1;
    else
        CurAssoc.invertDirection = 0;
    // ECR 6586 - end
// ECR 7707 - rolled back
//// Begin ECR 7707
//// Always invert direction if the association is reflexive
//    if (  CurAssoc.torole.from_node_id == CurAssoc.fromrole.from_node_id ) {
//        CurAssoc.invertDirection = 1;
////        message ("This is a reflexive assoc\n");
//    }
//// End ECR 7707
    return 0; // ECR 5501
}

//  ECR 3162 begin
// This is called when a user clicks on "Choose..." next to a stereotype
// input filed. It pops up and initializes the property sheet
//
int
ps_ChooseStereotype_cb(ui_event_tp event, ui_prop prop, int id, string value)
{
	ui_owner owner;
	string psname;
	list row_names;
	string s;	
	string tn;
    owner = to_ui_owner(prop);
    psname = ui_owner_name(owner);
	ui_prop_sheet_pop_up( "StereotypeChooser" );


	// Init all the fields
	//
	fill_stereot_ps( psname );
	
	// Register the callback, so we can deactivate the StereotypeChooser
	// sheet at the time a different object gets selected by the user
	//
//        gde_builtin("EditorActionDone 0214951139  StereotypeChooser_deactivate();"); // ECR 6880

	// Make sure all fields are active upon activation. This is necessary,
	// because they could have been deactivated by the above callback
	// previously
	//
	StereotypeChooser_activate( True );	


	return 0;
}

int
is_stereotype_editor() {
    string edt = current_editor_apptype();
    print_message( edt );
    return 1;
}
//  ECR 3162 end

// ECR 3786 begin
// Note: This is also used for Guards in seq_collab_messages.prop
int
ps_ChooseGuard_cb(ui_event_tp event, ui_prop prop, int id, string value)
{
    ui_owner owner;
    string psname;
    list row_names;
    string s;
    string tn;
    owner = to_ui_owner(prop);
    psname = ui_owner_name(owner);
    ui_prop_sheet_pop_up( "GuardChooser" );


    // Init all the fields
    //
    fill_guard_ps( psname );

    // Register the callback, so we can deactivate the StereotypeChooser
    // sheet at the time a different object gets selected by the user
    //
//    gde_builtin("EditorActionDone 0214951139  GuardChooser_deactivate();"); // ECR 6880

    // Make sure all fields are active upon activation. This is necessary,
    // because they could have been deactivated by the above callback
    // previously
    //
    GuardChooser_activate( True );


    return 0;
}

int
ps_ChooseInEvent_cb(ui_event_tp event, ui_prop prop, int id, string value)
{
    ui_owner owner;
    string psname;
    list row_names;
    string s;
    string tn;
    owner = to_ui_owner(prop);
    psname = ui_owner_name(owner);
    ui_prop_sheet_pop_up( "InEventChooser" );


    // Init all the fields
    //
    fill_inevent_ps( psname );

    // Register the callback, so we can deactivate the StereotypeChooser
    // sheet at the time a different object gets selected by the user
    //
//    gde_builtin("EditorActionDone 0214951139  InEventChooser_deactivate();"); // ECR 6880

    // Make sure all fields are active upon activation. This is necessary,
    // because they could have been deactivated by the above callback
    // previously
    //
    InEventChooser_activate( True );


    return 0;
}

int
ps_ChooseOutEvent_cb(ui_event_tp event, ui_prop prop, int id, string value)
{
    ui_owner owner;
    string psname;
    list row_names;
    string s;
    string tn;
    owner = to_ui_owner(prop);
    psname = ui_owner_name(owner);
    ui_prop_sheet_pop_up( "OutEventChooser" );


    // Init all the fields
    //
    fill_OutEvent_ps( psname );

    // Register the callback, so we can deactivate the StereotypeChooser
    // sheet at the time a different object gets selected by the user
    //
//    gde_builtin("EditorActionDone 0214951139  OutEventChooser_deactivate();"); // ECR 6880

    // Make sure all fields are active upon activation. This is necessary,
    // because they could have been deactivated by the above callback
    // previously
    //
    OutEventChooser_activate( True );


    return 0;
}

// Note: This is also used for Operations (name component) in seq_collab_messages.prop
int
ps_ChooseAction_cb(ui_event_tp event, ui_prop prop, int id, string value)
{
    ui_owner owner;
    string psname;
    list row_names;
    string s;
    string tn;
    owner = to_ui_owner(prop);
    psname = ui_owner_name(owner);
    ui_prop_sheet_pop_up( "ActionChooser" );


    // Init all the fields
    //
    fill_Action_ps( psname );

    // Register the callback, so we can deactivate the StereotypeChooser
    // sheet at the time a different object gets selected by the user
    //
//    gde_builtin("EditorActionDone 0214951139  ActionChooser_deactivate();"); // ECR 6880

    // Make sure all fields are active upon activation. This is necessary,
    // because they could have been deactivated by the above callback
    // previously
    //
    ActionChooser_activate( True );


    return 0;
}

// ECR 3786 end

// ECR 3789 begin
//              This is the function that is being called when the
// 				Description button is hit on a propsheet

// Some users wanted to see the name of the object as a temporary file.
// So we provide that functionality. However, the temp_file function is
// the default, as it is safe on all platforms. We activate the
// other functionality by a switch
//

#include "ct/rules/qrl/edit_note_desc.inc"

int
ps_NoteDescription_cb(ui_event_tp event, ui_prop prop, int id, string value)
{
    ui_owner owner = to_ui_owner(prop);
    string psname = ui_owner_name(owner);
    _ps_propsheet_tp ps = _ps_find(psname);
    _ps_field_tp field;
    int i,j,k;
    int objid;


    string dummy, dummy2;
    string itemname;

    if (ps == NULL)
        return 0;

    //
    // The target field is derived from the name of the button without
    // the first characters (i.e. NoteDesc )
    //

    string fieldname = ui_prop_name(prop);

    i = enh_ps_field_index( ps, fieldname );
    if( -1 == i )
    {
	ps_err("Button has invalid target "+fieldname+" in ps.inc/ps_NoteDescription_cb()" );
	return 0;
    }
    field = list_get(ps.propsheet, i);


    // 20000111 AG: Add handling for note description templates
    //
   		
   		
    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() == "gte") {
        node gte_node = list_get(gte_cell_mapped_oms_nodes(1, 1, _ps_LastFill.cur_row, _ps_LastFill.cur_col), 0);
        objid = gte_node.id;
    }
    else
    // ECR 6880 end
        objid = _ps_symbol_objid_and_label(ps, list_get(gde_selected_symbols(), 0), True);
        
    string tmp_file = ps_NoteDescription_temp_file( objid );
    if( field.generic_string_value != "" && field.generic_string_value != NULL )
    {
	write_file( tmp_file, field.generic_string_value );
    }
    else
    {

	node n;
	link l;
	string objectType;
   		
	n = find_by_query("node[id="+objid+"]" );
	if( n != NULL )
	    objectType = to_string( n.type );
	else
	{
	    l = find_by_query( "link[id="+objid+"]" );
	    objectType = to_string(l.type );
	}
	write_file( tmp_file, GetDefaultDescription( objid, objectType,
						     field.note_name ) );
    }
    if( NULL == toolinfo_variable("NoteDescEditor" ) )
    {
	ui_prop_sheet_pop_up( "UmlObjectDescriptionSheet" );
	fill_objectdescription_ps( psname, field, i, tmp_file, objid, field.note_name );
//	gde_builtin("EditorActionDone 0214951139  ObjectDescriptionPS_deactivate();"); // ECR 6880
	ObjectDescriptionPS_activate( True );

    }
    else
    {

	ps_edit_note_desc( tmp_file );

	if( ps_NoteDescription_cb_debug )
	    message( "New Desc:"+read_file( tmp_file ) ); // ECR 8008 - moved ':' inside string
	
	// For efficency check if the text has changed, so we do only
	// bother the (ng)oae if we really have to
	
	string newtext = read_file( tmp_file );
	

	if( newtext != field.generic_string_value )
	{
	    // We can think of different GUI policies:
	    // 1. After editing the Note Description with the external editor,
	    //    the user has to click OK on the PS to commit the changes.
	    //    I think, this makes the most sense.
	    // 2. Immediately set the Note Description after closing the
	    //    external editor. It seems, this is what the users expect
	    //    (at least the users at BMW).
	    //    So I chose to  make the 2nd policy standard but I still keep
	    //    the first policy in the code and allow it to be activated
	    //    by A TI.

	    if( NULL != toolinfo_variable("NoteDescWaitForOK" ) )
	    {
		field.modified = True;
		field.generic_string_value = newtext;
		list_set( ps.propsheet,i, field );
		_ps_update( ps );
	    }
	    else
	    {
		message("Setting directly" );
		message("Setting directly" );
	        string stmp_file = temp_file("not","xx" );
		string tf = temp_file( to_string(objid),
				       to_string( ps_NoteDescriptionExternal ) );
			
		write_file( stmp_file,
			     to_string(objid) +":"+field.note_name+":NULL:NULL:"+tf+"\n" );

		write_file( tf, newtext );
			
		int handle = editor_send_msg( NGOAE_CLASS,
					      "EditorQrlEval SetAnnotValuesFromFiles(\""+
					      string_escape(stmp_file,"\\")+"\");",
					      -1 );
			
		editor_send_msg( NGOAE_CLASS,
				 "EditorQrlEval 	CleanAnnotTmpFiles(\""+
				 string_escape(stmp_file,"\\")+"\");",
				 handle );

		// We still want this for later comparisons, if the text changed (i.e.,
		// if the button is clicked again. Note that the modified flag is _not_
		// set
		//
		field.generic_string_value = newtext;
		list_set( ps.propsheet,i, field );
		_ps_update( ps );

	    }
	}
	else
	{
	    if( ps_NoteDescription_cb_debug )
		message( "No Change" );
	}
    }
    delete_file( tmp_file );


    return 0;
}

string note_name_global_for_edit_note_desc = "";
// This is the routine for editing a note description
// with the CTRL-T accelerator
//
int
ps_NoteDescription_cb_acc(string note_name )
{

  int objid;
  node nod;
  link l;
  string objectType;
  note n;
  cntx cn;
  gde_symbol sym = list_get(gde_selected_symbols(), 0);
  list hl =   	gde_symbol_to_oms (sym );

  // This would break when called for an object that
  // does not yet have a name. Fix this. Now print
  // message similar to the code when trying to
  // press CTRL-A for an unnamed object.
  //
  if( hl == NULL || list_count( hl ) == 0 )
    {
      ps_err( "Object needs a name." );
      ps_err( "Edit object description failed." );
      return 0;
    }

  if( list_count( hl ) > 1 )
    {
      editor_item_selector( hl, "objectEditSelectorCB();", "Select object to edit",True );
      note_name_global_for_edit_note_desc = note_name;
      return 0;
    }
  else
    {
  if( list_get_type( hl,0) == "node" )
    {
      nod = list_get( hl, 0 );
      objid = nod.id;
      objectType = to_string( nod.type );
    }
  // allow also cntx fields
  else if( list_get_type( hl,0) == "cntx" )
    {
      cn = list_get( hl, 0 );
      objid = cn.id;
      objectType = to_string( cn.type );
    }
  else
    {
      l = list_get( hl, 0 );
      objid = l.id;
      objectType = to_string( l.type );
    }
    }
  return edit_note_desc_2ndlevel( note_name, objid, objectType );
}
void
objectEditSelectorCB()
{
  node n;
  link l;
  cntx c;
  int objid;
  string objectType;
  int oid = list_get (editor_selector_selected_objects(), 0) ;
  		
  if( n = find_by_query("node[id="+oid+"]" ) )
    {
	  objid = n.id;
	  objectType = to_string( n.type );
    }
  else if( l = find_by_query("link[id="+oid+"]" ) )
    {
	  objid = l.id;
	  objectType = to_string( l.type );
    }
  else if( c = find_by_query("cntx[id="+oid+"]" ) )
    {
	  objid = c.id;
	  objectType = to_string( c.type );
    }

  edit_note_desc_2ndlevel( note_name_global_for_edit_note_desc, objid, objectType );
}
int edit_note_desc_2ndlevel( string note_name,int objid, string objectType )
{
    // ECR 6335
    // Now let's see if we have a file lock.
    //
    file_lock lock_check_file_lock = find_by_query( "file_lock[file[ObjectAnnotation & contains_notes[obj_id="+objid+"]]]" );
    if( NULL!= lock_check_file_lock )
    {
        if( ! editor_confirm( "This object's description is locked by "+lock_check_file_lock.user+".\nAny changes in the description would not be saved, continue ?","OK","Cancel" ))
        {
            return 0;
        }
    }
    // end ECR 6335

  note n;
  string tmp_file = ps_NoteDescription_temp_file( objid );
  n = find_by_query( "note[${note_name} && obj_id=${objid}]" );
	
	
  if( n != NULL && n.desc != NULL && n.desc != "" )
    {
      write_file( tmp_file, n.desc );
    }
  else
    {
   		
      write_file( tmp_file, GetDefaultDescription( objid, objectType,
						   note_name ) );
    }
  if( NULL == toolinfo_variable("NoteDescEditor" ) )
    {
      ui_prop_sheet_pop_up( "UmlObjectDescriptionSheet" );
      fill_objectdescription_ps( "", NULL, 0, tmp_file, objid, note_name );
      od_notetype = note_name;
      //gde_builtin("EditorActionDone 0214951139  ObjectDescriptionPS_deactivate();");
      ObjectDescriptionPS_activate( True );

    }
  else
    {

      ps_edit_note_desc( tmp_file );

      if( ps_NoteDescription_cb_debug )
	message( "New Desc:"+read_file( tmp_file ) ); // ECR 8008 - moved ':' inside string
	
      // For efficency check if the text has changed, so we do only
      // bother the (ng)oae if we really have to

      string newtext = read_file( tmp_file );
	

      if( n == NULL  || newtext != n.desc )
      {
	  string stmp_file = temp_file("not","xx" );
	  string tf = temp_file( to_string(objid),
				 to_string( ps_NoteDescriptionExternal ) );
			
	  write_file( stmp_file,
		      to_string(objid) +":"+note_name+":NULL:NULL:"+tf+"\n" );

	  write_file( tf, newtext );
			
	  int handle = editor_send_msg( NGOAE_CLASS,
					"EditorQrlEval SetAnnotValuesFromFiles(\""+
					string_escape(stmp_file,"\\")+"\");",
					-1 );
			
	  editor_send_msg( NGOAE_CLASS,
			   "EditorQrlEval 	CleanAnnotTmpFiles(\""+
			   string_escape(stmp_file,"\\")+"\");",
			   handle );


      }
      else
	{
	  if( ps_NoteDescription_cb_debug )
	    message( "No Change" );
	}
    }
  delete_file( tmp_file );


  return 0;
}
//
// Find the index of a field with a givenname in the list.
// Necessary for convenience
//

int
enh_ps_field_index( _ps_propsheet_tp ps, string ifield )
{
	int i;
	_ps_field_tp field;

    for (i=0; i<list_count(ps.propsheet); i++)
    {
        field = list_get(ps.propsheet, i);
        if (field.fieldname == ifield )
        {

            return i;
        }
    }
    return -1;
}

string user_set_annot_file;

void
enh_ps_apply_cb_init( )
{
	user_set_annot_file = temp_file( "PSSET", "" );
	write_file( user_set_annot_file, "" );
}

void
enh_ps_apply_cb_post( int handle )
{
  editor_send_msg( NGOAE_CLASS,
		   "EditorQrlEval SetAnnotValuesFromFilesIntegrated(\""+
		   string_escape(user_set_annot_file,"\\")+"\");",
		   handle );

  // 000328-02 AG: Improve tempfile cleanup
  //
  editor_send_msg( "ngoae",
		   "EditorQrlEval 	CleanAnnotTmpFiles(\""+
		   string_escape(user_set_annot_file,"\\")+"\");",
		   handle );
}


// ECR 3789 end

// ECR 3855 begin
// create or modify association roles
void
ps_create_roles(int whichOne, string name, string psname)
{
  gde_arc     symassoc;
  gde_cntx    context=NULL;
  list        symbols = gde_selected_symbols();
  int         i;
  int         objid = 0;

  symassoc = to_gde_arc(list_get( symbols, 0));

  if( whichOne == 0) // create from role
    {
      context = gde_first_arcs_links_cntx_from_ct( symassoc, "RelLabelFrom");

      if( context == NULL) { // create new roles
	gde_builtin("SymbolBachLabels");
	context = gde_first_arcs_links_cntx_from_ct( symassoc, "RelLabelFrom");
      }

      if( context != NULL)
	gde_cntx_set_label(context, name);
      else
	print_error("Could not create / get role name " + name);

      // test whether to role has to be deleted
      if( name == "") {
	context = gde_first_arcs_links_cntx_from_ct( symassoc, "RelLabelFrom");
	gde_cntx_delete(context);
	}
    }
  else  // to label
    {
      context = gde_first_arcs_links_cntx_from_ct( symassoc, "RelLabelTo");
      if( context == NULL) { // create new roles
	gde_builtin("SymbolBachLabels");
	context = gde_first_arcs_links_cntx_from_ct( symassoc, "RelLabelTo");
      }

      if( context != NULL)
	gde_cntx_set_label(context, name);
      else
	print_error("Could not create / get role name " + name);

      // test whether to role has to be deleted
      if( name == "") {
	context = gde_first_arcs_links_cntx_from_ct( symassoc, "RelLabelTo");
	gde_cntx_delete(context);
      }
    }

  // reselect association & commit all
  gde_select_arc( symassoc);
  gde_map_all_symbols();

  // we need to re-set CurrAssoc, role names might change it into a new one ...
  gde_symbol the_arc = list_get(gde_selected_symbols(), 0);

  gde_link gl = list_get(gde_arc_links(to_gde_arc(the_arc)), 0);
  list l = gde_link_oms_nodes(gl);

  node new_assoc = list_get( l, 0 );
  if( new_assoc != NULL )
    _ps_get_assoc_info( new_assoc );

  return;


}
// ECR 3855 end

// Begin Add ECR 7169
int
ActiveIfNonAnonymousObject( list objects )
{
    string my_label = gde_node_label( to_gde_node( list_get( objects, 0 ) ) );
    if( my_label != "" && string_extract( my_label, 0, 1 ) == ":" )
        // anonymous object, inactive as long as we don't map all instances
        // to the same UmlObjectInstance ...
        return ACTIVE_FUNC_IS_INACTIVE;
	else
        return ACTIVE_FUNC_IS_ACTIVE;
}
// End Add ECR 7169
