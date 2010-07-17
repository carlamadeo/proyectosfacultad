// Begin ECR947, ECR 4494
// New common file
int
idehook_note_link_active_func()
{
    list            symbols;

    idehook_did_selection_change();

    if (idehook_note_link_active_func_lv != ACTIVE_FUNC_IS_UNDEFINED)
	return idehook_note_link_active_func_lv;

    idehook_note_link_active_func_lv = ACTIVE_FUNC_IS_INACTIVE;
    symbols = the_last_selection;

    if (list_count(symbols) == 2) {
	if (list_count(gde_symbols_find_with_types(symbols, uml_note_link_symbols())) == 1 &&
	    list_count(gde_symbols_find_with_types(symbols, string_to_list("Note", " "))) == 1)
	    idehook_note_link_active_func_lv = ACTIVE_FUNC_IS_ACTIVE;
    }
    return idehook_note_link_active_func_lv;
}

void
ConnectNoteToLink()
{
    list            symbols, links, comments;
    gde_arc         link;
    gde_node        comment;
    gde_cntx        anchor;

    symbols = gde_selected_symbols();
    links = gde_symbols_find_with_types(symbols, uml_note_link_symbols());
    comments = gde_symbols_find_with_types(symbols, string_to_list("Note", " "));

    if (list_count(links) != 1 || list_count(comments) != 1) {
	gde_print_error("Incorrect symbol(s) selected for Connect Note To Link.");
	return;
    }
    link = to_gde_arc(list_get(links, 0));
    anchor = create_anchor_on_arc(link, "CommentAnchor");

    comment = to_gde_node(list_get(comments, 0));
    gde_arc_create("NoteLink", comment, anchor);
}

// moved from uclassd_decorate_class.inc
gde_cntx
create_anchor_on_arc(gde_arc a, string type)
{
    gde_cntx    ca;
    int        fx, fy, tx, ty, x, y, i, dx, dy, nx, ny;
    list    symbols;
    gde_symbol    s, as, ps;
    gde_node    n;
    list    cxs;


    cxs = find_cntxs_on_parent(a, type);

    as = to_gde_symbol(a);
    symbols = gde_all_symbols();

    fx = gde_arc_fromx(a);
    fy = gde_arc_fromy(a);
    tx = gde_arc_tox(a);
    ty = gde_arc_toy(a);

    x = (fx + tx) / 2;
    y = (fy + ty) / 2;


    for(i = 0; i < list_count(cxs); i++)
    {
        ca = list_get(cxs, i);
        nx = gde_cntx_x(ca);
        ny = gde_cntx_y(ca);
        dx = x - nx;
        if (dx < 0)
            dx = -dx;
        dy = y - ny;
        if (dy < 0)
            dy = -dy;
        if (dx < 20 && dy < 20)
        {
            return(ca);
        }
    }

    ca = gde_cntx_create(type, a, x, y);
    return(ca);
}
// End ECR947, ECR 4494
