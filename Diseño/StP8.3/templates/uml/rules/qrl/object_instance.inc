//
// Functions for object symbol mappings
// parse the label and [to be added] GOR funcs

#include "rules/qrl/gde_utils.inc"

const        string        ANONYMOUS_MAP_PROP = "AnonymousName";
string cachedLabel = NULL;
list cachedCompList = NULL;

int doDebug=0;
int dbgNames = 0;
const        string    UML_OBJ_NAME_CLASS_DLM = ":";
const        string    UML_OBJ_STATE_OPEN_DLM = "[";
const        string    UML_OBJ_STATE_CLOSE_DLM = "]";
const        string    UML_OBJ_STATE_NAME_DLM = ",";

struct        obj_name_struct
{
    string        oname;
    string        cname;
    string        states; // a comma-delimited list of state names
    gde_node        gnode;
    string         label;
};

void
debug_obj_name_struct(string w, obj_name_struct r,int debugAnyway)
{
    if(doDebug || debugAnyway) {
        message("***Obj Name: " + w);
        if (r.oname == NULL)
            message("\toname: NULL");
        else
            message("\toname: '" + r.oname + "'");
        if (r.cname == NULL)
            message("\tcname: NULL");
        else
            message("\tcname: '" + r.cname + "'");
        if (r.states == NULL)
            message("\tstates: NULL");
        else
            message("\tstates: '" + r.states + "'");
        if (r.label != NULL)
            message("\tlabel: " + r.label);
        if(r.gnode!=NULL) {
            string pName = ANONYMOUS_MAP_PROP;
            string pVal = gde_node_prop(r.gnode, pName);
            message("\nProperties:: "+pName+"='"+pVal+"'");
            pName = ANONYMOUS_MAP_PROP + "Class";
            pVal = gde_node_prop(r.gnode, pName);
            message("\t "+pName+"='"+pVal+"'");
        }
    }
}

obj_name_struct uml_mapsym_to_obj_name_struct(gde_mapsymbol sym)
{
    obj_name_struct result;
    uml_debug_mapsymbol("uml_mapsym_to_obj_name_struct",sym);
    if (!gde_mapsymbol_is_node(sym)) {
        print_error("Internal inconsistency [uml_mapsym_to_obj_name_struct::symbol is not node]");
        result.oname = NULL;
        result.cname = NULL;
        result.states = NULL;
        result.gnode = NULL;
        result.label = NULL;
    } else {
        gde_node gNode = to_gde_node(sym);
        string aLabel = gde_node_label(gNode);
        result = uml_string_to_obj_name_struct(aLabel,gNode,True);
    }
    return result;
}

const string CLASS_PCKG_DLM = "::";

void uml_set_object_label(string lab,gde_node gn)
{
    obj_name_struct objInfo = uml_string_to_obj_name_struct(lab,gn,False);
    debug_obj_name_struct("uml_set_object_label::",objInfo,0);
    gde_node_set_label(objInfo.gnode,objInfo.label);
    if(doDebug)
        message("uml_set_object_label::lab='"+lab+"'");
}

obj_name_struct uml_string_to_obj_name_struct(string lab,gde_node gn,boolean changeLabel)
{
    // empty label translates into anonymous object of anonymous class
    // single name (no delimiters) -> named object of anonymous class 
    // "name:" ->  named object of anonymous class 
    // ":name" ->  anonymous object of named class 
    // "n1::n2" ->  anonymous object of named class (n1::n2-> n2)
    // "n0:n1::n2" ->  named (n0) object of named class (n1::n2-> n2)
    //
    // any of the above can have a list of states in brackets appended
    // such as:
    //    obj:c1[california, idaho]
    //
    int i;
    obj_name_struct result;
    string origLabel;
    result.gnode = gn;
    result.oname = "";
    result.cname = "";
    result.states = NULL;
    if(lab==NULL)
        result.label = "";
    else
        result.label = lab;
    origLabel = result.label;
    if (result.label != "") {
        int labLen = string_length(result.label);
        // parse out the object states and remove them so that the code below
        // doesn't get confused.

        int stateStart = string_find(result.label, 0,UML_OBJ_STATE_OPEN_DLM);
        int stateEnd = string_find(result.label, 0,UML_OBJ_STATE_CLOSE_DLM);
        if (stateStart != labLen && stateEnd != labLen && stateEnd-stateStart>1)
        {
            string states = string_extract(result.label, stateStart+1,
                    stateEnd-stateStart-1);
            states = replace_newlines(states, " ");
            list stateList = string_to_list(states, UML_OBJ_STATE_NAME_DLM);
            states="";

            string curState;
            for (i=0; i < list_count(stateList); i++)
            {
                curState = string_strip(list_get(stateList, i),"B"," \t");
                if ("" == curState)
                    continue;
                if (states != "")
                    states += ", ";
                states += curState;
            }

            // reuse curState variable as a temp string
            curState="";
            if (stateStart > 0)
                curState = string_extract(result.label,0,stateStart);
            if (labLen - stateEnd > 1)
                curState += string_extract(result.label,stateEnd+1,
// ECR 7217 begin
                        // labLen-stateEnd);
                        labLen-stateEnd-1);
// ECR 7217 end
            result.label = string_strip(curState,"B"," \n\r\t");
            result.states = states;
            labLen = string_length(result.label);
        }
        // do not use string_to_list so we can handle nested classes better
        int splitPoint = string_find(result.label,0,UML_OBJ_NAME_CLASS_DLM);
        if(splitPoint == labLen ) {
            // there is only one name - will treat it as object name
            result.oname = result.label;
        } else if(string_find(result.label,0,CLASS_PCKG_DLM)==splitPoint) {
            // there is no object name, but qualified class name only
            result.cname = result.label;
        } else {  // both names are there
            if(splitPoint < labLen) {
                result.oname=string_extract(result.label,0,splitPoint);
                if(splitPoint+1 < labLen)
                    result.cname = string_extract(result.label,splitPoint+1,labLen-splitPoint-1);
            }
        }
    }
    string newres, newlabel = "";
    // check for anonymous names
    if(strip_whitespace(result.oname)=="") {
        // anonymous object
        result.oname = uml_gde_node_unique_name(result.gnode,"","object",False);
    }
    if(strip_whitespace(result.cname)=="") {
        // anonymous class
        result.cname = uml_gde_node_unique_name(result.gnode,"Class","class",False);
    }
    // cleanup names
    if(!uml_is_anonymous_name(result.oname)) {
        uml_clear_anonymous_prop(result.gnode,"");
        newres = string_translate(result.oname,UML_BAD_CH,UML_GOOD_CH);
        if (newres != result.oname) {
            print_error("Object Name `"+result.oname+"' has invalid characters. Fixing name ...");
            result.oname = newres;
        }
        newlabel = result.oname;
    } else
        uml_set_anonymous_prop(result.gnode,"",result.oname);
    //newlabel += UML_OBJ_NAME_CLASS_DLM;
    if(!uml_is_anonymous_name(result.cname)) {
        uml_clear_anonymous_prop(result.gnode,"Class");
        newres = uml_base_name(result.cname);
        if (newres != result.cname) {
            print_error("Class Name `"+result.cname+"' is invalid. Fixing to '"+newres+"'");
            result.cname = newres;
        }
        newlabel += UML_OBJ_NAME_CLASS_DLM + result.cname;
    } else
        uml_set_anonymous_prop(result.gnode,"Class",result.cname);

    // append the state names, but only if the above code has decided there's
    // something worth putting in the label
    if (newlabel != "" && result.states != NULL)
    {
        // must remove trailing whitespace, lest multiple calls cause it to
        // build up
        newlabel = string_strip(newlabel,"T"," \n\r\t");
        newlabel += "\n" + UML_OBJ_STATE_OPEN_DLM + result.states +
            UML_OBJ_STATE_CLOSE_DLM;
        result.label = newlabel;
    }


    if(newlabel!=origLabel && !(result.label==UML_OBJ_NAME_CLASS_DLM &&
            newlabel==""))
    {
        string yesNo = "";
         if(result.gnode!=NULL && changeLabel) {
             gde_node_set_label(result.gnode, newlabel);
        } else
            yesNo = "NOT";
        //print_message("Label `"+result.label+"' "+yesNo+" changed to '"
        //+newlabel+"'");
        result.label = newlabel;
    }
    return result;
}


boolean uml_is_anonymous_name(string name)
{
    boolean result = False;
    if(string_find(name,0,"%%") == 0)
        result = True;
    if(doDebug)
        message("uml_is_anonymous_name::name='"+name+"' "+result);
    return result;
}

string uml_get_name_component_from_string(string aLabel,string separator,int pos)
{
    if(aLabel == "" || aLabel==NULL)
        return "";
    if(True) { //aLabel!=cachedLabel) {
        cachedLabel = aLabel;
        cachedCompList = NULL;
        cachedCompList = string_to_list(aLabel,separator);
        if(list_count(cachedCompList)==0)
            list_append(cachedCompList,"");
        if(string_extract(aLabel,0,1)==UML_OBJ_NAME_CLASS_DLM)
            list_insert(cachedCompList,0,"");
    }
    
    //message("uml_get_name_component::label='"+aLabel+"' components=["+cachedCompList+"] count="+list_count(cachedCompList));

    if(list_count(cachedCompList)<=pos)
        return "";

    // Check if the name as the whole is correct.  Must iterate ove
    // the whole list.
    // NOTE: To be implemented. - see below
    string result=list_get(cachedCompList,pos);
    if(result=="")
        return "";
    string newres;
    if(string_find(result,0,"%%")==0)//if(uml_is_anonymous_name(result)==True)
        newres = result;
    else {
        newres = string_translate(result,UML_BAD_CH_NO_COL,UML_GOOD_CH_NO_COL);
        if (newres != result) {
            // NOTE: will not change external name, i.e., label.  Do not
            // have the full name.  That should be done separately,
            // iterating through the whole list.  To avoid repeating the
            // test, can cache the value and check before the test if it is
            // the same as the cached value -- see above
            print_error("**Name `"+result+"' has invalid characters. Fixing Internal name ...");
        }
    }
    
    //message("uml_get_name_component::label='"+aLabel+"' pos="+pos+" newres='"+newres+"'");
    return strip_whitespace(newres);
}

void uml_debug_mapsymbol(string mssg, gde_mapsymbol sym)
{
    //message("uml_debug_mapsymbol is VISITED");
    //print_message("uml_debug_mapsymbol is VISITED");
    if(doDebug) {
        message("uml_debug_mapsymbol is ACTIVE!!! doDebug='"+doDebug+"'");
        print_message("uml_debug_mapsymbol is ACTIVE!!! doDebug='"+doDebug+"'");
        list syms = list_create("gde_mapsymbol", 0);
        list_append(syms,sym);
        debug_mapsyms(mssg,syms);
    }
}

string uml_get_name_component(gde_mapsymbol sym,string separator,int pos)
{
    uml_debug_mapsymbol("uml_get_name_component",sym);
    if (!gde_mapsymbol_is_node(sym)) {
        print_message("Internal inconsistency [uml_get_name_component::symbol is not node]");
        return("");
    }
    gde_node gNode = to_gde_node(sym);
    string aLabel = gde_node_label(gNode);
    return uml_get_name_component_from_string(aLabel,separator,pos);
}

string uml_qualified_object_name(gde_mapsymbol sym)
{
    if (!gde_mapsymbol_is_node(sym)) {
        print_message("Internal inconsistency [uml_qualified_object_name::symbol is not node]");
        //print_error("uml_qualified_object_name::SYMBOL is NOT a NODE!!");
        return("");
    }
    string result="Howdy";

    obj_name_struct objInfo = uml_mapsym_to_obj_name_struct(sym);
    result = objInfo.oname;

    // ECR 5736 begin
    // ECR 6863 begin
    // result = string_translate(result, UML_BAD_CH, UML_GOOD_CH);
    // result = strip_whitespace(result);
    if (!is_temporary_object_name(result))
    {
        result = string_translate(result, UML_BAD_CH, UML_GOOD_CH);
        result = strip_whitespace(result);
    }
    // ECR 6863 end
    // ECR 5736 end

    // message("uml_qualified_object_name result='" + result +"'");
    // trace("all");
    return result;
}


string uml_instance_owner_name(gde_mapsymbol sym)
{
    //trace("all");
    if (!gde_mapsymbol_is_node(sym)) {
        print_message("Internal inconsistency [uml_instance_owner_name::symbol is not node]");
        //print_error("uml_instance_owner_name::SYMBOL is NOT a NODE!!");
        return("");
    }

    string result;

    if(dbgNames) {
        result = uml_get_name_component(sym,UML_OBJ_NAME_CLASS_DLM,1);
 
        if(doDebug)
            message("uml_instance_owner_name='" + result +"'");

//         if (result == "" || result==NULL) {
//             // Anonymous objects - assign unique name to each!
//             result = uml_get_unique_name(sym,"Class","class",False);
// //    } else if(uml_is_anonymous_name(result) == True) {
//         } else if(string_find(result,0,"%%")==0) {
//             uml_set_anonymous_prop(to_gde_node(sym),"Class",result);
//             string newLabel =
//             uml_get_name_component(sym,UML_OBJ_NAME_CLASS_DLM,0)+UML_OBJ_NAME_CLASS_DLM;
//             gde_node_set_label(to_gde_node(sym), newLabel);
//         } else
//             uml_clear_anonymous_prop(to_gde_node(sym),"Class");
    } else {
        obj_name_struct objInfo = uml_mapsym_to_obj_name_struct(sym);
        result = objInfo.cname;
        debug_obj_name_struct("uml_instance_owner_name",objInfo,0);
    }
    if(doDebug)
        message("uml_instance_owner_name result='" + result +"'");
    return strip_whitespace(result);
}

string uml_object_states_name(gde_mapsymbol sym)
{
    if (!gde_mapsymbol_is_node(sym)) {
        print_message("Internal inconsistency [uml_object_states_name::symbol is not node]");
        //print_error("uml_qualified_object_name::SYMBOL is NOT a NODE!!");
        return("");
    }
    string result;

    gde_node gNode = to_gde_node(sym);
    string aLabel = gde_node_label(gNode);
    obj_name_struct objInfo = uml_string_to_obj_name_struct(aLabel,gNode,True);
    result = objInfo.states;
    // message("uml_object_states_name:" + result);
    return result;
}

void uml_set_anonymous_prop(gde_node gNode,string theType,string propVal)
{
    string        aProp, propName;

    if (theType != NULL)
        propName = ANONYMOUS_MAP_PROP + theType;
    else
        propName = ANONYMOUS_MAP_PROP;
    aProp = gde_node_prop(gNode, propName);
    if(doDebug)
        message("uml_set_anonymous_prop::propName='"+propName+"' [old]aProp='"+aProp+"'new propVal='"+propVal+"'");
    if (aProp != propVal) {
        gde_node_clear_prop(gNode, propName);
        gde_node_set_prop(gNode, propName, propVal);
    }
}

void uml_clear_anonymous_prop(gde_node gNode, string theType)
{
    string        aProp, propName;

    if (theType != NULL) {
        propName = ANONYMOUS_MAP_PROP + theType;
    } else {
        propName = ANONYMOUS_MAP_PROP;
    }
    aProp = gde_node_prop(gNode, propName);
    if (aProp != NULL && aProp != "") {
        gde_node_clear_prop(gNode, propName);
    }
}

// static vars used to produce unique names 
int lastTime = 0;
int uniqueSuffix=0;

string uml_get_unique_name(gde_mapsymbol sym, string theType,
                           string prefix, boolean verbose)
{
    // Not a bullet-proof method - assumes that a chance of two requests in
    // two different processes happenning on different machines returning
    // the same value is 0

    if (!gde_mapsymbol_is_node(sym)) {
        print_error("uml_get_unique_name::SYMBOL is NOT a NODE!!");
        return NULL;
    }
    gde_node gNode = to_gde_node(sym);
    return(uml_gde_node_unique_name(gNode,theType,prefix,verbose));
}
    
string uml_gde_node_unique_name(gde_node gNode, string theType,
                                string prefix, boolean verbose)
{
    // Not a bullet-proof method - assumes that a chance of two requests in
    // two different processes happenning on different machines returning
    // the same value is 0

    if (gNode==NULL) {
        print_message("uml_gde_node_unique_name::ignored NULL gNode");
        return NULL;
    }

    string propName;
    
    if (theType != NULL) {
        propName = ANONYMOUS_MAP_PROP + theType;
    } else {
        propName = ANONYMOUS_MAP_PROP;
    }

    string aProp = gde_node_prop(gNode, propName);
    if(doDebug)
        message("uml_get_unique_name::propName='"+propName+"' [old]aProp='"+aProp+"'");
    if (aProp == NULL) {
        int timeNow = time_now();
        string uniqueId = to_string(timeNow);
        if(timeNow==lastTime)
            uniqueId += "_" + to_string(uniqueSuffix++);
        else {
            lastTime = timeNow;
            uniqueSuffix = 0;
        }

        int myPid = getpid();
        uniqueId += "_" + to_string(myPid);

        string uniqueName = "%%" + prefix + "%%" + uniqueId;
        if(verbose == True)
            uniqueName = "%%(anonymous)" + uniqueName;

        aProp = uniqueName;
        gde_node_set_prop(gNode, propName, aProp);
        string ctrlVal = gde_node_prop(gNode, propName);
        if(doDebug)
            message("uml_get_unique_name::propName='"+propName+"' [new]aProp='"+aProp+"'");
        if(aProp!=ctrlVal)
            print_error("uml_get_unique_name::property not set??propName='"+propName+"' set to '"+aProp+"' got '"+ctrlVal+"'");
    }
//     if(doDebug)
//         uml_debug_mapsymbol("uml_get_unique_name:: propName='"+propName+"' aProp='"+aProp+"'",to_gde_symbol(gNode));
    return(aProp);
}

//
// Name function for object instance attribute
//

string
object_instance_attr_name(gde_mapsymbol ms)
{
// ECR 5736 begin
//    return(object_instance_attr_name_val(ms,
//                                        gde_node_label(to_gde_node(ms)), 0, 0));
    string result = object_instance_attr_name_val(ms,
                                        gde_node_label(to_gde_node(ms)), 0, 0);
	result = string_translate(result, UML_BAD_CH, UML_GOOD_CH);
    result = strip_whitespace(result);
    return result;
// ECR 5736 end
}

//
// Signature function for object instance attribute
//

string
object_instance_attr_value(gde_mapsymbol ms)
{
    return(object_instance_attr_name_val(ms, NULL, 1, -1));
}

string
object_instance_attr_name_val(gde_mapsymbol ms, string def, int low, int high)
{
    string        lab = def;
    const        string        INST_ATTR_VAL_DLM = "=";
    list        l;
    int                ll, lc;

    l = string_to_list(gde_node_label(to_gde_node(ms)), INST_ATTR_VAL_DLM);
    lc = list_count(l);
    if (lc > low)
    {
        if (high < low || high >= lc)
            high = lc-1;

        for(ll = low; ll <= high; ll = ll + 1)
        {
            if (ll == low)
                lab = list_get(l, ll);
            else
                lab = lab + INST_ATTR_VAL_DLM + list_get(l, ll);
        }
        lab = string_strip(lab, "B", " ");
    }
    return(lab);
}

void
object_instance_attr_label(gde_mapsymbol ms, node n, list rninfo,
                            gocha_tp gocha_type)
{
    string                name, value, label = NULL;
    rename_data                thisrename;
    gde_node                rnsym;
    int                        i;

    if ( gocha_type != gochaRename )
    {
        print_error( "object_instance_attr_label() can only handle renames" );
        return; 
    }

    if (!gde_mapsymbol_is_node(ms))
    {
        return;
    }
    rnsym = to_gde_node(ms);
    value = object_instance_attr_value(ms);

    for(i = 0; i < list_count(rninfo); i = i + 1)
    {
        thisrename = list_get(rninfo, i);
        if ((thisrename.dependency_name == GDE_GOR_REN_DEP_NAME) ||
           (thisrename.dependency_name == GDE_GOR_REN_DEP_SELF))
        {
            label = thisrename.label;
            break;
        }
    }
    if (label == NULL && n != NULL)
    {
        label = n.name;
    }
    if (value == NULL && n != NULL)
    {
        value = n.sig;
    }
    gde_node_set_label(rnsym, uml_attr_name_val_to_node_label(label, value));
}

string
uml_attr_name_val_to_node_label(string name, string val)
{
    const        string        INST_ATTR_VAL_DLM = "=";
    string        label = "";


    if (name != NULL)
    {
        label = name;
    }
    label = label + " " + INST_ATTR_VAL_DLM + " ";
    if (val != NULL)
    {
        label = label + val;
    }
    return(make_multi_line_label_from_string(label));
}

void object_instance_label(gde_mapsymbol ms, node n, list rninfo,
                            gocha_tp gocha_type)
{
    string                classname, objname, states, label = NULL;
    rename_data                thisrename;
    gde_node                rnsym;
    list lbl_parts;
    int                        i;
    int dbgLocal = 0;

    if ( gocha_type != gochaRename ) {
        print_error( "object_instance_label() can only handle renames" );
        return; 
    }
    if (!gde_mapsymbol_is_node(ms)) {
                return;
    }

    rnsym = to_gde_node(ms);
    label = gde_node_label(rnsym); 
    string oldVal = label; 
    obj_name_struct objInfo = uml_string_to_obj_name_struct(label,rnsym,False);
    if(dbgLocal)
        message("object_instance_label::objInfo='"+objInfo+"'");
    debug_obj_name_struct("object_instance_label::",objInfo,dbgLocal);
    objname = objInfo.oname;
    classname = objInfo.cname;
    if (objInfo.states != NULL)
        states=UML_OBJ_STATE_OPEN_DLM + objInfo.states+UML_OBJ_STATE_CLOSE_DLM;
    else
        states="";

    if(dbgLocal)
        message("object_instance_label::oN='"+objname+"' cN='"+classname+"' Node:" + n);
    if(dbgLocal) {
        message("GDE_GOR_REN_DEP_NAME='"+GDE_GOR_REN_DEP_NAME+"'");
        message("GDE_GOR_REN_DEP_SELF='"+GDE_GOR_REN_DEP_SELF+"'");
    }
    for(i = 0; i < list_count(rninfo); i = i + 1) {
        thisrename = list_get(rninfo, i);
        if(dbgLocal)
            message("object_instance_label::thisrename="+thisrename);
        if ((thisrename.dependency_name == GDE_GOR_REN_DEP_NAME) ||
            (thisrename.dependency_name == GDE_GOR_REN_DEP_SELF))
        {
            if ("UmlObjectInstance" == n.type) {
                objname = n.name;
            } else if ("UmlObjectClassScope" == n.type) {
                classname = n.name; // new class name
            } 
            //ECR 6433 begin
            else if ("UmlObjectInState" == n.type)
            {
                states = UML_OBJ_STATE_OPEN_DLM + n.name + UML_OBJ_STATE_CLOSE_DLM;
            }
            //ECR 6433 end

            if(dbgLocal)
                message("object_instance_label::roN='"+objname+"' rcN='"+classname+"'");
            break;
        }
        //ECR 6433 begin
        //else 
        //    if ("UmlObjectInState" == n.type) {
        //        states = UML_OBJ_STATE_OPEN_DLM + n.name +
        //            UML_OBJ_STATE_CLOSE_DLM;
        //   }
        //ECR 6433 end
    }
    // ECR 7884 begin
    if (list_count(rninfo) == 0)
    {
        if ("UmlObjectInstance" == n.type)
            objname = n.name; // new object name
        else if ("UmlObjectClassScope" == n.type)
            classname = n.name; // new class name
        else if ("UmlObjectInState" == n.type)
            states = UML_OBJ_STATE_OPEN_DLM + n.name + UML_OBJ_STATE_CLOSE_DLM;
    }
    // ECR 7884 end
    label = objname + UML_OBJ_NAME_CLASS_DLM + classname + states;
    // do one more pass, to clear anonymous names from the label
    objInfo = uml_string_to_obj_name_struct(label,rnsym,False);
    if(dbgLocal)
        message("object_instance_label::objInfo='"+objInfo+"'");
    label = objInfo.label;
    gde_node_set_label(rnsym, label);
    gde_print_rename_notification_message(ms,
        gde_mapsymbol_symbol_type_printname(ms) +
        " renamed from '" + oldVal +  "' to '" + label + "'");
}

// ECR 6863 begin
boolean
is_temporary_object_name(string name)
{
    return name!="" &&
           string_length(name)>=2 &&
           string_extract(name,0,2)=="%%";
}
// ECR 6863 end

list
object_name_completion_func(string pattern, string label, 
                            string extent, string apptype, int scopeid)
{
    list	names;
    list	compound_names;
    string	fullName;
    int		i;
    node classScope;

    //message("object_name_completion_func::pattern='"+pattern+"' label='"+label+"' extent='"+extent+"' apptype='"+apptype+"' scopeid="+scopeid);
    string srcStr = label;
    if(srcStr=="" || srcStr == NULL)
        srcStr = pattern;
    string objName = uml_get_name_component_from_string(srcStr,UML_OBJ_NAME_CLASS_DLM,0);
    string className = uml_get_name_component_from_string(srcStr,UML_OBJ_NAME_CLASS_DLM,1);
    //message("objName='"+objName+"' className='"+className+"'");
    // ECR 6863 begin
    // if(objName!="" && string_length(objName)>=2
    //    && string_extract(objName,0,2)=="%%") {
    if (is_temporary_object_name(objName)) {
    // ECR 6863 end
        // anonymous object, ignore the name
        objName = "";
    }
    if(className=="") {
        node objClassScope = find_by_query("node[${scopeid}]");
        if(objClassScope!=NULL)
            className = objClassScope.name;
    }
    // ECR 6863 begin
    // if(className!="" && string_length(className)>=2
    //    && string_extract(className,0,2)=="%%")
    if (is_temporary_object_name(className))
    // ECR 6863 end
        className = "";// anonymous class, ignore the name
    className += "*";
    //message("After:objName='"+objName+"' className='"+className+"'");
       
    //
    // call the standard routines to get all the class names that match
    // the pattern.
    //
    names = list_create("string", 0);
    int scope = 0;                // class objects have scopeId==0
    nc_get_names_from_repository(names,className,extent,"UmlClass",scope,False);
    nc_get_names_from_buffer(names,className,extent,"UmlClass",scope,False);

    // fetch ObjectClassScope with node refs
    list theClassScopes =
         list_select("node[UmlObjectClassScope"+
                     " && name $ '${className}' && node_refs]");

    for(i=0;i<list_count(theClassScopes);i++) {
        classScope = list_get(theClassScopes,i);
        // ignore anon
        if(classScope.name!="" && string_length(classScope.name)>=2
	    && string_extract(classScope.name,0,2) == "%%")
            continue;
        list_append(names,classScope.name);
    }

    //
    // then prepend each class name with the object part of the name.
    // If there is no colon in the label yet, then the entire label is
    // considered to be the object part of the name.
    //
    compound_names = list_create("string", 0);
    
    for (i = 0; i < list_count(names); i++) {
        fullName = objName + UML_OBJ_NAME_CLASS_DLM + list_get(names, i);
        list_append(compound_names, fullName);
        //message("Found:fullName='"+fullName+"'");
    }
    return (compound_names);
}

#include "rules/qrl/object_instance_rename.inc" //ECR 6433
