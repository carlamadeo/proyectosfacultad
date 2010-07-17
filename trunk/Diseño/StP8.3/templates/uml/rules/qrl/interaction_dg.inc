
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

list uml_split_string(string srcStr, string separator, boolean atEnd,
		      boolean inclSeparator)
{
    // srcStr = original string
    // separator = string separator
    // atEnd = what to do if there is no separator in the srcStr:
    // if atEnd==True then append an empty string, else prepend.
    // inclSeparator: if inclSeparator==True then include separator by
    // prepending it to the second element, if not empty. NOTE: done only 
    // for lists of length=2
    list result = NULL; //list_create("string", 2);
    int len = string_length(srcStr);
    if(len==0) {
	result = list_create("string", 0);
	list_append(result, "");
	list_append(result, "");
	//message("uml_split_string:srcStr='"+srcStr+"' sep='"+separator+"'"+result);
	//return(result);
    } else {
	int pos = string_find(srcStr, 0, separator);
	result = list_create("string", 0);
    
	if(pos < len) { // there is separator
	    result = string_to_list(srcStr,separator);
	    if(list_count(result)==2) {
		if(inclSeparator==True)
		    list_set(result,1,separator+list_get(result,1));
	    } else if(list_count(result)==0)
		list_append(result, "");
	    if(pos==len-1)
		list_append(result, "");
	    else if(pos==0)
		list_insert(result,0,"");
	} else { // separator not found
	    list_append(result, srcStr);
	    if (atEnd==True || list_count(result)==0)
		list_append(result, "");
	    else
		list_insert(result,0,"");
	}
	//message("DEBUG uml_split_string:srcStr='"+srcStr+"' sep='"+separator+"' pos="+pos+" len="+len+result);
    }
    return(result);
}

list ExtensionPoint_name_completion_func(string pattern,string label,
					 string extent,string apptype,int scopeid)
{
    list names = list_create("string",0);

    nc_get_names_from_repository(names,pattern,extent,"UmlUseCase",scopeid,False);
    nc_get_names_from_buffer(names,pattern,extent,"UmlExtensionPoint",scopeid,False);

    nc_get_names_from_repository(names,pattern,extent,"UmlUseCase",scopeid,False);
    nc_get_names_from_buffer(names,pattern,extent,"UmlExtensionPoint",scopeid,False);

    return names;
}

// ECR 4452 begin add functions for MakeObject Active/Passive
// formerly defined in ucollaborationd.qrl
list objectTypes = NULL;
list objectTypes()
{
    if (objectTypes == NULL)
    {
	objectTypes = list_create("string",0);
	list_append(objectTypes,"PassiveObject");
	list_append(objectTypes,"PassiveCompositeObject");
	list_append(objectTypes,"ActiveObject");
	list_append(objectTypes,"ActiveCompositeObject");
    }
    return(objectTypes);
}

void changeObjectActiveStatus()
{
    DoPassiveActivateObject(True);
}

int any_object_selected()
{
    return(DoPassiveActivateObject(False));
}

int DoPassiveActivateObject(boolean doit)
{
    list	symbols;
    gde_symbol	s;
    int		i;
    string	tp, ntp;
    int		foundPos;
    int		maxPos;
    int		dodefer;

    symbols = gde_selected_symbols();

    if (list_count(symbols) != 1)
    {
	return(ACTIVE_FUNC_IS_INACTIVE);
    }
    s = list_get(symbols, 0);
    if (gde_symbol_is_node(s) == True)
    {
	tp = gde_get_symbol_type(s);
	foundPos = list_find(objectTypes(), 0, tp);
	maxPos = list_count(objectTypes);
	if (foundPos < maxPos)
	{
	    if (doit == True)
	    {
		if (foundPos == 0)
		{
		    ntp = list_get(objectTypes(),2);
		}
		else if (foundPos == 1)
		{
		    ntp = list_get(objectTypes(),3);
		}
		else if (foundPos == 2)
		{
		    ntp = list_get(objectTypes(),0);
		}
		else if (foundPos == 3)
		{
		    ntp = list_get(objectTypes(),1);
		}
		gde_save_state();
		dodefer = gde_start_edit_transaction();
		gde_node_replace_nt(to_gde_node(s), ntp);
		gde_end_edit_transaction(0, dodefer, False);
        // Force a canvas refresh
        gde_builtin( "BufferUpdateDmarks" );
	    }
	    return(ACTIVE_FUNC_IS_ACTIVE);
	}
    }	
    return(ACTIVE_FUNC_IS_INACTIVE);
}

string objectActiveStatusLabel()
{
    list	symbols;
    gde_symbol	s;
    int		i;
    string	tp;
    string	noSelectReturn = 	   "Make Object Active/Passive";
    string	passiveSelectionReturn =   "Make Object Active";
    string 	activeSelectionReturn =    "Make Object Passive";
    int		foundPos;
    int 	maxPos;

    symbols = gde_selected_symbols();

    if (list_count(symbols) != 1)
    {
	return(noSelectReturn);
    }
    s = list_get(symbols, 0);
    if (gde_symbol_is_node(s) == True)
    {
	tp = gde_get_symbol_type(s);
	foundPos = list_find(objectTypes(),0,tp);
	maxPos = list_count(objectTypes);
	if (foundPos < maxPos)
	{
	   if ((foundPos == 0) || (foundPos == 1))
	   {
		return(passiveSelectionReturn);
	   }
	   else
	   {
		return(activeSelectionReturn);
	   }
	}
    }
    return(noSelectReturn);
}

// ECR 4452 end add functions for MakeObject Active/Passive


	    
