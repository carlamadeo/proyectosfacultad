//*****************************************************************************
// DESCRIPTION: UML_Externals_D.inc
//              ----------------------
//              (c) 1997 by AONIX
//*****************************************************************************
//
// Include file for the UML-Report with the definition of all german externals
// and their mapping to the globals used in the language independent parts.
//*****************************************************************************
//
// functions: - void   MapExternals()
//*****************************************************************************

script_help = "UML_Report_D generiert einen UML-Report, der wahlweise aus Kapiteln mit Anforderungen, Anwendersicht, logischer und physischer Systemarchitektur, Paketreport, Klassenreport, Stereotypen Definition und dem Glossar besteht. 

Die externen Variablen in der Auswahlliste rechter Hand erlauben Ihnen, die Auswertungen an Ihre Bedürfnisse anzupassen.
Wenn Sie die Auswertung auf bestimmte Dateien oder Klassen beschränken wollen, gilt für deren Eingabe als externe Variable folgende Syntax: 
<Angabe1> <Angabe2> <Angabe3> ...., 
wobei die einzelnen Angaben jeweils durch ein Leerzeichen zu trennen sind.
Sie können auch das Wildcardsymbol \"*\", wie z.B. Appl*, verwenden. Dann werden alle Angaben ausgewertet, die mit \"Appl\" anfangen.
Die Vorgabe ist immer \"*\", d.h. alle möglichen Dateien oder Symbole.

Um Ihre Eingaben für externe String-Variablen wieder zu entfernen, verwenden Sie den Befehl \"Delete External Value\" !

Der Report verwendet die Formatdatei \"stpreport\" oder eine darauf basierende Vorlage. Unterstützt werden die Zielformate Framemaker MIF und Word für Windows RTF.";


//*****************************************************************************
// EXTERNAL VARIABLES
//*****************************************************************************

external string DOKUMENT_INFORMATION__________ = "Wird nicht ausgewertet"; 
external_help = "-------------------";
     
external string __Firmenname = "Aonix";
external_help = "Der Firmenname wird auf dem Titelblatt ausgegeben.
Die Vorbelegung lautet \"Aonix\" und kann durch eine beliebige Angabe ersetzt werden.
Um Ihr Firmenlogo in den Seitenköpfen auszugeben, müssen Sie das Aonix Logo in den Formatdateien durch Ihr eigenes Logo ersetzen.";
     
external string __Abteilungsname = "Entwicklungs Team";
external_help = "Der Abteilungsname wird auf dem Titelblatt ausgegeben.
Die Vorbelegung lautet \"Entwicklungs Team\" und kann durch eine beliebige Angabe ersetzt werden.";
     
external string __Dokument_Verfasser = "Customization Team";
external_help = "Der Verfasser des Dokuments wird in den Fußzeilen ausgegeben.
Die Vorbelegung  lautet \"Customization Team\" und kann von Ihnen durch eine beliebige Angabe ersetzt werden.";

external string __Dokument_Titel = "UML-Report";
external_help = "Der Titel des Dokuments wird auf dem Titelblatt und in den Kopfzeilen ausgegeben.
Die Vorbelegung lautet \"UML-Report \" und kann durch eine beliebige Angabe ersetzt werden.";

external string __Dokument_Untertitel = "für <StP-System>";
external_help = "Der Untertitel des Dokuments wird auf dem Titelblatt und in den  Kopfzeilen unterhalb des Dokumenttitels ausgegeben.
Die Vorbelegung lautet \"für <StP-System>\", wobei <StP-System> durch den Namen Ihres StP-Systems ersetzt wird. 
Wenn Sie diese Angabe ändern, so erscheint nur der von Ihnen eingegebene Text.";

external string __Dokument_Version = "V1.0";
external_help = "Die Version des Dokuments wird auf der Titelseite und in den Fußzeilen ausgegeben.
Die Vorbelegung lautet \"V1.0\" und kann durch eine beliebige Versionsangabe ersetzt werden.";
     
external string __Dokument_Status = "Analyse";
external_help = "Der Status des Dokuments wird auf der Titelseite ausgegeben.
Der Vorbelegung lautet \"Analyse\" und kann von Ihnen durch eine beliebige Angabe, wie z.B. \"Design\", \"Entwurf\" oder \"Review\" ersetzt werden.";


external string DOKUMENT_LAYOUT_______________ = "Wird nicht ausgewertet"; 
external_help = "-------------------";
     
external string __Standard_Formatdatei = "stpreport";
external_help = "Formatdatei, nach deren Vorgabe das Dokument generiert werden soll.
Die Vorbelegung lautet \"stpreport\" und kann von Ihnen durch eine andere Vorlage (passend zum gewaehlten Zielformat) ersetzt werden. Wenn diese andersnamige Absatz- und Zeichenformate enthält, als stpreport, müssen Sie die Bezeichnungen in der Datei ALL_Formats.inc anpassen. 

Bitte stellen Sie sicher, dass Sie auf die angegebene Formatdatei zugreifen können (sie muß in der Liste der Formatdateien des Script Managers angezeigt werden). 

Der Report verwendet automatisch die zum gewählten Zielformat passende Extension (.mif or .rtf), d.h. wenn Sie im Run Script Dialog Framemaker als Zielformat auswählen, wird die Formatdatei \"stpreport.mif\" verwendet.";
     
external string __Zielsprache = "D";
external_help = "Sprache, in der das Dokument generiert werden soll.
Zur Auswahl stehen D (deutsch), E (englisch) oder F (französisch).";

external boolean __Erzeuge_Ein_HTML_File = False;
external_help = "Soll der HTML-Report in ein einziges oder mehrere Files generiert werden ?  Ein neues File für das HTML Target entspricht dabei einem Seitenumbruch in anderen Targets.";

external boolean __Optimierte_Diagramm_Ausgabe = True;
external_help  = "Soll die Größe der Diagramme druckoptimiert werden ? Wenn nicht, wird jedes Diagramm auf einer eigenen Seite ausgegeben.";

external string __Verwende_Druck_Muster = "";
external_help  = "Hier können Sie Ihr eigenes Druckmuster (StP Print Setting) für Diagramme und Tabellen angeben (speicern Sie alle Muster mit dem gleichen Namen).";

external string __Unterdruecke_Displaymarken = "UseCaseScenariosExist ClassTableExists AttributeIsDerived AttributeTypeAndValue AttributeVisibility AttrIsClass ClassInfo ClassParameters ClassScoping Navigability OperationReturnType OperationVisibility OpIsClass RefinedStateExists RefinementArguments StateTableExists Visibility";
external_help  = "Hier können Sie angeben, welche Displaymarken Sie bei der Ausgabe von Diagrammen unterdrücken wollen. Bitte verwenden Sie Blanks als Trennzeichen zwischen den einzelnen Displaymarkennamen.";

external boolean __Symbole_alphabetisch_sortieren = False;
external_help  = "Sollen Symbole, wie z.B. Use Cases in alphabetischer Reihenfolge oder entsprechend Ihrer vertikalen Anordnung im Diagramm ausgegeben werden ?";

external boolean __mit_Einleitungstexten = True ; 
external_help = "Soll zu Beginn eines Kapitels jeweils ein Absatz mit (in der Datei UML_Text.inc vordefiniertem) einleitendem Text ausgegeben werden ?";

external boolean __mit_Beschreibungen = True ; 
external_help = "Sollen die (mit dem OAE erfassten) Diagramm- und Symbolbeschreibungen ausgegeben werden ?";

external boolean ____auf_fehlende_Beschreibung_hinweisen = True; 
external_help = "Sollen Hinweise auf fehlende Diagramm- und Symbolbeschreibungen generiert werden ?";

external boolean ____Beschreibungen_mehrfach_ausgeben = False; 
external_help = "Soll die Beschreibung von Symbolen, wie z.B. Pakete oder Use Cases jedesmal oder nur beim ersten Auftreten ausgegeben werden ?";

external boolean __mit_Angaben_zur_Generierung = False ; 
external_help = "Sollen im OAE erfasste Angaben zu Verfasser und Generierungsdatum für Use Cases, Klassen und Diagramme gedruckt werden ?";


external string DOKUMENT_INHALT_____________ = "Wird nicht ausgewertet"; 
external_help = "-------------------";

external boolean __Generiere_das_Anforderungs_Kapitel = True;
external_help = "Wollen Sie das Kapitel mit den Anforderungen generieren ?";

external boolean ____in_Anforderungstabellen_Form = False;
external_help = "Sollen die Anforderungen wie in der Anforderungstabelle dargestellt oder in Form einer Gliederung ausgegeben werden werden ?
Die Tabellendarstellung wird nur für Tabellen mit maximal 4 Spalten, d.h. 3 Gliederungsebenen empfohlen, da die Darstellung sonst sehr unübersichtlich ist.";

external boolean ____unerfuellte_Anforderungen_kennzeichnen = False;
external_help = "Sollen noch nirgends erfüllte Anforderungen durch ein vorangestelltes Fragezeichen gekennzeichnet werden ?
Diese Option ist nicht wirksam, wenn die Ausgabe in Tabellenform erfolgt !";

external boolean ____mit_verfeinerten_Anforderungen = False;
external_help = "Sollen alle Anforderungen, d.h. auch die bereits verfeinerten, ausgegeben werden oder nur die jeweils feinste Ebene ?";

external boolean __Generiere_das_Anwendersicht_Kapitel = True;
external_help = "Wollen Sie ein Kapitel mit der Anwendersicht generieren (Use Cases und Szenarien in Interaktions- und Aktivitätsdiagrammen) ?";

external boolean ____mit_detailerter_Use_Case_Beschreibung = False;
external_help  = "Sollen detailierte Informationen über die in den Use Case (und Szenario Diagrammen) enthaltenen Symbole ausgegeben werden ?";

external boolean ____mit_Use_Case_Szenarios = True;
external_help = "Sollen die Szenarien (Sequenz-, Kollaborations- und Aktivitäts-Diagramme) der Use Cases ausgegeben werden ?";

external boolean __Generiere_das_Systemarchitektur_Kapitel = True;
external_help = "Wollen Sie das Kapitel mit Informationen über die logischen und physischen Komponenten des Systems generieren ?";

external boolean ____mit_logischer_Systemarchitektur = True;
external_help = "Soll ein Abschnitt mit der Paket-Hierarchie generiert werden ?";

external boolean ____mit_physischer_Systemarchitektur = True;
external_help = "Soll ein Abschnitt mit Modul- (Component) und Prozeß- (Deployment) Diagrammen generiert werden ?";

external boolean ______mit_detailierter_Architektur_Beschreibung = False;
external_help = "Sollen detailierte Informationen über enthaltenen Module und Prozesse ausgegeben werden ?";

external boolean __Generiere_das_Paketreport_Kapitel = True;
external_help = "Wollen Sie das Kapitel mit dem Paketreport generieren ?";

external boolean ____mit_detailierter_Paket_Beschreibung = False;
external_help = "Sollen detailierte Informationen über die Pakete ausgegeben werden ?
In der Anfangsphase des Projekts sollten Sie hier \"False\" angeben.";

external boolean ____mit_Paket_und_Klassen_Diagrammen = True;
external_help = "Sollen Paket- und Klassendiagramme ausgegeben werden ?";

external boolean __Generiere_das_Klassenreport_Kapitel = True;
external_help = "Wollen Sie das Kapitel mit dem Klassenreport generieren ?";

external boolean ____mit_detailierter_Klassen_Beschreibung = False;
external_help = "Sollen detailierte Informationen über die Klassen (sowie ggf. Attribute und Operationen) ausgegeben werden ?
In der Anfangsphase des Projekts sollten Sie hier \"False\" angeben.";

external boolean ____mit_Attributen_und_Operationen = True;
external_help = "Sollen Informationen über die Attribute und Operationen der Klassen ausgegeben werden ?";

external boolean ______in_Klassentabellen_Form = False;
external_help = "Sollen die Attribute und Operationen wie in der Klassentabelle dargestellt oder in Form einer Gliederung ausgegeben werden werden ?";

external boolean ______mit_Analyse_Informationen = False;
external_help = "Sollen vorhandene Analyse Informationen zu den Attributen und Operationen ausgegeben werden ?";

external boolean ______mit_Cxx_Informationen = False;
external_help = "Sollen vorhandene C++ Informationen zu den Attributen und Operationen ausgegeben werden ?";

external boolean ______mit_Java_Informationen = False;
external_help = "Sollen vorhandene Java Informationen zu den Attributen und Operationen ausgegeben werden ?";

external boolean ______mit_IDL_Informationen = False;
external_help = "Sollen vorhandene IDL Informationen zu den Attributen und Operationen ausgegeben werden ?";

external boolean ______mit_Ada95_Informationen = False;
external_help = "Sollen vorhandene Ada95 Informationen zu den Attributen und Operationen ausgegeben werden ?";

external boolean ____mit_Interaktions_Diagrammen = True;
external_help = "Sollen Sequenz- und Kollaborations-Diagramme ausgegeben werden, in denen die Klasse referenziert ist ?";

external boolean ____mit_Zustands_Diagrammen = True;
external_help = "Sollen die der Klasse zugeordneten Zustands- und Aktivitäts-Diagramme ausgegeben werden ?";

external boolean ____mit_Assoziations_Tabelle = True;
external_help = "Soll am Ende des Klassenreports eine tabellarische Übersicht ausgegeben werden, in der für alle enthaltenen Klassen die Assoziationen mit Rollen, Multiplizität und Beschreibung aufgelistet sind ?";

external boolean __Generiere_das_Stereotypen_Kapitel = True;
external_help = "Wollen Sie ein Kapitel mit den Definitionen sämtlicher im System enthaltener Stereotypen generieren ?";

external boolean ____mit_Stereotyp_Diagrammen = False;
external_help = "Sollen am Anfang des Kapitels die Stereotyp Diagramme ausgegeben werden ?";

external boolean __Generiere_das_Glossar = False;
external_help = "Wollen Sie am Ende des Reports ein Glossar generieren ?";

external boolean ____nur_fuer_Themen_des_Reports = False;
external_help = "Soll das Glossar nur Einträge zu den im Report enthaltenen Diagrammen und Symbolen enthalten ?";

external string AUSWERTUNG_EINSCHRAENKEN____ = "Wird nicht ausgewertet"; 
external_help = "-------------------";

external string __auf_den_Praefix = "*";
external_help = "Soll der ganze Report auf Diagramme und Symbole beschränkt werden, die mit dem angegebenen Präfix beginnen ? (Syntax, siehe \"Help on Script\")";

external string __auf_Pakete = "*";
external_help = "Soll der Report auf Diagramme, die passende Pakete enthalten, sowie Symbole, die in diesen Paketen enthalten sind, beschränkt werden ? (Syntax, siehe \"Help on Script\")";

external string __auf_Klassen = "*";
external_help = "Soll der Klassenreport auf Klassen beschränkt werden, die mit dem angegebenen Präfix beginnen ? (Syntax, siehe \"Help on Script\")";

external string __auf_Stereotypen = "*";
external_help = "Soll das Stereotypenkapitel auf Stereotypen beschränkt werden, die mit dem angegebenen Präfix beginnen ? (Syntax, siehe \"Help on Script\")";

external string __auf_Anforderungs_Tabellen = "*";
external_help = "Soll das Kapitel mit den Anforderungen auf Tabellen beschränkt werden, die mit dem angegebenen Präfix beginnen ? (Syntax, siehe \"Help on Script\")";

external string __auf_Use_Case_Diagramme = "*";
external_help = "Soll das Kapitel mit der Anwendersicht auf Use Case Diagramme beschränkt werden, die mit dem angegebenen Präfix beginnen ? (Syntax, siehe \"Help on Script\")";

external string __auf_Implementierungs_Diagramme = "*";
external_help = "Soll das Kapitel mit der physischen Systemarchitektur auf Modul- und Prozeß-Diagramme beschränkt werden, die mit dem angegebenen Präfix beginnen ? (Syntax, siehe \"Help on Script\")";

 
//*****************************************************************************
// MapExternals: This function maps all the german externals to their language
//               independent globaly defined equivalents
//*****************************************************************************
void MapExternals()
{
    // Document Information Section
    e_COMPANY               = __Firmenname;
    e_DIVISION              = __Abteilungsname;
    e_DOC_AUTHOR            = __Dokument_Verfasser;
    e_DOC_TITLE             = __Dokument_Titel;
    e_DOC_SUBTITLE          = __Dokument_Untertitel;
    e_DOC_VERSION           = __Dokument_Version;
    e_DOC_STATUS            = __Dokument_Status;

    // Document Layout Section
    e_DEFAULT_FORMAT        = __Standard_Formatdatei;
    e_DOCUMENT_LANGUAGE     = to_upper(__Zielsprache);
    e_GEN_SINGLE_HTML_FILE  = __Erzeuge_Ein_HTML_File;
    e_OPTIMIZED_PRINTING    = __Optimierte_Diagramm_Ausgabe;
	e_PRINT_SETTING         = __Verwende_Druck_Muster;
	e_NO_DISPLAYMARKS       = StripInfo(__Unterdruecke_Displaymarken);
    e_PRINT_ALPHA_ORDER     = __Symbole_alphabetisch_sortieren;
    e_GEN_INTRODUCTIONS     = __mit_Einleitungstexten;
    e_GEN_DESCRIPTIONS      = __mit_Beschreibungen;
    e_MISSING_DESCRIPTION   = ____auf_fehlende_Beschreibung_hinweisen;
    e_MULTIPLE_DESCRIPTION  = ____Beschreibungen_mehrfach_ausgeben;
    e_GENERATION_INFO       = __mit_Angaben_zur_Generierung;

    if (!e_GEN_DESCRIPTIONS)
       e_MISSING_DESCRIPTION = False;

    // Document Chapter Section

    e_GEN_REQUIREMENTS      = __Generiere_das_Anforderungs_Kapitel;
    e_IN_RQ_TABLE_FORM      = ____in_Anforderungstabellen_Form;
    e_SHOW_UNALLOCATED      = ____unerfuellte_Anforderungen_kennzeichnen;
    e_SUPERSEDED_REQ        = ____mit_verfeinerten_Anforderungen;

    e_GEN_USERS_VIEW        = __Generiere_das_Anwendersicht_Kapitel;
    e_UV_DETAILS            = ____mit_detailerter_Use_Case_Beschreibung;
    e_SCENARIOS             = ____mit_Use_Case_Szenarios;

    e_GEN_SYS_ARCH          = __Generiere_das_Systemarchitektur_Kapitel;
    e_LOG_ARCH              = ____mit_logischer_Systemarchitektur;
    e_PHYS_ARCH             = ____mit_physischer_Systemarchitektur;
    e_IMPL_DETAILS          = ______mit_detailierter_Architektur_Beschreibung;

    if (!e_LOG_ARCH && !e_PHYS_ARCH)
       e_GEN_SYS_ARCH = False;

    e_GEN_PACKAGE_REPORT    = __Generiere_das_Paketreport_Kapitel;
    e_PACK_DETAILS          = ____mit_detailierter_Paket_Beschreibung;
    e_PACK_DIAGRAMS         = ____mit_Paket_und_Klassen_Diagrammen;

    e_GEN_CLASS_REPORT      = __Generiere_das_Klassenreport_Kapitel;
    e_CLASS_DETAILS         = ____mit_detailierter_Klassen_Beschreibung;
    e_CL_ATT_OP             = ____mit_Attributen_und_Operationen;
    e_IN_CL_TABLE_FORM      = ______in_Klassentabellen_Form;
    e_WITH_ANALYSIS_INFO    = ______mit_Analyse_Informationen;
    e_WITH_CXX_INFO         = ______mit_Cxx_Informationen;
    e_WITH_JAVA_INFO        = ______mit_Java_Informationen;
    e_WITH_IDL_INFO         = ______mit_IDL_Informationen;
    e_WITH_ADA_INFO         = ______mit_Ada95_Informationen;
    e_CL_INTERACTION        = ____mit_Interaktions_Diagrammen;
    e_CL_BEHAVIOR           = ____mit_Zustands_Diagrammen;
    e_GEN_ASSOC_SUM_TBL	    = ____mit_Assoziations_Tabelle;

    e_GEN_STEREOTYPES       = __Generiere_das_Stereotypen_Kapitel;
    e_STEREOTYPE_FILES      = ____mit_Stereotyp_Diagrammen;

    e_GEN_GLOSSARY          = __Generiere_das_Glossar;
    e_REFD_GLOSSARY         = ____nur_fuer_Themen_des_Reports;

    // Reduce Evaluation Section
    e_PREFIX              = __auf_den_Praefix;
    e_PACKAGES            = __auf_Pakete;
    e_CLASSES             = __auf_Klassen;
    e_STEREOTYPES         = __auf_Stereotypen;
    e_RQTBL_FILES         = __auf_Anforderungs_Tabellen;
    e_USECASE_FILES       = __auf_Use_Case_Diagramme;
    e_IMPL_FILES          = __auf_Implementierungs_Diagramme;

    return;
}
