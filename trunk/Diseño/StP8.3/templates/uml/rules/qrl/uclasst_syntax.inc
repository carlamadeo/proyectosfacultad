//
//      sccsid[] = %W% %Y% %D%
//      StP/OO
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//

/**/


boolean
CheckSyntax()
{
    int             errors = 0;
    int             i, hs, vs, namecol, typecol, sigcol, firstrow, lastrow;
    string          member, name, sig, class_name;
    set             members;

    /*
     * get the name of the class, for nice error messages
     */
    class_name = gte_get_cell_label(gte_get_last_row(gte_lookup_symbol("ClassHsect")), 1);

    if (class_name == "") {
        gte_print_error("No class has been named.",
            gte_get_cell_id(gte_get_last_row(gte_lookup_symbol("ClassHsect")), 1));
        errors = errors + 1;
    }

    /*
       * don't allow duplicate attribute/operation rows to be defined because
       * the result of multiple definitions is undefined (i.e., which one wins
       * and actually gets generated to the repository?)
       */

    /* attributes by name */
    hs = gte_lookup_symbol("AttributeHsect");
    vs = gte_lookup_symbol("MainDefinitionVsect");
    namecol = gte_lookup_symbol("AttributeNameCol");
    typecol = gte_lookup_symbol("AttributeTypeCol");
    firstrow = gte_get_first_row(hs);
    lastrow = gte_get_last_row(hs);
    members = set_create("string");
    for (i = 1; i <= (lastrow - firstrow) + 1; i = i + 1)
    {
        name = gte_get_cell_label(gte_get_row(hs, i), gte_get_col(vs, namecol));
        if (name == NULL || name == "")
            continue;
        member = name;
        if (set_is_member(members, member))
        {
            gte_print_error("Attribute '" + class_name + "::" + member + "' must be unique within the table.",
                gte_get_cell_id(gte_get_row(hs, i),
                gte_get_col(vs, namecol)));
            errors = errors + 1;
        } else {
            set_add(members, member);
		}
    }

    /* operations by name,sig */
    hs = gte_lookup_symbol("OperationHsect");
    vs = gte_lookup_symbol("MainDefinitionVsect");
    namecol = gte_lookup_symbol("OperationNameCol");
    sigcol = gte_lookup_symbol("OperationSignatureCol");
    firstrow = gte_get_first_row(hs);
    lastrow = gte_get_last_row(hs);
    members = set_create("string");
    for (i = 1; i <= (lastrow - firstrow) + 1; i = i + 1)
    {
        name = gte_get_cell_label(gte_get_row(hs, i), gte_get_col(vs, namecol));
        if (name == NULL || name == "")
            continue;
        sig = gte_get_cell_label(gte_get_row(hs, i), gte_get_col(vs, sigcol));
        member = name + "(" + sig + ")";
        if (set_is_member(members, member))
        {
            gte_print_error("Operation '" + class_name + "::" + member + "' must be unique within the table.",
                gte_get_cell_id(gte_get_row(hs, i),
                gte_get_col(vs, namecol)));
            errors = errors + 1;
        } else
            set_add(members, member);
    }

    return (errors == 0);
}

