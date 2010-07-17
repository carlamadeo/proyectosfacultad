//*****************************************************************************
// DESCRIPTION: UML_Print.inc
//              ----------------------
//              (c) 1997 by AONIX 
//*****************************************************************************
//
// Include-File for UML-Reports with uml specific evaluation and printing
// functions.   
//*****************************************************************************
//
// functions: - void     PrintActivityDiagrams(set acty_set, string acty_ref, 
//                                             string acty_text, 
//                                             boolean detailed)
//            - void     PrintActorInfo(int file_id, set printed, 
//                                      string ref_text, boolean detailed)
//            - void     PrintAssocSummaryTable(set classes)
//            - void     PrintAttributes(set attributes, file class_tab)
//            - void     PrintCollSpecInfo(int file_id, string csp_type, 
//                                         string csp_name, set printed, 
//                                         string ref_text, boolean detailed,
//                                         string p_part)
//            - void     PrintExtInfo(int obj_id)
//            - void     PrintMessageInfo(int file_id, set printed, 
//                                        string ref_text, boolean detailed)
//            - void     PrintObjectInfo(int file_id, boolean is_col, set
//                                       printed, string ref_text, boolean
//                                       detailed, string p_cont,string p_part)
//            - void     PrintObjectsLinks(node the_object, string node_type, 
//                                         string to_from, string the_link, 
//                                         string in_out, string link_text)
//            - void     PrintOperations(set operations, file class_tab)
//            - void     PrintPackageDetail(node one_pac, int file_id,
//                                          string p_cont, string p_part)
//            - void     PrintPackageInfo(int file_id, set printed, 
//                                        string ref_text, boolean detailed, 
//                                        string p_cont, string p_part)
//            - void     PrintScenarioDiagrams(set sce_set, string sce_ref, 
//                                             boolean detailed)
//            - void     PrintStateDiagrams(set state_set, string state_ref)
//            - void     PrintSystemInfo(int file_id, set printed, 
//                                       string ref_text, boolean detailed, 
//                                       string p_cont)
//*****************************************************************************


//*****************************************************************************
// PrintActivityDiagrams: prints and describes all Activity Diagrams in 
//                        acty_set
//                        - acty_set:  set with activity files 
//                        - acty_ref:  reference text if already printed 
//                        - detailed:  print detailed desc, True or False 
//*****************************************************************************
void  PrintActivityDiagrams(set acty_set, string acty_ref, boolean detailed)
{
   boolean  print_order;

   int      acnt, ax,
            ccnt, cx,
            RET;

   file     acty_file;

   link     acty_link;

   node     acty_node;

   set      acty_symbols;


   print_order         = e_PRINT_ALPHA_ORDER;
   e_PRINT_ALPHA_ORDER = True;
   acnt                = set_count(acty_set);

   if (acnt < 1)
      return;

   for (ax = 0; ax < acnt; ax = ax + 1)
   {
       acty_file = set_get_element(acty_set, ax);

       PrintSectionTitle(act_diag + acty_file.name, HEAD4, BOLD12);

       //--------------------------------------------
       // Already described in another chapter ?
       //--------------------------------------------
       if (IsPrinted(acty_file.id, acty_ref + ">", PRINTED_FILES))
           continue;

       PrintObjectDesc(acty_file.id, BODY_NORMAL);

       PrintDiagram(acty_file, e_OPTIMIZED_PRINTING, e_PRINT_SETTING 
                  , e_NO_DISPLAYMARKS, BODY_NORMAL, CAPTION 
                  , diag_source, rep_diag); 

       //-----------------------------------------------------
       // Describe the States declared in each diagram
       // in alphabetical order (save e_PRINT_ALPHA_ORDER)
       //-----------------------------------------------------
       PrintStateInfo(acty_file.id, PRINTED_FILES, acty_ref + ", " 
                    + act_diag + acty_file.name + ">",  detailed);

       //-------------------------------------          
       // Collect Glossary items if demanded
       //-------------------------------------
       if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
          GlossaryForFile(acty_file.id);
   }
   e_PRINT_ALPHA_ORDER = print_order;

   return;
}


//*****************************************************************************
// PrintActorInfo: Prints all kind of information about actors 
//                 - file_id : id of the file with the actor reference
//                 - printed : set of already printed elements
//                 - ref_text: cross reference text (if printed)
//                 - detailed: print detailed information, True/False
//*****************************************************************************
void PrintActorInfo(int file_id, set printed, string ref_text
                  , boolean detailed)
{
   int   acnt, ax;

   node  one_act;

   set   actors;


   actors = set_create("node");
   actors = NodesOfTypeWithRefsWithFileIdSet(m_ACTOR_NODE , file_id, OX);
   acnt   = set_count(actors);

   for (ax = 0; ax < acnt; ax = ax + 1)
   {
       one_act = set_get_element(actors, ax);
       PrintSectionTitle(act_name + one_act.name, HEAD4, BOLD11);

       //--------------------------------------------
       // Already described for another diagram ?
       //--------------------------------------------
       if (IsPrinted(one_act.id, ref_text, printed))
          continue;

       PrintObjectDesc(one_act.id, BODY_NORMAL);

       if (!detailed)
          continue;

       //--------------------------------------------------
       // Actor extensibility, generalisation, inheritance, 
       // performs use cases, references
       //--------------------------------------------------
       PrintExtInfo(one_act.id);

       PrintObjectsLinks(one_act, m_ACTOR_NODE, "to" 
                       , m_GENERALIZATION_LINK, "out", superobject);

       PrintObjectsLinks(one_act, m_ACTOR_NODE, "from" 
                       , m_GENERALIZATION_LINK, "in", subobject);

       PrintObjectsLinks(one_act, m_USECASE_NODE, "from" 
                       , m_UC_INTERACTION_LINK, "in", performs);

       PrintRefInfo(one_act.id, one_act.name, "node", file_id);
   }
   set_clear(actors);

   return;
}


//*****************************************************************************
// PrintAssocSummaryTable: prints the association summary table for the given
//                         set of classes.
//                         - classes: set of the classes (scope = class report
//*****************************************************************************
void  PrintAssocSummaryTable(set classes)
{
   boolean  head_printed = False;

   int      acnt, ax,
            ccnt, cx, 
            assoc_number = 0,
            colon_pos = 0;

   node     classA,
            classB,
            one_assoc;

   item     multA, 
            multB,
            roleA_navig, // ECR 5594 introduced for avoiding duplicate entries.
            roleB_navig; // ECR 5594 introduced for avoiding duplicate entries.


   link     roleA,                              
            roleB; // ECR 5594 introduced for avoiding duplicate entries.
   
   string   assoc_entry; // ECR 5594 introduced for avoiding duplicate entries.
   list     dupl_assocs; // ECR 5594 introduced for avoiding duplicate entries.

   note     ass_desc;

   set      assoc_set;

   string   p_assoc, 
            p_classA, 
            p_classB,
            p_desc,
            p_multA, 
            p_multB, 
            p_role;

   const int width_1 = 140;
   const int width_2 = 100;
   const int width_3 = 65;
   const int width_4 = 140;
   const int width_5 = 65;
   const int width_6 = 140;
   const int width_7 = 240;

         
   //======================================================
   // Print the subchapter's headline and the introduction
   //======================================================
   PrintChapterTitle(assoc_heading, HEAD2, 1, assoc_intro, True);
   message(ass_start);

   ccnt = set_count(classes); // is always > 0
   acnt = 0;
   acnt = selection_count("node[UmlAssociation && in_links[UmlRole && "
                        + "from_node[UmlClass]]]");

   if (acnt < 1)
   {
      message(m_no_ass);
      PrintSectionTitle(no_assoc, HEAD4, BOLD12);
      return;
   }

   table_show_caption_set(False);     
   table_width_set(TAB_W_STD);
   table_cell_bold_paragraph_format_set(CELLHEAD);
   table_cell_paragraph_format_set(CELLBODY);

   Begin_Table(7);

     Begin_Row () ;
       Make_Cell(width_1, NULL, "Shade1", NULL, NULL, NULL, tab_classA);
       Make_Cell(width_2, NULL, "Shade1", NULL, NULL, NULL, tab_role);
       Make_Cell(width_3, NULL, "Shade1", NULL, NULL, NULL, tab_multA);
       Make_Cell(width_4, NULL, "Shade1", NULL, NULL, NULL, tab_classB);
       Make_Cell(width_5, NULL, "Shade1", NULL, NULL, NULL, tab_multB);
       Make_Cell(width_6, NULL, "Shade1", NULL, NULL, NULL, tab_assoc);
       Make_Cell(width_7, NULL, "Shade1", NULL, NULL, NULL, tab_desc);
     End_Row();

   //------------------------------------------------
   // Print the associations of each class
   //------------------------------------------------
   
   // ECR 5594 initialize the duplicates list
   dupl_assocs = list_create("string", 0);
       
   for (cx = 0; cx < ccnt; cx = cx + 1)
   {
       classA    = set_get_element(classes, cx);        
       assoc_set = set_create("node");
       assoc_set = NodesOfTypeWithLinkOfTypeToNodeIdSet("UmlAssociation", "in"
                                                      , "from", "UmlRole"
                                                      , classA.id);
       acnt         = set_count(assoc_set);
       assoc_number = assoc_number + acnt;

       for (ax = 0; ax < acnt; ax = ax + 1)
       {
           // ECR 5594 initialize the navigability items
           roleA_navig = NULL;
           roleB_navig = NULL;
                     
           p_assoc  = not_named;
           p_classA = classA.name;
           p_classB = "";
           p_desc   = not_def;
           p_multA  = "?";
           p_multB  = "?";
           p_role   = not_def;

           one_assoc = set_get_element(assoc_set, ax);
           assoc_entry = ""; // ECR 5594

           colon_pos = string_find(one_assoc.name, 0, ":");
           if (colon_pos != string_length(one_assoc.name))
              p_assoc = string_extract(one_assoc.name, 0, colon_pos);

           roleA = find_by_query("link[UmlRole && to_node_id = " + one_assoc.id
                               + " && from_node_id = " + classA.id + "]");
           if (  (roleA != NULL) && (roleA.name != ""))
              if   (p_assoc != string_search_and_replace(roleA.name, " ", ""))
                   p_role = roleA.name;
              else p_assoc = roleA.name;

           classB = find_by_query("node[UmlClass && id != " + classA.id
                                + " && out_links[UmlRole && to_node_id = " 
                                + one_assoc.id + "] && node_refs]");
           // Begin Change ECR 5594
           if   (classB != NULL) {
                p_classB = classB.name;
                assoc_entry = one_assoc.name + "_" + one_assoc.sig;
                roleB = find_by_query("link[UmlRole && to_node_id = " + one_assoc.id
                               + " && from_node_id = " + classB.id + "]");  
                roleA_navig = find_by_query("item[UmlRoleNavigability & value = 'True' " 
                               + " && obj_id = " + roleA.id + "]");
                roleB_navig = find_by_query("item[UmlRoleNavigability & value = 'True' " 
                               + " && obj_id = " + roleB.id + "]");                                                 
                if (((roleA_navig != NULL && roleB_navig == NULL) || (roleA_navig == NULL && roleB_navig != NULL)) && (list_find(dupl_assocs, 0, assoc_entry) < list_count(dupl_assocs))) {
                    list_delete(dupl_assocs, (list_count(dupl_assocs) - 1));
                    continue;
                }
                list_append(dupl_assocs, assoc_entry);
           }
           else { 
                p_classB = classA.name;  // reflexive ?
                assoc_entry = one_assoc.name + "_" + one_assoc.sig;
                list_append(dupl_assocs, assoc_entry);
           }
           // End Change ECR 5594
           multA = find_by_query("item[UmlMultiplicity && link[UmlRole "
                               + "&& to_node_id = " + one_assoc.id + " "
                               + "&& from_node_id = " + classA.id + "]]");
           if (multA != NULL) 
              p_multA = multA.value;

           multB = find_by_query("item[UmlMultiplicity && link[UmlRole "
                               + "&& to_node_id = " + one_assoc.id + " "
                               + "&& from_node[UmlClass && name = '"
                               + p_classB + "']]]");
           if (multB != NULL) 
              p_multB = multB.value;


           ass_desc = NoteOfTypeWithObjId("GenericObject", one_assoc.id);

           if ((ass_desc != NULL) && (ass_desc.desc != "")) 
              p_desc = StripInfo(ass_desc.desc);

           p_desc = string_escape(p_desc,"\""); // ECR 7243
   
           Begin_Row();

             Make_Cell(width_1, NULL, NULL, NULL, NULL, NULL, p_classA);
             Make_Cell(width_2, NULL, NULL, NULL, NULL, NULL, p_role);
             Make_Cell(width_3, NULL, NULL, NULL, NULL, NULL, p_multA);
             Make_Cell(width_4, NULL, NULL, NULL, NULL, NULL, p_classB);
             Make_Cell(width_5, NULL, NULL, NULL, NULL, NULL, p_multB);
             Make_Cell(width_6, NULL, NULL, NULL, NULL, NULL, p_assoc);
             Make_Cell(width_7, NULL, NULL, NULL, NULL, NULL, p_desc);

           End_Row(); 
       }
   }

   End_Table();

   if   (assoc_number < 1)
   {
        message(m_no_ass);
        PrintSectionTitle(no_assoc, HEAD4, BOLD12);
   }
   else 
   {
        table_column_range_set("");
        table_row_range_set("");
        table_header_row_range_set("1");
        print_table("AssocSummaryTable", "");
        paragraph(CAPTION);
        print(current_system() + ": " + assoc_heading);
   }

   return;
}


//*****************************************************************************
// PrintAttributes: prints the attribute information for the given list of
//                  attributes of the current class.
//                  - attributes: set of the classes attributes
//                  - class_tab:  the classes class table file (or NULL)
//*****************************************************************************
void  PrintAttributes(set attributes, file class_tab)
{
   T_ROWS  a_rows;

   boolean comma;

   int     acnt, ax;

   item    att_info;

   node    one_att;

   string  ada_items,
           att_items,
           cxx_items,
           idl_items,
           jav_items;

   acnt = set_count(attributes);

   if   (e_IN_CL_TABLE_FORM)
   {
        //===========================================
        // Print the Attributes from the Class table
        //===========================================
        if (class_tab == NULL)
        {
           PrintSectionTitle(no_ctab, HEAD4, BOLD10);
           return;
        }

        //---------------------------------------------------------
        // print the attribute section, if it contains attributes
        //---------------------------------------------------------
        a_rows = GetFirstLastRows(class_tab.id, m_ATTRIBUTE_NODE);

        if   (a_rows.first > 0)
        {
             PrintTable(class_tab, e_PRINT_SETTING, MEMBER_COL
                      , a_rows.first + "-" + a_rows.last, ATT_H_ROWS, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (e_WITH_ANALYSIS_INFO)
                PrintTable(class_tab, e_PRINT_SETTING, ANALYSIS_COL_A
                      , a_rows.first + "-" + a_rows.last, ATT_H_ROWS, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (e_WITH_CXX_INFO)
                PrintTable(class_tab, e_PRINT_SETTING, PROG_CXX_COL_A
                      , a_rows.first + "-" + a_rows.last, ATT_H_ROWS, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (e_WITH_JAVA_INFO)
                PrintTable(class_tab, e_PRINT_SETTING, PROG_JAVA_COL
                      , a_rows.first + "-" + a_rows.last, ATT_H_ROWS, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (e_WITH_IDL_INFO)
                PrintTable(class_tab, e_PRINT_SETTING, PROG_IDL_COL_A
                      , a_rows.first + "-" + a_rows.last, ATT_H_ROWS, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (e_WITH_ADA_INFO)
                PrintTable(class_tab, e_PRINT_SETTING, PROG_ADA_COL_A
                      , a_rows.first + "-" + a_rows.last, ATT_H_ROWS, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (acnt > (a_rows.last - a_rows.first + 1))
                PrintSectionTitle(missing_att, HEAD4, BOLD10);                
        }
        else PrintSectionTitle(print_empty, HEAD4, BOLD10);
   }
   else
   {
        att_items = m_MB_VISIBILITY_ITEM   + " || " 
                  + m_MB_IS_CLASS_ITEM     + " || " 
                  + m_AT_IS_DERIVED_ITEM;
        cxx_items = m_MB_CXX_CONST_ITEM    + " || " 
                  + m_MB_CXX_VISIB_ITEM    + " || " 
                  + m_AT_CXX_VOLATILE_ITEM;
        jav_items = m_MB_JAV_VISIB_ITEM    + " || " 
                  + m_MB_JAV_FINAL_ITEM    + " || " 
                  + m_AT_JAV_TRANS_ITEM    + " || " 
                  + m_AT_JAV_VOLATILE_ITEM;
        idl_items = m_AT_IDL_TYPE_ITEM     + " || " 
                  + m_AT_IDL_IS_RO_ITEM;
        ada_items = m_AT_ADA_ALIAS_ITEM    + " || " 
                  + m_MB_ADA_VISIB_ITEM;

        //===========================================
        // Print the Attributes from the Repository
        //===========================================  
        for (ax = 0; ax < acnt; ax = ax + 1)
        {
            one_att = set_get_element(attributes, ax);
            paragraph(BULLET); 
 
            character_format_set(BOLD11);
            print (one_att.name + " ");
            character_format_set(ITALIC);
            att_info = ItemOfTypeWithObjId(m_AT_TYPE_ITEM, one_att.id);
            if (att_info != NULL) 
               print(att_info.value);

            PrintTheItem(one_att.id, m_AT_DEFAULT_VALUE_ITEM, 
                         is_of_default, CBULLET, STANDARD, ITALIC);

            if (e_WITH_ANALYSIS_INFO)
            {
               comma     = False;

               if (GetNumberOfItems(one_att.id, att_items) > 0)
               {
                  PrintSectionTitle(is_status + ":", CBULLET, BOLD10);
                  paragraph(CBULLET);  
                  comma = PrintValueItem(one_att.id, m_MB_VISIBILITY_ITEM
                        , is_visible, comma);
                  comma = (PrintBooleanItem(one_att.id, m_MB_IS_CLASS_ITEM
                        , is_meta_att, comma) || comma);
                  comma = (PrintBooleanItem(one_att.id, m_AT_IS_DERIVED_ITEM
                        , is_derived, comma) || comma);
               }
            }


            if (e_WITH_CXX_INFO)
            {
               comma     = False;

               if (GetNumberOfItems(one_att.id, cxx_items) > 0)
               {
                  PrintSectionTitle("C++:", CBULLET, BOLD10);
                  paragraph(CBULLET);  

                  comma = PrintBooleanItem(one_att.id, m_MB_CXX_CONST_ITEM
                        , "Constant: ", comma);
                  comma = (PrintValueItem(one_att.id, m_MB_CXX_VISIB_ITEM
                        , is_visible, comma) || comma);
                  comma = (PrintBooleanItem(one_att.id, m_AT_CXX_VOLATILE_ITEM
                        , "Volatile: ", comma) || comma);
               }
            }

            if (e_WITH_JAVA_INFO)
            {
               comma     = False;

               if (GetNumberOfItems(one_att.id, jav_items) > 0)
               {
                  PrintSectionTitle("Java:", CBULLET, BOLD10);
                  paragraph(CBULLET);  

                  comma = PrintValueItem(one_att.id, m_MB_JAV_VISIB_ITEM
                        , is_visible, comma);
                  comma = (PrintBooleanItem(one_att.id, m_MB_JAV_FINAL_ITEM
                        , "Final: ", comma) || comma);
                  comma = (PrintBooleanItem(one_att.id, m_AT_JAV_TRANS_ITEM
                        , "Transient: ", comma) || comma);
                  comma = (PrintBooleanItem(one_att.id, m_AT_JAV_VOLATILE_ITEM
                        , "Volatile: ", comma) || comma);
               }
            }

            if (e_WITH_IDL_INFO)
            {
               comma     = False;

               if (GetNumberOfItems(one_att.id, idl_items) > 0)
               {
                  PrintSectionTitle("IDL:", CBULLET, BOLD10);
                  paragraph(CBULLET);  

                  comma = PrintValueItem(one_att.id, m_AT_IDL_TYPE_ITEM
                        , is_of_type, comma);
                  comma = (PrintBooleanItem(one_att.id, m_AT_IDL_IS_RO_ITEM
                        , "ReadOnly: ", comma) || comma);
               }
            }

            if (e_WITH_ADA_INFO)
            {
               comma     = False;

               if (GetNumberOfItems(one_att.id, ada_items) > 0)
               {
                  PrintSectionTitle("Ada95:", CBULLET, BOLD10);
                  paragraph(CBULLET);  

                  comma = PrintBooleanItem(one_att.id, m_AT_ADA_ALIAS_ITEM
                        , "Aliased: ", comma);
                  comma = (PrintValueItem(one_att.id, m_MB_ADA_VISIB_ITEM
                        , "CA Privacy: ", comma) || comma);
               }
            }

            PrintAnyDesc(one_att.id, m_AT_DEF_NOTE, CBULLET, 0);
        }
   }
   return;
}


//*****************************************************************************
// PrintCollSpecInfo: Prints all kind of information about symbols in
//                    collaboration diagrams (e.g. collaboration, type)   
//                    - file_id : id of the file with the package reference
//                    - csp_type: symbol type
//                    - csp_name: symbol name in section title
//                    - printed : set of already printed elements
//                    - ref_text: cross reference text (if printed)
//                    - detailed: print detailed information, True/False
//                    - p_part  : nodes that collaborations are part of
//*****************************************************************************
void PrintCollSpecInfo(int file_id, string csp_type, string csp_name
                     , set printed, string ref_text, boolean detailed
                     , string p_part)
{
   int   ccnt, cx;

   node  one_col;

   set   col_set;


   col_set = set_create("node");
   col_set = NodesOfTypeWithRefsWithFileIdSet(csp_type, file_id, OX);
   ccnt    = set_count(col_set);

   for (cx = 0; cx < ccnt; cx = cx + 1)
   {
       one_col = set_get_element(col_set, cx);
       PrintSectionTitle(col_name + one_col.name, HEAD4, BOLD11);

       //--------------------------------------------
       // Already described for another diagram ?
       //--------------------------------------------
       if (IsPrinted(one_col.id, ref_text, printed))
          continue;

       PrintObjectDesc(one_col.id, BODY_NORMAL);

       if (!detailed)
          continue;

       //--------------------------------------------------
       // Collaboration extensibility, part of, references
       //--------------------------------------------------
       PrintExtInfo(one_col.id);

       PrintObjectsLinks(one_col, p_part, "to", m_CONTAINS_LINK 
                       , "out", is_part_of);

       PrintRefInfo(one_col.id, one_col.name, "node", file_id);

   }
   set_clear(col_set);

   return;
}


//*****************************************************************************
// PrintExtInfo: Prints the items of the extensibility definition of the  
//               given object (if existing).
//               - obj_id:  id of the current object
//*****************************************************************************
void  PrintExtInfo(int obj_id)
{
   int   ccnt, cx;

   item  ext_item;

   set   ext_items;

   //-----------------------------------
   // Stereotype
   //-----------------------------------
   PrintTheItem(obj_id, m_STEREOTYPE_ITEM, stereo_name, HEAD4, BOLD10, STANDARD);

   //-----------------------------------
   // Constraints
   //-----------------------------------
   PrintAllItems(obj_id, m_CONSTRAINT_ITEM, constraints, HEAD4, BOLD10);

   //-----------------------------------
   // Tagged Values
   //-----------------------------------
   PrintAllItems(obj_id, m_TAGGED_VALUE_ITEM, tval_name, HEAD4, BOLD10);

   return;
}


//*****************************************************************************
// PrintImplInfo: Prints all kind of information about nodes in component- and  
//                deployment diagrams.
//                - file_id : id of the implementation diagram
//                - printed : set of already printed elements
//                - ref_text: cross reference text (if printed)
//                - detailed: print detailed information, True/False
//*****************************************************************************
void PrintImplInfo(int file_id, set printed, string ref_text, boolean detailed)
{
   int    icnt, ix;

   node   one_impl;

   set    impl_nodes;

   string impl_names,
          the_name;


   impl_names = m_DEPLOYMENT_NODE     + " || " + m_COMPONENT_IF_NODE  + " || "
              + m_COMPONENT_BIN_NODE  + " || " + m_COMPONENT_EX_NODE + " || "
              + m_COMPONENT_SRC_NODE  + " || " + m_COMPONENT_OBJECT_NODE;

   impl_nodes = set_create("node");
   impl_nodes = NodesOfTypeWithRefsWithFileIdSet(impl_names, file_id, OX);
   icnt       = set_count(impl_nodes);

   for (ix = 0; ix < icnt; ix = ix + 1)
   {
       one_impl = set_get_element(impl_nodes, ix);

       if      (one_impl.type == m_DEPLOYMENT_NODE)
               the_name = unit_name;
       else if (one_impl.type == m_COMPONENT_OBJECT_NODE)
               the_name = obj_name;
       else if (one_impl.type == m_COMPONENT_IF_NODE)
               the_name = module_if;
       else if (one_impl.type == m_COMPONENT_BIN_NODE)
               the_name = module_bin;
       else if (one_impl.type == m_COMPONENT_EX_NODE)
               the_name = module_ex;
       else if (one_impl.type == m_COMPONENT_SRC_NODE)
               the_name = module_src;


       PrintSectionTitle(the_name + one_impl.name, HEAD4, BOLD11);

       //--------------------------------------------
       // Already described for another diagram ?
       //--------------------------------------------
       if (IsPrinted(one_impl.id, ref_text, printed))
          continue;

       PrintObjectDesc(one_impl.id, BODY_NORMAL);

       if (!detailed)
          continue;

       //---------------------------------------------------------
       // State information: extensibility, contains, references
       //---------------------------------------------------------
       PrintExtInfo(one_impl.id);                                     

       PrintRefInfo(one_impl.id, one_impl.name, "node", file_id);
   }
   set_clear(impl_nodes);

   return;
}


//*****************************************************************************
// PrintMessageInfo: Prints all kind of information about events and messages   
//                   - file_id : id of the file with the system reference
//                   - printed : set of already printed elements
//                   - ref_text: cross reference text (if printed)
//                   - detailed: print detailed information, True/False
//*****************************************************************************
void PrintMessageInfo(int file_id, set printed, string ref_text
                    , boolean detailed)
{
   int     mcnt, mx;

   link    one_msg;

   set     messages;

   string  msg_links,
           the_name;


   msg_links  = m_SIMPLE_MESSAGE_LINK  + " || " + m_SYNCHR_MESSAGE_LINK  + " || "
              + m_ASYNCHR_MESSAGE_LINK + " || " + m_BALKING_MESSAGE_LINK + " || "
              + m_TIMEOUT_MESSAGE_LINK + " || " + m_RETURN_MESSAGE_LINK;

   messages = set_create("node");
   messages = LinksOfTypeWithRefsWithFileIdSet(msg_links, file_id, OX);
   mcnt     = set_count(messages);

   for (mx = 0; mx < mcnt; mx = mx + 1)
   {
       one_msg = set_get_element(messages, mx);
       the_name = "";

       if ((one_msg.name == NULL) || (one_msg.name == ""))
          continue;

       if   (string_find(one_msg.name, 0, "(") < string_length(one_msg.name))
            the_name = msg_name;
       else the_name = evt_name;

       if      (one_msg.type == m_SYNCHR_MESSAGE_LINK)
               the_name = sync_msg + the_name;
       else if (one_msg.type == m_ASYNCHR_MESSAGE_LINK)
               the_name = async_msg + the_name;
       else if (one_msg.type == m_BALKING_MESSAGE_LINK)
               the_name = balk_msg + the_name;
       else if (one_msg.type == m_TIMEOUT_MESSAGE_LINK)
               the_name = tout_msg + the_name;
       else if (one_msg.type == m_RETURN_MESSAGE_LINK)
               the_name = ret_msg + the_name;

       PrintSectionTitle(the_name + one_msg.name, HEAD4, BOLD11);

       //--------------------------------------------
       // Already described for another diagram ?
       //--------------------------------------------
       if (IsPrinted(one_msg.id, ref_text, printed))
          continue;

       PrintObjectDesc(one_msg.id, BODY_NORMAL);

       if (!detailed)
          continue;

       //---------------------------------------------------------
       // Message information: extensibility, contains, references
       //---------------------------------------------------------
       PrintExtInfo(one_msg.id);                                     

       PrintRefInfo(one_msg.id, one_msg.name, "link", file_id);
    }
   set_clear(messages);

   return;
}


//*****************************************************************************
// PrintObjectInfo: Prints all kind of information about objects in   
//                  interaction diagrams.
//                  - file_id : id of the file with the object reference
//                  - is_col  : is it a collaboration diagram ?
//                  - printed : set of already printed elements
//                  - ref_text: cross reference text (if printed)
//                  - detailed: print detailed information, True/False
//                  - p_cont  : nodes that objects contain
//                  - p_part  : nodes that objects are part of
//*****************************************************************************
void PrintObjectInfo(int file_id, boolean is_col, set printed, string ref_text
                   , boolean detailed, string p_cont, string p_part)
{
   int     ocnt, ox,
           RET;

   node    act_inst,
           obj_inst,
           one_obj;
            
   set     objects;

   string  the_name;


   objects = set_create("node");
   objects = NodesOfTypeWithRefsWithFileIdSet(m_OBJECT_CLASS_NODE, file_id,OX);
   ocnt    = set_count(objects);

   for (ox = 0; ox < ocnt; ox = ox + 1)
   {
       one_obj  = set_get_element(objects, ox);
       the_name = "";
       RET      = 0;

       if   (  (one_obj.name != NULL) && (string_length(one_obj.name) > 0) )
            the_name = ": " + one_obj.name;
       else break; // anonymous class ?!

       obj_inst = NULL;
       obj_inst = ScopedNodeOfTypeInSameFile(one_obj.id,m_OBJECT_NODE,file_id);

       if (  (obj_inst != NULL) && (obj_inst.name != NULL)
          && (string_length(obj_inst.name) > 0) )
       {
          if   (  (string_length(obj_inst.name) > 8)
               && (string_extract(obj_inst.name, 0, 8) == "%%object"))
               the_name = ano_obj + obj_name + the_name;
          else the_name = obj_name + obj_inst.name + " " + the_name;
       }
       else continue; // no label !?

       if (is_col)
       {
          act_inst = NULL;
          act_inst = ScopedNodeOfTypeInSameFile(one_obj.id
                                              , m_ACTIVE_OBJECT_NODE, file_id);
          if (act_inst != NULL)
             the_name = act_obj + the_name;
       }
           
           if (the_name == "")
              continue;  // no label !?

       PrintSectionTitle(the_name, HEAD4, BOLD11);


       //--------------------------------------------
       // Already described for another diagram ?
       //--------------------------------------------
       if (IsPrinted(one_obj.id, ref_text, printed))
          continue;

       RET = RET + PrintNoteDesc(one_obj.id, lang_obj, BODY_NORMAL);

       if (obj_inst != NULL)
          RET = RET + PrintNoteDesc(obj_inst.id, lang_obj, BODY_NORMAL);

       if ((is_col) && (act_inst != NULL))
          RET = RET + PrintNoteDesc(act_inst.id, lang_obj, BODY_NORMAL);

       if ((RET == 0) && e_MISSING_DESCRIPTION)
          PrettyPrint(no_desc, BODY_NORMAL);    

       if (!detailed)
          continue;

       //--------------------------------------------------------
       // Object extensibility, contains, is part of, references
       //--------------------------------------------------------
       PrintExtInfo(one_obj.id);

       if (is_col)
          PrintObjectsLinks(one_obj, p_cont, "from", m_CONTAINS_LINK 
                          , "in", consists_of);

       PrintObjectsLinks(one_obj, p_part, "to", m_CONTAINS_LINK
                       , "out", is_part_of);

       PrintRefInfo(one_obj.id, the_name, "node", file_id);
   }
   set_clear(objects);

   return;
}


//*****************************************************************************
// PrintObjectsLinks: Prints all from or to nodes that are connected to the  
//                    given object by the given in or out link.
//                    - the_object: the given object
//                    - node_type : node_types, we're interested in
//                    - to_from   : to_nodes or from_nodes ?
//                    - the_link  : node_types, we're interested in
//                    - in_out    : in_links or out_links ?
//                    - link_text : text, describing the link
//*****************************************************************************
void  PrintObjectsLinks(node the_object, string node_type, string to_from, 
                        string the_link, string in_out, string link_text)
{
   int   ccnt, cx;

   node  one_node;

   set   node_set;

   node_set = set_create("node");
   node_set = NodesOfTypeWithLinkOfTypeToNodeIdSet(node_type, in_out, to_from,
                                                   the_link, the_object.id);
   ccnt     = set_count(node_set);

   if (ccnt > 0)
      if   (the_object.type == m_OBJECT_CLASS_NODE)
           PrintSectionTitle(print_the + obj_name + link_text, HEAD4, BOLD10);
      else PrintSectionTitle(the_object.name + link_text, HEAD4, BOLD10);

   for (cx = 0; cx < ccnt; cx = cx + 1) 
   {       
       paragraph(BULLET);
       one_node = set_get_element(node_set, cx);

       if      (one_node.type == m_PACKAGE_NODE)
               print(pac_name);
       else if (one_node.type == m_SYSTEM_NODE)
               print(sys_name);
       else if (one_node.type == m_ACTOR_NODE)
               print(act_name);
       else if (one_node.type == m_USECASE_NODE)
               print(uca_name);
       else if (one_node.type == m_CLASS_NODE)
               print(class_name);
       else if (one_node.type == m_COLABORATION_NODE)
               print(col_name);
       else if (one_node.type == m_TYPE_NODE)
               print(type_name);
       else if (one_node.type == m_STEREOTYPE_NODE)
               print(stereo_name);
       else if (  (one_node.type == m_OBJECT_NODE) 
               || (one_node.type == m_OBJECT_CLASS_NODE))
               print(obj_name);
       else if (one_node.type == m_ACTIVE_OBJECT_NODE)
               print(actobj_name);
       else    print(one_node.type + ": ");

       print(one_node.name);
   }

   set_clear(node_set);

   return;
}


//*****************************************************************************
// PrintOperations: prints the operation information for the given set of
//                  operation of the current class.
//                  - operations: set of the classes operations
//                  - class_tab:  the classes class table file (or NULL)
//*****************************************************************************
void PrintOperations(set operations, file class_tab)
{
   T_ROWS  o_rows;

   boolean comma;

   int     ocnt, ox;

   item    op_info;

   node    one_op;

   string  op_items,
           o_h_rows;

   ocnt = set_count(operations);

 
   if   (e_IN_CL_TABLE_FORM)
   {
        //===========================================
        // Print the Operations from the Class table
        //===========================================
        if (class_tab == NULL)
        {
           PrintSectionTitle(no_ctab, HEAD4, BOLD10);
           return;
        }
        //---------------------------------------------------------
        // print the operation section, if it contains operations
        //---------------------------------------------------------
        o_rows = GetFirstLastRows(class_tab.id, m_OPERATION_NODE);

        if   (o_rows.first > 0)
        {
             o_h_rows = to_string(o_rows.first - 1);

             PrintTable(class_tab, e_PRINT_SETTING, MEMBER_COL
                      , o_rows.first + "-" + o_rows.last, o_h_rows, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (e_WITH_ANALYSIS_INFO)
                PrintTable(class_tab, e_PRINT_SETTING, ANALYSIS_COL_O
                      , o_rows.first + "-" + o_rows.last, o_h_rows, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (e_WITH_CXX_INFO)
                PrintTable(class_tab, e_PRINT_SETTING, PROG_CXX_COL_O
                      , o_rows.first + "-" + o_rows.last, o_h_rows, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (e_WITH_JAVA_INFO)
                PrintTable(class_tab, e_PRINT_SETTING, PROG_JAVA_COL
                      , o_rows.first + "-" + o_rows.last, o_h_rows, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (e_WITH_IDL_INFO)
                PrintTable(class_tab, e_PRINT_SETTING, PROG_IDL_COL_O
                      , o_rows.first + "-" + o_rows.last, o_h_rows, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (e_WITH_ADA_INFO)
                PrintTable(class_tab, e_PRINT_SETTING, PROG_ADA_COL_O
                      , o_rows.first + "-" + o_rows.last, o_h_rows, CELLHEAD
                      , CELLBODY, CAPTION , diag_source, rep_table, False); 

             if (ocnt > (o_rows.last - o_rows.first + 1))
                PrintSectionTitle(missing_op, HEAD4, BOLD10);                
        }
        else print_section_title(print_empty, HEAD4, BOLD10);
   }
   else
   {
        //===========================================
        // Print the Operations from the Repository
        //===========================================
        ocnt = set_count(operations);

        for (ox = 0; ox < ocnt; ox = ox + 1)
        {
            one_op = set_get_element(operations, ox);
            paragraph(BULLET);  
            character_format_set(ITALIC);

            op_info = ItemOfTypeWithObjId(m_OP_RETURN_TYPE_ITEM, one_op.id);
            if (op_info != NULL) 
               print(op_info.value + " ");

            character_format_set(BOLD11);
            print(one_op.name);

            character_format_set(STANDARD);
            print(" (" + one_op.sig + ") ");

            if (e_WITH_ANALYSIS_INFO)
            {
               comma    = False;
               op_items = m_MB_VISIBILITY_ITEM  + " || " 
                        + m_MB_IS_CLASS_ITEM    + " || "
                        + m_OP_IS_ABSTRACT_ITEM + " || "
                        + m_OP_THROWS_ITEM;

               if (GetNumberOfItems(one_op.id, op_items) > 0)
               {
                  PrintSectionTitle(is_status + ":", CBULLET, BOLD10);
                  paragraph(CBULLET);  

                  comma = PrintValueItem(one_op.id, m_MB_VISIBILITY_ITEM
                        , is_visible, comma);
                  comma = (PrintBooleanItem(one_op.id, m_MB_IS_CLASS_ITEM
                        , is_meta_op, comma) || comma);
                  comma = (PrintBooleanItem(one_op.id, m_OP_IS_ABSTRACT_ITEM
                        , is_abstract, comma) || comma);
                  comma = (PrintValueItem(one_op.id, m_OP_THROWS_ITEM
                        , is_throws, comma) || comma);
               }
            }

            if (e_WITH_CXX_INFO)
            {
               comma    = False;
               op_items = m_MB_CXX_CONST_ITEM  + " || " 
                        + m_MB_CXX_VISIB_ITEM    + " || "
                        + m_OP_CXX_VIRTUAL_ITEM + " || "
                        + m_OP_CXX_INLINE_ITEM;

               if (GetNumberOfItems(one_op.id, op_items) > 0)
               {
                  PrintSectionTitle("C++:", CBULLET, BOLD10);
                  paragraph(CBULLET);  

                  comma = PrintBooleanItem(one_op.id, m_MB_CXX_CONST_ITEM
                        , "Constant: ", comma);
                  comma = (PrintValueItem(one_op.id, m_MB_CXX_VISIB_ITEM
                        , is_visible, comma) || comma);
                  comma = (PrintBooleanItem(one_op.id, m_OP_CXX_VIRTUAL_ITEM
                        , "Virtual: ", comma) || comma);
                  comma = (PrintBooleanItem(one_op.id, m_OP_CXX_INLINE_ITEM
                        , "Inline: ", comma) || comma);
                  PrintTheItem(one_op.id, m_OP_CXX_CTILIST_ITEM
                             , "Ctor Init List: ", CBULLET, STANDARD, ITALIC);
               }
            }

            if (e_WITH_JAVA_INFO)
            {
               comma    = False;
               op_items = m_MB_JAV_VISIB_ITEM  + " || " 
                        + m_MB_JAV_FINAL_ITEM  + " || "
                        + m_OP_JAV_NATIVE_ITEM + " || "
                        + m_OP_JAV_SYNCH_ITEM;

               if (GetNumberOfItems(one_op.id, op_items) > 0)
               {
                  PrintSectionTitle("Java:", CBULLET, BOLD10);
                  paragraph(CBULLET);  

                  comma = PrintValueItem(one_op.id, m_MB_JAV_VISIB_ITEM
                        , is_visible, comma);
                  comma = (PrintBooleanItem(one_op.id, m_MB_JAV_FINAL_ITEM
                        , "Final: ", comma) || comma);
                  comma = (PrintBooleanItem(one_op.id, m_OP_JAV_NATIVE_ITEM
                        , "Native: ", comma) || comma);
                  comma = (PrintBooleanItem(one_op.id, m_OP_JAV_SYNCH_ITEM
                        , "Synchronized: ", comma) || comma);
               }
            }

            if (e_WITH_IDL_INFO)
            {
               comma    = False;
               op_items = m_OP_IDL_SIGNATURE_ITEM  + " || " 
                        + m_OP_IDL_RTYPE_ITEM  + " || "
                        + m_OP_IDL_ATTRIBUTE_ITEM + " || "
                        + m_OP_IDL_CONTEXT_ITEM;

               if (GetNumberOfItems(one_op.id, op_items) > 0)
               {
                  PrintSectionTitle("Java:", CBULLET, BOLD10);
                  paragraph(CBULLET);  

                  comma = PrintValueItem(one_op.id, m_OP_IDL_SIGNATURE_ITEM
                        , "Signature: ", comma);
                  comma = (PrintValueItem(one_op.id, m_OP_IDL_RTYPE_ITEM
                        , "Return Type: ", comma) || comma);
                  comma = (PrintValueItem(one_op.id, m_OP_IDL_ATTRIBUTE_ITEM
                        , "Attribute: ", comma) || comma);
                  comma = (PrintValueItem(one_op.id, m_OP_IDL_CONTEXT_ITEM
                        , "Context: ", comma) || comma);
               }
            }

            if (e_WITH_ADA_INFO)
            {
               comma    = False;
               op_items = m_MB_ADA_VISIB_ITEM   + " || " 
                        + m_OP_ADA_SUBUNIT_ITEM + " || "
                        + m_OP_ADA_INLINE_ITEM;

               if (GetNumberOfItems(one_op.id, op_items) > 0)
               {
                  PrintSectionTitle("Ada95:", CBULLET, BOLD10);
                  paragraph(CBULLET);  

                  comma = PrintValueItem(one_op.id, m_MB_ADA_VISIB_ITEM
                        , is_visible, comma);
                  comma = (PrintBooleanItem(one_op.id, m_OP_ADA_SUBUNIT_ITEM
                        , "Subunit: ", comma) || comma);
                  comma = (PrintBooleanItem(one_op.id, m_OP_ADA_INLINE_ITEM
                        , "Inline: ", comma) || comma);
               }
            }

            PrintAnyDesc(one_op.id, m_OP_DEF_NOTE, CBULLET, 0);
        }
   }
   return;
}


//*****************************************************************************
// PrintPackageDetail: Prints detailed information about the given packages   
//                     - one_pac: the current package to describe
//                     - file_id: id of the file with the package reference
//                     - p_cont : nodes that packages contain
//                     - p_part : nodes that packages are part of
//*****************************************************************************
void PrintPackageDetail(node one_pac, int file_id
                      , string p_cont, string p_part)
{
   //---------------------------------------------------------
   // Package extensibility, contains, is part of, references
   //---------------------------------------------------------
   PrintExtInfo(one_pac.id);

   PrintObjectsLinks(one_pac, p_cont, "from" , m_CONTAINS_LINK, "in"
                   , consists_of);

   PrintObjectsLinks(one_pac, p_part, "to", m_CONTAINS_LINK, "out"
                   , is_part_of);

   PrintObjectsLinks(one_pac, p_part, "to", m_DEPENENCY_LINK, "out"
                   , is_depending);

   PrintRefInfo(one_pac.id, one_pac.name, "node", file_id);

   return;
}


//*****************************************************************************
// PrintPackageInfo: Prints all kind of information about packages depending  
//                   on the calling chapter.
//                    - file_id : id of the file with the package reference
//                    - printed : set of already printed elements
//                    - ref_text: cross reference text (if printed)
//                    - detailed: print detailed information, True/False
//                    - p_cont  : nodes that packages contain
//                    - p_part  : nodes that packages are part of
//*****************************************************************************
void PrintPackageInfo(int file_id, set printed, string ref_text
                    , boolean detailed, string p_cont, string p_part)
{
   int   pcnt, px;

   node  one_pac;

   set   packages;


   packages = set_create("node");
   packages = NodesOfTypeWithRefsWithFileIdSet(m_PACKAGE_NODE , file_id, OX);
   pcnt     = set_count(packages);

   for (px = 0; px < pcnt; px = px + 1)
   {
       one_pac = set_get_element(packages, px);
       PrintSectionTitle(pac_name + one_pac.name, HEAD4, BOLD11);

       //--------------------------------------------
       // Already described for another diagram ?
       //--------------------------------------------
       if (IsPrinted(one_pac.id, ref_text, printed))
          continue;

       PrintObjectDesc(one_pac.id, BODY_NORMAL);

       if (detailed)
          PrintPackageDetail(one_pac, file_id, p_cont, p_part);

   }
   set_clear(packages);

   return;
}


//*****************************************************************************
// PrintScenarioDiagrams: prints and describes all scenario files, i.e. 
//                        Sequence andCollaboration Diagrams in the given set.
//                        - sce_set:  set with scenario files 
//                        - sce_ref:  reference text if already printed 
//                        - printed:  already printed in current chapter
//                        - detailed: print detailed description, True or False 
//*****************************************************************************
void  PrintScenarioDiagrams(set sce_set, string sce_ref, set printed
                          , boolean detailed)
{
   boolean  is_col,
            is_seq,
            p_details;

   int      ccnt, cx,
            scnt, sx,
            RET;

   file     scenario;

   link     sce_link;

   node     act_inst,
            obj_inst,
            sce_inst,
            sce_node;

   set      sce_symbols;

   string   cont_nodes,
            msg_links,
            part_nodes,
            sce_text,
            sce_title,
            the_name;


   scnt = set_count(sce_set);
   if (scnt < 1)
      return;

   for (sx = 0; sx < scnt; sx = sx + 1)
   {
       scenario = set_get_element(sce_set, sx);

       if (scenario.type == m_SEQUENCE_FILE)
       {
          is_seq     = True;
          is_col     = False;
          cont_nodes = m_PACKAGE_NODE + " || " + m_OBJECT_NODE + " || "
                     + m_OBJECT_CLASS_NODE;
          part_nodes = m_PACKAGE_NODE;
       }
       else
       {
          is_col     = True;
          is_seq     = False;
          cont_nodes = m_PACKAGE_NODE + " || " + m_OBJECT_CLASS_NODE  + " || "
                     + m_OBJECT_NODE  + " || " + m_ACTIVE_OBJECT_NODE + " || "
                     + m_TYPE_NODE    + " || " + m_COLABORATION_NODE;
          part_nodes = m_PACKAGE_NODE + " || " + m_OBJECT_CLASS_NODE  + " || "
                     + m_OBJECT_NODE  + " || " + m_ACTIVE_OBJECT_NODE;
       }


       sce_inst = NULL;
       sce_inst = NodeOfTypeWithRefsWithFileId(m_SCENARIO_NODE, scenario.id);

       if  (sce_inst == NULL)
            sce_title = scenario.name;
       else sce_title = sce_inst.name;

       if   (is_col)
            sce_text = col_diag;
       else sce_text = seq_diag;

       PrintSectionTitle(sce_text + sce_title, HEAD4, BOLD12);

       //--------------------------------------------
       // Already described for another chapter ?
       //--------------------------------------------
       if (IsPrinted(scenario.id, sce_ref + ">", PRINTED_FILES))
           continue;

       PrintObjectDesc(scenario.id, BODY_NORMAL);

       PrintDiagram(scenario, e_OPTIMIZED_PRINTING, e_PRINT_SETTING 
                  , e_NO_DISPLAYMARKS, BODY_NORMAL, CAPTION 
                  , diag_source, rep_diag); 


       //------------------------------------------------
       // Describe the Packages in each diagram
       // No details if the package report is generated !
       //------------------------------------------------
       if   (e_GEN_PACKAGE_REPORT)
            p_details = False;
       else p_details = detailed;

       PrintPackageInfo(scenario.id, printed, sce_ref + ", " + sce_text  
                      + sce_title + ">", p_details, cont_nodes, part_nodes);


       //--------------------------------------------------------
       // Describe the collaborations in each col diagram
       //--------------------------------------------------------
       if (is_col)
          PrintCollSpecInfo(scenario.id, m_COLABORATION_NODE, col_name, printed
                          , sce_ref + ", " + sce_text + sce_title + ">"
                          , detailed, part_nodes);


       //-----------------------------------------------------
       // Describe the Types declared in each col diagram
       //-----------------------------------------------------
       if (is_col)
          PrintCollSpecInfo(scenario.id, m_TYPE_NODE, type_name, printed
                          , sce_ref + ", " + sce_text + sce_title + ">"
                          , detailed, part_nodes);


       //---------------------------------------
       // Describe the Actors in each diagram
       //---------------------------------------
       PrintActorInfo(scenario.id, printed, sce_ref + ", " + sce_text + 
                      sce_title + ">", detailed);


       //---------------------------------------
       // Describe the Objects of each diagram
       //---------------------------------------
       PrintObjectInfo(scenario.id, is_col, printed, sce_ref + ", " + sce_text  
                     + sce_title + ">", detailed, cont_nodes, part_nodes);


       //-----------------------------------------------------
       // Describe the Messages in each diagram
       //-----------------------------------------------------
       PrintMessageInfo(scenario.id, printed, sce_ref + ", " + sce_text  
                     + sce_title + ">", detailed);

          
       //-------------------------------------          
       // Collect Glossary items if demanded
       //-------------------------------------
       if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
          GlossaryForFile(scenario.id);
   }

   return;
}


//*****************************************************************************
// PrintStateDiagrams: prints and describes all State Diagrams in the given set
//                     - state_set:  set with state diagrams 
//                     - state_ref:  reference text if already printed 
//*****************************************************************************
void  PrintStateDiagrams(set state_set, string state_ref)
{
   boolean  print_order;

   int      acnt, ax,
            ccnt, cx,
            RET;

   file     state_file;

   link     state_link;

   node     state_node;

   set      state_symbols;


   print_order         = e_PRINT_ALPHA_ORDER;
   e_PRINT_ALPHA_ORDER = True;
   acnt                = set_count(state_set);

   if (acnt < 1)
      return;

   for (ax = 0; ax < acnt; ax = ax + 1)
   {
       state_file = set_get_element(state_set, ax);

       PrintSectionTitle(dyn_diag + state_file.name, HEAD4, BOLD12);

       //--------------------------------------------
       // Already described in another chapter ?
       //--------------------------------------------
       if (IsPrinted(state_file.id, state_ref + ">", PRINTED_FILES))
           continue;

       PrintObjectDesc(state_file.id, BODY_NORMAL);

       PrintDiagram(state_file, e_OPTIMIZED_PRINTING, e_PRINT_SETTING 
                  , e_NO_DISPLAYMARKS, BODY_NORMAL, CAPTION 
                  , diag_source, rep_diag); 

       //-----------------------------------------------------
       // Describe the States declared in each diagram
       // in alphabetical order (save e_PRINT_ALPHA_ORDER)
       //-----------------------------------------------------
       PrintStateInfo(state_file.id, PRINTED_FILES, state_ref + ", " 
                    + dyn_diag + state_file.name + ">",  e_CLASS_DETAILS);

       //-------------------------------------          
       // Collect Glossary items if demanded
       //-------------------------------------
       if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
          GlossaryForFile(state_file.id);
   }

   e_PRINT_ALPHA_ORDER = print_order;

   return;
}


//*****************************************************************************
// PrintStateInfo: Prints all kind of information about states in state- and  
//                 activity diagrams.
//                 - file_id : id of the state or activity diagram
//                 - printed : set of already printed elements
//                 - ref_text: cross reference text (if printed)
//                 - detailed: print detailed information, True/False
//*****************************************************************************
void PrintStateInfo(int file_id, set printed, string ref_text, boolean detailed)
{
   int   scnt, sx;

   node  one_state;

   set   states;

   string state_nodes,
          the_name;


   state_nodes = m_STATE_NODE         + " || " + m_INIT_STATE_NODE  + " || "
               + m_ACTION_STATE_NODE  + " || " + m_FINAL_STATE_NODE + " || "
               + m_HISTORY_STATE_NODE + " || " + m_JUNCTION_POINT + " || " +
               m_DYNAMIC_CHOICE_POINT; // ECR 7215
               

   states = set_create("node");
   states = NodesOfTypeWithRefsWithFileIdSet(state_nodes, file_id, OX);
   scnt   = set_count(states);

   for (sx = 0; sx < scnt; sx = sx + 1)
   {
       one_state = set_get_element(states, sx);

       if      (one_state.type == m_STATE_NODE)
               the_name = std_state;
       else if (one_state.type == m_INIT_STATE_NODE)
               the_name = start_state;
       else if (one_state.type == m_ACTION_STATE_NODE)
               the_name = act_state;
       else if (one_state.type == m_FINAL_STATE_NODE)
               the_name = final_state;
       else if (one_state.type == m_HISTORY_STATE_NODE)
               the_name = hist_state;
       // ECR 7215
       else if (one_state.type == m_JUNCTION_POINT)
               the_name = junction_point;
       else if (one_state.type == m_DYNAMIC_CHOICE_POINT)
               the_name = dynamic_choice_point;

       PrintSectionTitle(the_name + one_state.name, HEAD4, BOLD11);

       //--------------------------------------------
       // Already described for another diagram ?
       //--------------------------------------------
       if (IsPrinted(one_state.id, ref_text, printed))
          continue;

       PrintObjectDesc(one_state.id, BODY_NORMAL);

       if (!detailed)
          continue;

       //---------------------------------------------------------
       // State information: extensibility, contains, references
       //---------------------------------------------------------
       PrintExtInfo(one_state.id);                                     

       PrintRefInfo(one_state.id, one_state.name, "node", file_id);
   }
   set_clear(states);

   return;
}


//*****************************************************************************
// PrintSystemInfo: Prints all kind of information about system symbols   
//                  - file_id : id of the file with the system reference
//                  - printed : set of already printed elements
//                  - ref_text: cross reference text (if printed)
//                  - detailed: print detailed information, True/False
//                  - p_cont  : nodes that systems contain
//*****************************************************************************
void PrintSystemInfo(int file_id, set printed, string ref_text
                   , boolean detailed, string p_cont)
{
   int   scnt, sx;

   node  one_sys;

   set   systems;


   systems = set_create("node");
   systems = NodesOfTypeWithRefsWithFileIdSet(m_SYSTEM_NODE , file_id, OX);
   scnt    = set_count(systems);

   for (sx = 0; sx < scnt; sx = sx + 1)
   {
       one_sys = set_get_element(systems, sx);
       PrintSectionTitle(sys_name + one_sys.name, HEAD4, BOLD11);

       //--------------------------------------------
       // Already described for another diagram ?
       //--------------------------------------------
       if (IsPrinted(one_sys.id, ref_text, printed))
          continue;

       PrintObjectDesc(one_sys.id, BODY_NORMAL);

       if (!detailed)
          continue;

       //---------------------------------------------------------
       // System information: extensibility, contains, references
       //---------------------------------------------------------
       PrintExtInfo(one_sys.id);                                     

       PrintObjectsLinks(one_sys, p_cont, "from" 
                       , m_CONTAINS_LINK, "in", consists_of);

       PrintRefInfo(one_sys.id, one_sys.name, "node", file_id);
   }
   set_clear(systems);

   return;
}

