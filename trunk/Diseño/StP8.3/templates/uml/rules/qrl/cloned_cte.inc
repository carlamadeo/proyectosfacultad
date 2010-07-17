//
//      StP/UML
//      Confidential property of Aonix
//      Copyright (c) 1997
//      All rights reserved
//

// functions for inheriting operations
#include "rules/qrl/uml_inherit_ops.inc"

/***************************************************************************************
 * STARTUP FUNCTION
 */

// ECR 6626 begin - commented
//void
//EditorStartupFunction()
//{
//    gte_load_default_visibility();  // ECR 4072
//    return;
//}
// ECR 6626 end

/***************************************************************************************
 * DRAG AND DROP SUPPORT
 */

void
drop_node(string proj, string system, string apptype, string label, int x, int y)
{
  /* when the cte supports drag and drop, this will need to be defined */

  gte_print_error("Drag & drop not supported in UCLASST.");
  return;
}

/***************************************************************************************
 * rebuild queries
 */
const string classname_attr_query = "node[UmlAttribute && scope_node_id==${class_oid} && node_refs${ref_restriction}]";
const string classname_op_query = "node[UmlOperation && scope_node_id==${class_oid} && node_refs${ref_restriction}]";
const string diagram_definitions_ref_restriction = "[file[UmlClassDiagram]]";

// rebuild the table
const string attr_type_query = "item[UmlAttributeType && obj_id=${attr.id}]";
const string attr_default_value_query = "item[UmlAttributeDefaultValue && obj_id=${attr.id}]";
const string op_return_type_query = "item[UmlOperationReturnType && obj_id=${op.id}]";
const string attr_design_visibility_query = "item[UmlMemberVisibility && obj_id=${attr.id}]";
const string attr_design_is_class_query = "item[UmlMemberIsClass && obj_id=${attr.id}]";
const string attr_design_is_derived_query = "item[UmlAttributeIsDerived && obj_id=${attr.id}]";
const string op_design_visibility_query = "item[UmlMemberVisibility && obj_id=${op.id}]";
const string op_design_is_class_query = "item[UmlMemberIsClass && obj_id=${op.id}]";
const string op_design_is_abs_query = "item[UmlOperationIsAbstract && obj_id=${op.id}]";
const string op_design_throws_query = "item[UmlOperationThrows && obj_id=${op.id}]";

const string attr_impl_cxx_priv_query = "item[UmlMemberCxxVisibility && obj_id=${attr.id}]";
const string attr_impl_cxx_const_query = "item[UmlMemberCxxIsConst && obj_id=${attr.id}]";
const string attr_impl_cxx_vol_query = "item[UmlAttributeCxxIsVolatile && obj_id=${attr.id}]";
const string op_impl_cxx_priv_query = "item[UmlMemberCxxVisibility && obj_id=${op.id}]";
const string op_impl_cxx_const_query = "item[UmlMemberCxxIsConst && obj_id=${op.id}]";
const string op_impl_cxx_virt_query = "item[UmlOperationCxxIsVirtual && obj_id=${op.id}]";
const string op_impl_cxx_inline_query = "item[UmlOperationCxxIsInline && obj_id=${op.id}]";
const string op_impl_cxx_ctor_query = "item[UmlOperationCxxCtorInitList && obj_id=${op.id}]";


const string attr_impl_ada95_alias_query = "item[UmlAttributeAda95IsAliased && obj_id=${attr.id}]";
const string attr_impl_ada95_priv_query = "item[UmlMemberAda95Visibility && obj_id=${attr.id}]";
const string op_impl_ada95_priv_query = "item[UmlMemberAda95Visibility && obj_id=${op.id}]";
const string op_impl_ada95_subunit_query = "item[UmlOperationAda95IsSubunit && obj_id=${op.id}]";
const string op_impl_ada95_inline_query = "item[UmlOperationAda95IsInline && obj_id=${op.id}]";

const string attr_impl_java_acc_query = "item[UmlMemberJavaVisibility && obj_id=${attr.id}]";
const string attr_impl_java_final_query = "item[UmlMemberJavaIsFinal && obj_id=${attr.id}]";
const string attr_impl_java_trans_query = "item[UmlAttributeJavaIsTransient && obj_id=${attr.id}]";
const string attr_impl_java_vol_query = "item[UmlAttributeJavaIsVolatile && obj_id=${attr.id}]";
const string op_impl_java_acc_query = "item[UmlMemberJavaVisibility && obj_id=${op.id}]";
const string op_impl_java_final_query = "item[UmlMemberJavaIsFinal && obj_id=${op.id}]";
const string op_impl_java_nat_query = "item[UmlOperationJavaIsNative && obj_id=${op.id}]";
const string op_impl_java_sync_query = "item[UmlOperationJavaIsSynchronized && obj_id=${op.id}]";

const string attr_impl_idl_type_query = "item[UmlAttributeIDLType && obj_id=${attr.id}]";
const string attr_impl_idl_ro_query = "item[UmlAttributeIDLIsReadOnly && obj_id=${attr.id}]";
const string op_impl_idl_param_query = "item[UmlOperationIDLSignature && obj_id=${op.id}]";
const string op_impl_idl_ret_query = "item[UmlOperationIDLReturnType && obj_id=${op.id}]";
const string op_impl_idl_attr_query = "item[UmlOperationIDLAttribute && obj_id=${op.id}]";
const string op_impl_idl_cntx_query = "item[UmlOperationIDLContext && obj_id=${op.id}]";

const string attr_impl_tool_priv_query = "item[UmlTOOLAttributePrivilege && obj_id=${attr.id}]";
const string attr_impl_tool_set_query = "item[UmlTOOLVirtualAttributeSet && obj_id=${attr.id}]";
const string attr_impl_tool_get_query = "item[UmlTOOLVirtualAttributeGet && obj_id=${attr.id}]";
const string op_impl_tool_priv_query = "item[UmlTOOLOperationPrivilege && obj_id=${op.id}]";
const string op_impl_tool_return_query = "item[UmlTOOLOperationReturnEvent && obj_id=${op.id}]";
const string op_impl_tool_exception_query = "item[UmlTOOLOperationExceptionEvent && obj_id=${op.id}]";
const string op_impl_tool_copy_return_query = "item[UmlTOOLOperationCopyReturn && obj_id=${op.id}]";
const string op_impl_tool_kind_query = "item[UmlTOOLOperationKind && obj_id=${op.id}]";
 
const string CONCRETE_OPS_RESTRICTION = " && (!items[UmlOperationIsAbstract] || items[UmlOperationIsAbstract && value='False'])";
const string ABSTRACT_OPS_RESTRICTION = " && items[UmlOperationIsAbstract && value='True']";

int
idehook_table_decorator_active_func()
{
  string classname = UmlClassTableGetClassCellLabel();

  if (classname == "")
    return ACTIVE_FUNC_IS_INACTIVE;

  return ACTIVE_FUNC_IS_ACTIVE;
}

// Begin add ECR 6633
void
UmlClassTableInitializeFromSeqCollab(int class_id)
{
    UmlClassTableInitialize( class_id );
    UmlClassTableReInitializeFromAllDefinitions( False, False );
	FileSaveAndCommit(False);
    return;
}
// End add ECR 6633

void
UmlClassTableInitialize(int class_id)
{
  node class;
  string classname;

  EditorShowOnNavReceive();


	// get class from repository
	class = node_from_oid(class_id);

	// abort if fetch class fails (unusual condition)
	if (class == NULL) {
		gte_print_error("Cannot find object with id " + class_id);
		return;
	}

	// new file name 
	gte_stpem("FileLoad " + class.name);

	// if the table file already exists, do not overwrite.
	// the test for this is to see if the class cell contains a label
	classname = UmlClassTableGetClassCellLabel();
	if (classname == NULL || classname == "") {

	// no preexisting table -- load the table with the attributes and operations
		UmlClassTableInitializeFromName(class.name, diagram_definitions_ref_restriction, False, False, False);

		// no preexisting table -- show inherited operations
		UmlInheritedOperationsHsect();

		// save the initialized table
		FileSaveAndCommit(False);

	}
}

// Added implement_interface argument for ECR3000
void
UmlClassTableReInitializeFromDiagramDefinitions(boolean inherit_abstract, boolean implement_interface)
{
  string classname = UmlClassTableGetClassCellLabel();

  UmlClassTableInitializeFromName(classname, diagram_definitions_ref_restriction, inherit_abstract, 
    implement_interface, False);
  gte_refresh_table();
}

// Added implement_interface argument for ECR3000
void
UmlClassTableReInitializeFromAllDefinitions(boolean inherit_abstract, boolean implement_interface)
{
  string classname = UmlClassTableGetClassCellLabel();

  UmlClassTableInitializeFromName(classname, "", inherit_abstract, 
    implement_interface, True);
}


string
UmlClassTableGetClassCellLabel()
{
  // returns the label in the class cell
  
  return gte_get_cell_label(gte_get_last_row(gte_lookup_symbol("ClassHsect")), 1);
}

// --- START ECR 6635 
struct uml_message_struct
{
	string objectName;
	string returnType;
	string messageArgs;
};
// --- END ECR 6635 

// Added implement_interface argument for ECR3000
void
UmlClassTableInitializeFromName(string classname, string ref_restriction, boolean inherit_abstract, 
    boolean implement_interface, boolean add_messages)
{
  int	class_oid;
  node	the_class;
  int i;
  /* initialize an Uml class table */
  
  list attrs, ops;
  string savevis;
  
  classname = strip_whitespace(classname);

  classname = to_oms_string(classname);
  if (classname == "")
    return;

  attrs = list_create("node", 0);
  ops = list_create("node", 0);

  /* save the current hideshows as ft expression */
  savevis = gte_visibility_filltable_expression();

  /* clear table */   
  gte_inittable_string("FillTable " + gte_get_table_type() + "{}");

  /* fill class name */
  gte_filltable_string
    ("FillTable{{Hsect{Index ClassHsect}{Vsect {Index MainDefinitionVsect}{Row {Cell {Label \"" + filltable_string_escape(classname) + "\"}}}}}}");

  /* get attributes and operations */
  the_class = node_from_name_type(classname, "UmlClass");
  if (the_class == NULL)
  {
    class_oid = 0;
  } else {
    class_oid = the_class.id;
  }
  attrs = list_select(classname_attr_query);
  // Added implement_interface parameter for ECR3000
  ops = UmlClassTableGetOperations(classname, ref_restriction, inherit_abstract, implement_interface, class_oid);
  // --- START ECR 6635 
  uml_message_struct ums; 
  list tmp_umlobjectnames; // FL: type = "strings"
  list messages = list_create("uml_message_struct", 0); // FL: type = "uml_message_struct"
  if (add_messages) {
	messages = uml_message_struct_list_for_class("*", the_class);
	// --- END ECR 6635 

	  // Messages can be sent to an object of a class that inherits operations
	  // to implement them. So, it is not right to add operations for the
	  // messages because they are already there via inheritance. So, get all
	  // the inherited operations and remove messages from the list that have
	  // the same name as inherited operations
		if (list_count(messages) > 0) {
			// --- START ECR 6635 
		  	tmp_umlobjectnames = list_create("string", list_count(messages));
			for (i=0; i < list_count(messages); i++) {
				ums = list_get(messages, i);
				list_set(tmp_umlobjectnames, i, ums.objectName);
			}
			// --- END ECR 6635 
            // Added implement_interface argument for ECR3000
            set inh_ops = uml_classname_inherited_ops(the_class.name,"","",implement_interface);
			// append the local ops
			for(i=0;i<list_count(ops);i++) {
				set_add(inh_ops,list_get(ops,i));
			}

			node op;
			int loc;
			for(i=0;i<set_count(inh_ops);i++) {
				op = set_get_element(inh_ops,i);
				// --- START ECR 6635 
				loc = list_find(tmp_umlobjectnames, 0, op.name);
				if (loc != list_count(tmp_umlobjectnames)) {
				// --- END ECR 6635 
					// found a message already covered by an inh operation.
					// Remove it from the list before we build the table.
					list_delete(messages,loc);
					// --- START ECR 6635 
					list_delete(tmp_umlobjectnames, loc);
					// --- END ECR 6635 
				}
			}
		}
  } else {
	  // --- START ECR 6635 
	  messages = list_create("uml_message_struct", 0);
	  // --- END ECR 6635 
  }

  /* fill attributes and operations from diagram definitions */
	gte_filltable_string("FillTable{" + UmlAttributesHsectExpression(attrs) +
		UmlOperationsHsectExpression(ops, messages, class_oid) + "}");

  /* restore the hideshows */
  gte_filltable_string(savevis);

  /* refresh */
  gte_refresh_table();
}
// --- START ECR 6635 

void 
init_uml_message_struct(uml_message_struct ums) 
{
	ums.objectName  = "";
	ums.returnType  = "";
	ums.messageArgs = "";
}

boolean
compare_uml_message_struct(uml_message_struct first_ums, uml_message_struct second_ums) {
	if (first_ums.objectName  == second_ums.objectName &&
	    first_ums.returnType  == second_ums.returnType &&
	    first_ums.messageArgs == second_ums.messageArgs)
	     	return True;
	else 
	  	return False;
}


list
uml_message_struct_list_for_class(string pattern, node class)
{
        int idx;
        int length;
        boolean is_equal;
	link message_link;

	item objectName,           
	     returnType,   
	     messageArgs;
	
	uml_message_struct ums;
	list message_struct_list = list_create("uml_message_struct", 0);

	string item_query_part = " && obj_id == ${message_link.id}";

	// The message types were cloned from the usequenced code. If you change
	// this, make sure it's in sync with the thing from which it was copied.
	string link_query = "link[(UmlSimpleMessage|UmlSynchronousMessage|UmlBalkingMessage|"  +
	         	    "UmlTimeoutMessage|UmlAsynchronousMessage) && name $ '${pattern}'" +
		            " && to_node[scope_node[name='${class.name}']] && link_refs]";

	for_each_in_select(link_query, message_link) {
                idx      = 0;
                length   = 0;
                is_equal = False;

		objectName  = find_by_query("item[UmlObjectName"  + item_query_part + "]");
		returnType  = find_by_query("item[UmlReturnType"  + item_query_part + "]");
		messageArgs = find_by_query("item[UmlMessageArgs" + item_query_part + "]");
		
		init_uml_message_struct(ums);
		ums.objectName  = objectName.value;
		ums.returnType  = returnType.value;
		ums.messageArgs = messageArgs.value;

		// now look in the list already built so far, if our 
                // newly created message is already contained.
                length = list_count(message_struct_list);
                for (;idx < length; idx++) {
                        if (compare_uml_message_struct(ums, list_get(message_struct_list, idx))) {
                        	is_equal = True;
                                break;  
                        }
                }
                if (! is_equal)	list_append(message_struct_list, ums);
	}
	return message_struct_list;
}
// --- END ECR 6635 

// Added implement_interface argument for ECR3000
list
UmlClassTableGetOperations(string classname, string ref_restriction,
		boolean inherit_abstract, boolean implement_interface, int class_oid)
{
  // return a list of operations for class named classname 
  // ref_restriction is used to restrict operations by the file type of references -- e.g. object model only
  // if inherit_abstract is True, include abstract operations inherited from superclasses
  
  list ops;
  set inherited_ops, shadowed_ops;
  node op, inh_op;
  int i, j;

  // get already-defined local ops
  ops = list_select(classname_op_query);

  if (inherit_abstract) {

    // get inherited abstract ops
    // Added implement_interface parameter for ECR3000
    inherited_ops = uml_classname_inherited_ops(classname, ref_restriction, ABSTRACT_OPS_RESTRICTION,
        implement_interface);
    
    // compute the set of shadowed ops
    // an inherited operation is shadowed by a local operation if the names and signatures are the same
    shadowed_ops = set_create("node");
    for (i = 0; i < set_count(inherited_ops); i = i + 1) {
      inh_op = set_get_element(inherited_ops, i);
      for (j = 0; j < list_count(ops); j = j + 1) {
	op = list_get(ops, j);
	if (inh_op.name == op.name && inh_op.sig == op.sig) {
	  set_add(shadowed_ops, inh_op);
	  break;
	}
      }
    }

    // append (inherited_ops - shadowed_ops) to ops; now inherited ops will appear in the table
    for (i = 0; i < set_count(inherited_ops); i = i + 1) {
      inh_op = set_get_element(inherited_ops, i);
      if (!set_is_member(shadowed_ops, inh_op))
	list_append(ops, inh_op);
    }
  }
  return ops;
}

void
UmlClassTableInitializeFromStrings(string classname, list attrs, list ops)
{
  /* initialize an Uml class table from strings */
  
  string savevis;
  
  classname = strip_whitespace(classname);
  if (classname == "")
    return;

  /* save the current hideshows as ft expression */
  savevis = gte_visibility_filltable_expression();

  /* clear table */   
  gte_inittable_string("FillTable " + gte_get_table_type() + "{}");

  /* fill class name */
  gte_filltable_string
    ("FillTable{{Hsect{Index ClassHsect}{Vsect {Index MainDefinitionVsect}{Row {Cell {Label \"" + filltable_string_escape(classname) + "\"}}}}}}");

  /* fill attributes and operations from given definitions */
  gte_filltable_string("FillTable{" + UmlAttributesHsectExpressionFromStrings(attrs) + UmlOperationsHsectExpressionFromStrings(ops) + "}");

  /* restore the hideshows */
  gte_filltable_string(savevis);
  
  /* refresh */
  gte_refresh_table();
}

string
UmlAttributesHsectExpressionFromStrings(list attrs)
{
  string hsect_expression_main; 
  string rows_expression_main = "";
  string attr;
  int i;

  for (i = 0; i < list_count(attrs); i = i + 1)
    {
      attr = list_get(attrs, i);
      rows_expression_main = rows_expression_main + UmlAttributeRowExpressionMainFromValues(attr, "", "");
    }

  hsect_expression_main = "{Hsect{Index UmlAttributeHsect}{Vsect{Index MainDefinitionVsect}" + rows_expression_main + "}}";
  return(hsect_expression_main);
}

/***************************************************************************************
 * rebuild attribute&operation rows
 */
string
UmlAttributesHsectExpression(list attrs)
{
  // maps a list of Uml class attributes to a filltable hsect expression 

  string hsect_expression_main, hsect_expression_design, hsect_expression_impl_cxx, 
         hsect_expression_impl_ada95, hsect_expression_impl_java,
         hsect_expression_impl_idl, hsect_expression_impl_tool; 
  string rows_expression_main = "", rows_expression_design = "", rows_expression_impl_cxx = "", 
         rows_expression_impl_ada95 = "", rows_expression_impl_java = "",
         rows_expression_impl_idl = "", rows_expression_impl_tool = ""; 
  node attr;
  int i;

  for (i = 0; i < list_count(attrs); i = i + 1)
    {
      attr = list_get(attrs, i);
      rows_expression_main = rows_expression_main + UmlAttributeRowExpressionMain(attr, attr_type_query, attr_default_value_query, False);
      rows_expression_design = rows_expression_design + UmlAttributeRowExpressionDesign(attr);
      rows_expression_impl_cxx = rows_expression_impl_cxx + UmlAttributeRowExpressionImplCxx(attr);
      rows_expression_impl_ada95 = rows_expression_impl_ada95 + UmlAttributeRowExpressionImplAda95(attr);
      rows_expression_impl_java = rows_expression_impl_java + UmlAttributeRowExpressionImplJava(attr);
      rows_expression_impl_idl = rows_expression_impl_idl + UmlAttributeRowExpressionImplIdl(attr);
      rows_expression_impl_tool = rows_expression_impl_tool + UmlAttributeRowExpressionImplTOOL(attr);
    }

  hsect_expression_main = "{Hsect{Index AttributeHsect}{Vsect{Index MainDefinitionVsect}" + rows_expression_main + "}}";
  hsect_expression_design = "{Hsect{Index AttributeHsect}{Vsect{Index SecondaryDefinitionVsect}" + rows_expression_design + "}}";
  hsect_expression_impl_cxx = "{Hsect{Index AttributeHsect}{Vsect{Index CxxDefinitionVsect}" + rows_expression_impl_cxx + "}}";
  hsect_expression_impl_ada95 = "{Hsect{Index AttributeHsect}{Vsect{Index Ada95DefinitionVsect}" + rows_expression_impl_ada95 + "}}";
  hsect_expression_impl_java = "{Hsect{Index AttributeHsect}{Vsect{Index JavaDefinitionVsect}" + rows_expression_impl_java + "}}";
  hsect_expression_impl_idl = "{Hsect{Index AttributeHsect}{Vsect{Index IDLDefinitionVsect}" + rows_expression_impl_idl + "}}";
  hsect_expression_impl_tool = "{Hsect{Index AttributeHsect}{Vsect{Index TOOLDefinitionVsect}" + rows_expression_impl_tool + "}}";
  return(hsect_expression_main + hsect_expression_design + hsect_expression_impl_cxx + 
                                 hsect_expression_impl_ada95 + hsect_expression_impl_java + 
                                 hsect_expression_impl_idl + hsect_expression_impl_tool  );
}

string
UmlOperationsHsectExpression(list ops, list messages, int c_oid)
{
  // maps a list of Uml class operations to a filltable vsect expression 

  string hsect_expression_main, hsect_expression_design, hsect_expression_impl_cxx, 
         hsect_expression_impl_ada95, hsect_expression_impl_java,
         hsect_expression_impl_idl, hsect_expression_impl_tool; 
  string rows_expression_main = "", rows_expression_design = "", rows_expression_impl_cxx = "", 
         rows_expression_impl_ada95 = "", rows_expression_impl_java = "",
         rows_expression_impl_idl = "", rows_expression_impl_tool = "";
  node op;
  int i;

  for (i = 0; i < list_count(ops); i = i + 1)
    {
      op = list_get(ops, i);
      rows_expression_main = rows_expression_main + UmlOperationRowExpressionMain(op);
      rows_expression_design = rows_expression_design + UmlOperationRowExpressionDesign(op, c_oid);
      rows_expression_impl_cxx = rows_expression_impl_cxx + UmlOperationRowExpressionImplCxx(op);
      rows_expression_impl_ada95 = rows_expression_impl_ada95 + UmlOperationRowExpressionImplAda95(op);
      rows_expression_impl_java = rows_expression_impl_java + UmlOperationRowExpressionImplJava(op);
      rows_expression_impl_idl = rows_expression_impl_idl + UmlOperationRowExpressionImplIdl(op);
      rows_expression_impl_tool = rows_expression_impl_tool + UmlOperationRowExpressionImplTOOL(op);
    }
  // JED - now, add the messages
  string msg;
  // --- START ECR 6635 
  uml_message_struct ums;
  string objectName;
  string returnType;
  string messageArgs;
  // --- END ECR 6635 
  for (i = 0; i < list_count(messages); i = i + 1)
    {
	  // --- START ECR 6635 
      ums = list_get(messages, i);
      objectName = ums.objectName;
      returnType = ums.returnType;
      messageArgs = ums.messageArgs;
      rows_expression_main = rows_expression_main + UmlOperationRowExpressionMainFromValues(objectName, messageArgs, returnType);
      // --- END ECR 6635 
    }


  hsect_expression_main = "{Hsect{Index OperationHsect}{Vsect{Index MainDefinitionVsect}" + rows_expression_main + "}}";
  hsect_expression_design = "{Hsect{Index OperationHsect}{Vsect{Index SecondaryDefinitionVsect}" + rows_expression_design + "}}";
  hsect_expression_impl_cxx = "{Hsect{Index OperationHsect}{Vsect{Index CxxDefinitionVsect}" + rows_expression_impl_cxx + "}}";
  hsect_expression_impl_ada95 = "{Hsect{Index OperationHsect}{Vsect{Index Ada95DefinitionVsect}" + rows_expression_impl_ada95 + "}}";
  hsect_expression_impl_java = "{Hsect{Index OperationHsect}{Vsect{Index JavaDefinitionVsect}" + rows_expression_impl_java + "}}";
  hsect_expression_impl_idl = "{Hsect{Index OperationHsect}{Vsect{Index IDLDefinitionVsect}" + rows_expression_impl_idl + "}}";
  hsect_expression_impl_tool = "{Hsect{Index OperationHsect}{Vsect{Index TOOLDefinitionVsect}" + rows_expression_impl_tool + "}}";
  return(hsect_expression_main + hsect_expression_design + hsect_expression_impl_cxx +  
                                 hsect_expression_impl_ada95 + hsect_expression_impl_java +
                                 hsect_expression_impl_idl + hsect_expression_impl_tool);
}

string
UmlAttributeRowExpressionMain(node attr, string type_query, string default_value_query, boolean first)
{
  // construct an expression to fill a row with an Uml class attribute
    
  item item;
  string name_cell_exp, type_cell_exp = "", default_value_cell_exp = "";
  
  name_cell_exp = "{Cell{Index AttributeNameCol}{Label \"" + filltable_string_escape(attr.name) + "\"}}";  
  
  item = find_by_query(type_query);
  if (item != NULL)
    type_cell_exp = type_cell_exp + "{Cell{Index AttributeTypeCol}{Label \"" + filltable_string_escape(item.value) + "\"}}";  
  
  item = find_by_query(default_value_query);
  if (item != NULL)
    default_value_cell_exp = default_value_cell_exp + "{Cell{Index AttributeDefaultCol}{Label \"" + filltable_string_escape(item.value) + "\"}}";  
  
  if (first)
    return ("{Row{Index Append}" + name_cell_exp + type_cell_exp + default_value_cell_exp + "}");
  else
    return ("{Row" + name_cell_exp + type_cell_exp + default_value_cell_exp + "}");
}

string
UmlAttributeRowExpressionDesign(node attr)
{
  // construct an expression to fill a row with an Uml class attribute's design items
    
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(attr_design_visibility_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeDesignVisibilityCol}{Label \"" + item.value + "\"}}";  
  }
  item = find_by_query(attr_design_is_class_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeDesignClassCol}{Label \"" + item.value + "\"}}";  
  }
  item = find_by_query(attr_design_is_derived_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeDesignDerivedCol}{Label \"" + item.value + "\"}}";  
  }
  
  return ("{Row" + items_type_cell_exp + "}"); 
}

string
UmlAttributeRowExpressionImplCxx(node attr)
{
  // construct an expression to fill a row with an Uml class attribute's c++ implementation items
    
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(attr_impl_cxx_priv_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplCxxPrivCol}{Label \"" + item.value + "\"}}";  
  }
  item = find_by_query(attr_impl_cxx_const_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplCxxConstCol}{Label \"" + item.value + "\"}}";  
  }
  item = find_by_query(attr_impl_cxx_vol_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplCxxVolatileCol}{Label \"" + item.value + "\"}}";  
  }
  
  return ("{Row" + items_type_cell_exp + "}"); 
}

string
UmlAttributeRowExpressionImplAda95(node attr)
{
    
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(attr_impl_ada95_alias_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplAda95AliasedCol}{Label \"" + item.value + "\"}}";  
  }
  item = find_by_query(attr_impl_ada95_priv_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplAda95ClassAttrPrivacyCol}{Label \"" + item.value + "\"}}";  
  }
  
  return ("{Row" + items_type_cell_exp + "}"); 
}

string
UmlAttributeRowExpressionImplJava(node attr)
{
    
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(attr_impl_java_acc_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplJavaVisibilityCol}{Label \"" + item.value + "\"}}";  
  }
  item = find_by_query(attr_impl_java_final_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplJavaFinalCol}{Label \"" + item.value + "\"}}";  
  }
  item = find_by_query(attr_impl_java_trans_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplJavaTransientCol}{Label \"" + item.value + "\"}}";  
  }
  item = find_by_query(attr_impl_java_vol_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplJavaVolatileCol}{Label \"" + item.value + "\"}}";  
  }
  
  return ("{Row" + items_type_cell_exp + "}"); 
}

string
UmlAttributeRowExpressionImplIdl(node attr)
{
    
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(attr_impl_idl_type_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplIDLTypeCol}{Label \"" + item.value + "\"}}";  
  }
  item = find_by_query(attr_impl_idl_ro_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplIDLReadOnlyCol}{Label \"" + item.value + "\"}}";  
  }
  
  return ("{Row" + items_type_cell_exp + "}"); 
}

string
UmlAttributeRowExpressionImplTOOL(node attr)
{
  item item;
  string items_type_cell_exp = "";
 
  item = find_by_query(attr_impl_tool_priv_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplTOOLPrivilegeCol}{Label \"" + item.value + "\"}}";
  }
  item = find_by_query(attr_impl_tool_set_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplTOOLSetCol}{Label \"" + item.value + "\"}}";
  }
  item = find_by_query(attr_impl_tool_get_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeImplTOOLGetCol}{Label \"" + item.value + "\"}}";
  }
 
  return ("{Row" + items_type_cell_exp + "}");

}

string
UmlOperationRowExpressionMain(node op)
{
  // construct an expression to fill a row with an Uml class operation
    
  item item;
  string name_cell_exp, signature_cell_exp, return_type_cell_exp = "";
  
  name_cell_exp = "{Cell{Index OperationNameCol}{Label \"" + filltable_string_escape(op.name) + "\"}}";  
  signature_cell_exp = "{Cell{Index OperationSignatureCol}{Label \"" + filltable_string_escape(op.sig) + "\"}}";  
  
  item = find_by_query(op_return_type_query);
  if (item != NULL)
    return_type_cell_exp = return_type_cell_exp + "{Cell{Index OperationReturnTypeCol}{Label \"" + filltable_string_escape(item.value) + "\"}}";  
  
  return ("{Row" + name_cell_exp + signature_cell_exp + return_type_cell_exp + "}"); 
}

string
UmlOperationRowExpressionDesign(node op, int c_oid)
{
  // construct an expression to fill a row with a class operation's design items
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(op_design_visibility_query);
  if (item != NULL)
  {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationDesignVisibilityCol}{Label \"" + item.value + "\"}}";  
  }
  item = find_by_query(op_design_is_class_query);
  if (item != NULL)
  {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationDesignClassCol}{Label \"" + item.value + "\"}}";  
  }
  // make an op concrete if it is inherited (even if it was abstract in the parent)
  // JED - This is accomplished by omitting the IsAbstract item altogether.
  // The default is concrete.
  if (c_oid == op.scope_node_id)
  {
      item = find_by_query(op_design_is_abs_query);
      if (item != NULL)
      {
	items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationDesignAbstractCol}{Label \"" + item.value + "\"}}";  
      }
  }
  
  return ("{Row" + items_type_cell_exp + "}"); 
}

string
UmlOperationRowExpressionImplCxx(node op)
{
  // construct an expression to fill a row with an Uml class operation's c++ implementation items
    
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(op_impl_cxx_priv_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplCxxPrivCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_cxx_const_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplCxxConstCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_cxx_virt_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplCxxVirtCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_cxx_inline_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplCxxInline}{Label \"" + item.value + "\"}}";  
  }
  
  // SPR 2845 - string_escape needed to fix UML->Contruct Class from All Definitions
  item = find_by_query(op_impl_cxx_ctor_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplCxxCtorInitList}{Label \"" + string_escape(item.value,"\"") + "\"}}";  
  }
  
  
  return ("{Row" + items_type_cell_exp + "}"); 
}

string
UmlOperationRowExpressionImplAda95(node op)
{
    
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(op_impl_ada95_priv_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplAda95VisibilityCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_ada95_subunit_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplAda95SubunitCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_ada95_inline_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplAda95InlineCol}{Label \"" + item.value + "\"}}";  
  }

  return ("{Row" + items_type_cell_exp + "}"); 
}


string
UmlOperationRowExpressionImplJava(node op)
{
    
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(op_impl_java_acc_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplJavaVisibilityCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_java_final_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplJavaFinalCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_java_nat_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplJavaNativeCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_java_sync_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplJavaSynchronizedCol}{Label \"" + item.value + "\"}}";  
  }

  return ("{Row" + items_type_cell_exp + "}"); 
}
string
UmlOperationRowExpressionImplIdl(node op)
{
    
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(op_impl_idl_param_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplIDLSignatureCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_idl_ret_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplIDLReturnTypeCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_idl_attr_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplIDLAttributeCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_idl_cntx_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplIDLContextCol}{Label \"" + item.value + "\"}}";  
  }

  return ("{Row" + items_type_cell_exp + "}"); 
}
string
UmlOperationRowExpressionImplTOOL(node op)
{
  // construct an expression to fill a row with an class operation's TOOL implementation items
    
  item item;
  string items_type_cell_exp = "";
  
  item = find_by_query(op_impl_tool_priv_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplTOOLPrivilegeCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_tool_return_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplTOOLReturnEventCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_tool_exception_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplTOOLExceptionEventCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_tool_copy_return_query);
  if (item != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplTOOLCopyReturnCol}{Label \"" + item.value + "\"}}";  
  }

  item = find_by_query(op_impl_tool_kind_query);
  if (item != NULL) {
  string value = item.value;

//The following conversion is for backward compatibility
  if(value == "True")
	value = "Event";
  else if(value == "False")
	value = "Method";

    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationImplTOOLKindCol}{Label \"" + value + "\"}}";  
  }
  
  return ("{Row" + items_type_cell_exp + "}"); 
}

string
UmlAttributeRowExpressionMainFromValues(string name, string type, string default)
{
  // construct an expression to fill a row with an Uml class attribute (by values)
  string name_cell_exp = "", type_cell_exp = "", default_value_cell_exp = "";

  if (name != NULL)
    name_cell_exp = "{Cell{Index AttributeNameCol}{Label \"" + filltable_string_escape(name) + "\"}}";  
  if (type != NULL)
    type_cell_exp = "{Cell{Index AttributeTypeCol}{Label \"" + filltable_string_escape(type) + "\"}}";  
  if (default != NULL)
    default_value_cell_exp = "{Cell{Index AttributeDefaultCol}{Label \"" + filltable_string_escape(default) + "\"}}";  
  
  return ("{Row" + name_cell_exp + type_cell_exp + default_value_cell_exp + "}"); 
}

string
UmlAttributeRowExpressionDesignFromValues(string visibility, string isclass, string isderived)
{
  // construct an expression to fill a row with an Uml class attribute's design items (by values)
    
  string items_type_cell_exp = "";
  
  if (visibility != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeDesignVisibilityCol}{Label \"" + visibility + "\"}}";  
  }
  if (isclass != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeDesignClassCol}{Label \"" + isclass + "\"}}";  
  }
  if (isderived != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index AttributeDesignDerivedCol}{Label \"" + isderived + "\"}}";  
  }
  
  return ("{Row" + items_type_cell_exp + "}"); 
}

string
UmlInheritedOperationRowExpressionMain(node op)
{
  // construct an expression to fill a row with an Uml class::operation
    
  item item;
  string name_cell_exp, signature_cell_exp, return_type_cell_exp = "";

  node class = find_by_query("node[id = ${op.scope_node_id}]");
  
  name_cell_exp = "{Cell{Index InheritedOperationNameCol}{Label \"" + filltable_string_escape(class.name) + "::" + filltable_string_escape(op.name) + "\"}}";  
  signature_cell_exp = "{Cell{Index InheritedOperationSignatureCol}{Label \"" + filltable_string_escape(op.sig) + "\"}}";  
  
  item = find_by_query(op_return_type_query);
  if (item != NULL)
    return_type_cell_exp = return_type_cell_exp + "{Cell{Index InheritedOperationReturnTypeCol}{Label \"" + filltable_string_escape(item.value) + "\"}}";  
  
  return ("{Row" + name_cell_exp + signature_cell_exp + return_type_cell_exp + "}"); 
}

string
UmlOperationRowExpressionMainFromValues(string name, string sig, string returntype)
{
  // construct an expression to fill a row with an Uml class operation (by values)
    
  string name_cell_exp = "", signature_cell_exp = "", return_type_cell_exp = "";
  
  if (name != NULL)
    name_cell_exp = "{Cell{Index OperationNameCol}{Label \"" + filltable_string_escape(name) + "\"}}";  
  if (sig != NULL)
    signature_cell_exp = "{Cell{Index OperationSignatureCol}{Label \"" + filltable_string_escape(sig) + "\"}}";  
  
  if (returntype != NULL)
    return_type_cell_exp = "{Cell{Index OperationReturnTypeCol}{Label \"" + filltable_string_escape(returntype) + "\"}}";  
  
  return ("{Row" + name_cell_exp + signature_cell_exp + return_type_cell_exp + "}"); 
}

string
UmlOperationRowExpressionDesignFromValues(string visibility, string isclass, string isabstract)
{
  // construct an expression to fill a row with an Uml class operation's design items (by values)
    
  string items_type_cell_exp = "";
  
  if (visibility != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationDesignVisibilityCol}{Label \"" + visibility + "\"}}";  
  }
  if (isclass != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationDesignClassCol}{Label \"" + isclass + "\"}}";  
  }
  if (isabstract != NULL) {
    items_type_cell_exp = items_type_cell_exp + "{Cell{Index OperationDesignAbstractCol}{Label \"" + isabstract + "\"}}";  
  }
  
  return ("{Row" + items_type_cell_exp + "}"); 
}

string
UmlOperationsHsectExpressionFromStrings(list ops)
{
  string hsect_expression_main; 
  string rows_expression_main = ""; 
  string op, sig;
  int i, p, l;

  for (i = 0; i < list_count(ops); i = i + 1)
    {
      op = list_get(ops, i);
      l = string_length(op);
      p = string_find(op, 0, "(");
      if (p < l)
      {
	 sig = string_extract(op, p, l - p);
	 if (sig == "()")
	   sig = "";
         else
           sig = string_extract(sig, 1, string_length(sig) - 2);
	 op = string_extract(op, 0, p); 
      }
      rows_expression_main = rows_expression_main + UmlOperationRowExpressionMainFromValues(op, sig, "");
    }
  hsect_expression_main = "{Hsect{Index OperationHsect}{Vsect{Index MainDefinitionVsect}" + rows_expression_main + "}}";
  return(hsect_expression_main);
}

string
UmlInheritedOperationsHsectExpression(list ops)
{
  // maps a list of Uml class operations to a filltable vsect expression 

  string hsect_expression_main; 
  string rows_expression_main = ""; 
  node op;
  int i;

  for (i = 0; i < list_count(ops); i = i + 1)
    {
      op = list_get(ops, i);
      rows_expression_main = rows_expression_main + UmlInheritedOperationRowExpressionMain(op);
    }

  hsect_expression_main = "{Hsect{Index InheritedOperationHsect}{Vsect{Index MainDefinitionVsect}" + rows_expression_main + "}}";
  return(hsect_expression_main);
}



void
uml_operation_inverse_map_func(node uml_operation, 
			       list rename_data_list,
			       int hsect, int vsect, int row, int col, gocha_tp gocha_type)
{
    if ( gocha_type != gochaRename )
    {
	print_error( "uml_operation_inverse_map_func() can only handle renames" );
	return; 
    }

  // bypass the label preservation from the rename data list, because it combines both the name and the signature
  // and there is no need to parse, because the fields of the operation node are derived directly from the cell labels
  
  string old_operation_name = gte_get_cell_label(gte_get_row(hsect, row), gte_get_col(vsect, gte_lookup_symbol("OperationNameCol")));
  if (old_operation_name != uml_operation.name) {
    gte_invert_cell_label(uml_operation.name, hsect, vsect, row, gte_lookup_symbol("OperationNameCol"));
    gte_print_global_rename_message(old_operation_name, uml_operation.name);
  }
  
  string old_operation_sig = gte_get_cell_label(gte_get_row(hsect, row), gte_get_col(vsect, gte_lookup_symbol("OperationSignatureCol")));
  if (old_operation_sig != uml_operation.sig) {
    gte_invert_cell_label(uml_operation.sig, hsect, vsect, row, gte_lookup_symbol("OperationSignatureCol"));
    gte_print_global_rename_message(old_operation_sig, uml_operation.sig);
  }
}

void
uml_signal_inverse_map_func(node uml_signal, 
			       list rename_data_list,
			       int hsect, int vsect, int row, int col, gocha_tp gocha_type)
{
    if ( gocha_type != gochaRename )
    {
	print_error( "uml_signal_inverse_map_func() can only handle renames" );
	return; 
    }

  // bypass the label preservation from the rename data list, because it combines both the name and the signature
  // and there is no need to parse, because the fields of the signal node are derived directly from the cell labels
  
  string old_signal_name = gte_get_cell_label(gte_get_row(hsect, row), gte_get_col(vsect, gte_lookup_symbol("SignalNameCol")));
  if (old_signal_name != uml_signal.name) {
    gte_invert_cell_label(uml_signal.name, hsect, vsect, row, gte_lookup_symbol("SignalNameCol"));
    gte_print_global_rename_message(old_signal_name, uml_signal.name);
  }
  
  string old_signal_sig = gte_get_cell_label(gte_get_row(hsect, row), gte_get_col(vsect, gte_lookup_symbol("SignalSignatureCol")));
  if (old_signal_sig != uml_signal.sig) {
    gte_invert_cell_label(uml_signal.sig, hsect, vsect, row, gte_lookup_symbol("SignalSignatureCol"));
    gte_print_global_rename_message(old_signal_sig, uml_signal.sig);
  }
}

string
UncommentedAttribute(int hsect, int vsect, int row, int col)
{
  return UncommentedMember(hsect, vsect, row, gte_lookup_symbol("AttributeNameCol"));
}

string
UncommentedOperation(int hsect, int vsect, int row, int col)
{
  return UncommentedMember(hsect, vsect, row, gte_lookup_symbol("OperationNameCol"));
}

string
UncommentedMember(int hsect, int vsect, int row, int namecol)
{
  string label;
  label = gte_get_cell_label(gte_get_row(hsect, row), gte_get_col(vsect, namecol));

  if (label == "" || label == NULL)
    return (NULL);
  
  if (string_length(label) >= 3 && string_extract(label, 0, 3) == " //") 
    return (NULL);
  
  return label;
}

string
OperationIsConstructor(int hsect, int vsect, int row, int col)
{
  /* return "1" if the operation named by the current cell is a constructor, "0" otherwise. */
  
  string classname = UmlClassTableGetClassCellLabel();
  string label = gte_get_cell_label(gte_get_row(hsect, row), gte_get_col(vsect, col));
  
  if (classname == label)
    return "1";
  else
    return "0";
}



/*******************
 * qrl functions to implement accelerators 
 */

void
create_new_attribute()
{
  create_new_member(gte_lookup_symbol("AttributeHsect"));
  return;
}

void
create_new_operation()
{
  create_new_member(gte_lookup_symbol("OperationHsect"));
  return;
}

void
create_new_member(int hs)
{
  /* Create a new attribute or operation. */ 
  
  int current = gte_get_current_cell_row();
  if (gte_get_first_row(hs) <= current && current <= gte_get_last_row(hs))
    
    /* already in the relevant section, so add a new one after the current one */
    insert_new_member(hs, current);
  else
    
    /* not in the relevant section, so add a new one to the end of the relevant section */
    insert_new_member(hs, gte_get_last_row(hs));
  return;
}

void
insert_new_member(int hs, int r)
{
  string rows, cells;
  int vs, vsmax, c, cmax, r_relative;
  
  /* add a new member after the current member */
  
  /* turn off refresh for insert row  */
  gte_disable_refresh();
  
  /* start an undo transaction */
  gte_log_undo_start();
  
  /* insert one row */
  gte_insert_row_after(r, 1);
  
  /* select new row */
  gte_set_selection_quietly(r + 1, gte_get_first_col(), r + 1, gte_get_last_col());
  
  /* refresh table */
  gte_enable_refresh();
  gte_refresh_table();
  return;
}


void
UmlInheritedOperationsHsect()
{
    string classname = UmlClassTableGetClassCellLabel();
    list iops;
    int i;
	int lastRow;
  
    classname = strip_whitespace(classname);
    classname = to_oms_string(classname);
    if (classname == "")
        return;
  
    // Added implement_interface argument for ECR3000
    iops = set_to_list(uml_classname_inherited_ops(classname, "", "",False));
  
	lastRow = gte_get_last_row(gte_lookup_symbol("InheritedOperationHsect"));

    for (i=gte_get_first_row(gte_lookup_symbol("InheritedOperationHsect")); 
         i<=lastRow; 
         i++)
    {
		CoordCellLabel(i, 1, "");
		CoordCellLabel(i, 2, "");
		CoordCellLabel(i, 3, "");
    }

    /* fill attributes and operations from diagram definitions */
    gte_filltable_string("FillTable{" + UmlInheritedOperationsHsectExpression(iops) + "}");
  
    /* refresh */
    gte_refresh_table();
    return;

}
