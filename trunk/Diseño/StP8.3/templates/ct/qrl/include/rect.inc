//
//      sccsid[] = %W% %Y% %D%
//      StP/Booch
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1992-1995
//      All rights reserved
//


struct	rect_tp
{
    int		minx;
    int		maxx;
    int		miny;
    int		maxy;
    int		cx;
    int		cy;
};

const	int	RECT_TP_HUGE = 1000000;

string
rect_string(rect_tp	r)
{
    return("(" + r.cx + "," + r.cy + ") [" + r.minx + ", " + r.miny + "]-[" + r.maxx + ", " + r.maxy + "]");
}

void
debug_rect_tp(string what, rect_tp r)
{
    message(what + ":" + rect_string(r));
}



void
rect_null(rect_tp r)
{
    r.minx = 0;
    r.miny = 0;
    r.maxx = 0;
    r.maxy = 0;
    r.cx = 0;
    r.cy = 0;
}

void
rect_union(rect_tp res, rect_tp src)
{
    if (src.maxx > res.maxx)
	res.maxx = src.maxx;
    if (src.maxy > res.maxy)
	res.maxy = src.maxy;
    if (src.minx < res.minx)
	res.minx = src.minx;
    if (src.miny < res.miny)
	res.miny = src.miny;
    res.cx = (res.minx + res.maxx) / 2;
    res.cy = (res.miny + res.maxy) / 2;
}


rect_tp
gde_symbol_rect(gde_symbol s)
{
    gde_node	n;
    gde_arc	a;
    int		w, h, x, y;
    rect_tp	r;

    if (gde_symbol_is_node(s) || gde_symbol_is_cntx(s))
    {
	n = to_gde_node(s);
	w = gde_node_width(n);
	h = gde_node_height(n);
	x = gde_node_x(n);
	y = gde_node_y(n);
	r.minx = x - w/2;
	r.maxx = x + w/2;
	r.miny = y - h/2;
	r.maxy = y + h/2;
	r.cx = x;
	r.cy = y;
    } else if (gde_symbol_is_arc(s))
    {
	a = to_gde_arc(s);
	r.minx = gde_arc_fromx(a);
	r.maxx = gde_arc_tox(a);
	r.miny = gde_arc_fromy(a);
	r.maxy = gde_arc_toy(a);
	r.cx = (r.minx + r.maxx) / 2;
	r.cy = (r.miny + r.maxy) / 2;
    } else {
	return(NULL);
    }
    return(r);
}

rect_tp
rect_calculate_for_arcs(list segs)
{
    gde_symbol	s;
    int		i;
    rect_tp	r, rr;

    rr.minx = RECT_TP_HUGE;
    rr.maxx = -rr.minx;
    rr.miny = RECT_TP_HUGE;
    rr.maxy = -rr.miny;

    for(i = 0; i < list_count(segs); i = i + 1)
    {
	s = list_get(segs, i);
	if (gde_symbol_is_arc(s))
	{
	    r = gde_symbol_rect(s);
	    if (r == NULL)
	    {
		continue;
	    }
	    if (r.minx < rr.minx)
	    {
		rr.minx = r.minx;
	    }
	    if (r.miny < rr.miny)
	    {
		rr.miny = r.miny;
	    }
	    if (r.maxx > rr.maxx)
	    {
		rr.maxx = r.maxx;
	    }
	    if (r.maxy > rr.maxy)
	    {
		rr.maxy = r.maxy;
	    }
	}
    }
    return(rr);
}



rect_tp
rect_calculate_for_segs(list segs)
{
    list	lsegs;
    gde_symbol	s;
    int		i;
    gde_cntx	c;
    rect_tp	r, rr;

    rr.minx = RECT_TP_HUGE;
    rr.maxx = -rr.minx;
    rr.miny = RECT_TP_HUGE;
    rr.maxy = -rr.miny;

    lsegs = list_copy(segs);


    for(i = 0; i < list_count(lsegs); i = i + 1)
    {
	s = list_get(lsegs, i);
	r = gde_symbol_rect(s);
	if (r == NULL)
	{
	    continue;
	}
	if (r.minx < rr.minx)
	{
	    rr.minx = r.minx;
	}
	if (r.miny < rr.miny)
	{
	    rr.miny = r.miny;
	}
	if (r.maxx > rr.maxx)
	{
	    rr.maxx = r.maxx;
	}
	if (r.maxy > rr.maxy)
	{
	    rr.maxy = r.maxy;
	}
    }
    rr.cx = (rr.minx + rr.maxx) / 2;
    rr.cy = (rr.miny + rr.maxy) / 2;
    return(rr);
}

