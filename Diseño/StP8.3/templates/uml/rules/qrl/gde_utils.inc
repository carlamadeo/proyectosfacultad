//
//      StP/Booch
//      StP/OMT
//      StP/OO
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//


//#include	"rules/qrl/gde_qrl_std.inc"

void
print_segs(string what, list syms)
{
    int		i;
    gde_symbol	s;

    message(what + ": Symbols: " + list_count(syms));

    for(i = 0; i < list_count(syms); i = i + 1)
    {
	s = list_get(syms, i);
	print_symbol("", s);
    }
    message("\n");
}

void
print_symbol(string what, gde_symbol s)
{
    gde_node	n;
    gde_arc	a;
    gde_cntx	c;

    if (s == NULL)
    {
	message("    " + w + ": NULL symbol");
	return;
    }

    if (gde_symbol_is_node(s))
    {
	n = to_gde_node(s);
	print_node(what, n);
    } else if (gde_symbol_is_cntx(s))
    {
	c = to_gde_cntx(s);
	print_cntx(what, c);
    } else if (gde_symbol_is_arc(s))
    {
	a = to_gde_arc(s);
	print_arc(what, a);
    }
}

void
print_arc(string w, gde_arc a)
{
    string l;
    string ds;

    if (a == NULL)
    {
	message("    " + w + ": NULL arc");
	return;
    }

    l = gde_arc_label(a);
    if (l == NULL)
    {
	l = "";
    }

    ds = " offs (" + gde_arc_from_dx(a) + "," + gde_arc_from_dy(a) + ")->(" +
		gde_arc_to_dx(a) + "," + gde_arc_to_dy(a) + ")";

    message("    " + w + ": arc '" + l +
	"'  id: " + gde_arc_psymid(a) +
	" (" + gde_arc_fromx(a) + "," + gde_arc_fromy(a) + ")->(" +
		gde_arc_tox(a) + "," + gde_arc_toy(a) + ")" + ds);
}

void
print_node(string w, gde_node n)
{
    string l;

    if (n == NULL)
    {
	message("    " + w + ": NULL node");
	return;
    }

    l = gde_node_label(n);
    if (l == NULL)
    {
	l = "";
    }

    message("    " + w + ": node '" + l +
	"' id: " + gde_node_psymid(n) +
	" (" + gde_node_x(n) + "," + gde_node_y(n) + ")");
}

void
print_cntx(string w, gde_cntx c)
{
    string l;
    gde_node	n;


    if (c == NULL)
    {
	message("    " + w + ": NULL cntx");
	return;
    }

    n = to_gde_node(to_gde_symbol(c));

    l = gde_node_label(n);
    if (l == NULL)
    {
	l = "";
    }

    message("    " + w + ": cntx '" + l +
	"' id: " + gde_node_psymid(n) +
	" (" + gde_node_x(n) + "," + gde_node_y(n) + ")");
}


void
print_node_list(list l, string s)
{
    gde_node	n;
    int		i;
    message("node list: " + s);
    for(i = 0; i < list_count(l); i = i + 1)
    {
	n = list_get(l, i);
	print_node("", n);
    }
}

void
print_arc_list(list l, string s)
{
    gde_arc	n;
    int		i;
    message("arc list: " + s);
    for(i = 0; i < list_count(l); i = i + 1)
    {
	n = list_get(l, i);
	print_arc("", n);
    }
}
