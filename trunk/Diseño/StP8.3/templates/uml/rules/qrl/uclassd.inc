// ECR 2653 begin
list
general_arc_name_func(gde_mapsymbol sym)
{
	list		arcs = gde_link_arcs(to_gde_link(sym));
	string		strLabel;
	string		strFinal;
	list		lstLabels;
	int			i;
    
	lstLabels = list_create("string", 0);
	strFinal = "";

	for(i=0;i<list_count(arcs);i++) {
		strLabel = gde_arc_label(list_get(arcs, i));
		strLabel = string_translate(strLabel, UML_BAD_CH,
			UML_GOOD_CH);
		// ECR 4792 begin
		// gde_arc_set_label(list_get(arcs, i), strLabel);
		strFinal += strLabel;
		// ECR 4792 end
	}

    // ECR 5736 begin
    strFinal = strip_whitespace(strFinal);
    // ECR 5736 end

	list_append(lstLabels, strFinal);
	return lstLabels;
}
// ECR 2653 end

// ECR 5184 start 
list
uclassd_class_name_completion_func(string pattern, string label, string extent,
				 string apptype, int scopeid)
{
        node classScope;
        int i;

	list l = gde_default_name_completion_func(pattern,label,extent,apptype,scopeid);
        // fetch ObjectClassScope with node refs
        list theClassScopes =
               list_select("node[UmlObjectClassScope"+
                           " && name $ '${pattern}' && node_refs]");



        for(i=0;i<list_count(theClassScopes);i++) {
            classScope = list_get(theClassScopes,i);
            // ignore anon classes and append others
            if(classScope.name!="" && string_length(classScope.name)>=2
	        && string_extract(classScope.name,0,2) == "%%")
                continue;
            list_append(l,classScope.name);
        }


	return l;
}

// ECR 5184 end
