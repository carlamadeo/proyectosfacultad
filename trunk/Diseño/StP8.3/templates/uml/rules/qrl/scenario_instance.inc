
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

#include "rules/qrl/interaction_dg.inc"

// Parse the label into component parts. Since this will be invoked several
//consecutive times for the same label, do some caching
string cachedSiLabel = "";
list cachedSiList = NULL;

list uml_parse_scenarioInstance_label(gde_mapsymbol sym)
{
    string aLabel;

    if (gde_mapsymbol_is_node(sym))
    {
	aLabel = gde_node_label(to_gde_node(sym));
    } else {
 	print_error("is NOT node!!");
 	return NULL;
    }
    return(uml_parse_scenarioInstance_label_string(aLabel));
}

list uml_parse_scenarioInstance_label_string(string aLabel)
{
    if(aLabel == cachedSiLabel) {
	//message("uml_get_label_components::REUSE for label '"+aLabel+"'" + cachedSiList);
        return cachedSiList;
    } else {
	// new label
	cachedSiLabel = aLabel;
	cachedSiList = list_create("string", 0);
    }

    if(aLabel=="" || aLabel==NULL) {
	//message("uml_get_label_components::EMPTY aLabel '"+aLabel+"'");
	return(cachedSiList);
    }
    //cachedSiList = uml_parse_message_label(aLabel);
    string parentStr = "";
    string scenarioName = "";
    //string remaining = aLabel;
    int strLen;

    list tempSplit = uml_split_string(aLabel, "(", True,False);
    //message("uml_get_label_components::split '"+aLabel+"' into"+tempSplit);
    scenarioName = list_get(tempSplit,0);
    parentStr = list_get(tempSplit,1);
    string parentType = "";
    string parentName = "";

    if(parentStr!="" && parentStr!=NULL) {
	strLen = string_length(parentStr);
	if(string_extract(parentStr,strLen-1,1)==")")
	    parentStr = string_extract(parentStr,0,strLen-1);
	tempSplit = uml_split_string(parentStr, ":", True,False);
	//message("uml_get_label_components::split '"+parentStr+"' into"+tempSplit);
	parentType = list_get(tempSplit,0);
	parentName = list_get(tempSplit,1);
    }
    // assemble result
    //list cachedSiList = list_create("string", 0);
    list_append(cachedSiList,scenarioName);
    list_append(cachedSiList,parentType);
    list_append(cachedSiList,parentName);
    //message("uml_get_label_components::new label '"+aLabel+"'" + cachedSiList);
    return(cachedSiList);
}

string uml_get_scenarioInstance_component(gde_mapsymbol sym,int pos)
{
    //message("uml_get_scenarioInstance_component @ pos="+pos);
    list allComps = uml_parse_scenarioInstance_label(sym);
    if(allComps==NULL)
	return NULL;
    int count = list_count(allComps);
    if(count<=pos)
	return NULL;
    else
	return(strip_whitespace(list_get(allComps,pos)));
}


string uml_scenario_instance_signature(gde_mapsymbol sym)

{

    string result = uml_get_scenarioInstance_component(sym,2);

    result += "_" + uml_get_scenarioInstance_component(sym,0);

    return result;

}

    
string uml_gde_node_scenarioInstance(gde_mapsymbol sym)
{
    string result = uml_get_scenarioInstance_component(sym,0);
    //message("uml_gde_node_scenarioInstance='"+result+"'");
    return result;
}

string uml_scenarioInstanceNote_name(gde_mapsymbol sym)
{
    // make the note name dg_type specific, to avoid colisions between
    // notes and items in different diagrams.  Each note is already
    // specific to which scenario instance it belongs, but not to a
    // file (i.e., a diagram whgere it resides). Hence, this does not
    // preclude having the same scenario instance in several diagrams
    // of the same type.  Since there should not be multiple
    // definitions for the same scenario, we do not wantto make
    // scenario instance dg_name specific, as semantic checks can and
    // should catch potential inconsistencies involving multiple
    // definitions of a scenario (i.e., multiple ocurrence of a
    // scenario instance in several diagrams of the same type (either
    // seq or collaboration).
    string result = current_editor_apptype();
    //message("DEBUG uml_scenarioInstanceNote_name='"+result+"'");
    return result;
}



string uml_scenarioInstanceParentType(gde_mapsymbol sym)
{
    string result = uml_get_scenarioInstance_component(sym,1);
    //message("uml_scenarioInstanceParentName='"+result+"'");
    return result;
}

string uml_scenarioInstanceParentName(gde_mapsymbol sym)
{
    string result = uml_get_scenarioInstance_component(sym,2);
    //message("uml_scenarioInstanceParentName='"+result+"'");
    return result;
}

void
ScenarioInstanceSet_inverse_item_map_func(gde_mapsymbol sym,
					       item parentName,list renameList,
					       gocha_tp gocha_type)
{
    if ( gocha_type != gochaRename ) {
	print_error( "ScenarioInstanceSet_inverse_item_map_func() can only handle renames" );
	return; 
    }
    //message("ScenarioInstanceSet_inverse_item_map_func::sym='"+sym+"' pn='"+parentName+"'+rnlist="+renameList);
    rename_data lastRename = list_get(renameList,0);
    string oldLabel = lastRename.label;
    list allComps = uml_parse_scenarioInstance_label_string(oldLabel);
    //message("oldLabel='"+oldLabel+"' allComps="+allComps);
    string newLabel = uml_gde_node_scenarioInstance(sym) + "(" +
	uml_scenarioInstanceParentType(sym) + ":" + parentName.value + ")";
    //message("newLabel='"+newLabel+"'");
    gde_node rnsym = to_gde_node(sym);
    gde_node_set_label(rnsym,newLabel);
}
    
