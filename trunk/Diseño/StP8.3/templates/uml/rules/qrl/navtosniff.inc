// New file for ECR 4179

// This file includes functions to navigate to SNiFF
//  by using sniffaccess.
//


// If the navigation type is "" then SNIFF pops up the Class Browser
void
SNiFFNavToClass(int id, string SNiFFTool, string SNiFFOption)
{
    node n = node_from_oid(id);

    if (n == NULL)
    {
	print_error("Problem with " + navtype + " navigation.");
	return;
    }

    SNiFFBrowseTo(n.name, SNiFFTool, SNiFFOption);	
}


void
SNiFFNavToClassPart(int id, string SNiFFTool, string SNiFFOption)
{
    node n = node_from_oid(id);

    if (n == NULL)
    {
	print_error("Problem with " + navtype + " navigation.");
	return;
    }

    node class_node = get_parent_node(n);
//
// ToDo: this just works for C++
//
    SNiFFBrowseTo(class_node.name + "::" + n.name, SNiFFTool, SNiFFOption);	
}


//
// Call SNiFF via sniffaccessw.
// Without a symboltype we call the Class Browser.
// The symboltype can have the following values:
// 		CLASS, definition of a Class
//		INSTVAR, definbition of an instance variable
//		METHOD_DEF, definition of a method
//		METHOD_IMPL, implementation of a method
//
// We start sniffaccess without a SNiFF Session. If you want to use
// a special Session Name define the environment SNiFF_SESSION_ID.
// 
void
SNiFFBrowseTo(string symbol, string SNiFFTool, string SNiFFOption)
{
    string SNiFFProject;
    
    SNiFFProject = environment_variable("SNiFF_PROJECT");
    
    if (SNiFFProject == NULL)
        SNiFFProject = "*/";
    else 
        SNiFFProject += "/";
        
//    print_message ("sniffaccessw " +  SNiFFTool + " " + SNiFFProject + " " + symbol + " " + SNiFFOption);
    // ECR 7361
    message("SNIFF_DIR4: " + environment_variable("SNIFF_DIR4"));
    if(environment_variable("SNIFF_DIR4") == NULL)
        system ("sniffaccessw " +  SNiFFTool + " " + SNiFFProject + " " + symbol + " " + SNiFFOption);
    else {
        if(SNiFFTool == "hierarchy" && SNiFFOption == "RESTRICTED")
            SNiFFTool = "relative_hierarchy";
        message ("sniffpython " + path_compose(environment_variable("SNIFF_DIR4"), "python", "stptosniff", "stptosniff.py")
                + " " + environment_variable("SNIFF_PORT") + " " + SNiFFTool + " " + symbol);
        system ("sniffpython " + path_compose(environment_variable("SNIFF_DIR4"), "python", "stptosniff", "stptosniff.py")
                + " " + environment_variable("SNIFF_PORT") + " " + SNiFFTool + " " + symbol);
    }
}

//
// Check if ToolInfo variable is set to SNiFF
//
boolean is_sniff_ok()
{
    if(toolinfo_variable( "ProgrammingEnvironment" ) == "SNiFF")
        return True;

    return False;
}

// ECR 7361
boolean is_sniff3_ok()
{
    if(toolinfo_variable( "ProgrammingEnvironment" ) == "SNiFF" && environment_variable("SNIFF_DIR4") == NULL)
        return True;

    return False;
}

node 
get_parent_node(node child)
{
    return find_by_query("node[id=${child.scope_node_id}]");

}
