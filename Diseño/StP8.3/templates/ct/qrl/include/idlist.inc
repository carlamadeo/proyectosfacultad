//////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = %W% %Y% %D%
//	StP Core
//      Confidential property of Interactive Development Environments, Inc.
//      Copyright (c) 1994
//      All rights reserved
//
//////////////////////////////////////////////////////////////////////////////

// Utilities to deal with id-lists

int
idlist_update_from_list(string idl1, string tp, list oids)
{
    link	l;
    node	n;
    cntx	c;
    file	f;
    set		ids;
    int		oid, i;

    ids = set_create("int");
    for(i = 0; i < list_count(oids); i = i + 1)
    {
	set_add(ids, list_get(oids, i));
    }
    if (list_count(oids) > 0)
    {
	oid = list_get(oids, 0);
    } else {
	oid = 0;
    }
    if (set_count(ids) > 0)
    {
       id_list_update(idl1, tp, ids, NULL); 
    }
    return(list_count(oids));
}


int
idlist_from_oid_list(string idl1, string tp, list oids)
{
    link	l;
    node	n;
    cntx	c;
    file	f;
    set		ids;
    int		oid, i;

    ids = set_create("int");
    for(i = 1; i < list_count(oids); i = i + 1)
    {
	set_add(ids, list_get(oids, i));
    }
    if (list_count(oids) > 0)
    {
	oid = list_get(oids, 0);
    } else {
	oid = 0;
    }
    id_list_create(tp + "[id=${oid}]", idl1);
    if (set_count(ids) > 0)
    {
       id_list_update(idl1, tp, ids, NULL); 
    }
    return(list_count(oids));
}


int
merge_link_idlists(string idl1, string idl2, string tp)
{
    link	l;
    node	n;
    cntx	c;
    file	f;
    item        i;
    set		ids;

    ids = set_create("int");
    if (tp == "node")
    {
	for_each_in_select("${tp}[${idl2}]", n)
	{
	    set_add(ids, n.id);
	}
    } else if (tp == "link")
    {
	for_each_in_select("${tp}[${idl2}]", l)
	{
	    set_add(ids, l.id);
	}
    } else if (tp == "item")
    {
	for_each_in_select("${tp}[${idl2}]", i)
	{
	    set_add(ids, i.id);
	}
    } else if (tp == "cntx")
    {
	for_each_in_select("${tp}[${idl2}]", c)
	{
	    set_add(ids, c.id);
	}
    } else if (tp == "file")
    {
	for_each_in_select("${tp}[${idl2}]", f)
	{
	    set_add(ids, f.id);
	}
    } else {
	return(0);
    }

    if (set_count(ids) > 0)
    {
       id_list_update(idl1, tp, ids, NULL); 
    }
    return(set_count(ids));
}
