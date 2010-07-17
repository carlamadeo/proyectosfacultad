//*****************************************************************************
// DESCRIPTION: UML_Strings.inc
//              ----------------------
//              (c) 1997 by AONIX 
//*****************************************************************************
//
// Include file for UML-Reports with the definitions of messages and text-  
// strings for the generated document in english, german and french language.
// Francois Xavier David, Aonix France provided the french translation.
//*****************************************************************************
//
// functions: - int   SetTargetLanguage(string def_lang),
//            - void  SetUILanguage(string rep_lang).
//*****************************************************************************

//----------------------------
// Global Textvariables 
//----------------------------
string act_diag;
string act_name;
string act_obj;
string act_state;
string agg_comp;
string agg_part;
string ano_obj;
string ass_class;
string ass_label;
string ass_start;
string assoc_heading;
string assoc_intro;
string async_msg;
string att_title;

string balk_msg;
string between;

string class_heading;
string class_intro;
string class_name;
string class_op;
string class_qual;
string class_role;
string cls_diag;
string cls_start;
string col_diag;
string col_name;
string constraint;
string constraints;
string consists_of;
string comp_diag;
string cover_msg;
string cross_ref;
string ctx_title;

string date_format;
string defined_in;
string depl_diag;
string diag_name;
string diag_source;
string dyn_diag;

string evt_name;
string ext_subtitle;
string ext_sys;

string final_msg;
string final_state;

// ECR 7215
string junction_point;

string gen_author;
string gen_date;
string gls_heading;
string gls_intro;
string gls_start;

string hist_state;

string info_date;
string info_dsource;
string info_heading;
string info_intro;
string info_msg;
string info_pages;
string info_ssource;
string info_status;
string info_stpsys;
string info_title;
string info_version;
string int_diag;
string interfaces;
string is_abstract;
string is_author;
string is_depending;
string is_derived;
string is_generated;
string is_imported;
string is_meta_att;
string is_meta_op;
string is_multi;
string is_of_default;
string is_of_scope;
string is_of_type;
string is_performed;
string is_status;
string is_parameter;
string is_part_of;
string is_throws;
string is_visible;
string is_version;

string lang_msg;
string lang_obj;      
string lsa_heading;
string lsa_intro;
string lsa_start;

string m_no_ass;
string m_no_chap;
string m_no_cls;
string m_no_glsd;
string m_no_glsr;
string m_no_lang;
string m_no_lsa;
string m_no_pac;
string m_no_pack;
string m_no_psa;
string m_no_req;
string m_no_stt;
string m_no_sarc;
string m_no_targ;
string m_no_uca;
string miscel;
string missing_att;
string missing_op;
string module_bin;
string module_ex;
string module_if;
string module_src;
string msg_name;

string no_assoc;
string no_class;
string no_ctab;
string no_ctx;
string no_desc;
string no_glsd;
string no_glsr;
string no_lsa;
string no_pac;
string no_psa;
string no_req;
string no_stt;
string no_sarc;
string no_uca;
string not_def;
string not_named;

string obj_name;
string obj_refs;
string of_the_class;

string pac_diag;
string pac_heading;
string pac_intro;
string pac_name;
string pac_start;
string performs;
string print_and;
string print_empty;
string print_from;
string print_in;
string print_no;
string print_pages;
string print_the;
string print_to;
string print_yes;
string psa_heading;
string psa_intro;
string psa_start;

string rep_class;
string rep_diag;
string rep_pack;
string rep_table;
string rep_uca;
string req_heading;
string req_intro;
string req_intro1;
string req_intro2;
string req_start;
string reqtab_title;
string ret_msg;

string sarc_heading;
string sarc_intro;
string sarc_start;
string seq_diag;
string start_msg;
string start_state;
string stereo_name;
string state_machine;
string std_state;
string stt_heading;
string stt_intro;
string stt_start;
string subobject;
string superobject;
string sync_msg;
string sys_name;

string tab_assoc;
string tab_classA;
string tab_classB;
string tab_desc;
string tab_multA;
string tab_multB;
string tab_role;
string time_format;
string toc_name;
string tout_msg;
string tval_name;
string type_name;

string uc_is_ext;
string uc_is_used;
string uc_extends;
string uc_uses;
string uca_except;
string uca_intro;
string uca_heading;
string uca_name;
string uca_postcond;
string uca_precond;
string uca_start;
string uca_title;
string unit_name;


//*****************************************************************************
// SetUILanguage: Assigns the reportlanguage of the OO-Report (defined by
//                    the language of the master script
//                    rep_lang - all messages are printed in this language
//*****************************************************************************

void SetUILanguage(string rep_lang)
{    
    string targetl;

    //=======================================================================
    // If the language is not defined, use the language of the master script
    //=======================================================================   

    if (rep_lang == "D")
    {
       if   (to_upper(e_DOCUMENT_LANGUAGE) == "E")
            targetl = " englischer Sprache ";
       else if   (to_upper(e_DOCUMENT_LANGUAGE) == "F")
                 targetl = " französischer Sprache ";
            else targetl = " deutscher Sprache ";

       start_msg  = "Der UML-Report wird vorbereitet .... ";
       lang_msg   = "\nDer Report wird als " + target() + "-Dokument wird in" 
                  + targetl + "generiert ...";
       final_msg  = "\nDie Reportgenerierung wurde erfolgreich beendet.";

       cover_msg  = "\nGeneriere die Titelseite ...";
       info_msg   = "\nGeneriere die Dokumentinformation ...";
       req_start  = "\nGeneriere das Kapitel mit den Anforderungen ...";
       uca_start  = "\nGeneriere das Kapitel mit der Anwendersicht ...";
       sarc_start = "\nGeneriere das Kapitel mit der Systemarchitektur ...";
       lsa_start  = "- Generiere die logische Systemarchitektur ...";
       psa_start  = "- Generiere die physische Systemarchitektur ...";
       pac_start  = "\nGeneriere das Kapitel mit der Paketübersicht ...";
       cls_start  = "\nGeneriere das Kapitel mit der Klassenübersicht ...";
       ass_start  = "- Generiere die Assoziations Tabelle ...";
       stt_start  = "\nGeneriere das Kapitel mit der Stereotypen " 
                  + "Definition ...";
       gls_start  = "\nGeneriere den Anhang mit dem Glossar ...";

       m_no_lang  = "Unbekannte Dokumentsprache. Das Dokument wird daher " 
                  + "in deutscher Sprache generiert !";
       m_no_chap  = "\nSie haben keine Kapitel für die Generierung " 
                  + "ausgewählt !";
       m_no_pack  = "\nDas System enthält keines der Pakete, auf die der " 
                  + "Report beschränkt werden soll !";
       m_no_targ  = "\nDieser Report kann nur Dokumente in Framemaker MIF " 
                  + "und RTF-Format generieren!";
       m_no_req   = "Es sind keine Anforderungen definiert !";
       m_no_uca   = "Es ist keine Anwendersicht definiert !";
       m_no_sarc  = "Es ist keine Systemarchitekur definiert !";
       m_no_lsa   = "  Es ist keine logische Systemarchitekur definiert !";
       m_no_psa   = "  Es ist keine physische Systemarchitekur definiert !";
       m_no_pac   = "Es sind keine Pakete definiert !";
       m_no_cls   = "Es sind keine Klassen definiert !";
       m_no_ass   = "  Es sind keine Assoziationen definiert !";
       m_no_stt   = "Es sind keine Stereotypen definiert !";
       m_no_glsd  = "Es sind keine Begriffe definiert !";
       m_no_glsr  = "Es sind keine Begriffe referenziert !";

       rep_uca    = "  - Auswertung des Use Case: ";
       rep_class  = "  - Auswertung der Klasse: ";
       rep_pack   = "  - Auswertung des Pakets: ";
       rep_diag   = "    - Auswertung des Diagramms: ";
       rep_table  = "    - Auswertung der Tabelle: ";

       return; 
    } 

    if (rep_lang == "E") 
    {
       if   (  (to_upper(e_DOCUMENT_LANGUAGE) == "G") 
            || (to_upper(e_DOCUMENT_LANGUAGE) == "D"))
            targetl = " german language ";
       else if   (to_upper(e_DOCUMENT_LANGUAGE) == "F")
                 targetl = " french language ";
            else targetl = " english language ";

       start_msg  = "Preparing the UML-Report ...";
       lang_msg   = "\nThe report is generated as " + target() 
                  + "-file in" + targetl + " ...";
       final_msg  = "\nReport generation completed.";

       cover_msg  = "\nGenerating the cover page ...";
       info_msg   = "\nGenerating the document information ...";
       req_start  = "\nGenerating the requirement chapter ...";
       uca_start  = "\nGenerating the users view chapter ...";
       sarc_start = "\nGenerating the system architecture ...";
       lsa_start  = "- Generating the logical system architecture ...";
       psa_start  = "- Generating the physical system architecture ...";
       pac_start  = "\nGenerating the package report ...";
       cls_start  = "\nGenerating the class report ...";
       ass_start  = "  Generating the association table ...";
       stt_start  = "\nGenerating the stereotype chapter ...";
       gls_start  = "\nStarting generation of the glossary appendix ...";

       m_no_lang  = "Undefined document language. Therefore the document "
                  + "is generated in english !";
       m_no_chap  = "\nYou did not choose any chapter for the generation !";
       m_no_pack  = "\nThe system does not contain any of the packages the " 
                  + "report is reduced to !";
       m_no_targ  = "\nThis report generates only Framemaker MIF and RTF"
                  + "target documents!";
       m_no_req   = "There are no requirements defined !";
       m_no_uca   = "There is no user's view defined !";
       m_no_sarc  = "There is no system architecture defined !";
       m_no_lsa   = "  There is no logical system architecture defined !";
       m_no_psa   = "  There is no physical system architecture defined !";
       m_no_pac   = "There are no packages defined !";
       m_no_cls   = "There are no classes defined !";
       m_no_ass   = "  There are no associations defined !";
       m_no_stt   = "There are no stereotypes defined !";
       m_no_glsd  = "There are no topics defined !";
       m_no_glsr  = "There are no topics referenced !";

       rep_uca    = "  - Reporting on the Use Case: ";
       rep_class  = "  - Reporting on the class: ";
       rep_pack   = "  - Reporting on the package: ";
       rep_diag   = "    - Reporting on the diagram: ";
       rep_table  = "    - Reporting on the table: ";

       return; 
    } 

    if (rep_lang == "F") 
    {
       if   (  (to_upper(e_DOCUMENT_LANGUAGE) == "G") 
            || (to_upper(e_DOCUMENT_LANGUAGE) == "D"))
            targetl = " Allemand ";
       else if   (to_upper(e_DOCUMENT_LANGUAGE) == "E")
                 targetl = " Anglaise ";
            else targetl = " Francais ";

       start_msg  = "Le UML-Rapport est en cours de génération ....";
       lang_msg   = "\nGénération demandée pour " + target() + " en langue"
                  + targetl + "....";
       final_msg  = "\nGénération du document terminée.";

       cover_msg  = "\nDémarrage génération de la page de titre ...";
       info_msg   = "\nDémarrage génération d'information du document ...";
       req_start  = "\nDémarrage génération des exigences ...";
       uca_start  = "\nDémarrage génération des uses cases ...";
       sarc_start = "\nDémarrage génération de l'architecture du système ...";
       lsa_start  = "- Démarrage génération de l'architecture logique ...";
       psa_start  = "- Démarrage génération de l'architecture physique ...";
       pac_start  = "\nDémarrage génération du rapport sur les packages ...";
       cls_start  = "\nDémarrage génération du rapport sur les classes ...";
       ass_start  = "  Démarrage génération de la table des associations ...";
       stt_start  = "\nDémarrage génération des stéréotypes ...";
       gls_start  = "\nDémarrage génération du glossaire ...";

       m_no_lang  = "La langue du document n'est pas définis. Génération "
                  + "en langue Française !";
       m_no_chap  = "\nVous n'avez pas choisi les chapitres pour la génération!";
       m_no_pack  = "\nAucun des packages indiques sont définis dans ce " 
                  + "système !";
       m_no_targ  = "\nCe report génères seulement Framemaker MIF et RTF "
                  + "documents!";
       m_no_req   = "Aucune exigence définis !";
       m_no_uca   = "Aucun use case définis !";
       m_no_sarc  = "Aucune architecture du système définis !";
       m_no_lsa   = "  Aucune architecture logique définis !";
       m_no_psa   = "  Aucune architecture physique définis !";
       m_no_pac   = "Aucun package défini !";
       m_no_cls   = "Aucune classe définie !";
       m_no_ass   = "  Aucune association définie !";
       m_no_stt   = "Aucun stéréotype défini !";
       m_no_glsd  = "Aucun item défini !";
       m_no_glsr  = "Aucun item référencé !";

       rep_uca    = "  - Evaluation du use case : ";
       rep_class  = "  - Evaluation de la classe : ";
       rep_pack   = "  - Evaluation du package: ";
       rep_diag   = "    - Evaluation du diagramme : ";
       rep_table  = "    - Evaluation du table: ";

       return; 
    }
    // If we reach this point, something definitely went wrong !!
    return; 
}


//*****************************************************************************
// SetTargetLanguage: Assigns the targetlanguage of the OO-Report (defined in
//                    in the ext. var. Document_Language (E), Dokument_Sprache 
//                    (D) or Language_de_Document (F)
//                    def_lang - if doc_lang is undefined, def_lang is used
//*****************************************************************************
int SetTargetLanguage(string def_lang)
{
    int status = IS_OK;
    
    //=======================================================================
    // If the language is not defined, use the language of the master script
    //=======================================================================
    e_DOCUMENT_LANGUAGE = to_upper(e_DOCUMENT_LANGUAGE);

    if (e_DOCUMENT_LANGUAGE == "G")    // map German to Deutsch
       e_DOCUMENT_LANGUAGE = "D";

    if (  (e_DOCUMENT_LANGUAGE != "D") 
       && (e_DOCUMENT_LANGUAGE != "E")  
       && (e_DOCUMENT_LANGUAGE != "F"))
    {
       e_DOCUMENT_LANGUAGE = def_lang;
       status             = NOT_OK;
    }

    if (e_DOCUMENT_LANGUAGE == "D") 
    {
       info_heading  = "Zum Inhalt dieses Dokuments";
       ext_subtitle  = "für <StP-System>";
       info_title    = "für ";
       info_version  = "Version:  \t";
       is_version    = "V1.0";
       info_date     = "Generiert am:  \t";
       info_status   = "Status:  \t";
       is_status     = "Analyse";
       is_author     = "Customization Team";
       info_dsource  = "Ablage:  \t";
       info_pages    = "Umfang:  \t";
       info_stpsys   = "StP-System:  \t";
       info_ssource  = "StP-Projekt:  \t";
       info_intro    = "Dieses Dokument enthält die Auswertung des StP-"
                     + "Systems \"" + current_system() + "\".\n"
                     + "Es wurde mit dem QRL-Skript \"UML-Report\" generiert "
                     + "und besteht aus folgenden Kapiteln:";

       toc_name   = "Inhalt";

       req_heading   = "Anforderungen"; 
       req_intro     = "In diesem Kapitel werden sämtliche, bis zu diesem " 
                     + "Zeitpunkt in Form von Tabelleneinträgen erfaßten " 
                     + "Anforderungen aufgelistet, die das System erfüllen " 
                     + "muß.\nWenn das System aus mehreren Paketen besteht, " 
                     + "so können die Anforderungen in eigene Tabellen für " 
                     + "jedes dieser Pakete unterteilt sein.\n";
       req_intro1    = "Im Folgenden werden diese Tabelle(n) ohne weitere " 
                     + "Angaben ausgegeben.";
       // ECR 6660 typo in string
       req_intro2    = "Im Folgenden wird der Inhalt dieser Tabelle(n) in Form" 
                     + " einer Gliederung ausgegeben, wobei bereits verfeiner" 
                     + "te Anforderungen kursiv dargestellt sind und noch " 
                     + "nicht erfüllte Anforderungen ggf. durch ein vorange" 
                     + "stelltes Fragezeichen \"?\" gekennzeichnet werden.";
       reqtab_title  = "Anforderungen an: ";
       no_req        = " - Es sind keine Anforderungen definiert - \n";

       uca_heading   = "Anwendersicht";
       uca_intro     = "In diesem Kapitel werden die einzelnen Anwendungs" 
                     + "fälle (Use Cases) des Systems aus der Sicht des " 
                     + "Benutzers beschrieben. Sofern vorhanden, werden für " 
                     + "jeden Use Case die zugehörigen Szenarien in Form von " 
					 + "Sequenz-, Kollaborations- und Aktivitäts-Diagrammen " 
                     + "mit ausgegeben.";
       uca_title     = "Use Cases in: ";
       act_name      = "Aktor: ";
	   performs      = " führt aus: ";
       uca_name      = "Use Case: ";
       uca_precond   = "Vorbedingungen: ";
       uca_postcond  = "Nachbedingungen: ";
       uca_except    = "Ausnahmen: ";
	   is_performed  = " wird ausgeführt von: ";
	   uc_is_ext     = " wird erweitert von: ";
	   uc_is_used    = " wird verwendet von: ";
	   uc_extends    = " erweitert: ";
	   uc_uses       = " verwendet: ";
       no_uca        = " - Es sind keine Use Cases definiert - \n";

       int_diag      = "Interaktionsdiagramm: ";
       col_name      = "Kollaboration: ";
       col_diag      = "Kollaborationsdiagramm: ";
       seq_diag      = "Sequenzdiagramm: ";
       type_name     = "Typ: ";
	   act_obj       = "Aktives ";
	   ano_obj       = "Anonymes ";
	   obj_name      = "Objekt ";
       of_the_class  = "der Klasse ";
       sync_msg      = "Synchrone(s) ";
       async_msg     = "Asynchrone(s) ";
       tout_msg      = "TimeOut ";
       balk_msg      = "Balking ";
       ret_msg       = "Return ";
       evt_name      = "Ereignis: ";
       msg_name      = "Nachricht: ";

       sarc_heading  = "Systemarchitektur";
       sarc_intro    = "In diesem Kapitel wird die Architektur des Systems \"" 
                     + current_system() + "\" beschrieben.";
       no_sarc       = " - Es wurde keine Information zur Architektur gefunden - ";
       lsa_heading   = "Logische Systemarchitektur";
       lsa_intro     = "Die logische Architektur des Systems \"" + current_system()
                     + "\" wird durch die folgende Hierarchie der enthaltenen "
                     + "Pakete beschrieben.";
       no_lsa        = " - Es wurden keine Pakete gefunden - ";
	   sys_name      = "System: ";
       psa_heading   = "Physische Systemarchitektur";
       psa_intro     = "Die physische Architektur des Systems wird durch "
                     + "Implementierungsdiagramme mit dem Zusammenspiel von Hard-"
                     + " und Software-Komponenten beschrieben.";
       no_psa        = " - Es wurden keine Implementierungsdiagramme gefunden - ";
       comp_diag     = "Moduldiagramm:";
       depl_diag     = "Prozeßdiagramm:";
       unit_name     = "Einheit: ";
       module_bin    = "Binary: ";
       module_ex     = "Executable: ";
       module_if     = "Schnittstelle: ";
       module_src    = "Source: ";

       pac_heading   = "Paketübersicht";
       pac_intro     = "In diesem Kapitel werden die Pakete des Systems " 
                     + "beschrieben.";
	   pac_name      = "Paket: ";
       pac_diag      = "Paketdiagramme:";
       cls_diag      = "Klassendiagramme:";
       no_pac        = " - Es sind keine Pakete definiert, Ausdruck vorhandener Klassen Diagramme - ";

       class_heading = "Klassenübersicht";
       class_intro   = "In diesem Kapitel werden die Klassen des Systems in " 
                     + "alphabetischer Reihenfolge beschrieben.";
       class_name    = "Klasse: ";
       no_class      = " - Es sind keine Klassen definiert -";
       no_ctab       = " - Zu dieser Klasse gibt es keine Klassentabelle - ";
       ass_class     = " ist eine \"Assoziationsklasse\" und beschreibt eine" +
                       " Beziehung";
       ass_label     = " namens ";
       between       = " zwischen den Klassen ";
       miscel        = "Allgemeine Informationen: ";
       is_derived    = "Abgeleitet: ";
       is_abstract   = "Abstrakt: ";
       is_imported   = "Importiert: ";
       is_parameter  = "Parameter: ";
       is_of_scope   = "Gültigkeitsbereich: ";
       is_multi      = "Multiplizität: ";
       constraint    = "Einschränkung: ";
       constraints   = "Einschränkungen: ";
       class_role    = "Rolle(n): ";
       class_qual    = "qualifiziert durch: ";
       interfaces    = "Schnittstellen: ";
       superobject   = " ist eine Generalisierung von: ";
       subobject     = " ist eine Spezialisierung von: ";
       agg_comp      = " besteht aus der Aggregation der Klasse(n): ";
       agg_part      = " ist Bestandteil der Aggregation der Klasse(n): ";
       att_title     = "Attribute:";
       missing_att   = "Die Tabelle enthält nicht alle Attribute !";
       is_of_type    = "Typ: ";
       is_visible    = "Visibility: ";
       is_meta_att   = "Metaattribut: ";
       is_of_default = "Vorbelegung: ";
       class_op      = "Operationen: ";
       missing_op    = "Die Tabelle enthält nicht alle Operationen !";
       is_meta_op    = "Metaoperation: ";
       is_throws     = "Throws: ";

       dyn_diag      = "Zustandsdiagramm: ";
       act_diag      = "Aktivitätsdiagramm: ";
       state_machine = "Zustandsmaschine: ";
       std_state     = "Zustand: ";
       act_state     = "Aktionszustand: ";
       hist_state    = "Historie: ";
       start_state   = "Startzustand: ";
       final_state   = "Endzustand: ";
       // ECR 7215
       junction_point= "Verzweigungspunkt: ";

       assoc_heading = "Assoziationstabelle";
       assoc_intro   = "In diesem Abschnitt werden die Assoziationen "
                     + "zwischen den im Report enthaltenen Klassen in " 
                     + "Tabellenform dargestellt.";
       no_assoc      = " - Es sind keine Assoziationen definiert -";
       tab_assoc     = "Assoziation";
       tab_classA    = "Klasse A";
       tab_classB    = "Klasse B";
       tab_desc      = "Beschreibung";
       tab_multA     = "Mult. A";
       tab_multB     = "Mult. B";
       tab_role      = "Rolle";
       not_named     = "unbenannt";

       stt_heading   = "Stereotypen"; 
	   stereo_name   = "Stereotyp: ";
       stt_intro     = "";
       no_stt        = " - Es sind keine Stereotypen definiert - \n";

       gls_heading   = "Glossar"; 
       gls_intro     = "Das Glossar enthält eine Sammlung systemspezifischer " +
                       "Begriffe mit den zugehörigen Erklärungen. Diese " +
                       "wurden in StP in der Note: Glossar zum angegebenen " +
                       "Definitionsobjekt erfaßt.\n";
       no_glsd       = " - Es sind keine Begriffe definiert - \n";
       no_glsr       = " - Es sind keine Begriffe referenziert - \n";

	   consists_of   = " enthält: ";
	   cross_ref     = "Beschrieben in ";
       date_format   = "%d. %B %Y";    //"%e. %B %Y";
       defined_in    = "definiert in: ";
       diag_name     = "Das Diagramm: ";
       diag_source   = "Quelle: ";
       gen_author    = "von ";
       gen_date      = " am %d. %B %Y";
	   is_depending  = " ist abhängig von: ";
       is_generated  = "Generiert ";
	   is_part_of    = " ist enthalten in: ";
       lang_obj      = "GenericObject"; 
       no_desc       = " - Die Erläuterung fehlt noch - ";
       not_def       = "- keine -";
       obj_refs      = " ist in folgenden Diagrammen und Tabellen enthalten: ";
       print_and     = " und ";
       print_empty   = " - die Tabelle ist leer - ";
       print_from    = " von ";
       print_in      = " in ";
       print_no      = "Nein";
       print_pages   = " Seiten";
       print_the     = "Das ";
       print_to      = " an ";
       print_yes     = "Ja";
       time_format   = " um %H:%M";
	   tval_name     = "Weitere Informationen: ";
    } 

    if (e_DOCUMENT_LANGUAGE == "E") 
    {
       info_heading  = "About this Document";
       ext_subtitle  = "for <StP-system>";
       info_title    = "for ";
       info_version  = "Version:  \t";
       is_version    = "V1.0";
       info_date     = "Date of generation:  \t";
       info_status   = "Status:  \t";
       is_status     = "Analysis";
       is_author     = "Customization Team";
       info_dsource  = "Document Base:  \t";
       info_pages    = "Pages:  \t";
       info_stpsys   = "StP-System:  \t";
       info_ssource  = "StP-Project:  \t";
       info_intro    = "This document describes the evaluation of the StP-" 
                     + "System \"" + current_system() + "\". \nIt was " 
                     + "generated by the QRL-Script \"UML-Report\" and " 
                     + "consists of the following chapters:";
       toc_name   = "Table of Contents";

       req_heading   = "Requirements";
       req_intro     = "This chapter shows a list of all requirements, the " 
                     + "system has to fulfill.\nIf the system consists of " 
                     + "more than one package, the requirements might be " 
                     + "split into several tables for those packages.";
       req_intro1    = "The requirement tables are presented without any " 
                     + "further information.";
       req_intro2    = "The requirements are presented in textual form with " 
                     + "superseded descriptions printed in italics and " 
                     + "unallocated requirements marked with a \"?\".";
       reqtab_title  = "Requirements for: ";
       no_req        = " - No requirements defined - \n";

       uca_heading   = "Users View";
       uca_intro     = "This chapter describes the user's view of the system. " 
                     + "Existing scenarios in form of Sequence-, Collabora" 
					 + "tion and Activity-Diagrams are printed after the " 
                     + "description of the corresponding Use Case.";
       uca_title     = "Use Cases in: ";
       act_name      = "Actor: ";
	   performs      = " performs: ";
       uca_name      = "Use Case: ";
       uca_precond   = "Preconditions: ";
       uca_postcond  = "Postconditions: ";
       uca_except    = "Exceptions: ";
	   is_performed  = " is performed by: ";
	   uc_is_ext     = " is extended by: ";
	   uc_is_used    = " is used by: ";
	   uc_extends    = " extends: ";
	   uc_uses       = " uses: ";
       no_uca        = " - No Use Cases defined - \n";

       int_diag      = "Interaction Diagram: ";
       col_name      = "Collaboration: ";
       col_diag      = "Collaboration Diagram: ";
       seq_diag      = "Sequence Diagram: ";
       type_name     = "Type: ";
	   act_obj       = "Active ";
	   ano_obj       = "Anonymous ";
	   obj_name      = "Object ";
       of_the_class  = "of the class ";
       sync_msg      = "Synchronous ";
       async_msg     = "Asynchronous ";
       tout_msg      = "TimeOut ";
       balk_msg      = "Balking ";
       ret_msg       = "Return ";
       evt_name      = "Event: ";
       msg_name      = "Message: ";

       sarc_heading  = "System Architecture";
       sarc_intro    = "This chapter describes the architecture of the \"" 
                     + current_system() + "\" System";
       no_sarc       = " - No architectural information defined - ";
       lsa_heading   = "Logical System Architecture";
       lsa_intro     = "The logical architecture of the system \"" 
                     + current_system() + "\" is described by the following "
                     + "hierarchy of its packages.";
       no_lsa        = " - No packages defined - ";
	   sys_name      = "System: ";
       psa_heading   = "Physical System Architecture";
       psa_intro     = "The physical architecture of the system is described by "
                     + "Implementation Diagrams showing the Hard- and Software- "
                     + "Components and their deployment.";
       no_psa        = " - No Implementation Diagrams defined - ";
       comp_diag     = "Component Diagram:";
       depl_diag     = "Deployment Diagram:";
       unit_name     = "Unit: ";
       module_bin    = "Binary: ";
       module_ex     = "Executable: ";
       module_if     = "Interface: ";
       module_src    = "Source: ";

       pac_heading   = "Package Report";
       pac_intro     = "This chapter consists of a detailed description of " 
                     + "the system's packages.";
	   pac_name      = "Package: ";
       pac_diag      = "Package Diagrams:";
       cls_diag      = "Class Diagrams:";
       no_pac        = " - No packages defined, but printing class diagrams if they exist - ";

       class_heading = "Class Report";
       class_intro   = "This chapter consists of a detailed description of "
                     + "the system's classes in alphabetical order.";
       class_name    = "Class: ";
       no_class      = " - No classes defined -";
       no_ctab       = " - No class table defined - ";
       ass_class     = " is an \"Association Class\" and describes a relation";
       ass_label     = " named ";
       between       = " between the classes ";
       miscel        = "Miscellaneous Information: ";
       is_derived    = "Derived: ";
       is_abstract   = "Abstract: ";
       is_imported   = "Imported: ";
       is_parameter  = "Parameters: ";
       is_of_scope   = "Scope: ";
       is_multi      = "Multiplicity: ";
       constraint    = "Constraint: ";
       constraints   = "Constraints: ";
       class_role    = "Role(s): ";
       class_qual    = "qualified by: ";
       interfaces    = "Interfaces: ";
       superobject   = " is a generalisation of: ";
       subobject     = " is a specialisation of: ";
       agg_comp      = " is composed by agregation of the class(es): ";
       agg_part      = " is a part of the agregation of the class(es): ";
       att_title     = "Attributes:";
       missing_att   = "The table does not contain all attributes !";
       is_of_type    = "Type: ";
       is_visible    = "Visibility: ";
       is_meta_att   = "Class Attribute: ";
       is_of_default = "Default: ";
       class_op      = "Operations: ";
       missing_op    = "The table does not contain all operations !";
       is_meta_op    = "Class Operation.";
       is_throws     = "Throws: ";

       dyn_diag      = "State Diagram: ";
       act_diag      = "Activity Diagram: ";
       state_machine = "State Machine: ";
       std_state     = "State: ";
       act_state     = "Action State: ";
       hist_state    = "History State: ";
       start_state   = "Initial State: ";
       final_state   = "Final State: ";
       // ECR 7215
       junction_point= "Junction Point: ";

       assoc_heading = "Association Table";
       assoc_intro   = "This chapter consists of a table with detailed "
                     + "description of the system's classes, "
                     + "multiplicities and associations.";
       no_assoc      = " - No associations defined -";
       tab_assoc     = "Association";
       tab_classA    = "Class A";
       tab_classB    = "Class B";
       tab_desc      = "Description";
       tab_multA     = "Mult. A";
       tab_multB     = "Mult. B";
       tab_role      = "Role";
       not_named     = "unnamed";

       stt_heading   = "Stereotypes"; 
	   stereo_name   = "Stereotype: ";
       stt_intro     = "";
       no_stt        = " - No stereotypes defined - \n";

       gls_heading   = "Glossary"; 
       gls_intro     = "The Glossary consists of a collection of " +
                       "systemspecific topics with their explanations, " +
                       "defined in StP's Glossary Note for the shown " +
                       "definition object.\n";
       no_glsd       = " - No topics defined - \n";
       no_glsr       = " - No topics referenced - \n";

	   consists_of   = " contains ";
	   cross_ref     = "Described in ";
       date_format   = "%B %d, %Y";
       defined_in    = "defined in: ";
       diag_name     = "The diagram: ";
       diag_source   = "Source: ";
       gen_author    = "by ";
       gen_date      = " on %B %d, %Y";
	   is_depending  = " is depending on: ";
       is_generated  = "Generated ";
	   is_part_of    = " is part of: ";
       lang_obj      = "GenericObject";        
       no_desc       = " - No textual description - ";
       not_def       = "- none -";
       obj_refs      = " is referenced in the following diagrams and tables:";
       print_and     = " and ";
       print_empty   = " - the table is empty - ";
       print_from    = " from ";
       print_in      = " in ";
       print_no      = "No";
       print_pages   = " Pages";
       print_the     = "The ";
       print_to      = " to ";
       print_yes     = "Yes";
       time_format   = " at %I:%M %p";
	   tval_name     = "Tagged Values: ";
    }

    if (e_DOCUMENT_LANGUAGE == "F") 
    {
       info_heading  = "A propos de ce document";
       ext_subtitle  = "pour <Systeme-StP>";
       info_title    = "pour ";
       info_version  = "Version:  \t";
       is_version    = "V1.0";
       info_date     = "Générée le:  \t";
       info_status   = "Status:  \t";
       is_status     = "Analyse";
       is_author     = "Customization Team";
       info_dsource  = "Document maitre:  \t";
       info_pages    = "Pages:  \t";
       info_stpsys   = "StP-Système:  \t";
       info_ssource  = "StP-Projet:  \t";
       info_intro    = "Ce document décrit l'évaluation du StP-Système \"" 
                     +  current_system() + "\". \n"
                     + "Il est génerée par le QRL-Script \"UML-Report\" et "
                     + "présente les chapitres suivantes:";
       toc_name   = "Table des Matières";

       req_heading   = "Exigences";
       req_intro     = "";
       req_intro1    = "";
       req_intro2    = "";
       reqtab_title  = "Exigences pour: ";
       no_req        = " - Aucun exigence définis - \n";

       uca_heading   = "Modèle des Use Cases";
       uca_intro     = "";
       uca_title     = "Use Cases en: ";
       act_name      = "Acteur: ";
	   performs      = " exécute: ";
       uca_name      = "Use Case: ";
       uca_precond   = "Pre-Conditions : ";
       uca_postcond  = "Post-Conditions : ";
       uca_except    = "Exceptions: ";
	   is_performed  = " est traite par : ";
	   uc_is_ext     = " est étendue par : ";
	   uc_is_used    = " est utilise par : ";
	   uc_extends    = " étend: ";
	   uc_uses       = " utilise: ";
       no_uca        = " - Aucun Use Case défini - \n";

       int_diag      = "Diagramme d'interaction: ";
       col_name      = "Collaboration: ";
       col_diag      = "Diagramme de Collaboration: ";
       seq_diag      = "Diagramme d'interaction: ";
       type_name     = "Type: ";
	   act_obj       = "Active ";
	   ano_obj       = "Anonyme ";
	   obj_name      = "Objet ";
       of_the_class  = "de la classe ";
       sync_msg      = "Synchrone ";
       async_msg     = "Asynchrone ";
       tout_msg      = "Temporise ";
       balk_msg      = "Rejete ";
       ret_msg       = "Retour ";
       evt_name      = "Evénement: ";
       msg_name      = "Message: ";

       sarc_heading  = "Architecture du système";
       sarc_intro    = "Ce chapitre décrit l'architecture du système \"" 
                      + current_system() + "\"";
       no_sarc        = " - Aucun architecture du système défini - ";
       lsa_heading   = "L'architecture logique";
       lsa_intro     = "";
       no_lsa        = " - Aucun package défini - ";
	   sys_name      = "Système: ";
       psa_heading   = "L'architecture physique";
       psa_intro     = "";
       no_psa        = " - Aucun diagramme d'implémentation définis - ";
       comp_diag     = "Diagramme de Module:";
       depl_diag     = "Diagramme de deploiement:";
       unit_name     = "Unité: ";
       module_bin    = "Binary: ";
       module_ex     = "Executable: ";
       module_if     = "Interface: ";
       module_src    = "Source: ";

       pac_heading   = "Présentation des packages";
       pac_intro     = "";
	   pac_name      = "Package: ";
       pac_diag      = "Les diagrammes des packages:";
       cls_diag      = "Les diagrammes des classes:";
       no_pac      = " - Emballages (packages) ne sont pas definiés, mais on peut imprimer diagrammes de classes si elles existent -";

       class_heading = "Présentation des classes";
       class_intro   = "";
       class_name    = "Classe: ";
       no_class      = " - Aucune classe définie -";
       no_ctab       = " - Aucune table de classe définie - ";
       ass_class     = " est une \"Classe d'association\" et décrit une" +
                       " relation";
       ass_label     = " appelée ";
       between       = " entre les classes ";
       miscel        = "Diverses informations: ";
       is_derived    = "Dérivée: ";
       is_abstract   = "Abstraite: ";
       is_imported   = "Importée: ";
       is_parameter  = "Paramètres: ";
       is_of_scope   = "Domaine: ";
       is_multi      = "Multiplicité: ";
       constraint    = "Limitation: ";
       constraints   = "Limitations: ";
       class_role    = "Rôle(s): ";
       class_qual    = "qualifiée de: ";
       interfaces    = "Interfaces: ";
       superobject   = " est une généralisation de: ";
       subobject     = " est une spécialisation de: ";
       agg_comp      = " est constitué par aggrégation des classes: ";
       agg_part      = " est un constituant par aggrégation des classes: ";
       att_title     = "Attributs:";
       missing_att   = "La table ne contient pas tous les attributs !";
       is_of_type    = "Type: ";
       is_visible    = "Visibilité: ";
       is_meta_att   = "Méta attribut: ";
       is_of_default = "Valeur par default: ";
       class_op      = "Opérations: ";
       missing_att   = "La table ne contient pas toutes les opérations !";
       is_meta_op    = "Méta opération: ";
       is_throws     = "Exceptions: ";

       dyn_diag      = "Diagramme des états: ";
       act_diag      = "Diagramme des activités: ";
       state_machine = "Machine des états: ";
       std_state     = "État: ";
       act_state     = "action d'etat: ";
       hist_state    = "État historise: ";
       start_state   = "Etat initial";
       final_state   = "Etat final";
       // ECR 7215
       junction_point= "Point de Jonction: ";

       assoc_heading = "Table des associations";
       assoc_intro   = "";
       no_assoc      = " - Aucune association définie -";
       tab_assoc     = "Association";
       tab_classA    = "Classe A";
       tab_classB    = "Classe B";
       tab_desc      = "Description";
       tab_multA     = "Mult. A";
       tab_multB     = "Mult. B";
       tab_role      = "Rôle";
       not_named     = "non nommée";

       stt_heading   = "Stéréotypes";  // ECR 5703
	   stereo_name   = "Stéréotype: ";
       stt_intro     = "";
       no_stt        = " - Pas de stéréotype défini - \n";

       gls_heading   = "Glossaire"; 
       gls_intro     = "";
       no_glsd       = " - Pas d'item défini - \n";
       no_glsr       = " - Pas d'item référencé - \n";

	   consists_of   = " consiste en ";
	   cross_ref     = "Décrit en ";
       date_format   = "%d. %B %Y";    //"%e. %B %Y";
       defined_in    = "définis dans: ";
       diag_name     = "Diagramme: ";
       diag_source   = "Source: ";
       gen_author    = "de ";
       gen_date      = " le %d. %B %Y";
	   is_depending  = " est dépendant de: ";
       is_generated  = "Généré ";
	   is_part_of    = " est un element de: ";
       lang_obj      = "GenericObject";        
       no_desc       = " - Pas de description textuelle - ";
       not_def       = "- aucune -";
       obj_refs      = " est référencé dans les diagrammes et tables: ";
       print_and     = " et ";
       print_empty   = " - Table vide - ";
       print_from    = " de ";
       print_in      = " en ";
       print_no      = "Non";
       print_pages   = " pages";
       print_the     = "Le ";
       print_to      = " vers ";
       print_yes     = "Oui";
       time_format   = " a %H:%M";
	   tval_name     = "Etiquette: ";
    }

    return status;
}
