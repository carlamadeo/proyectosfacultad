//*****************************************************************************
// DESCRIPTION: ALL_Report.inc
//              ----------------------
//              (c) 1997 by AONIX 
//*****************************************************************************
//
// Include file for all StP-Reports with globaly used functions.
//*****************************************************************************
//
// functions: - void    BuildGraphHierarchy(graph the_graph, string the_link,
//                                          string f_node, string t_node)
//            - void    FirstPage() 
//            - int     GlossaryChapter()
//            - void    GlossaryForFile(int the_id)
//            - void    GlossaryForObject(int the_id)
//            - string  GlossarySourceInfo(int the_id)
//            - boolean IsPrinted(int obj_id, string ref_string, set ref_set)
//            - int     ListToSet(list the_list, set the_set)
//            - node    NextReqInScope(string the_node, int the_id, 
//                                     string to_or_from)
//            - void    PrintRequirement(node the_req, int level)
//            - void    PrintSucNodes(graph hierarchy, string the_node, 
//                                    int level)
//            - string  ReduceToFiles(string reduction).
//            - string  ReduceToObjects(string obj_type, string rel_type, 
//                                      int rel_dir, string reduction)
//            - int     ReqParents(node child, set req_printed, set req_levels)
//            - int     RequirementChapter()
//            - int     SetToList(set the_set, list the_list)
//            - string  SetToString(set the_set, string separator)
//            - int     StringToSet(set the_set, string the_string, 
//                                    string separator)
//            - string  StripInfo(string the_string)
//            - boolean TableOfContents() 
//*****************************************************************************
 

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Global Document Information Section
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// Constants for Document Information Section
//----------------------------------------------------------------------------
// Global RTF-Field-Tag for the TOC 
const string RTF_TOC = "\\par {\\field {\\*\\fldinst TOC \\\\o \"1-3\" }}";


//*****************************************************************************
// FirstPage: Prints the documents title page and sets field values (RTF) /
//            doc-variables (MIF) for page headers and footers
//*****************************************************************************

void  FirstPage() 
{  
   string  head_version;

   //==================================
   // Preprocessing
   //==================================

   e_DOC_TITLE = StripInfo(e_DOC_TITLE);

   if (e_DOC_TITLE == "")
      e_DOC_TITLE = " ";
   

   e_DOC_SUBTITLE = StripInfo(e_DOC_SUBTITLE);

   if   (e_DOC_SUBTITLE == ext_subtitle)
        e_DOC_SUBTITLE = info_title + current_system();
   else e_DOC_SUBTITLE = " ";
   

   e_DOC_AUTHOR = StripInfo(e_DOC_AUTHOR);
   
   if   (e_DOC_AUTHOR == "")
        e_DOC_AUTHOR = " ";
   else e_DOC_AUTHOR = "\t" + e_DOC_AUTHOR;


   e_COMPANY  = StripInfo(e_COMPANY);

   if (e_COMPANY == "")
      e_COMPANY = " ";


   e_DIVISION = StripInfo(e_DIVISION);

   if   (e_DIVISION == "")
         e_DIVISION = " ";
   else e_DIVISION = "\t" + e_DIVISION;


   e_DOC_VERSION = StripInfo(e_DOC_VERSION);

   if (e_DOC_VERSION == "")
       e_DOC_VERSION = " ";

   head_version   = e_DOC_VERSION;
   e_DOC_VERSION  = info_version + e_DOC_VERSION;

   
   e_DOC_STATUS = StripInfo(e_DOC_STATUS);
 
   if   (e_DOC_STATUS == "")
         e_DOC_STATUS = " ";
   else e_DOC_STATUS = "\n" + info_status + e_DOC_STATUS + "\n";


   //==================================
   // Print the First Page
   //==================================
   paragraph(TITLE); // twice to get some distance
   paragraph(TITLE);
   print(e_DOC_TITLE);
   paragraph(SUBTITLE);
   print(e_DOC_SUBTITLE);

   paragraph(DOCAUTHOR);
   print(e_DOC_AUTHOR);
   paragraph(COMPANY);
   print("\t" + e_COMPANY);
   paragraph(DIVISION);
   print(e_DIVISION);
   if (e_DIVISION != " ")
      print("\n");

   paragraph(DOCTITLE);
   print("\t" + e_DOC_TITLE);
   paragraph(DOCSUBTITLE);
   print("\t" + e_DOC_SUBTITLE);

   paragraph(DOCINFO);
   paragraph(DOCVERSION);
   PrintFrameMarker("0", head_version);
   print(e_DOC_VERSION);
   paragraph(DOCINFO);
   print(info_date + time_to_string(time_now(), date_format + time_format));
   print(e_DOC_STATUS);

   // For HTML:
   // Publishing the filename is not appropriate.
   // It would be useful to provide a count of the number of files
   // produced.  This requires an addition to QRL of a function 
   // to print the number of files generated.
   if (target() != HTML)
   {
       paragraph(DOCINFO);
       print(info_dsource);
       character_format_set(EXTRASMALL);
       PrintDocVar("Filename (Long)", "293", "FILENAME \\\\p", "UMLREPORT.RTF");
       paragraph(DOCINFO);
       print(info_pages);
       PrintDocVar("Page Count", "286", "NUMPAGES", "1");
       print(print_pages + "\n");
   }

   paragraph(DOCINFO);
   print(info_stpsys + current_system() + "\n");
   print(info_ssource);
   character_format_set(EXTRASMALL);
   print(current_projdir() + "\n");

   return;
}
   
//*****************************************************************************
// TableOfContents: Prints the Table of Contents for RTF targets 
//*****************************************************************************

boolean  TableOfContents() 
{  
   if   (target() == RTF)
   {
        PrintChapterTitle(toc_name, HEAD1NOIVZ, 1, "", True);
        print_raw(RTF_TOC);

        return True;  
   }

   // Note that for the HTML target, the QRL table of contents
   // functions are being used elsewhere.

   return False;
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Requirements Section
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
// Constants for Requirements
//----------------------------------------------------------------------------
const string REQ_H_ROWS = "5";    // Constants for table rows to be printed,
                                  // must be checked for new StP versions

//----------------------------------------------------------------------------
// Data Types for Requirements
//----------------------------------------------------------------------------
struct P_REQ                      // Struct of a requirement and its level.
{                                 // Needed for structured printing
       node req;
       int  level;
};


//*****************************************************************************
// RequirementChapter: generates the chapter with the requirements
//*****************************************************************************
int RequirementChapter()
{ 
   file     req_file;

   int      level,
            need_rows,
            print_from = 6,
            print_to = 6,
            ocnt, ox,
            rcnt, rx,
            req_number,
            status;

   node     parent,
            the_req;

   node_ref ord_req;
             
   set      req_files,
            req_ord,
            req_printed,
            req_levels;

   string   print_rows,
            req_app_type;

   P_REQ    pos_req;


   //====================================================
   // Print the chapter's headline and the introduction
   //====================================================
   PrintChapterTitle(req_heading, HEAD1, 1, req_intro, True);

   if   (e_IN_RQ_TABLE_FORM)
        print(req_intro1);
   else print(req_intro2);
 
   //=============================================
   // Find requirement table(s) and print them 
   //=============================================
   req_files = set_create("file");
   req_files = MatchingFilesOfTypeSet(m_REQ_FILE, " && node_refs", e_RQTBL_FILES);
   rcnt      = set_count(req_files);   

   if   (rcnt < 1)
   {
        PrintSectionTitle(no_req, HEAD4, BOLD12);
        req_number = 0; // nothing found
   }

   else    
   { 
        req_number = 1; // requirement tables found
		print_rows = "";

        for (rx = 0; rx < rcnt; rx = rx + 1)
        {
            req_file = set_get_element(req_files, rx); 

            PrintChapterTitle(reqtab_title + req_file.name, HEAD2, rx, "", False);
            PrintObjectDesc(req_file.id, BODY);

            if   (e_IN_RQ_TABLE_FORM)
            {
                 req_app_type = m_REQ_NODE + " && !in_links[" + 
				                m_REQ_PARENT_LINK + "]";
                 need_rows    = GetNumberOfRows(req_file.id, req_app_type);

                 if   (need_rows > 0)
                 {
                      if   (e_SUPERSEDED_REQ)
                           print_rows = print_from + "-*";
                      else
                      {
                           print_to = print_from + need_rows - 1;
                           print_rows = print_from + "-" + print_to;
                      }
                      PrintTable(req_file, e_PRINT_SETTING, 
					             STD_TAB_COLS, print_rows, REQ_H_ROWS, 
							     CELLHEAD, CELLBODY, CAPTION, 
							     diag_source, rep_table, True); 
                 }
                 else PrintSectionTitle(print_empty, HEAD4, BOLD10);
            }
            else 
            {
                 req_ord      = set_create("node_ref");
	  	         req_ord      = NodesRefsInFileSet(m_REQ_NODE, req_file.id,
  				                                   "ycoord, xcoord");
                 ocnt         = set_count(req_ord);   

                 if   (ocnt < 1)
                      PrintSectionTitle(print_empty, HEAD4, BOLD10);
                 else
                 {
                      req_printed = set_create("node");
                      req_levels  = set_create("P_REQ");

                      for (ox = 0; ox < ocnt; ox = ox + 1)
                      {
                          ord_req = set_get_element(req_ord, ox); 
                          the_req = NodeWithObjId(ord_req.node_id);

                          if (! set_is_member(req_printed, the_req))
                          {
                             level = ReqParents(the_req, req_printed,
                                                req_levels);
                             PrintRequirement(the_req, level);
                             set_add(req_printed, the_req);
                          }
                      }
                      message(rep_table + req_file.name);

                      set_clear(req_printed);
                      set_clear(req_levels);
                 }                 
                 set_clear(req_ord);
            }  

            //-------------------------------------          
            // Collect Glossary items if demanded
            //-------------------------------------
            if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
               GlossaryForFile(req_file.id);
			          
        }
   }
   set_clear(req_files);
   return req_number;
}


//*****************************************************************************
// ReqParents: returns the level of the parent requirement
//             - child      : current requirement
//             - req_levels : set of already printed requirements
//             - req_levels : set of parent's levels
//*****************************************************************************
int  ReqParents(node child, set req_printed, set req_levels)
{
   int      level,
            rcnt, rx;

   node     parent;

   string   req_app_type;

   P_REQ    pos_req;


   rcnt   = set_count(req_levels);   
   parent = NextReqInScope(m_REQ_NODE, child.id, "from");

   if   ( (parent != NULL) &&
          (set_is_member(req_printed, parent)) &&
          (rcnt > 0) )
   {
        for (rx = 0; rx < rcnt; rx = rx + 1)
        {
            pos_req = set_get_element(req_levels, rx);
            if (pos_req.req == parent)
            {
               level = pos_req.level;
               break;
            }
        }
   }
   else level = 0; // Errorproof !

   // maximal depth is 10
   if (level < 10)
      level = level + 1;

   pos_req.req   = child;
   pos_req.level = level;
   set_add(req_levels, pos_req);

   return level;
}


//*****************************************************************************
// NextReqInScope: returns the current requirements parent/child or NULL
//                 - the_node : application type of the node
//                 - the_id :  id of the node referenced in the node_ref
//                 - to_or_from: node is to_node or from_node
//*****************************************************************************
node  NextReqInScope(string the_node, int the_id, string to_or_from)
{
      return (find_by_query("node[" + the_node + " && in_links[ReqParent && " +
                            to_or_from + "_node_id = " + the_id + "]]"));
}


//*****************************************************************************
// PrintRequirement: prints the current requirement concerning its level 
//                   - the_req: requirement to be printed
//                   - level: level (needed for correct paragraph)
//*****************************************************************************
void  PrintRequirement(node the_req, int level)
{
   boolean is_child;

   int     status;

   item    alloc_item;

   note    req_desc;

   string  req_items,
           the_desc,
           the_format; 


   if   (NextReqInScope(m_REQ_NODE, the_req.id, "to") == NULL)
        is_child = True;
   else is_child = False;

   paragraph(EBENE + level);

   if   (is_child && e_SHOW_UNALLOCATED)
   {
        req_items  = "(" + m_REQ_ANALYSIS_ASS_ITEM + " || " +
                           m_REQ_DESIGN_ASS_ITEM   + " || " +
                           m_REQ_IMPL_ASS_ITEM     + " || " +
                           m_REQ_TEST_ASS_ITEM     + ")";

        alloc_item = NULL;
        alloc_item = ItemOfTypeWithValue(req_items, the_req.name);

        if   (alloc_item == NULL)
        {
             character_format_set(BOLD12);
             print("?\t");
        } 
        else print("\t");
   }
   else print("\t");

   if   (level == 1)
        the_format = BOLD12;
   else if   (level == 2)
             the_format = BOLD11;
        else the_format = BOLD10;

   character_format_set(the_format);
   print(the_req.name);

   alloc_item = NULL;
   alloc_item = ItemOfTypeWithObjId(m_REQ_INF_NAME_ITEM, the_req.id);

   if ((alloc_item != NULL) && 
       (string_length(the_desc = StripInfo(alloc_item.value)) > 0))
      print(" " + the_desc);


   if (is_child || e_SUPERSEDED_REQ)
   {
      req_desc = NULL;
      req_desc = NoteOfTypeWithObjId(m_REQ_DEFINITION_NOTE, the_req.id);

      if ((req_desc != NULL) && 
          (string_length(the_desc = StripInfo(req_desc.desc)) > 0))
      {
         print(":\n");
         if   (is_child)
              character_format_set(STANDARD);
         else character_format_set(ITALIC);
         print(the_desc);
      }

      req_desc = NULL;
      req_desc = NoteOfTypeWithObjId(m_REQ_SOURCE_NOTE, the_req.id);

      if ((req_desc != NULL) && 
          (string_length(the_desc = StripInfo(req_desc.desc)) > 0))
      {
         print("\n");
         if   (is_child)
              character_format_set(STANDARD);
         else character_format_set(ITALIC);
         print(the_desc);
      }
   }
   return;
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Glossary Section
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//*****************************************************************************
// GlossaryChapter: generates the appendix-chapter with the glossary
//*****************************************************************************
int GlossaryChapter()
{ 
   int     bcnt, bx,
           rcnt, rx,
           missing_item,
           RET = 0; 

   item    glos_item;

   note    glos_note;

   set     glossary_items;

   string  glos_desc,
           glos_src = "?\n";

   //====================================================
   // Print the chapter's headline and the introduction
   //====================================================
   PrintChapterTitle(gls_heading, HEAD1, 1, gls_intro, True);

   //==========================================
   // Search the objects and print the chapter 
   //==========================================
   glossary_items = set_create("item");
   glossary_items = ItemsOfTypeSet(m_GLOSSARY_ITEM);
   bcnt           = set_count(glossary_items);
   rcnt           = set_count(GLOSSARY_ITEMS);

   if   (bcnt < 1)
   {    // no glossary items defined

        PrintSectionTitle(no_glsd, HEAD4, BOLD12);
        missing_item = 1; // nothing found
   }

   else if   (e_REFD_GLOSSARY && (rcnt < 1))
   {         // empty reference list
             PrintSectionTitle(no_glsr, HEAD4, BOLD12);
             missing_item = 2; // nothing found
   }
   else    
   { 
        missing_item = 0; // glossary items found

        for (bx = 0; bx < bcnt; bx = bx + 1)
        {
            glos_item = set_get_element(glossary_items, bx);

            if (e_REFD_GLOSSARY && !set_is_member(GLOSSARY_ITEMS, glos_item))
               continue;

            glos_src  = GlossarySourceInfo(glos_item.obj_id);
            glos_note = NoteWithId(glos_item.note_id);

            if (glos_item.value != "")
            {
               paragraph(BULLET);
               character_format_set(BOLD12);
               print(glos_item.value);
               paragraph(CBULLET);
               character_format_set(ITALIC);
               print(glos_src);

               paragraph(CBULLET);
               glos_desc = StripInfo(glos_note.desc);
               RET       = string_length(glos_desc);

               if   (RET > 0)
                    print(glos_desc);
               else print(no_desc);
            }
        }
   }

   return missing_item;
}
 

//*****************************************************************************
// InitGlossary: Initialisation of globaly defined glossary set
//*****************************************************************************
void InitGlossary()
{
   GLOSSARY_ITEMS = set_create("item");

   return;
}
 

//*****************************************************************************
// FreeGlossary: Clear globaly defined glossary set
//*****************************************************************************
void FreeGlossary()
{
   set_clear(GLOSSARY_ITEMS);

   return;
}


//*****************************************************************************
// GlossaryForObject: Adds all Glossary-Items of the current object to 
//                    the global set of GLOSSARY_ITEMS
//                    - the_id: id of the annotated object.
//*****************************************************************************
void GlossaryForObject(int the_id)
{
   int  RET;
   set  obj_glossary = set_create("item");

   obj_glossary = ItemsOfTypeWithObjIdSet(m_GLOSSARY_ITEM, the_id);
   RET          = set_union(GLOSSARY_ITEMS, obj_glossary);

   set_clear(obj_glossary);
   return;
}


//*****************************************************************************
// GlossaryForFile: Adds all Glossary-Items of all objects referenced 
//                  in the current file to the global set of GLOSSARY_ITEMS 
//                  Calls glossary_for_object for the file-object itself.
//                  - the_id: id of the current file.
//*****************************************************************************
void GlossaryForFile(int the_id)
{
   int  RET;
   set  gl_items;

   gl_items = set_create("item");
   gl_items = ItemsOfTypeWithObjRefsInFileSet(m_GLOSSARY_ITEM, the_id, "node");
   RET      = set_union(GLOSSARY_ITEMS, gl_items);
   set_clear(gl_items);

   gl_items = set_create("item");
   gl_items = ItemsOfTypeWithObjRefsInFileSet(m_GLOSSARY_ITEM, the_id, "link");
   RET      = set_union(GLOSSARY_ITEMS, gl_items);
   set_clear(gl_items);

   gl_items = set_create("item");
   gl_items = ItemsOfTypeWithObjRefsInFileSet(m_GLOSSARY_ITEM, the_id, "cntx");
   RET      = set_union(GLOSSARY_ITEMS, gl_items);
   set_clear(gl_items);

   GlossaryForObject(the_id);

   return;
}


//*****************************************************************************
// GlossarySourceInfo: Returns the source-objects name or apptype
//                     - the_id: id of the source object.
//*****************************************************************************
string GlossarySourceInfo(int the_id)
{
   cntx def_obj_c = NULL;
   file def_obj_f = NULL;
   link def_obj_l = NULL;
   node def_obj_n = NULL;

   string source_type = "",
          source_name = "";

   def_obj_n = find_by_query("node[id = ${the_id}]");
   if   (def_obj_n != NULL)
   {
        source_type = app_type_print_string(def_obj_n);
        source_name = def_obj_n.name;
   }
   else 
   {
        def_obj_f = find_by_query("file[id = ${the_id}]");
        if   (def_obj_f != NULL)
        {
             source_type = app_type_print_string(def_obj_f);
             source_name = def_obj_f.name;
        }
        else 
        {
             def_obj_l = find_by_query("link[id = ${the_id}]");
             if   (def_obj_l != NULL)
             {
                  source_type = app_type_print_string(def_obj_l);
                  source_name = def_obj_l.name;
             }
             else 
             {
                  def_obj_c = find_by_query("cntx[id = ${the_id}]");
                  if   (def_obj_c != NULL)
                  {
                       source_type = app_type_print_string(def_obj_c);
                       source_name = def_obj_c.name;
                  }
             }
        }
   }

   if (source_name == "")   
      source_name = is_unnamed;

   return defined_in + source_type + " " + source_name;
}



//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// Global Utility Section
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

//*****************************************************************************
// ReduceToFiles: Reduce the Generation to the file-objects given in reduction.
//                (from one of the external reduction-variables)
//                - reduction : string with files to report about
//*****************************************************************************
string ReduceToFiles(string reduction)
{
   int    ocnt, ox;
   list	  red_to_objects;

   string prefix;
   string request  = "";
   string operator = "";
   string one_obj  = "";
   string op_link  = "";

   prefix    = StripInfo(e_PREFIX);
   reduction = StripInfo(reduction);

   if   ((prefix == "*") || (prefix == ""))
        prefix = "";

   if   ((reduction == "*") || (reduction == ""))
        reduction = prefix;
   else	reduction = reduction + " " + prefix;


   if   ((reduction != "*") && (reduction != ""))
   {
    	red_to_objects = list_create("string", 0);
        red_to_objects = string_to_list(reduction, " ");
        ocnt           = list_count(red_to_objects);

        for (ox = 0; ox < ocnt; ox = ox + 1) 
        {
            one_obj = list_get(red_to_objects, ox);

            if   (string_find(one_obj, 0, "*") < string_length(one_obj))
                 operator = " $ ";
            else operator = " = ";

            request = request + op_link + " (name" + operator 
                              +  "'" + one_obj + "')";
            op_link = " || "; 
        } 
        if (ocnt > 1)
           request = "(" + request + ")";
   }

   else request = ANY;

   return (request);
}


//*****************************************************************************
// ReduceToObjects: returns a string of object names (and child objects), 
//                  that have to be referenced in file objects or contain 
//                  modeling elements we are looking for
//                  - obj_type  : app_type of the objects we are looking for
//                  - rel_type  : app_type of the relation link 
//                  - rel_dir   : direction of the relation link (0 in, 1 out)
//                  - reduction : string with objects to report about
//*****************************************************************************
string ReduceToObjects(string obj_type, string rel_type, int rel_dir
                     , string reduction)
{
   int     ccnt, cx,
           ocnt, ox;

   list	   objects;

   node    p_object,
           c_object;

   set     contained;

   string  l_dir, 
           n_dir;

   string  request  = "";
   string  operator = " = ";
   string  op_link  = "";

   reduction = ReduceToFiles(reduction);

   // no reduction given
   if (reduction == ANY)
      return reduction;

   objects = list_create("node",0);
   objects = MatchingNodesOfTypeList(obj_type, reduction);
   ocnt    = list_count(objects);

   if (ocnt < 1)
   {
      list_clear(objects);
      return request;
   }

   if   (rel_dir == 1)
   {
        l_dir = "out";
        n_dir = "to";
   }
   else
   {
        l_dir = "in";
        n_dir = "from";
   }

   for (ox = 0; ox < list_count(objects); ox = ox + 1) 
   {
       p_object  = list_get(objects, ox);
       request   = request + op_link + " (name = '" + p_object.name + "')";
       op_link   = " || "; 

       contained = set_create("node");
       contained = NodesOfTypeWithLinkOfTypeToNodeIdSet(obj_type, l_dir, n_dir
                                                      , rel_type, p_object.id);
       ccnt      = set_count(contained);

       for (cx = 0; cx < ccnt; cx = cx + 1) 
       {
           c_object = set_get_element(contained, cx);
           // avoid inheritance cycles
           if (list_find(objects, 0, c_object) == list_count(objects))
              list_append(objects, c_object);
       }

       set_clear(contained);
   }

   list_clear(objects);

   return (request);
}


//*****************************************************************************
// BuildGraphHierarchy: Adds arcs for each link of the given type between
//                      two nodes of matching types already part of the graph
//                      - the_graph: already created graph filled with nodes
//                      - the_link : application type of the link
//                      - f_node   : application type of the from_node
//                      - t_node   : application type of the to_node
//*****************************************************************************
void BuildGraphHierarchy(graph the_graph, string the_link
                       , string f_node, string t_node)
{
   int     ccnt, cx;

   link    one_link;

   node    one_node;

   set     link_set;

   string  c_node,
           p_node;


   link_set = set_create("link");
   link_set = LinksOfTypeFromNodeToNode(the_link, f_node, t_node);
   ccnt     = set_count(link_set);

   for (cx = 0; cx < ccnt; cx = cx + 1)
   {
       one_link = set_get_element(link_set, cx);

       // From Node
       one_node = NodeWithObjId(one_link.from_node_id);
       if   (one_node != NULL) 
            p_node = one_node.name;
       else continue; // no from node ?!

       // To Node
       one_node = NodeWithObjId(one_link.to_node_id);
       if   (one_node != NULL) 
            c_node = one_node.name;
       else continue; // no from node ?!

       if   (  graph_is_node(the_graph, p_node)
            && graph_is_node(the_graph, c_node))
            graph_add_arc(the_graph, p_node, c_node);
   }
   set_clear(link_set);

   return;
}


//*****************************************************************************
// PrintSucNodes: Prints all successing nodes of the given node in the given  
//                graph with correct level
//                - hierarchy: the graph
//                - the_node : node with successors contained in the graph 
//                - level:     level of indentation
//*****************************************************************************
void  PrintSucNodes(graph hierarchy, string the_node, int level)
{
   int     pcnt, px,
           rank;

   list    successors;

   string  part_node;


   successors = list_create("string", 0);
   successors = graph_node_successors(hierarchy, the_node);
   pcnt       = list_count(successors);

   for (px = 0; px < pcnt; px = px + 1)
   {
        part_node = list_get(successors, px);
        paragraph(EBENE + level);
        print("\t- " + pac_name + part_node);

        rank     = graph_node_rank(hierarchy, part_node);
        if (rank > 0)
        {
           print(":");
           PrintSucNodes(hierarchy, part_node, level + 1);
        }
   }

   list_clear(successors);
   return;
}


//*****************************************************************************
// StripInfo: Strips the given string from leading and tailing blanks, 
//            newlines and tabs and looks for real characters. 
//            - the_string : string containing text, like note descriptions
//*****************************************************************************
string  StripInfo(string the_string)
{
   int    scnt, sx;
   string the_char;

   the_string = string_strip(the_string, "B", " ");
   the_string = string_strip(the_string, "B", "\n");
   the_string = string_strip(the_string, "B", "\t");
   the_string = string_strip(the_string, "B", " ");
   scnt       = string_length(the_string);

   for (sx = 0; sx < scnt; sx = sx + 1)
   {
       the_char = string_extract(the_string, sx, 1);
       if ((the_char != " ") && (the_char != "\n") && (the_char != "\t"))
          break;
   }

   if   (sx < scnt)
        the_char = the_string;
   else the_char = "";

   return the_char; 
}


//*****************************************************************************
// SetToString: converts the members of a set to a string separated by the 
//              given separator string.  
//              - the_set  : set to be converted to a string
//              - separator: separator string
//*****************************************************************************
string  SetToString(set the_set, string separator)
{
    int     scnt, sx;

    item    one_item;

    file    one_file;

    link    one_link;

    node    one_node;

    string  the_string,
            the_type;

    the_string = "";

    if ((the_set != NULL) && ((scnt = set_count(the_set)) > 0))
    {
       scnt       = set_count(the_set);
       the_type   = set_get_type(the_set, 0);

       if (the_type == "string")
           for (sx = 0; sx < scnt; sx = sx + 1)
           {
               the_string = the_string + set_get_element(the_set, sx);

               if (sx < scnt - 1)
                  the_string = the_string + separator;
           }

       if (the_type == "file")
           for (sx = 0; sx < scnt; sx = sx + 1)
           {
               one_file   = set_get_element(the_set, sx);
               the_string = the_string + one_file.type + ": " + one_file.name;

               if (sx < scnt - 1)
                  the_string = the_string + separator;
           }

       if (the_type == "node")
           for (sx = 0; sx < scnt; sx = sx + 1)
           {
               one_node   = set_get_element(the_set, sx);
               the_string = the_string + one_node.name;

               if (sx < scnt - 1)
                  the_string = the_string + separator;
           }

       if (the_type == "link")
           for (sx = 0; sx < scnt; sx = sx + 1)
           {
               one_link   = set_get_element(the_set, sx);
               the_string = the_string + one_link.name;

               if (sx < scnt - 1)
                  the_string = the_string + separator;
           }

       if (the_type == "item")
           for (sx = 0; sx < scnt; sx = sx + 1)
           {
               one_item   = set_get_element(the_set, sx);
               the_string = the_string + one_item.value;

               if (sx < scnt - 1)
                  the_string = the_string + separator;
           }

    }
    return the_string;
}


//*****************************************************************************
// StringToSet: converts the elements of a string separated by the 
//              given separator part-string to an already defined set (of
//              string), using the built-in string_to_list and the function
//              ListToSet. Returns the number of the set-elements.
//              - the_set   : resulting set
//              - the_string: string to be converted to a set
//              - separator : separator string
//*****************************************************************************
int  StringToSet(set the_set, string the_string, string separator)
{
    return ListToSet(string_to_list(the_string, separator), the_set);
}


//*****************************************************************************
// ListToSet: converts the given list to an already created but empty set
//            of the same type. Returns the length of the set.
//            - the_list: list to be converted to a set
//            - the_set:  set to be filled with elements of the list
//*****************************************************************************
int  ListToSet(list the_list, set the_set)
{
    int  scnt, sx;

    scnt = list_count(the_list);

    for (sx = 0; sx < scnt; sx = sx + 1)
        set_add(the_set, list_get(the_list, sx));
    
    return set_count(the_set);
}


//*****************************************************************************
// SetToList: converts the given set to an already created but empty list
//            of the same type. Returns the length of the list.
//            - the_set:  set to be converted to a list
//            - the_list: list to be filled with elements of the set
//*****************************************************************************
int  SetToList(set the_set, list the_list)
{
    int  scnt, sx;

    scnt = set_count(the_set);

    for (sx = 0; sx < scnt; sx = sx + 1)
        list_append(the_list, set_get_element(the_set, sx));
    
    return list_count(the_list);
}


//*****************************************************************************
// IsPrinted: Returns True and prints the cross reference text if single
//            description and the given object-id is already a member of the
//            given reference set.
//            Otherwise returns False and adds the object to the set.
//            - obj_id    : id of the given object
//            - ref_string: reference to object description
//            - ref_set   : reference set (chapter specific or global)
//*****************************************************************************
boolean  IsPrinted(int obj_id, string ref_string, set ref_set)
{
   boolean is_printed;
  
   int     pcnt, px;

   PRT_IN  new_object,
           pr_object;


   if (e_MULTIPLE_DESCRIPTION)
      return False;

   is_printed           = False;
   pcnt                 = set_count(ref_set);
   new_object.OBJ_ID    = obj_id;
   new_object.REFERENCE = ref_string;


   for (px = 0; px < pcnt; px = px + 1)
   {
		pr_object = set_get_element(ref_set, px);

		if  (obj_id == pr_object.OBJ_ID)
		{
             is_printed = True;
			 break;
		}
   }

   if  (is_printed)
        PrintSectionTitle(pr_object.REFERENCE, BODY_NORMAL, ITALIC);
   else set_add(ref_set, new_object);

   return is_printed;
}
