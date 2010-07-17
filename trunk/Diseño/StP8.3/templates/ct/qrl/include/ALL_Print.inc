//*****************************************************************************
// DESCRIPTION: ALL_Print.inc
//              ----------------------
//              (c) 1997 by AONIX 
//*****************************************************************************
//
// Include-File for all reports with the functions for printing text and  
// with optimized diagram printing.
//*****************************************************************************
//
// functions: - int       PrettyPrint(string the_string, string the_para)
//            - void      PrintAllItems(int oid, string the_item, string
//                                      the_header, string p_format, 
//                                      string c_format)
//            - void      PrintAllNotesDesc(int oid, string the_note, string
//                                          the_header, string if_not)
//            - void      PrintAnyDesc(int oid, string desc_print, 
//                                     string the_para, int RET)
//            - boolean   PrintBooleanItem(int oid, string the_item, 
//                                         string the_title, boolean comma)
//            - void      PrintBulletList(string the_head, list the_list)
//            - void      PrintBulletSet(string the_head, set the_set)
//            - void      PrintChapterTitle(string the_title, string the_para,
//                                          int new_page, string the_intro, 
//                                          boolean in_head)
//            - void      PrintGenInfo(int obj_id)
//            - int       PrintNoteDesc(int oid, string the_note, 
//                                      string the_para)
//            - void      PrintObjectDesc(int id, string pr_para)
//            - void      PrintRefInfo(int obj_id, string obj_name, string 
//                                     pdm_obj, int c_file_id)
//            - void      PrintSectionTitle(string the_title, string the_para,
//                                          string the_format)
//            - int       PrintTheItem(int oid, string an_item,   
//                                     string the_title, string p_format,
//                                     string t_format, string v_format)
//            - boolean   PrintValueItem(int oid, string the_item, 
//                                       string the_title, boolean comma)
//*****************************************************************************



//*****************************************************************************
// PrintObjectDesc: This procedure searches the textual description of the 
//                  generic object annotation OBJECT, which is defined within 
//                  the OAE, and prints eather this note's description (if 
//                  there is one) or a predefined dummystring for the missing
//                  description, if with_dummy_comment is true.
//                  - oid:      object_id of the annotated object
//                  - the_para: paragraph format
//*****************************************************************************
void PrintObjectDesc(int oid, string the_para)
{
   int    RET;
   note   one_note;

   if (!e_GEN_DESCRIPTIONS)
      return;
   
   one_note = NoteOfTypeWithObjId(lang_obj, oid); 

   if   (one_note != NULL) 
   {
        RET = PrettyPrint(one_note.desc, the_para);
     	if ((RET == 0) && e_MISSING_DESCRIPTION)
           PrettyPrint(no_desc, the_para);	
   } 
   else if (e_MISSING_DESCRIPTION)
           PrettyPrint(no_desc, the_para);	

   return;
}


//*****************************************************************************
// PrintAnyDesc: This procedure searches the textual descriptions of the 
//               notes OBJECT and XYZ-Definition and prints eather these 
//               note's descriptions (if there are some) or a predefined 
//               dummystring for the missing description, if external
//               e_MISSING_DESCRIPTION is true .
//               - oid:        object_id of the annotated object
//               - desc_print: string with a list of notes to evaluate
//               - the_para :  paragraph format
//               - RET:        number of descriptions already printed
//*****************************************************************************
void PrintAnyDesc(int oid, string desc_print, string the_para, int RET)
{
   int    icnt, ix;
   list   desc_list = string_to_list(desc_print, " ");
   string one_desc;

   if (!e_GEN_DESCRIPTIONS)
      return;

   icnt = list_count(desc_list);

   for (ix = 0 ; ix < icnt; ix = ix + 1)
   {
       one_desc = list_get(desc_list, ix); 
       RET = RET + PrintNoteDesc(oid, one_desc, the_para);
   }

   if   (RET == 0)
        PrintObjectDesc(oid, the_para);
   else 
   {
        PrintNoteDesc(oid, lang_obj, the_para);
   }

   return;
}


//*****************************************************************************
// PrintAllItems: This procedure searches the values of the given item
//                and prints them eather in form of a bulleted list or
//                prints the given dummystring for the missing description.
//                - oid        : object_id of the annotated object
//                - the_item   : application type of the item
//                - the_header : Heading of the section
//                - p_format   : paragraph format of the title
//                - c_format   : char format of the title
//*****************************************************************************
void PrintAllItems(int oid, string the_item, string the_header,
                   string p_format, string c_format)
{
   int    scnt, sx;
   item   info_item;
   set    info_set;
   string the_info;


   info_set = set_create("item");
   info_set = ItemsOfTypeWithObjIdSet(the_item, oid);
   scnt     = set_count(info_set);

   if (scnt > 0)
   {
      PrintSectionTitle(the_header, p_format, c_format);

      for (sx = 0; sx < scnt; sx = sx + 1)
      {
          info_item = set_get_element(info_set, sx);
          PrettyPrint(info_item.value, BULLET);
      }
   }

   set_clear(info_set);
   return;
}


//*****************************************************************************
// PrintNoteDesc: This procedure searches the textual description of the  
//                given note, and prints only existing text.
//                - oid        : object_id of the annotated object
//                - the_note   : application type of the note
//                - the_para   : paragraph format
//*****************************************************************************
int  PrintNoteDesc(int oid, string the_note, string the_para)
{
   int    RET = 0;
   note   desc_note;

   desc_note = NoteOfTypeWithObjId(the_note, oid);
   if (desc_note != NULL) 
      RET = PrettyPrint(desc_note.desc, the_para);

   return RET;
}


//*****************************************************************************
// PrintAllNotesDesc: This procedure searches the textual descriptions of the 
//                    given note, and prints them eather in form of a bulleted
//                    list or the given dummystring for the missing descr.
//                    - oid        : object_id of the annotated object
//                    - the_note   : application type of the note
//                    - the_header : Heading of the section
//                    - if_not     : dummystring, if nothing is found
//*****************************************************************************
void PrintAllNotesDesc(int oid, string the_note, string the_header, 
                       string if_not)
{
   int    scnt, sx;
   note   desc_note;
   set    desc_set;
   string the_desc;

   PrintSectionTitle(the_header, BODY, BOLDSLIM);

   desc_set = set_create("note");
   desc_set = NotesOfTypeWithObjIdSet(the_note, oid);
   scnt     = set_count(desc_set);

   if   (scnt == 0)
   {
        character_format_set(STANDARD);
        print(if_not);
   }
   else
        for (sx = 0; sx < scnt; sx = sx + 1)
        {
            desc_note = set_get_element(desc_set, sx);
            PrettyPrint(desc_note.desc, BULLET);
        }
   set_clear(desc_set);
   return;
}


//*****************************************************************************
// PrintValueItem: This procedure searches the  value of the given item and 
//                 prints title and value.
//                 - oid       : object_id of the annotated object
//                 - the_item  : application type of the item
//                 - the_title : title of the item
//                 - comma     : print a comma ?
//*****************************************************************************
boolean  PrintValueItem(int oid, string the_item, string the_title, 
                        boolean comma)
{
   item  info;

   string striped_info;


   info = ItemOfTypeWithObjId(the_item, oid);

   if (info != NULL) 
   {
      striped_info = StripInfo(info.value);

      if (string_length(striped_info) < 1)
         return False;

      character_format_set(STANDARD);

      if (comma)
         print(", ");

      print(the_title);

      character_format_set(ITALIC);
      print(striped_info);

      return True;
   }

   return False;
}


//*****************************************************************************
// PrintBooleanItem: This procedure searches the boolean value of the given  
//                   item and prints title and yes or no.
//                   - oid       : object_id of the annotated object
//                   - the_item  : application type of the item
//                   - the_title : title of the item
//                   - comma     : print a comma ?
//*****************************************************************************
boolean  PrintBooleanItem(int oid, string the_item, string the_title, 
                          boolean comma)
{
   item   info;


   info = ItemOfTypeWithObjId(the_item, oid);

   if (info != NULL)
   {
      character_format_set(STANDARD);

      if (comma)
         print(", ");

      print(the_title);

      character_format_set(ITALIC);

      if   (info.value == "True")
           print(print_yes);
      else print(print_no);

      return True;
   }

   return False;
}


//*****************************************************************************
// PrintTheItem: This procedure searches the value of the given item, strips
//               it and prints it with its name, if its value is not empty.
//               - oid       : object_id of the annotated object
//               - an_item   : application type of the item
//               - the_title : Title of the item
//               - p_format   : paragraph format
//               - t_format   : char format of the title
//               - v_format   : char format of the item value
//*****************************************************************************
int  PrintTheItem(int oid, string an_item, string the_title, 
                  string p_format, string t_format, string v_format)
{
   int    RET = 0;
   item   info;
   string striped_info;

   info = ItemOfTypeWithObjId(an_item, oid);

   if (info != NULL) 
   {
      striped_info = StripInfo(info.value);
      RET = string_length(striped_info);

      if (RET > 0)
      {
         if (p_format != "")
            paragraph(p_format);
         character_format_set(t_format);
         print(the_title);
         character_format_set(v_format);
         print(striped_info);
      }
   }
   return RET;
}


//*****************************************************************************
// PrettyPrint: Calls StripInfo for the given description string and prints 
//              it in the given format, if some characters are left after the
//              stripping. 
//              If the graphic import tag is found, PrintPicture is called
//              instead of the normal print function.
//              PrettyPrint returns the length of the printed string or 0,
//              if no printable characters were left after the stripping.
//              - the_string : string containing the note description
//              - the_para   : paragraph format
//*****************************************************************************
int  PrettyPrint(string the_string, string the_para)
{
   int  RET;

   the_string = StripInfo(the_string);
   RET        = string_length(the_string);

   if (RET > 0)
   {
      paragraph(the_para);
      if   (string_find(the_string, 0, PIC_TAG) < RET)
           PrintPicture(RET, the_string);
      else print(the_string);
   }
   return RET; 
}


//*****************************************************************************
// PrintBulletList: This procedure prints the given list in bulleted form,
//                  headed by the given header.
//                  (by transforming it in a set and using PrintBulletSet)
//                  - the_header : Heading of the section
//                  - the_list   : the list to be printed
//*****************************************************************************
void PrintBulletList(string the_head, list print_list)
{
   int   icnt; 
   set   print_set;

   print_set = set_create(list_get_type(the_list, 0));
   icnt      = ListToSet(print_list, print_set);
    
   PrintBulletSet(the_head, print_set);

   set_clear(print_set);

   return;
}


//*****************************************************************************
// PrintBulletSet: This procedure prints the given set in bulleted form,
//                 headed by the given header.
//                 - the_header : Heading of the section
//                 - the_set    : the set to be printed
//*****************************************************************************
void PrintBulletSet(string the_head, set the_set)
{
   int     icnt, ix; 
   string  the_type;

   file    one_file;
   link    one_link;
   node    one_node;

   icnt = set_count(the_set);
    
   PrintSectionTitle(the_head, HEAD4, BOLD10);

   the_type = set_get_type(the_set, 0);

   if (the_type == "file")
      for ( ix = 0; ix < icnt; ix = ix + 1) 
      {       
          paragraph(BULLET);
          one_file = set_get_element(the_set, ix);
          print(one_file.type + ": " + one_file.name);
      }

   if (the_type == "link")
      for ( ix = 0; ix < icnt; ix = ix + 1) 
      {       
          paragraph(BULLET);
          one_link = set_get_element(the_set, ix);
          print(one_link.name);
      }

   if (the_type == "node")
      for ( ix = 0; ix < icnt; ix = ix + 1) 
      {       
          paragraph(BULLET);
          one_node = set_get_element(the_set, ix);
          print(one_node.name);
      }

   if (the_type == "note")
      for ( ix = 0; ix < icnt; ix = ix + 1) 
      {       
          paragraph(BULLET);
          one_node = set_get_element(the_set, ix);
          print(one_note.name);
      }

   if (the_type == "item")
      for ( ix = 0; ix < icnt; ix = ix + 1) 
      {       
          paragraph(BULLET);
          one_node = set_get_element(the_set, ix);
          print(one_node.value);
      }

   if (the_type == "string")
      for ( ix = 0; ix < icnt; ix = ix + 1) 
      {       
          paragraph(BULLET);
          one_node = set_get_element(the_set, ix);
          print(one_node);
      }

   return;
}


//*****************************************************************************
// PrintSectionTitle: This procedure prints the title of a new section, 
//                    using the given paragraph and format.
//                    - the_title  : Heading of the section
//                    - the_para   : paragraph of the title
//                    - the_format : character format of the title
//*****************************************************************************
void  PrintSectionTitle(string the_title, string the_para, string the_format)
{
   if (the_title != "")
   {
      if (the_para != "")
         paragraph(the_para);
      if (the_format != "")
         character_format_set(the_format);
      print(the_title);   
   }
   return;
}


//*****************************************************************************
// PrintChapterTitle: This procedure prints the title of a new chapter, 
//                      using the given paragraph, 
//                      starting on a new page if demanded, 
//                      followed by an introducing text if gen_phase_titles.
//                    - the_title : Heading of the section
//                    - the_para  : paragraph of the title (default HEAD1)
//                    - new_page  : start a new page if != 0
//                    - the_intro : introducing text or empty string
//                    - in_head   : print tag for rtf header field
//*****************************************************************************
void  PrintChapterTitle(string the_title, string the_para, int new_page,
                          string the_intro, boolean in_head)
{
   if   (the_para == "")
        the_para = HEAD1;

   if   (new_page == 0)
        paragraph(the_para);    
   else page(the_para);

   if ((target() == RTF) && in_head) 
      character_format_set(INHEAD);

   print(the_title); 
 
   if ((target() == RTF) && in_head) 
      character_format_set(STANDARD);

   if (e_GEN_INTRODUCTIONS && (the_intro != ""))
   {
      paragraph(BODY);
      print(the_intro);
   }
   return;
}


//*****************************************************************************
// PrintRefInfo: Lists all diagrams and tables with references of the  given
//               object.
//               - obj_id    : the current object큦 id
//               - obj_name  : the current object큦 name
//               - pdm_obj   : object큦 pdm type (node, link, cntx)
//               - c_file_id : id of the current file
//*****************************************************************************
void  PrintRefInfo(int obj_id, string obj_name, string pdm_obj, int c_file_id)
{
   set file_set;

   file_set = set_create("file");
   file_set = FilesWithRefsOfObjIdSet(obj_id, pdm_obj, c_file_id);

   if (set_count(file_set) > 0) 
      PrintBulletSet(obj_name + obj_refs, file_set);

   set_clear(file_set);  
}


//*****************************************************************************
// PrintGenInfo: Prints generation info (author, date and time) of the  given
//               object.
//               - obj_id    : the current object큦 id
//*****************************************************************************
void  PrintGenInfo(int obj_id)
{
   int      gen_len,
            int_date;

   item     gen_time;

   string   obj_time,
            obj_date,
            obj_items;


   obj_items = m_AUTHOR + " || " + m_GENERATED;

   if (GetNumberOfItems(obj_id, obj_items) > 0)
   {
      PrintSectionTitle(is_generated, BODY_NORMAL, ITALIC);

      gen_time = ItemOfTypeWithObjId(m_AUTHOR, obj_id);
      gen_len  = string_length(gen_time.value);

      if ((gen_time != NULL) && (gen_len > 0)) 
         print(gen_author + gen_time.value);

      gen_time = ItemOfTypeWithObjId(m_GENERATED, obj_id);
      gen_len  = string_length(gen_time.value);

      if ((gen_time != NULL) && (gen_len > 0)) 
      {
         obj_time = string_extract(gen_time.value, 0, 8);
         obj_date = StripInfo(string_strip(gen_time.value, "L", obj_time));
         int_date = string_to_time(obj_date, NULL);
         print(time_to_string(int_date, gen_date));
         int_date = string_to_time(obj_time, NULL);
         print(time_to_string(int_date, time_format));
      }

   }

   return;
}
