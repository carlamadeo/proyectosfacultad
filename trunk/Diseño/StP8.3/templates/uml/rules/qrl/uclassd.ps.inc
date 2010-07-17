// ECR 5184 created this file
void
UmlClass_ps_show()

{
    // ECR 6880 Property sheets from table editors
    PropSheet__show("UmlClass_ps");
    // gde_builtin("PropertySheetShow UmlClass_ps");
    // ECR 6880 end

    // reset

    UmlClass_ps_init();
}

boolean UmlClass_ps_initted = False;

void
UmlClass_ps_init()
{
    if (UmlClass_ps_initted)
    {
        // just refresh the prop sheet
        ps_fill("UmlClass_ps");
        return;
    }
    UmlClass_ps_initted = True;
    // ECR 6880 Property sheets from table editors
//    if (PropSheet__editor_type() == "gte")
//	gte_stpem("EditorActionDone 0214951139  UmlClass_ps_ActionDone();");
//    else // if (PropSheet__editor_type() == "gde")
//    // ECR 6880 end
//    gde_builtin("EditorActionDone 0214951139  UmlClass_ps_ActionDone();");

    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() == "gte")
        ps_init("UmlClass_ps", "ObjectName", ps_Node, string_to_list("ClassHsect NameVsect", " "), ps_Node);
    else // if (PropSheet__editor_type() == "gde")
        ps_init("UmlClass_ps", "ObjectName", ps_Node, string_to_list("Class ClassName InstantiatedClass Interface", " "), ps_Node);
    // ECR 6880
    
    // ECR 3789 begin
    ps_append_field("UmlClass_ps", "ObjectDescription",
    "GenericObject", "", ps_NoteDescriptionExternal, NULL);
    // ECR 3789 end
    ps_append_field("UmlClass_ps", "Stereotype", "UmlExtensibilityDefinition", "UmlStereotype", ps_StringAnt, NULL);
    ps_append_field("UmlClass_ps", "Constraints", "UmlExtensibilityDefinition", "UmlConstraintItem", ps_StringList, NULL);
    ps_append_field("UmlClass_ps", "TaggedValues", "UmlExtensibilityDefinition", "UmlTaggedValue", ps_StringList, NULL);

    // ECR 4654
    // a field of type StringChoice and the corresponding StringAnt field must be in exactly that order
    ps_append_field("UmlClass_ps", "MultiplicityChoice", "UmlClassDefinition", "UmlMultiplicity", ps_StringChoice, string_to_list("_ 0..1 0..* 1..* 1 *", " "));
    // ECR 4654 end
    ps_append_field("UmlClass_ps", "Multiplicity", "UmlClassDefinition", "UmlMultiplicity", ps_StringAnt, NULL);
    //ps_append_field("UmlClass_ps", "ElaboratedType", "UmlExtensibilityDefinition", "UmlStereotype", ps_Choice, string_to_list("Class ParameterizedClass InstantiatedClass", " "));
    ps_append_field("UmlClass_ps", "External", "UmlClassDefinition", "UmlClassIsImported", ps_TrueFalse, NULL);
    ps_append_field("UmlClass_ps", "Abstract", "UmlClassDefinition", "UmlClassIsAbstract", ps_TrueFalse, NULL);
    // Added _ for ECR4270
    ps_append_field("UmlClass_ps", "Visibility", "UmlClassDefinition", "UmlClassVisibility", ps_Choice, string_to_list("_ public protected private", " " ));
    
    ps_fill("UmlClass_ps");

    //ps_dump();

    return;
}

// --- START PATCH ECR 6726 ---
//-----------------------
// Attributes
//-----------------------
void
UmlAttribute_ps_show()
{
    // ECR 6880 Property sheets from table editors
    PropSheet__show("UmlAttribute_ps");
    // gde_builtin("PropertySheetShow UmlAttribute_ps");
    // End ECR 6880
    // reset
    UmlAttribute_ps_init();
}

boolean UmlAttribute_ps_initted = False;

void
UmlAttribute_ps_init()
{
    if (UmlAttribute_ps_initted)
    {
        // just refresh the prop sheet
        ps_fill("UmlAttribute_ps");
        return;
    }
    UmlAttribute_ps_initted = True;
    // gde_builtin("EditorActionDone 0214951139  UmlAttribute_ps_ActionDone();"); ECR 6880

    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() == "gte")
        ps_init("UmlAttribute_ps", "ObjectName", ps_Node, string_to_list("AttributeHsect AttributeNameCol", " "), ps_Node);
    else // if (PropSheet__editor_type() == "gde")
        ps_init("UmlAttribute_ps", "ObjectName", ps_Node, string_to_list(" Attribute ", " "), ps_Node);
    // ECR 6880 end

    ps_append_field("UmlAttribute_ps", "ObjectDescription", "GenericObject", "", ps_NoteDescriptionExternal, NULL);
    ps_append_field("UmlAttribute_ps", "Stereotype", "UmlExtensibilityDefinition", "UmlStereotype", ps_StringAnt, NULL);
    ps_append_field("UmlAttribute_ps", "Constraints", "UmlExtensibilityDefinition", "UmlConstraintItem", ps_StringList, NULL);
    ps_append_field("UmlAttribute_ps", "TaggedValues", "UmlExtensibilityDefinition", "UmlTaggedValue", ps_StringList, NULL);
    ps_fill("UmlAttribute_ps");
    return;
}

//-----------------------
// Operations
//-----------------------

void
UmlOperation_ps_show()
{
    // ECR 6880 Property sheets from table editors
    PropSheet__show("UmlOperation_ps");
    // gde_builtin("PropertySheetShow UmlOperation_ps");
    // ECR 6880
    // reset
    UmlOperation_ps_init();
}

boolean UmlOperation_ps_initted = False;

void
UmlOperation_ps_init()
{
    if (UmlOperation_ps_initted)
    {
        // just refresh the prop sheet
        ps_fill("UmlOperation_ps");
        return;
    }
    UmlOperation_ps_initted = True;
    // gde_builtin("EditorActionDone 0214951139  UmlOperation_ps_ActionDone();"); // ECR 6880

    // ECR 6880 Property sheets from table editors
    if (PropSheet__editor_type() == "gte")
        ps_init("UmlOperation_ps", "ObjectName", ps_Node, string_to_list("OperationHsect OperationNameCol", " "), ps_Node);
    else // if (PropSheet__editor_type() == "gde")
        ps_init("UmlOperation_ps", "ObjectName", ps_Node, string_to_list(" Operation ", " "), ps_Node);
    // ECR 6880 end

    ps_append_field("UmlOperation_ps", "ObjectDescription", "GenericObject", "", ps_NoteDescriptionExternal, NULL);
    ps_append_field("UmlOperation_ps", "Stereotype", "UmlExtensibilityDefinition", "UmlStereotype", ps_StringAnt, NULL);
    ps_append_field("UmlOperation_ps", "Constraints", "UmlExtensibilityDefinition", "UmlConstraintItem", ps_StringList, NULL);
    ps_append_field("UmlOperation_ps", "TaggedValues", "UmlExtensibilityDefinition", "UmlTaggedValue", ps_StringList, NULL);
    ps_fill("UmlOperation_ps");
    return;
}
// --- END PATCH ECR 6726 ---
