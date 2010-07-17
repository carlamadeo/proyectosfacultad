//*****************************************************************************
// DESCRIPTION: UML_Report.inc
//              ----------------------
//              (c) 1997 by AONIX 
//*****************************************************************************
//
// Include file for all UML_Reports with the main generation functions.
//*****************************************************************************
//
// functions: - int      ClassReportChapter() 
//            - int      GenUMLReport(string ui_language)
//            - void     IntroPage(int new_page) 
//            - int      LogSysArchChapter(graph hierarchy)
//            - int      PackageReportChapter()
//            - int      StereotypeChapter()
//            - int      UsersViewChapter()
//*****************************************************************************
 
//*****************************************************************************
// GenUMLReport: This function is called from the language depending UML_Report
//               script and does all the work. 
//               - ui_language: string with the language for ui-messages
//*****************************************************************************
int  GenUMLReport(string ui_language)
{
    //==============================================
    // declarations 
    //==============================================
    int     any_choices,
            status = IS_OK;

    graph   pack_graph;

    set     pack_set;
    string  class_links,
            pack_names;
    
    //----------------------------------------------------------
    // initialise global variables with corresponding externals 
    //----------------------------------------------------------
    MapExternals();    

    //================================================           
    // assign the reporting and the document language 
    //================================================           
    SetUILanguage(ui_language);
    status = SetTargetLanguage(e_DOCUMENT_LANGUAGE);
    if (status != IS_OK)
       message(m_no_lang);       

    //================================================
    // set print-formats according to print target
    // function (MIF or RTF) in ALL_Formats.inc;
    // If everything is ok, set the default format
    //================================================
    status = SetTargetFormat();
    if (status != IS_OK)
    {
       message(m_no_targ); 
       return IS_OK;
    }     
    format(e_DEFAULT_FORMAT);

    //
    // Specify whether page() calls generate new document files.
    //
    if (!e_GEN_SINGLE_HTML_FILE)
        page_equals_document(HTML);

    message(start_msg);
    message(lang_msg);


    if (  !e_GEN_REQUIREMENTS   && !e_GEN_USERS_VIEW   && !e_GEN_SYS_ARCH
       && !e_GEN_PACKAGE_REPORT && !e_GEN_CLASS_REPORT && !e_GEN_STEREOTYPES)
    {
       message(m_no_chap);
       return IS_OK;
    }

    //===========================================           
    // find the package hierarchy
    //===========================================     
    pack_set = set_create("string"); 

    if   (StringToSet(pack_set, SetToString(NodesOfTypeSet(m_PACKAGE_NODE)
                    , " "), " ") > 0)     
         pack_graph = graph_create(pack_set);
    else pack_graph = graph_create();

    set_clear(pack_set);

    BuildGraphHierarchy(pack_graph, m_CONTAINS_LINK
                      , m_PACKAGE_NODE, m_PACKAGE_NODE);

    if   ((e_PACKAGES = ReduceToObjects(m_PACKAGE_NODE, m_CONTAINS_LINK, IN
                                    , e_PACKAGES)) == "")
    {
         message(m_no_pack);
         return IS_OK;
    }

    class_links = "( " + m_GENERALIZATION_LINK + " || "
                       + m_DEPENENCY_LINK      + " || "
                       + m_IMPLEMENTS_LINK     + " || "
                       + m_REFINES_LINK        + " )";

    e_CLASSES   = ReduceToObjects(m_CLASS_NODE, class_links, OUT, e_CLASSES);

    //===========================================           
    // Init global sets and lists if demanded
    //===========================================           
    if (e_GEN_GLOSSARY)
       InitGlossary();

    PRINTED_FILES = set_create("PRT_IN");

    //===========================================           
    // generate the document information 
    //===========================================           
    FirstPage();
    message(cover_msg);

    if   (TableOfContents())
	     IntroPage(0); // no extra page break
    else IntroPage(1); // page break

    message(info_msg);
 
    //===============================================           
    // generate the requirement chapter, if demanded 
    //===============================================           
    if (e_GEN_REQUIREMENTS) 
    {        
       message(req_start);
       e_RQTBL_FILES = ReduceToFiles(e_RQTBL_FILES);
       status        = RequirementChapter();

       if   (status == 0)
            message(m_no_req);
    }


    //======================================           
    // generate the use cases, if demanded 
    //======================================         
    if (e_GEN_USERS_VIEW) 
    {          
       message(uca_start);
       e_USECASE_FILES = ReduceToFiles(e_USECASE_FILES);
       status          = UsersViewChapter();

       if   (status == 0)
            message(m_no_uca);
    }


    //======================================           
    // generate the sys.arch., if demanded 
    //======================================         
    if (e_GEN_SYS_ARCH) 
    {          
       //-----------------------------------------------------
       // Print the chapter's headline and the introduction
       //-----------------------------------------------------
       message(sarc_start);
       PrintChapterTitle(sarc_heading, HEAD1, 1, sarc_intro, True);
       any_choices = 0;

       //-----------------------------------------------------
       // Print the logical subchapter, if demanded
       //-----------------------------------------------------
       if (e_LOG_ARCH)
       {
          message(lsa_start);
          status = LogSysArchChapter(pack_graph);

          if   (status == 0)
               message(m_no_lsa);

          any_choices = any_choices + status;
       }
       //-----------------------------------------------------------------
       // Print the physical subchapter, if demanded
       //-----------------------------------------------------------------
       if (e_PHYS_ARCH)
       {
          message(psa_start);
          e_IMPL_FILES = ReduceToFiles(e_IMPL_FILES);
          status       = PhysSysArchChapter();

          if   (status == 0)
               message(m_no_psa);

          any_choices = any_choices + status;
       }

       if   (any_choices == 0)
            message(m_no_sarc);
    }


    //============================================           
    // generate the package report, if demanded 
    //============================================         
    if (e_GEN_PACKAGE_REPORT) 
    {          
       message(pac_start);
       status = PackageReportChapter();

       if   (status == 0)
            message(m_no_pac);
    }

    //============================================           
    // generate the class report, if demanded 
    //============================================         
    if (e_GEN_CLASS_REPORT) 
    {          
       message(cls_start);
       status = ClassReportChapter();

       if   (status == 0)
            message(m_no_cls);
    }


    //============================================           
    // generate the stereotype defs, if demanded 
    //============================================         
    if (e_GEN_STEREOTYPES) 
    {          
       message(stt_start);
       e_STEREOTYPES = ReduceToFiles(e_STEREOTYPES);
       status        = StereotypeChapter();

       if   (status == 0)
            message(m_no_stt);
    }


    //==================================================           
    // generate the appendix with glossary, if demanded 
    //==================================================         
    if (e_GEN_GLOSSARY) 
    {          
       message(gls_start);

       if   (e_REFD_GLOSSARY &&
            !e_GEN_REQUIREMENTS && !e_GEN_USERS_VIEW )
            status = 2; 
       else status = GlossaryChapter();

       if   (status == 1)
            message(m_no_glsd);
       else if   (status == 2)
            message(m_no_glsr);
    }

    //===========================================           
    // Free global sets and lists if existent
    //===========================================           
    if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
       FreeGlossary();

	set_clear(PRINTED_FILES);
    graph_clear(pack_graph);

    message(final_msg);
    return IS_OK;
}


//*****************************************************************************
// IntroPage: Prints the documents generation information
//*****************************************************************************
void  IntroPage(int new_page) 
{  
   PrintChapterTitle(info_heading, HEAD1NOIVZ, new_page, info_intro, True);

   if (target() == HTML)
   {
       table_of_contents_entry(HEAD1, BULLET);
       table_of_contents_print();
   }
   else
   {
       if (e_GEN_REQUIREMENTS)
           PrintSectionTitle(req_heading, BULLET, "");
       if (e_GEN_USERS_VIEW)
           PrintSectionTitle(uca_heading, BULLET, "");
       if (e_GEN_SYS_ARCH)
           PrintSectionTitle(sarc_heading, BULLET, "");
       if (e_GEN_PACKAGE_REPORT)
           PrintSectionTitle(pac_heading, BULLET, "");
       if (e_GEN_CLASS_REPORT)
           PrintSectionTitle(class_heading, BULLET, "");
       if (e_GEN_STEREOTYPES)
           PrintSectionTitle(stt_heading, BULLET, "");
   }

   return;
}


//*****************************************************************************
// UsersViewChapter: generates the chapter with the Users View with:
//                   - Use Case-Diagrams: 
//                     - optional with detailed description of 
//                       System, Package and Actor symbols
//                     - use case descriptions for each usecase in 
//                         vertical or alphabetical order
//                       - sequence diagrams for each use case 
//                         - optional with detailed descriptions
//                       - collaboration diagrams for each use case 
//                         - optional with detailed descriptions
//                       - activity diagrams for each use case 
//                         - optional with detailed descriptions
//                   Can be reduced to UseCase Diagrams, Packages, Stereotypes
//*****************************************************************************
int UsersViewChapter()
{
   boolean detailed;

   file    uca_file;

   int     ccnt, cx,
           scnt, sx,
           ucnt, ux,
           uvw_number;

   node    uc_ref,
           uc_symbol,
           use_case;

   set     reduced,
           printed,
		   scenarios,
           uc_symbols,
           uca_files,
	  	   uca_refs,
	  	   use_cases;

   string  cont_nodes,
           part_nodes,
		   ref_text,
		   sce_files,
		   sce_ref;

   //====================================================
   // Print the chapter's headline and the introduction
   //====================================================
   PrintChapterTitle(uca_heading, HEAD1, 1, uca_intro, True);

   //--------------------------------------------------------------
   // Build a set of all use case diagrams matching the reductions
   //--------------------------------------------------------------
   uca_files = set_create("file");
   uca_files = MatchingFilesOfTypeSet(m_USECASE_FILE, " && node_refs" 
                                      , e_USECASE_FILES);
   ucnt      = set_count(uca_files);

   if ((ucnt > 0) && (e_PACKAGES != ANY))
   {
      reduced   = set_create("file");
      reduced   = FilesOfTypeWithNodesOfTypeAndNameSet(m_USECASE_FILE
	                                                 , m_PACKAGE_NODE
                                                     , e_PACKAGES);
      ucnt      = set_intersection(uca_files, reduced);

	  set_clear(reduced);
   }
   
   if   (ucnt < 1)
   { 
        PrintSectionTitle(no_uca, HEAD4, BOLD12);
        uvw_number = 0; // nothing found
   }
   else    
   { 
        //---------------------------------------
        // Print the Use Case diagrams
        //---------------------------------------
        uvw_number = 1; // use case diagrams found to start with
		cont_nodes = m_PACKAGE_NODE  + " || " + m_USECASE_NODE;
		part_nodes = m_PACKAGE_NODE  + " || " + m_SYSTEM_NODE;
		sce_files  = m_SEQUENCE_FILE + " || " + m_COLLABORATION_FILE;
        printed    = set_create("PRT_IN");

        for (ux = 0; ux < ucnt; ux = ux + 1)
        {
            uca_file = set_get_element(uca_files, ux); 
			ref_text = cross_ref + "<" + uca_heading + ", "  
                     + uca_title + uca_file.name + ">";

            PrintChapterTitle(uca_title + uca_file.name, HEAD2, ux, "", False);
            PrintObjectDesc(uca_file.id, BODY_NORMAL);

            PrintDiagram(uca_file, e_OPTIMIZED_PRINTING, e_PRINT_SETTING
			           , e_NO_DISPLAYMARKS, BODY_NORMAL, CAPTION
					   , diag_source, rep_diag); 


            //----------------------------------------------------------
            // Describe the System symbol (only if part of the diagram)
            //----------------------------------------------------------
            PrintSystemInfo(uca_file.id, printed, ref_text, e_UV_DETAILS
                          , cont_nodes);

            //------------------------------------------------
            // Describe the Packages of each diagram
            // No details if the package report is generated !
            //------------------------------------------------
            if   (e_GEN_PACKAGE_REPORT)
                 detailed = False;
            else detailed = e_UV_DETAILS;

            PrintPackageInfo(uca_file.id, printed, ref_text, detailed
                           , cont_nodes, part_nodes);

            //---------------------------------------
            // Describe the Actors of each diagram
            //---------------------------------------
            PrintActorInfo(uca_file.id, printed, ref_text, e_UV_DETAILS);


            //-------------------------------------------------------------
            // Build the list of Use cases in alphabetical or vertical
            // order (depending on print_vertical_order)
            //-------------------------------------------------------------
            use_cases = set_create("node");
		    use_cases = NodesOfTypeWithRefsWithFileIdSet(m_USECASE_NODE 
			                                           , uca_file.id, OY + "," + OX); // ECR 7714
	   	    ccnt      = set_count(use_cases);

            if ((ccnt > 0) && (e_PACKAGES != ANY))
            {
               reduced   = set_create("node");
               reduced   = NodesOfTypeContainedInSet(m_USECASE_NODE 
			                                       , m_PACKAGE_NODE
                                                   , e_PACKAGES);
               ccnt      = set_intersection(use_cases, reduced);

       	       set_clear(reduced);
            }

            for (cx = 0; cx < ccnt; cx = cx + 1)
            {
                use_case = set_get_element(use_cases, cx);

                if   (e_SCENARIOS)
                     PrintChapterTitle(uca_name + use_case.name, HEAD3, 1
                                     , "" , False);
                else PrintChapterTitle(uca_name + use_case.name, HEAD3, 0
                                     , "" , False);

                message(rep_uca + use_case.name);

				//--------------------------------------------
                // Already described for another diagram ?
				//--------------------------------------------
				if (IsPrinted(use_case.id, ref_text, printed))
				   continue;

                PrintAnyDesc(use_case.id, m_USECASE_DEF_NOTE, BODY_NORMAL, 0);

                if (e_GENERATION_INFO)
                {
                   //---------------------------
                   // Generation information
                   //---------------------------
                   PrintGenInfo(use_case.id);
                }

                if (e_UV_DETAILS)
                {
			    	//----------------------------------------------
			        // Use Case extensibility ...
			    	//----------------------------------------------
				    PrintExtInfo(use_case.id);

				    //---------------------------------------------
				    // Use Case pre-, postcondition, exception ...
			    	//----------------------------------------------
				    PrintTheItem(use_case.id, m_UC_PRECONDITION_ITEM 
			    			   , uca_precond, HEAD4, BOLD11, STANDARD);
				    PrintTheItem(use_case.id, m_UC_POSTCONDITION_ITEM 
							   , uca_postcond, HEAD4, BOLD11, STANDARD);
				    PrintTheItem(use_case.id, m_UC_EXCEPTION_ITEM  
							   , uca_except,HEAD4, BOLD11, STANDARD);

			    	//----------------------------------------------
				    // Use case is part of ...
			    	//----------------------------------------------
				    PrintObjectsLinks(use_case, part_nodes, "to"
					                , m_CONTAINS_LINK, "out", is_part_of);

			    	//----------------------------------------------
				    // Use case is performed by ...
			    	//----------------------------------------------
				    PrintObjectsLinks(use_case, m_ACTOR_NODE, "to" 
					                , m_UC_INTERACTION_LINK, "out"
                                    , is_performed);

			    	//----------------------------------------------
				    // Use Case uses ...
			    	//----------------------------------------------
				    PrintObjectsLinks(use_case, m_USECASE_NODE, "from" 
				                    , m_UC_USES_LINK, "in", uc_uses);

			    	//----------------------------------------------
				    // Use Case is used by ...
			    	//----------------------------------------------
				    PrintObjectsLinks(use_case, m_USECASE_NODE, "to" 
				                    , m_UC_USES_LINK, "out", uc_is_used);

			    	//----------------------------------------------
				    // Use Case extends ...
			    	//----------------------------------------------
				    PrintObjectsLinks(use_case, m_USECASE_NODE, "from" 
				                    , m_UC_EXTENDS_LINK, "in", uc_extends);

			    	//----------------------------------------------
				    // Use Case is extended by ...
			    	//----------------------------------------------
				    PrintObjectsLinks(use_case, m_USECASE_NODE, "to" 
				                    , m_UC_EXTENDS_LINK, "out", uc_is_ext);

			    	//----------------------------------------------
				    // Use Case references ...
			    	//----------------------------------------------
				    PrintRefInfo(use_case.id, use_case.name, "node"
                               , uca_file.id);
                }


				//--------------------------------------------
				// Print Scenarios connected to the Use Case
				//--------------------------------------------
                if (!e_SCENARIOS)
                   continue;

			    sce_ref = cross_ref + "<" + uca_heading + ", " 
                         + uca_name + use_case.name;

				//--------------------------------------------
				// Sequence and Collaboration Diagrams
				//--------------------------------------------
                scenarios = set_create("file");
		        scenarios = UmlScenarioInstanceFileSet(sce_files 
				                                     , m_USECASE_NODE
                                                     , use_case.name);

                PrintScenarioDiagrams(scenarios, sce_ref, printed
                                    , e_UV_DETAILS);
                set_clear(scenarios);

				//--------------------------------------------
				//  Activity Diagrams 
				//--------------------------------------------
                scenarios = set_create("file");
		        scenarios = FilesOfTypeWithNodesOfTypeAndNameSet
                                 (m_ACTIVITY_FILE, m_STATE_MACHINE_NODE 
                                , " name $ '" + use_case.name + "'");

                PrintActivityDiagrams(scenarios, sce_ref, e_UV_DETAILS);
                set_clear(scenarios);
            }

            set_clear(use_cases);
        }

        //-------------------------------------          
        // Collect Glossary items if demanded
        //-------------------------------------
        if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
           GlossaryForFile(uca_file.id);

        set_clear(printed);
   }

   set_clear(uca_files);

   return uvw_number;
}


//*****************************************************************************
// LogSysArchChapter: generates the subchapter with the logical system 
//                    architecture 
//                    - hierarchy: graph with package hierarchy
//*****************************************************************************
int LogSysArchChapter(graph hierarchy)
{
   int     pcnt, px,
           level,
           lsa_number,
           rank;

   node    one_pack;

   set     packages;

   //====================================================
   // Print the chapter's headline and the introduction
   //====================================================
   PrintChapterTitle(lsa_heading, HEAD2, 0, lsa_intro, False);

   //========================================================
   // Search the packages in the graph and print the chapter 
   //========================================================
   packages = set_create("node");
   packages = TopPackagesSet();
   pcnt     = set_count(packages);

   if   ( (pcnt < 1) || (graph_count(hierarchy) < 1) )
   {
        PrintSectionTitle(no_lsa, HEAD4, BOLD12);
        lsa_number = 0; // nothing found
   }
   else    
   { 
        lsa_number = 1; // packages found
        level      = 1;

        for (px = 0; px < pcnt; px = px + 1)
        {
            one_pack = set_get_element(packages, px);

            if ((one_pack.name == NULL) || (one_pack.name == ""))
               continue;

            paragraph(EBENE + level);
            character_format_set(BOLD12);
            print("- " + pac_name + one_pack.name);

            rank     = graph_node_rank(hierarchy, one_pack.name);
            if (rank > 0)
            {
               print(":");
               PrintSucNodes(hierarchy, one_pack.name, level + 1);
            }
       }
   }
   set_clear(packages);
   return lsa_number; 
}


//*****************************************************************************
// PhysSysArchChapter: generates the subchapter with the physical system  
//                     architecture 
//*****************************************************************************
int PhysSysArchChapter()
{
    int     pcnt, px,
            psa_number;

    file    impl_diag;

    set     impl_files;

    string  impl_diags,
            impl_type;

   //====================================================
   // Print the chapter's headline and the introduction
   //====================================================
   PrintChapterTitle(psa_heading, HEAD2, 1, psa_intro, False);

   //==========================================
   // Search the objects and print the chapter 
   //==========================================

   impl_diags = "(" + m_COMPONENT_FILE + " || " + m_DEPLOYMENT_FILE + ")";
   impl_files = set_create("file");
   impl_files = MatchingFilesOfTypeSet(impl_diags, " && node_refs"
                                     , e_IMPL_FILES);
   pcnt       = set_count(impl_files);

   if   (pcnt < 1)
   {
        PrintSectionTitle(no_psa, HEAD4, BOLD12);
        psa_number = 0; // nothing found
   }
   else    
   { 
        psa_number = 1; // implementation diagrams found

        for (px = 0; px < pcnt; px = px + 1)
        {
            impl_diag   = set_get_element(impl_files, px);
            if  (px == 0)
                 PrintChapterTitle(impl_diag.name, HEAD3, 0, "", False);
            else PrintChapterTitle(impl_diag.name, HEAD3, 1, "", False);

            PrintObjectDesc(impl_diag.id, BODY_NORMAL);

            PrintDiagram(impl_diag, e_OPTIMIZED_PRINTING, e_PRINT_SETTING 
	                   , e_NO_DISPLAYMARKS, BODY_NORMAL, CAPTION 
				       , diag_source, rep_diag); 

            if   (impl_diag.type == m_COMPONENT_FILE) 
                 impl_type = comp_diag;       
                 impl_type = depl_diag;       

            PrintImplInfo(impl_diag.id, PRINTED_FILES, psa_heading + ", " 
                        + impl_type + impl_diag.name + ">",  e_IMPL_DETAILS);

            //-------------------------------------          
            // Collect Glossary items if demanded
            //-------------------------------------
            if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
               GlossaryForFile(impl_diag.id);
       }
   }
   set_clear(impl_files);
   return psa_number; 
}


//*****************************************************************************
// PackageReportChapter: generates the Chapter with the package report 
//*****************************************************************************
int PackageReportChapter()
{
    file    one_file;

    int     fcnt, fx,
            pcnt, px,
            pac_number;

    node    one_pack;

    set     cls_files,
            pac_files,
            packages;

    string  cont_nodes,
            part_nodes,
            pac_ref;

   //====================================================
   // Print the chapter's headline and the introduction
   //====================================================
   PrintChapterTitle(pac_heading, HEAD1, 1, pac_intro, True);

   //==========================================
   // Search the objects and print the chapter 
   //==========================================
   cont_nodes = m_PACKAGE_NODE  + " || " + m_CLASS_NODE;
   part_nodes = m_PACKAGE_NODE  + " || " + m_SYSTEM_NODE;

   packages = set_create("node");
   packages = MatchingNodesOfTypeSet(m_PACKAGE_NODE, e_PACKAGES);
   pcnt     = set_count(packages);

   if   (pcnt < 1)
   {
        PrintSectionTitle(no_pac, HEAD4, BOLD12);
        pac_number = 0; // nothing found
   }
   else    
   { 
        pac_number = 1; // packages found
   }
        // BUG 2741, 6/24/99, MHS
        // Changed the flow here to go ahead and print class diagrams if there
        // are any.  Changed the message to indicate that.

        for (px = 0; px < pcnt; px = px + 1)
        {
            one_pack = set_get_element(packages, px);
            pac_ref  = pac_name + one_pack.name;

            message(rep_pack + one_pack.name);
            PrintChapterTitle(pac_ref, HEAD2, 0, "", False);

            PrintObjectDesc(one_pack.id, BODY_NORMAL);

            if (e_PACK_DETAILS)
               PrintPackageDetail(one_pack, 0, cont_nodes, part_nodes);
       }

       if (e_PACK_DIAGRAMS)
       {
		  //------------------------
		  // Package diagrams ...
		  //------------------------
		  pac_ref   = cross_ref + "<" + pac_heading + ", " + pac_diag + ">";
          pac_files = set_create("file");
          pac_files = FilesOfTypeWithNodesOfTypeAndNameSet(m_CLASS_FILE
                                                         , m_PACKAGE_NODE
                                                         , e_PACKAGES);
          fcnt      = set_count(pac_files);
          if (fcnt > 0)
             PrintChapterTitle(pac_diag, HEAD2, 1, "", False);

          for (fx = 0; fx < fcnt; fx = fx + 1)
          {
              one_file = set_get_element(pac_files, fx);

              PrintSectionTitle(one_file.name, HEAD3, "");

	          //--------------------------------------------
              // Already described in another chapter ?
	          //--------------------------------------------
   	          if (IsPrinted(one_file.id, pac_ref, PRINTED_FILES))
  	             continue;

              PrintObjectDesc(one_file.id, BODY_NORMAL);

              PrintDiagram(one_file, e_OPTIMIZED_PRINTING, e_PRINT_SETTING 
	                     , e_NO_DISPLAYMARKS, BODY_NORMAL, CAPTION 
				         , diag_source, rep_diag); 

              //-------------------------------------          
              // Collect Glossary items if demanded
              //-------------------------------------
              if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
                 GlossaryForFile(one_file.id);
          }

		  //------------------------
		  // Class diagrams ...
		  //------------------------
		  pac_ref   = cross_ref + "<" + pac_heading +", " + cls_diag + ">";
          cls_files = set_create("file");
          cls_files = FilesOfTypeWithNodesOfTypeAndNameSet(m_CLASS_FILE
                                                         , m_CLASS_NODE
                                                         , e_CLASSES);
          fcnt      = set_count(cls_files);

          if (fcnt > 0)
             PrintChapterTitle(cls_diag, HEAD2, 1, "", False);

          for (fx = 0; fx < fcnt; fx = fx + 1)
          {
              one_file = set_get_element(cls_files, fx);

              if (set_is_member(pac_files, one_file))
                 continue;

              PrintSectionTitle(one_file.name, HEAD3, "");

	          //--------------------------------------------
              // Already described in another chapter ?
	          //--------------------------------------------
   	          if (IsPrinted(one_file.id, pac_ref, PRINTED_FILES))
  	             continue;

              PrintObjectDesc(one_file.id, BODY_NORMAL);

              PrintDiagram(one_file, e_OPTIMIZED_PRINTING, e_PRINT_SETTING 
	                     , e_NO_DISPLAYMARKS, BODY_NORMAL, CAPTION 
				         , diag_source, rep_diag); 

              //-------------------------------------          
              // Collect Glossary items if demanded
              //-------------------------------------
              if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
                 GlossaryForFile(one_file.id);
          }
          set_clear(cls_files);
          set_clear(pac_files);
       }
   set_clear(packages);

   return pac_number; 
}


//*****************************************************************************
// ClassReportChapter: generates the chapter for the detailed class report 
//*****************************************************************************
int  ClassReportChapter()
{
   boolean  comma;

   file     class_tab;

   int      ccnt, cx,
            dcnt, dx,
            class_number;

   node     assoc_class,
            one_class;

   set      classes,    
            file_set,
            link_set,
            node_set,
            printed;

   string   cls_items,
            cls_text,
            int_files;         
     
   //===================================================================
   // Print the chapter's headline and the introduction
   //===================================================================
   PrintChapterTitle(class_heading, HEAD1, 1, class_intro, True);

   //=================================================================== 
   // Are some classes left to report about ? (evaluate restrictions)
   //=================================================================== 
   // Find the matching classes for the restrictions in e_CLASSES that
   // are part of a packes matching e_PACKAGES
   //==================================================================
   classes = set_create("node");

   if   (e_PACKAGES == ANY)
        classes = MatchingNodesOfTypeSet(m_CLASS_NODE, e_CLASSES);
   else classes = MatchingNodeContainedInPackageSet(m_CLASS_NODE, e_CLASSES
                                                  , e_PACKAGES);
   ccnt    = set_count(classes);

   //===================================
   // Search and print the class report 
   //===================================
   if   (ccnt < 1)
   {
        PrintSectionTitle(no_class, HEAD4, BOLD12);
        class_number = 0; // nothing found
   }
   else    
   { 
        class_number = 1; // classes found
		int_files    = "( " + m_SEQUENCE_FILE + " || " 
                     + m_COLLABORATION_FILE   + ")";
        cls_items    = m_CL_VISIBILITY_ITEM   + " || " 
                     + m_CL_PARAMETERS_ITEM   + " || "
                     + m_CL_IS_ABSTRACT_ITEM  + " || "
                     + m_CL_IS_IMPORTED_ITEM  + " || "
                     + m_ENC_SCOPE_ITEM       + " || " + m_MULTIPLICITY_ITEM;
        printed      = set_create("PRT_IN");

        //------------------------------------------------
        // Print the description of each class
        //------------------------------------------------
        for (cx = 0; cx < ccnt; cx = cx + 1)
        {
            one_class = set_get_element(classes, cx);        
            message(rep_class + one_class.name);

            if   (  (cx > 0) 
                 && (e_CL_ATT_OP || e_CL_INTERACTION || e_CL_BEHAVIOR))
                 PrintChapterTitle(class_name + one_class.name, HEAD2, 1
                                 , "" , False);
            else PrintChapterTitle(class_name + one_class.name, HEAD2, 0
                                 , "" , False);

            PrintAnyDesc(one_class.id, m_CLASS_DEF_NOTE, BODY_NORMAL, 0);


            if (e_GENERATION_INFO)
            {
               //---------------------------
               // Generation information
               //---------------------------
               PrintGenInfo(one_class.id);
            }

            if (e_CLASS_DETAILS)
            {
               //---------------------------
               // Miscellaneous information
               //---------------------------

               if (GetNumberOfItems(one_class.id, cls_items) > 0)
               {
                  PrintSectionTitle(miscel, HEAD4, BOLD10);
                  paragraph(BODY_NORMAL);  

                  comma = False;
                  comma = PrintBooleanItem(one_class.id, m_CL_IS_IMPORTED_ITEM
                        , is_imported, comma);
                  comma = (PrintBooleanItem(one_class.id, m_CL_IS_ABSTRACT_ITEM
                        , is_abstract, comma) || comma);
                  comma = (PrintValueItem(one_class.id, m_CL_VISIBILITY_ITEM
                        , is_visible, comma) || comma);

                  if (comma)
                  {
                     paragraph(BODY_NORMAL); 
                     comma = False;
                  }
 
                  comma = (PrintValueItem(one_class.id, m_ENC_SCOPE_ITEM
                        , is_of_scope, comma) || comma);
                  comma = (PrintValueItem(one_class.id, m_MULTIPLICITY_ITEM
                        , is_multi, comma) || comma);
 
                  if (comma)
                  {
                     paragraph(BODY_NORMAL); 
                     comma = False;
                  }
 
                  comma = (PrintValueItem(one_class.id, m_CL_PARAMETERS_ITEM
                        , is_parameter, comma) || comma);
               }

		       //------------------------
		       // Class extensibility ...
		       //------------------------
		       PrintExtInfo(one_class.id);

 		       //------------------------
 		       // Class is part of ...
		       //------------------------
		       PrintObjectsLinks(one_class, m_PACKAGE_NODE, "to"
                               , m_CONTAINS_LINK, "out", is_part_of);

               //-----------------------------------------
               // Superclass (generalisation) information
               //-----------------------------------------
		       PrintObjectsLinks(one_class, m_CLASS_NODE, "to" 
					           , m_GENERALIZATION_LINK, "out", superobject);
    
               //-----------------------
               // Subclass information
               //-----------------------
			   PrintObjectsLinks(one_class, m_CLASS_NODE, "from" 
					           , m_GENERALIZATION_LINK, "in", subobject);

               //------------------------------------------------------------
               // Aggregation information 
               //------------------------------------------------------------
//		       PrintObjectsLinks(one_class, m_CLASS_NODE, "to" 
//					           , m_AGGREGATION_LINK, "out", agg_comp);
 
               //-----------------------
               // Partclass information
               //-----------------------
//			   PrintObjectsLinks(one_class, m_CLASS_NODE, "from" 
//					           , m_AGGREGATION_LINK, "in", agg_part);

		       //------------------------
		       // Class references ...
		       //------------------------
		       PrintRefInfo(one_class.id, one_class.name, "node", 0);
            }


             //---------------------------
             // Attributes and Operations
             //---------------------------
             if (e_CL_ATT_OP)
             {
                if   (e_IN_CL_TABLE_FORM)
                     class_tab = FileOfTypeWithNodeRefOfId(m_CLASS_TABLE
                                                         , one_class.id);                 
                else class_tab = NULL;
    
                //-----------------------
                // Attribute information
                //-----------------------
                node_set = set_create("node");
                node_set = ScopedNodesOfTypeSet(one_class.id
                                               , m_ATTRIBUTE_NODE);
 
                if   (set_count(node_set) > 0) 
                {
                     PrintSectionTitle(att_title, HEAD4, BOLD12);
                     PrintAttributes(node_set, class_tab);
                }

                set_clear(node_set);
    
                //-----------------------
                // Operation information
                //-----------------------
                node_set = set_create("node");
                node_set = ScopedNodesOfTypeSet(one_class.id
                                              , m_OPERATION_NODE);
 
                if   (set_count(node_set) > 0) 
                {
                     PrintSectionTitle(class_op, HEAD4, BOLD12);
                     PrintOperations(node_set, class_tab);
                }

                set_clear(node_set);
            }
     
			cls_text = cross_ref + "<" + class_heading + ", " 
                     + class_name + one_class.name;

            if (e_CL_INTERACTION)
            {
				//---------------------------------------------------
				// Print Interaction diagrams connected to the Class
				//---------------------------------------------------
                file_set = set_create("file");
		        file_set = FilesOfTypeWithNodesOfTypeAndNameSet(int_files
                                , m_OBJECT_CLASS_NODE 
                                , " name $ '" + one_class.name + "'");

                if (set_count(file_set) > 0)
                   PrintScenarioDiagrams(file_set, cls_text, printed
                                       , e_CLASS_DETAILS);
                set_clear(file_set);
            }

            if (e_CL_BEHAVIOR)
            {
				//----------------------------------------------------------
				// Print activity diagrams connected to the Class
				//----------------------------------------------------------
                file_set = set_create("file");
		        file_set = FilesOfTypeWithNodesOfTypeAndNameSet
                                 (m_ACTIVITY_FILE, m_STATE_MACHINE_NODE 
                                , "( name $ '" + one_class.name + "' || "
                                + "name $ '" + one_class.name + "::*')");

                if (set_count(file_set) > 0)
                   PrintActivityDiagrams(file_set, cls_text, e_CLASS_DETAILS);
                set_clear(file_set);

				//----------------------------------------------------------
				// Print state diagrams connected to the Class
				//----------------------------------------------------------
                file_set = set_create("file");
		        file_set = FilesOfTypeWithNodesOfTypeAndNameSet
                                 (m_STATE_DIAGRAM, m_STATE_MACHINE_NODE 
                                , " name $ '" + one_class.name + "'");

                if (set_count(file_set) > 0)
                   PrintStateDiagrams(file_set, cls_text);
                set_clear(file_set);
            }


            //-------------------------------------          
            // Collect Glossary items if demanded
            //-------------------------------------
            if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
               GlossaryForObject(one_class.id);

        }
        set_clear(printed);

        if (e_GEN_ASSOC_SUM_TBL)
        {
           //-------------------------------------
           // Print the Association Summary Table
           //-------------------------------------
           PrintAssocSummaryTable(classes);
        }
   } 
   set_clear(classes);

   return class_number;      
}


//*****************************************************************************
// StereotypeChapter: generates the chapter with the stereotype definition  
//                    and usage 
//*****************************************************************************
int StereotypeChapter()
{
    cntx    one_cntx;

    int     scnt, sx,
            ucnt, ux,
            stt_number;

    file    st_diag;

    item    i_st_type;

    link    one_link;

    node    n_st_type,
            one_node;

    set     st_types,
            c_used_in,
            l_used_in,
            n_used_in;


   //====================================================
   // Print the chapter's headline and the introduction
   //====================================================
   PrintChapterTitle(stt_heading, HEAD1, 1, stt_intro, True);

   stt_number = 0;

   //==========================================
   // Print stereotype diagrams, if demanded 
   //==========================================

   if (e_STEREOTYPE_FILES)
   {
       st_types = set_create("file");
       st_types = FilesOfTypeWithNodesOfTypeAndNameSet(m_STEREOTYPE_FILE
	                                                 , m_STEREOTYPE_NODE
                                                     , e_STEREOTYPES);
       scnt     = set_count(st_types);

       if (scnt > 0)
          stt_number = 1; // diagrams found

       for (sx = 0; sx < scnt; sx = sx + 1)
       {
            st_diag = set_get_element(st_types, sx);
            PrintChapterTitle(stt_heading + print_in + st_diag.name, HEAD2
                            , 0, "", False);

            PrintObjectDesc(st_diag.id, BODY_NORMAL);

            PrintDiagram(st_diag, e_OPTIMIZED_PRINTING, e_PRINT_SETTING 
	                   , e_NO_DISPLAYMARKS, BODY_NORMAL, CAPTION 
				       , diag_source, rep_diag); 
         
            //-------------------------------------          
            // Collect Glossary items if demanded
            //-------------------------------------
            if (e_GEN_GLOSSARY && e_REFD_GLOSSARY)
               GlossaryForFile(st_diag.id);
       }
       set_clear(st_types);
   }


   //==============================================
   // Search the stereotypes and print their usage 
   //==============================================
   st_types = set_create("node");
   st_types = MatchingNodesOfTypeSet(m_STEREOTYPE_NODE, e_STEREOTYPES);
   scnt     = set_count(st_types);

   if ((scnt < 1) && (stt_number == 0))
      PrintSectionTitle(no_stt, HEAD4, BOLD12);

   for (sx = 0; sx < scnt; sx = sx + 1)
   {
       n_st_type = set_get_element(st_types, sx);
       PrintChapterTitle(stereo_name + n_st_type.name, HEAD2, 0, "", False);

       PrintObjectDesc(n_st_type.id, BODY_NORMAL);

	   //--------------------------------
	   // Stereotype is generalisation of ...
	   //--------------------------------
	   PrintObjectsLinks(n_st_type, m_STEREOTYPE_NODE, "to" 
					   , m_STEREOTYPE_INH_LINK, "out", superobject);

	   //--------------------------------
	   // Stereotype inherits from ...
	   //--------------------------------
	   PrintObjectsLinks(n_st_type, m_STEREOTYPE_NODE, "from" 
					   , m_STEREOTYPE_INH_LINK, "in", subobject);

	   //--------------------------------
	   // Stereotype is used in ...
	   //--------------------------------
       n_used_in = set_create("node");
       n_used_in = ObjectsWithItemOfTypeAndValueSet("node", m_STEREOTYPE_ITEM
                                                  , n_st_type.name);
       l_used_in = set_create("link");
       l_used_in = ObjectsWithItemOfTypeAndValueSet("link", m_STEREOTYPE_ITEM
                                                  , n_st_type.name);
       c_used_in = set_create("cntx");
       c_used_in = ObjectsWithItemOfTypeAndValueSet("cntx", m_STEREOTYPE_ITEM
                                                  , n_st_type.name);

       if ((set_count(n_used_in) + set_count(l_used_in) 
                                 + set_count(c_used_in)) > 0)
          PrintSectionTitle(n_st_type.name + uc_is_used, HEAD4, BOLD10);

       ucnt = (set_count(n_used_in));
       for (ux = 0; ux < ucnt; ux = ux + 1)
       {
           paragraph(BULLET);
           one_node = set_get_element(n_used_in, ux);
           print(one_node.type + " : " + one_node.name);
       }
       set_clear(n_used_in);

       ucnt = (set_count(l_used_in));
       for (ux = 0; ux < ucnt; ux = ux + 1)
       {
           paragraph(BULLET);
           one_link = set_get_element(l_used_in, ux);
           print(one_link.type + " : " + one_link.name);
       }
       set_clear(l_used_in);

       ucnt = (set_count(c_used_in));
       for (ux = 0; ux < ucnt; ux = ux + 1)
       {
           paragraph(BULLET);
           one_cntx = set_get_element(c_used_in, ux);
           print(one_cntx.type + " : " + one_cntx.name);
       }
       set_clear(c_used_in);

   }
   set_clear(st_types);

   return stt_number; 
}

