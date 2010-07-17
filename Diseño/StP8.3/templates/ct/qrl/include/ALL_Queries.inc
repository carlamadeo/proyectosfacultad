//*****************************************************************************
// DESCRIPTION: ALL_Queries.inc
//              ----------------------
//              (c) 1997 by AONIX GmbH
//*****************************************************************************
//
// Include file for all stpeports with basic query-functions returning 
// objects, lists or sets.
//*****************************************************************************
//
// functions: 
//            - file   FileOfTypeWithName(string the_file, string the_name)
//            - file   FileOfTypeWithNodeRefOfId(string the_file, int the_id)
//            - set    FilesOfTypeWithNameSet(string the_file, string the_name)
//            - set    FilesOfTypeWithNodesOfTypeAndNameSet(string file_type,  
//                                          string node_type, string node_name)
//            - set    FilesWithRefsOfObjIdSet(int obj_id, string pdm_type,
//                                             int c_file_id)
//            - T_ROWS GetFirstLastRows(int table_id, string the_node)
//            - int    GetNumberOfItems(int the_id, string the_item)
//            - int    GetNumberOfRows(int table_id, string the_node)
//            - item   ItemOfTypeWithObjId(string the_item, int the_id)
//            - item   ItemOfTypeWithValue(string the_item, string the_value)
//            - set    ItemsOfTypeSet(string the_item)
//            - set    ItemsOfTypeWithObjIdSet(string the_item, int the_id)
//            - set    ItemsOfTypeWithObjRefsInFileSet(string the_item, int 
//                                                     the_id, string the_ref)
//            - link   LinkWithObjId(int the_id)
//            - set    LinksOfTypeFromNodeToNode(string the_link, string 
//                                               f_node, string t_node)
//            - set    LinksOfTypeWithRefsWithFileIdSet(string the_link, int
//                                                      file_id, string order)
//            - set    LinksRefsInFileSet(string the_link, int the_id, 
//                                        string sort_by)
//            - set    MatchingFilesOfTypeSet(string the_file,string need_ref,
//                                            string reduce_to)
//            - set    MatchingNodeContainedInPackageSet(string the_node, 
//                                            string red_node, string red_pack)
//            - list   MatchingNodesOfTypeList(string the_node, reduce_to)
//            - set    MatchingNodesOfTypeSet(string the_node, reduce_to)
//            - node   NodeOfTypeWithName(string the_node, string the_name)
//            - node   NodeOfTypeWithRefsWithFileId(string the_node,int the_id)
//            - set    NodesOfTypeContainedInSet(string c_node, string p_node, 
//                                               string reduce_to)
//            - node   NodeWithObjId(int the_id)
//            - set    NodesOfTypeSet(string the_node)
//            - set    NodesOfTypeWithLinkOfTypeToNodeIdSet(string node_type,  
//                                          string link_dir, string node_dir, 
//                                          string cont_type, string node_name)
//            - set    NodesOfTypeWithRefsWithFileIdSet(string the_node, int
//                                                      file_id, string order)
//            - set    NodesRefsInFileSet(string the_node, int the_id, 
//                                        string sort_by)
//            - note   NoteOfTypeWithObjId(string the_note, int the_id)
//            - note   NoteWithId(int note_id)
//            - set    NotesOfTypeWithObjIdSet(string the_note, int the_id)
//            - set    ObjectsWithItemOfTypeAndValueSet(string o_type, string 
//                                                      i_type, string i_value)
//            - node   ScopedNodeOfTypeInSameFile(int sc_id, string the_type, 
//                                                int file_id)
//            - set    ScopedNodesOfTypeSet(int scope_id, string the_node)
//            - set    TopPackagesSet()
//            - set    UmlScenarioInstanceFileSet(string file_type, string 
//                                                p_type, string p_name)
//*****************************************************************************


//*****************************************************************************
// FileOfTypeWithName: returns the matching file or NULL
//                     - the_file : application type of the file
//                     - the_name : name of the file
//*****************************************************************************
file  FileOfTypeWithName(string the_file, string the_name)
{
   return (find_by_query("file[" + the_file + " && name $ '${the_name}']"));
}


//*****************************************************************************
// FileOfTypeWithNodeRefOfId: returns the matching file or NULL
//                            - the_file : application type of the file
//                            - the_id :   id of the node referenced in the file
//*****************************************************************************
file  FileOfTypeWithNodeRefOfId(string the_file, int the_id)
{
   return (find_by_query("file[" + the_file + " && node_refs[node_id = " 
                       + "${the_id}]]"));
}


//*****************************************************************************
// FilesOfTypeWithNameSet: returns a set of matching files
//                         - the_file : application type of the file
//                         - the_name : name of the file
//*****************************************************************************
set  FilesOfTypeWithNameSet(string the_file, string the_name)
{
   return (set_select("file[" + the_file + " && name $ '${the_name}']"));
}


//*****************************************************************************
// FilesOfTypeWithNodesOfTypeAndNameSet: returns a set of files of the given  
//                                       filetype with noderefs of nodes of the
//                                       given nodetype and nodenames. 
//                                       - file_type : app type of the files
//                                       - node_type : app type of the nodes
//                                       - node_name : string with node names
//*****************************************************************************
set  FilesOfTypeWithNodesOfTypeAndNameSet(string file_type, string node_type 
                                        , string node_name)
{
   return (set_select("file[" + file_type + " && node_refs[node[" + node_type 
                    + " && (" + node_name + ")]]] sort by name"));
}


//*****************************************************************************
// FilesWithRefsOfObjIdSet: returns a set of files with references of the  
//                          object with the given Id. 
//                          If c_file_id != 0, the current file is excluded
//                          - obj_id    : id of the current object
//                          - pdm_obj   : object´s pdm type (node, link, cntx)
//                          - c_file_id : id of the current file
//*****************************************************************************
set  FilesWithRefsOfObjIdSet(int obj_id, string pdm_obj, int c_file_id)
{
   if   (c_file_id == 0)
   {
        return (set_select("file[" + pdm_obj + "_refs[" + pdm_obj + "_id = "  
                         + obj_id + "]] sort by type, name"));
   }
   else
   {
        return (set_select("file[" + pdm_obj + "_refs[" + pdm_obj + "_id = " 
                         + obj_id + "] && id != " + c_file_id 
						 + "] sort by type, name"));
   }
}


//*****************************************************************************
// FilesWithViewpointsOfObjIdSet: returns a set of files with viewpoints of the  
//                                node with the given Id. 
//                                - obj_id  : id of the current object
//*****************************************************************************
set  FilesWithViewpointsOfObjIdSet(int obj_id)
{
     return (set_select("file[node_refs[node_id = " + obj_id + " && viewpoints"
                      + "]] sort by type, name"));
}


//*****************************************************************************
// GetFirstLastRows: returns a T_ROWS struct filled with the first and last row
//                   occupied by the node of the given type in the given table 
//                  - table_id : id of the current table file
//                  - the_node : application type of the node
//*****************************************************************************
T_ROWS  GetFirstLastRows(int table_id, string the_node)
{
   T_ROWS    fila;
   int       rcnt, rx;
   node_ref  row_ref;
   set       ref_set;

   fila.first = 0;
   fila.last  = 0;

   ref_set = set_create("node_ref");
   ref_set = set_select("node_ref[file_id = ${table_id} && node[" + the_node
                      + "]] sort by ycoord");
   rcnt    = set_count(ref_set);

   if (rcnt > 0)
   {
      row_ref = set_get_element(ref_set, 0);
      if (row_ref.ycoord != NULL)
         fila.first = row_ref.ycoord;

      row_ref = set_get_element(ref_set, rcnt -1);
      if (row_ref.ycoord != NULL)
         fila.last = row_ref.ycoord;
   }

   set_clear(ref_set);

   return fila;
}


//*****************************************************************************
// GetNumberOfItems: returns the number of items of the given type for the
//                   given object 
//                   - the_id:   id of the current table file
//                   - the_item: application type of the item
//*****************************************************************************
int  GetNumberOfItems(int the_id, string the_item)
{
   return (selection_count("item[(" + the_item + ") && obj_id = ${the_id}]"));
}


//*****************************************************************************
// GetNumberOfRows: returns the number of rows occupied by the node of the
//                  given type in the current table 
//                  - table_id : id of the current table file
//                  - the_node : application type of the node
//*****************************************************************************
int  GetNumberOfRows(int table_id, string the_node)
{
   return (selection_count("node[" + the_node + " && node_refs[file_id = " 
                         + "${table_id}]]"));
}


//////////////////////////////////////////////////////////////////////////////
// ItemsOfTypeSet: returns a set of all items of the given item_type 
//                 - the_item : application type of the item
//////////////////////////////////////////////////////////////////////////////
set  ItemsOfTypeSet(string the_item)
{
   return (set_select("item[" + the_item + "] sort by value"));
}


//*****************************************************************************
// ItemOfTypeWithObjId: returns the matching item or NULL
//                      - the_item : application type of the item
//                      - the_id   : id of the described object
//*****************************************************************************
item  ItemOfTypeWithObjId(string the_item, int the_id)
{
   return (find_by_query("item[" + the_item + " && obj_id = ${the_id}]"));
}


//////////////////////////////////////////////////////////////////////////////
// ItemsOfTypeWithObjIdSet: returns a set of all items of the given item_type 
//                          belonging to the object with the given id.
//                          - the_item : application type of the item
//                          - the_id   : obj_id of the annotated object
//////////////////////////////////////////////////////////////////////////////
set  ItemsOfTypeWithObjIdSet(string the_item, int the_id)
{
   return (set_select("item[" + the_item + " && obj_id = ${the_id}]"));
}


//////////////////////////////////////////////////////////////////////////////
// ItemsOfTypeWithObjRefsInFileSet: returns a set of all items of the given  
//                                  item_type of an object with references in
//                                  the file with the given id.
//                                  - the_item : application type of the item
//                                  - the_id   : file_id of the current file
//                                  - the_ref  : node, link or cntx reference
//////////////////////////////////////////////////////////////////////////////
set  ItemsOfTypeWithObjRefsInFileSet(string the_item, int the_id
                                   , string the_ref)
{
   return (set_select("item[" + the_item + " && " + the_ref + "[" + the_ref 
                    + "_refs[file_id = ${the_id}]] ]"));
}


//*****************************************************************************
// ItemOfTypeWithValue: returns the matching item or NULL
//                      - the_item  : application type of the item
//                      - the_value : matching item value 
//*****************************************************************************
item  ItemOfTypeWithValue(string the_item, string the_value)
{
   return (find_by_query("item[" + the_item + " && value = '${the_value}']"));
}


//*****************************************************************************
// LinkWithObjId: returns the matching link or NULL
//                - the_id :  id of the link 
//*****************************************************************************
link  LinkWithObjId(int the_id)
{
   return (find_by_query("link[id = ${the_id} && link_refs]"));
}


//*****************************************************************************
// LinksOfTypeFromNodeToNode: returns a set of all refd links of the given 
//                            link_type from nodes of f_type to nodes of t_type
//                            - the_link : application type of the link
//                            - f_node   : application type of the from_node
//                            - t_node   : application type of the to_node
//*****************************************************************************
set  LinksOfTypeFromNodeToNode(string the_link, string f_node, string t_node)
{
   return (set_select("link[" + the_link + " && link_refs && from_node["
                    + f_node + "] && to_node[" + t_node + "]]"));
}


//*****************************************************************************
// LinksOfTypeWithRefsWithFileIdSet: returns a set of the links of the given 
//                                   link_type referenced in the current file
//                                   in alphabetical or vertical (diagrams) or 
//                                   horizontal (table) order
//                                   - the_link : app type of the link
//                                   - file_id :  file_id of the current file
//                                   - order:     x- or y-coord. (if !alphab.)
//*****************************************************************************
set  LinksOfTypeWithRefsWithFileIdSet(string the_link, int file_id
                                    , string order)
{
   int      scnt, sx,
            set_size;

   link     one_link;

   link_ref one_ref;

   set      ordered_set,
            via_link_refs;

   ordered_set = set_create("link");

   if   (e_PRINT_ALPHA_ORDER)
        ordered_set = set_select("link[(" + the_link + ") && link_refs[" 
		            + "file_id = " + file_id + "]] sort by name");
   else  
   {
        via_link_refs = set_create("link_ref");
        via_link_refs = LinksRefsInFileSet(the_link, file_id, order);
        scnt          = set_count(via_link_refs);

        for (sx = 0; sx < scnt; sx = sx + 1)
        {
	        one_ref  = set_get_element(via_link_refs, sx);
            one_link = LinkWithObjId(one_ref.link_id);

            if (one_link != NULL)
               set_size = set_add(ordered_set, one_link);
        }
        set_clear(via_link_refs);
   }
   return ordered_set;
}


//*****************************************************************************
// LinksRefsInFileSet: returns a set of the link_refs of the given link_type 
//                     referenced in the current file. 
//                     - the_link : application type of the link
//                     - the_id   : file_id of the current file
//		               - sort_by  : sort by attribute of the link_ref object
//*****************************************************************************
set  LinksRefsInFileSet(string the_link, int the_id, string sort_by)
{
   return (set_select("link_ref[file_id = " + the_id + " && link[(" + the_link
		            + ")]] sort by " + sort_by ));
}



//*****************************************************************************
// MatchingFilesOfTypeSet: returns a set of all referenced files of the  
//                         given file type matching with the restrictions 
//                         in reduce_to
//                         - the_file  : application type of the files
//                         - need_ref  : do they have to be referenced  ?
//                         - reduce_to : string with filenames
//*****************************************************************************
set  MatchingFilesOfTypeSet(string the_file, string need_ref, string reduce_to)
{
   return (set_select("file[" + the_file + need_ref + " && (" + reduce_to  
                    + ")] sort by name"));
}


//*****************************************************************************
// MatchingNodeContainedInPackageSet: returns a set of refd nodes of the given 
//                                    node_type contained in a package, 
//                                    matching the given reductions 
//                                    - the_node : app type of the node
//                                    - red_node : string with node names
//                                    - red_pack : string with package names
//*****************************************************************************
set  MatchingNodeContainedInPackageSet(string the_node, string red_node
                                     , string red_pack)
{
   return (set_select("node[" + the_node + " && node_refs && (" + red_node 
                    + ") && in_links[" + m_CONTAINS_LINK + " && from_node[" 
                    + m_PACKAGE_NODE + " && node_refs && (" + red_pack 
                    + ")]]] sort by name"));
}


//*****************************************************************************
// MatchingNodesOfTypeList: returns a list of refd nodes of the given node_type
//                          matching the given reduction 
//                          - the_node : application type of the node
//                          - reduce_to : string with filenames
//*****************************************************************************
list  MatchingNodesOfTypeList(string the_node, string reduce_to)
{
   return (list_select("node[" + the_node + " && node_refs && (" + reduce_to 
                     + ")] sort by name"));
}


//*****************************************************************************
// MatchingNodesOfTypeSet: returns a set of refd nodes of the given node_type
//                         matching the given reduction 
//                         - the_node : application type of the node
//                         - reduce_to : string with filenames
//*****************************************************************************
set  MatchingNodesOfTypeSet(string the_node, string reduce_to)
{
   return (set_select("node[" + the_node + " && node_refs && (" + reduce_to 
                    + ")] sort by name"));
}


//*****************************************************************************
// NodeOfTypeWithName: returns the matching node 
//                     - the_node : application type of the node
//                     - the_name : name of the node
//*****************************************************************************
node  NodeOfTypeWithName(string the_node, string the_name)
{
   return (find_by_query("node[" + the_node + " && node_refs && " 
                       + "name = '${the_name}']"));
}


//*****************************************************************************
// NodeOfTypeWithRefsWithFileId: returns the matching node
//                               - the_node : application type of the node
//                               - the_id   : file_id of the referenced file
//*****************************************************************************
node  NodeOfTypeWithRefsWithFileId(string the_node, int the_id)
{
   return (find_by_query("node[" + the_node + " && node_refs[file_id = " 
                       + the_id + "]]"));
}


//*****************************************************************************
// NodesOfTypeContainedInSet: returns a set of all refd nodes of the given 
//                            node_type contained in nodes of the given type
//                            and names matching the reduction string
//                            - c_node    : type of the nodes we are looking for
//                            - p_node    : type of the containing node
//                            - reduce_to : string with node names
//*****************************************************************************
set  NodesOfTypeContainedInSet(string c_node, string p_node, string reduce_to)
{
   return (set_select("node[" + c_node + " && node_refs && in_links ["
                    + m_CONTAINS_LINK + " && link_refs && from_node[" 
                    + p_node + " && node_refs && (" + reduce_to + ")]]]"));
}


//*****************************************************************************
// NodesOfTypeSet: returns a set of all refd nodes of the given node_type 
//                 - the_node : application type of the node
//*****************************************************************************
set  NodesOfTypeSet(string the_node)
{
   return (set_select("node[" + the_node + " && node_refs] sort by name"));
}


//*****************************************************************************
// NodesOfTypeWithLinkOfTypeToNodeIdSet: returns a set of nodes of the given 
//                                       type that are linked by the given 
//                                       link to a node of the given id
//                                       - node_type : app type of the nodes
//                                       - link_dir  : in or out link
//                                       - node_dir  : from or to node
//                                       - link_type : app type of the inlink
//                                       - cont_id   : int with container id
//*****************************************************************************
set  NodesOfTypeWithLinkOfTypeToNodeIdSet(string node_type, string link_dir
                                        , string node_dir, string link_type
										, int cont_id)
{
   // SPR 2507 - unreferenced links not being filtered out of query
   return (set_select("node[(" + node_type + ") && " + link_dir + "_links["  
                    + link_type + " && " + node_dir + "_node_id = " + cont_id 
					+ "&& link_refs] && node_refs] sort by type"));
}


//*****************************************************************************
// NodesOfTypeWithRefsWithFileIdSet: returns a set of the nodes of the given 
//                                   node_type referenced in the current file
//                                   in alphabetical, vertical (diagrams) or 
//                                   horizontal (table) order
//                                   - the_node : app type of the node
//                                   - file_id :  file_id of the current file
//                                   - order:     x- or y-coord. (if !alphab.)
//*****************************************************************************
set  NodesOfTypeWithRefsWithFileIdSet(string the_node, int file_id
                                    , string order)
{
   int      scnt, sx,
            set_size;

   node     one_node;

   node_ref one_ref;

   set      ordered_set,
            via_node_refs;

   ordered_set = set_create("node");

   if   (e_PRINT_ALPHA_ORDER)
        ordered_set = set_select("node[(" + the_node + ") && node_refs[" 
		            + "file_id = " + file_id + "]] sort by type, name");
   else  
   {
        via_node_refs = set_create("node_ref");
        via_node_refs = NodesRefsInFileSet(the_node, file_id, order);
        scnt          = set_count(via_node_refs);

        for (sx = 0; sx < scnt; sx = sx + 1)
        {
	        one_ref  = set_get_element(via_node_refs, sx);
            one_node = NodeWithObjId(one_ref.node_id);

            if (one_node != NULL)
               set_size = set_add(ordered_set, one_node);
        }
        set_clear(via_node_refs);
   }
   return ordered_set;
}


//*****************************************************************************
// NodeWithObjId: returns the matching node or NULL
//                - the_id :  id of the node 
//*****************************************************************************
node  NodeWithObjId(int the_id)
{
   return (find_by_query("node[id = ${the_id} && node_refs]"));
}


//*****************************************************************************
// NodesRefsInFileSet: returns a set of the noderefs of the given node_type 
//                     referenced in the current file. 
//                     - the_node : application type of the node
//                     - the_id   : file_id of the current file
//		               - sort_by  : sort by attribute of the node_ref object
//*****************************************************************************
set  NodesRefsInFileSet(string the_node, int the_id, string sort_by)
{
   return (set_select("node_ref[file_id = " + the_id + " && node[(" + the_node
		            + ")]] sort by " + sort_by ));
}


//*****************************************************************************
// NoteOfTypeWithObjId: returns the matching note for the given id or NULL
//                      - the_note : application type of the note
//                      - the_id   : id of the described object
//*****************************************************************************
note  NoteOfTypeWithObjId(string the_note, int the_id)
{
   return (find_by_query("note[" + the_note + " && obj_id = ${the_id}]"));
}


//*****************************************************************************
// NoteWithId: returns the note with the given note_id or NULL (?!)
//                     - the_id: id of the owning note
//*****************************************************************************
note  NoteWithId(int note_id)
{
   return (find_by_query("note[id = ${note_id}]"));
}


//*****************************************************************************
// NotesOfTypeWithObjIdSet: returns a set of all notes of the given note_type 
//                          and the given object_id
//                          - the_note : application type of the note
//                          - the_id : obj_id of the annotated object
//*****************************************************************************
set  NotesOfTypeWithObjIdSet(string the_note, int the_id)
{
   return (set_select("note[" + the_note + " && obj_id = ${the_id}] "
                    + "sort by name"));
}


//*****************************************************************************
// ObjectsWithItemOfTypeAndValueSet: returns a set of objects with an item of 
//                                   the given type and value
//                                   - o_type : type of the object
//                                   - i_type : type of the item
//                                   - i_value: value of the item 
//*****************************************************************************
set  ObjectsWithItemOfTypeAndValueSet(string o_type, string i_type
                                    , string i_value)
{
   return (set_select(o_type + "[" + o_type + "_refs && items[" + i_type 
                    + " && value = '" + i_value + "']] sort by type"));
}


//*****************************************************************************
// ScopedNodeOfTypeInSameFile: returns the matching scoped node with
//                             identical node_refs in the same file
//                             - sc_id   :  the given scope node id
//                             - the_type:  application type of child node
//                             - file_id :  id of the current file
//*****************************************************************************
node  ScopedNodeOfTypeInSameFile(int sc_id, string the_type, int file_id)
{
   return (find_by_query("node[" + the_type + " && scope_node_id = " + sc_id
                       + " && node_refs[file_id = " + file_id + "]]"));
}


//*****************************************************************************
// ScopedNodesOfTypeSet: returns a list of all scoped nodes of the current 
//                       node having the given app type
//                       - scope_id : obj_id of the current (scope) node
//                       - the_node : application type of the node
//*****************************************************************************
set  ScopedNodesOfTypeSet(int scope_id, string the_node)
{
   return (set_select("node[" + the_node + " && node_refs && scope_node_id "
                    + "= ${scope_id}] sort by name"));
}


//*****************************************************************************
// TopPackagesSet: returns a set of packages not contained in other packages
//                  (we need this set to start the package hierarchy)
//*****************************************************************************
set  TopPackagesSet()
{
   return (set_select("node[" + m_PACKAGE_NODE + " && node_refs && !in_links ["
                    + m_CONTAINS_LINK + " && link_refs && from_node[" 
                    + m_PACKAGE_NODE + "]]] sort by name"));
}


//*****************************************************************************
// UmlScenarioInstanceFileSet: returns a set of scenario files of the given  
//                             filetype for the UmlParent with the given
//                             given type and names. 
//                             - file_type : app type of the files
//                             - p_type    : app type of the parent node
//                             - p_name    : name of the parent node 
//*****************************************************************************
set  UmlScenarioInstanceFileSet(string file_type, string p_type, string p_name)
{
   return (set_select("file[(" + file_type + ") && node_refs[node[" 
                    + m_SCENARIO_NODE + " && items[" + m_PARENT_TYPE_ITEM 
					+ " && value = '" + p_type + "'] && items[" 
					+ m_PARENT_NAME_ITEM + " && value = '" + p_name 
					+ "']]]] sort by type, name"));
}

