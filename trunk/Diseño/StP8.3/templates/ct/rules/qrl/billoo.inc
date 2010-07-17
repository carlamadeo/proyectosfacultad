
/* oo-specific routines that use the bill */
 
int Booch_node_x_spacing = 200;
int OMT_node_x_spacing = 180;
int node_y_spacing = 70;

void
DrawClassHierarchyFromGraph(list classnames, graph G, string method)
{
  /* method is either "OMT", "Booch" or "UML", depending on the product */

  int             i, j, x, y;
  gde_node        c, subclass, superclass;
  list            subs, supers, nodes, drawn_nodes, drawn_links;
  string          sub, super;

  gde_save_state();
  int             defer = gde_start_edit_transaction();

  string classname, linkname;
  gde_symbol class;
  nodes = list_create( "gde_node", 0);
  drawn_nodes = list_create( "string", 0 );
  drawn_links = list_create( "string", 0 );
  BILL_Digraph    BDG = bill_digraph_create();

  int k;
  for( k = 0; k < list_count( classnames ); k++ )
    {
      classname = list_get( classnames, k );

      if (!graph_is_node(G, classname ))
	continue;

      /* create a layout-digraph and load it with the class hierarchy below classname */
      layout_subclasses(classname, G, BDG, method);

      list_append(nodes, create_class_symbol(classname, G, BDG, method));
      list_append( drawn_nodes, classname );
      
      /* draw all the subclasses and their descending triangles */
      subs = graph_node_indirect_successors(G, classname);
      for (i = 0; i < list_count(subs); ++i)
	{
	  sub = list_get(subs, i);
	  if ( list_find( drawn_nodes, 0, sub ) == list_count( drawn_nodes ))
	    {
	      list_append( nodes, create_class_symbol(sub, G, BDG, method));
	      list_append( drawn_nodes, sub );
	    }
	}

      /* draw all the uplinks */
      for (i = 0; i < list_count(subs); ++i)
	{
	  sub = list_get(subs, i);
	  subclass = find_class_by_label(nodes, sub);
	  if (subclass != NULL)
	    {
	      supers = graph_node_predecessors(G, sub);
	      for (j = 0; j < list_count(supers); ++j)
		{
		  super = list_get(supers, j);
		  // if we have fan-in, then store the links we have done
		  // before to prevent us drawing twice or more
		  if ( list_count( supers ) > 1 )
		    {
		      linkname = sub + "," + super;
		      if ( list_find( drawn_links, 0, linkname ) <
			   list_count( drawn_links ) )
			  continue;
		    }
		  else
		    linkname = NULL;

		  if (method == "OMT")
		    superclass = find_generalization_by_label(nodes, super);
		  else
		    superclass = find_class_by_label(nodes, super);
		  
		  if (superclass != NULL)
		    {
		      create_superclass_link(sub, super, subclass,
					     superclass, G, BDG, method);
		      if ( linkname != NULL )
			list_append( drawn_links, linkname );
		    }
		}
	    }
	}
    }
  gde_end_edit_transaction(0, defer, False);
  return;
}

void
layout_subclasses(string classname, graph G, BILL_Digraph BDG, string method)
{
  // layout all the subclasses of the chosen class 

  list            subs, supers;
  int i, j;
  string          sub, super;

  // for each direct and indirect subclass
  subs = graph_node_indirect_successors(G, classname);
  for (i = 0; i < list_count(subs); ++i) {
    sub = list_get(subs, i);
    supers = graph_node_predecessors(G, sub);
    for (j = 0; j < list_count(supers); ++j) {
      super = list_get(supers, j);

      // add the subclass link, from the super to the sub
      bill_digraph_add_link(BDG, super, sub);
    }
  }

  // do a layout
  if (method == "OMT" || method == "UML" )
    bill_digraph_set_spacing(BDG, node_y_spacing, OMT_node_x_spacing);
  else 
    bill_digraph_set_spacing(BDG, node_y_spacing, Booch_node_x_spacing);

  bill_digraph_layout(BDG);
}

gde_node
create_class_symbol(string classname, graph G, BILL_Digraph BDG, string method)
{
  // create a class symbol at the position specified by the layout digraph
  // if the class has subclasses, and the method is OMT
  // create the attached generalization triangle also

  list subs;
  gde_node c, g;
  int x, y;

  x = bill_digraph_node_get_x(BDG, classname);
  y = bill_digraph_node_get_y(BDG, classname);
//ECR 197 begin
    if (method == "UML")
    {
        list ln = gde_nodes_find_by_type(gde_all_symbols(), "Class");
        if(ln != NULL)
        {
            c = find_class_by_label( ln, classname);
            if(c != NULL)
              return c;
        }
    }
//ECR 197 end
  c = gde_node_create("Class", x, y);
  gde_node_set_label(c, classname);
  if (method == "OMT") {
    subs = graph_node_successors(G, classname);
    if (list_count(subs) > 0) {
      g = gde_node_create("ExclusiveGeneralization", x, y + node_y_spacing);
      gde_arc_create("GeneralizationParent", g, c);
    }
  }
  return c;
}


void
create_superclass_link(string sub, string super, gde_node subclass, gde_node superclass, graph G, BILL_Digraph BDG,
		       string method)
{
  // create a link from the subclass to the superclass, interposing dummy vertices if needed

  int vi;
  list vertices;
  gde_node last_node, v;
  BILL_Coord vertex;
  string linktype;
  
//ECR 197 begin  
  list arcs, links;
  gde_arc arc;
  gde_node n;
  int i, j;

    if ( method == "UML" )
    {
        linktype = "Generalization";
        arcs = gde_node_inarcs(superclass);
        for (i = 0; arcs != NULL && i < list_count(arcs); i++ ) 
        {
            arc = list_get(arcs, i);
            if (linktype == gde_lt_name(gde_arc_lt(arc))) 
            {
                links = gde_arc_links(list_get(arcs, i));
                for (j = 0; links != NULL && j < list_count(links); j++)
                {
                    n = gde_link_fromnode(list_get(links, j));
                    if (n == subclass)
                        return;
                }
            }
        }
    }
//ECR 197 end
    else if (method == "OMT")
        linktype = "GeneralizationChild";
    else
        linktype = "Inheritance";

  if (bill_digraph_link_is_long(BDG, super, sub)) {
    last_node = subclass;
    vertices = bill_digraph_link_get_dummy_vertices(BDG, super, sub);
    for (vi = 0; vi< list_count(vertices); vi++) {
      vertex = list_get(vertices, vi);
      v = gde_node_create("Vertex", vertex.x, vertex.y);
      gde_arc_create(linktype, last_node, v);
      last_node = v;
    }
    gde_arc_create(linktype,  last_node, superclass);
  }
  else
    gde_arc_create(linktype, subclass, superclass);
}

gde_node
find_class_by_label(list gde_nodes, string label) 
{
  int i;
  gde_node c;
  for (i = 0; i < list_count(gde_nodes); ++i) {
    c = list_get(gde_nodes, i);
    if (gde_node_label(c) == label)
      return (c);
  }
  return (NULL);
}

gde_node
find_generalization_by_label(list gde_nodes, string label) 
{
  int i,j,k;
  list arcs, links;
  gde_node c;
  gde_arc arc;
  gde_link link;

  for (i = 0; i < list_count(gde_nodes); ++i) {
    c = list_get(gde_nodes, i);
    if (gde_node_label(c) == label) {
      arcs = gde_node_inarcs(c);
      for (j = 0; j < list_count(arcs); ++j) {
	arc = list_get(arcs, j);
	links = gde_arc_links(arc); 
	for (k = 0; k < list_count(links); ++k) {
	  link = list_get(links, k);
	  if (gde_lt_name(gde_link_lt(link)) == "GeneralizationParent")
	    return gde_link_fromnode(link);
	}
      }
    }
  }
  return (NULL);
}


