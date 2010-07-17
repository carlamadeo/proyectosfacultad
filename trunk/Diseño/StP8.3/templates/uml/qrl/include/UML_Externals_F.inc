//*****************************************************************************
// DESCRIPTION: UML_Externals_F.inc
//              ----------------------
//              (c) 1997 by AONIX
//*****************************************************************************
//
// Include file for the UML-Report with the definition of all french externals
// and their mapping to the globals used in the language independent parts.
// Francois Xavier David, Aonix France provided the french translation.
//*****************************************************************************
//
// functions: - void   MapExternals()
//*****************************************************************************

script_help = "UML_Report_E permet de generer un rapport a partir d'un modele UML, structure en chapitres :  Exigences, Vue Utilisateur, Architecture Logiciel et Materiel,  Description des packages, Description des classes, Glossaire et definition des stereotypes. 

Les variables externes presentes dans la partie droite de cette fenetre vous permettent d'influencer la structuration et le contenu du rapport a generer.
Si vous voulez limiter le traitement a certains fichiers ou classes, suivez la syntaxe suivante :
<item1> <item2> <item3> ...., 
utilisez les blancs comme delimiteurs et ne precisez pas les extensions fichiers.
Vous pouvez utiliser le meta-caractere \"*\", comme Appl*, pour evaluer tous les fichiers ou classes commencant par \"Appl\".
\"*\" est la valeur par defaut, c'est a dire tous les fichiers ou symboles d'un type donne.

Utilisez \"Delete External Value\" pour reinitialiser les variables externes a leur valeur initiale !

Ce rapport utilise la feuille de style \"stpreport\". Les formats en sortie peuvent etre FrameMaker MIF ou RTF pour Windows.";


//*****************************************************************************
// EXTERNAL VARIABLES
//*****************************************************************************

external string INFORMATION_DU_DOCUMENT_______ = "Pas evalue"; 
external_help = "-------------------";
     
external string __Nom_de_la_Compagnie = "Aonix";
external_help = "Le nom de la compagnie est insere sur la page de titre.
Le standard est \"Aonix\" et peut etre remplace par un autre nom. 
Si vous voulez indiquer le logo de votre compagnie dans les lignes d'entete, il vous faut remplacer le logo Aonix dans votre fichier de format.";
     
external string __Nom_de_la_Section = "Developpement";
external_help = "Le nom de la section est indique sur la page de titre.
Le standard est \"Developpement\" et peut etre remplace par un autre nom.";
     
external string __Nom_d_Auteur = "Equipe de Developpement";
external_help = "L'auteur de ce document est indique sur la page de titre et dans le bas de la page.
Le standard est \"Equipe de developpement\" et peut etre remplace par une autre nom.";

external string __Titre_du_Document = "Rapport UML";
external_help = "Le titre du document est indique sur la page de titre et dans l'entete.
Le standard est \"Rapport UML\" et peut etre remplace par un autre titre.";

external string __Soustitre_du_Document = "pour <Systeme-StP>";
external_help = "Le sous-titre du document est indique sur la page de titre et dans l'entete sous le titre du document.
Le standard est \"pour <Systeme-StP>\", pour le document genere <Systeme-StP> est remplace par le nom du systeme . 
Si vous changez cette valeur, l'instanciation du nom du systeme n'aura pas lieu.";

external string __Version_du_Document = "V1.0";
external_help = "La version du document est indique sur la page de titre et dans le bas de page.
Le standard est \"V1.0\" et peut etre remplace par toute autre valeur.";
     
external string __Etat_du_Document = "Analyse";
external_help = "Le type du document est indique sur la page de titre.
Le standard est \"Analyse\" et peut etre remplace par une autre phase telles que \"Conception\", \"Brouillon\" ou \"Revision\".";


external string LAYOUT_DU_DOCUMENT____________ = "Pas evalue"; 
external_help = "-------------------";
     
external string __Fichier_de_Format = "stpreport";
external_help = "Nom du fichier de format, utilise pour la generation du document.
Le standard est \"stpreport\"et peut etre remplace par un autre fichier de format existant. Si votre feuille de style contient des formats differents a l'echelle des paragraphes ou des caracteres, vous devez regulariser ces noms de format dans le fichier ALL_Formats.inc. 

Verifier cependant que le fichier de format choisi est accessible, pour ce faire vous pouvez utiliser l'option Browse Format File du script manager.

Les extensions (.mif ou .rtf) sont automatiquement ajoutees en fonction du type de format demande. Si vous choisissez FrameMaker comme cible dans la boite de dialogue du script manager, le fichier de format \"stpreport.mif\" est utilise.";
     
external string __Langue_du_Document = "F";
external_help = "Langue dans laquelle le document sera genere. Ce rapport peut generer des documents en Anglais (\"E\"), Allemand (\"D\") et Francais (\"F\").";

external boolean __Generer_un_fichier_HTML = False;
external_help = "Indiquer si la génération HTML s'effectue dans un ou plusieurs fichiers ? Il y a plusieurs fichiers si plusieurs pages consituent votre document habituellement générer pour les autres gestionnaires de documentation.";

external boolean __Optimized_Diagram_Printing = True;
external_help  = "Optimisation du placement des diagrammes ? (Sinon les diagrammes sont imprimes sur le propre page)";

external string __Use_Print_Setting = "";
external_help  = "Si vous voulez utiliser les parametres d'impressions specifiques a chaque diagramme ou table (utilisez un nom commun pour l'ensemble de ces diagrammes ou tables)";

external string __Reprime_Displaymarks = "UseCaseScenariosExist ClassTableExists AttributeIsDerived AttributeTypeAndValue AttributeVisibility AttrIsClass ClassInfo ClassParameters ClassScoping Navigability OperationReturnType OperationVisibility OpIsClass RefinedStateExists RefinementArguments StateTableExists Visibility";
external_help  = "Vous pouvez redefinir le jeu de displaymarks a supprimer. Utilisez un blanc entre chaque nom.";

external boolean __Symbols_en_Ordre_Alphabetique = False;
external_help  = "Impression des symboles par ordre alphabetique (True) ou en fonction de leur postionnement dans le diagramme (verticalement) ou dans la table (horizontalement) ?";

external boolean __avec_Introductions = True ; 
external_help = "Impression d'un paragraphe d'introduction au debut de chaque chapitre ?";

external boolean __avec_Descriptions = True ; 
external_help = "Impression des descriptions textuelles pour les diagrammes et symboles (saisies au travers de l'OAE) ?";

external boolean ____Averte_Descriptions_Manqees = True; 
external_help = "Impression du message indiquant l'absence de description pour les diagrammes ou symboles ?";

external boolean ____Imprinte_descriptions_multiples = False; 
external_help = "Impression des descriptions textuelles pour chaque occurence de symbole (package, use case...) dans chaque chapitre ou seulement a la premiere apparition ?";

external boolean __avec_Information_du_Generation = False ; 
external_help = "Impression du nom de l'auteur et date de creation (saisis dans l'OAE) pour les use cases, classes et diagrammes ?";


external string CHAPITRES_DU_DOCUMENT____________ = "Pas evalue"; 
external_help = "-------------------";

external boolean __Chapitre_des_Exigences = True;
external_help = "Voulez vous generer le chapitre des exigences ?";

external boolean ____comme_la_Table_d_Exigence = False;
external_help = "Impression des exigences comme montre dans la table ou sous une forme purement textuelle ?
Choisissez sous forme de table seulement si votre table comporte moins de 4 niveaux de structure, sinon les colonnes sont trop petites pour etre integrees a la page.";

external boolean ____Marque_Exigences_pas_accomplit = False;
external_help = "Signaler les exigences non allouees par un marqueur ?
Cette option n'est valable que pour une impression des exigences sous forme purement textuelle !";

external boolean __avec_Exigences_intermediaires = False;
external_help = "Impression des exigences de niveaux intermediaires ?";

external boolean __Chapitre_de_la_Vue_Utilisateur = True;
external_help = "Generer le chapitre de la vue utilisateur (Diagrammes de types Use Case, Scenarios, Collaboration et Activites)  ?";

external boolean ____avec_Use_Case_Descriptions_en_detail = False;
external_help  = "Impression des informations detaillees pour les symboles contenus dans les Use Case (et scenarios associes) ?";

external boolean ____avec_Use_Case_Scenarios = True;
external_help = "Impression des Scenarios (Sequence, Collaboration, Activite) pour chaque Use Case ?";

external boolean __Chapitre_d_Architecture_du_Systeme = True;
external_help = "Generer le chapitre sur les composants logiciel et materiel du systeme ?";

external boolean ____avec_l_Architecture_logique = True;
external_help = "Generer un sous-chapitre avec la structure du package ?";

external boolean ____avec_l_Architecture_physique = True;
external_help = "Generer un sous-chapitre contenant les diagrammes de composants et de deploiement ?";

external boolean ______avec_Architecture_Description_en_detail = False;
external_help = "Impression des informations detaillees sur module et processus ?";

external boolean __Chapitre_du_Presentation_des_Packages = True;
external_help = "Generer le chapitre decrivant les packages ?";

external boolean ____avec_Package_Descriptions_en_detail = False;
external_help = "Impression des informations detaillees sur les packages ? 
Mettre a False dans les phases de demarrage du projet.";

external boolean ____avec_Diagrammes_de_Packages_et_Classes = True;
external_help = "Impression des diagrammes de classes et packages ?";

external boolean __Chapitre_du_Presentation_des_Classes = True;
external_help = "Generer le chapitre decrivant les classes ?";

external boolean ____avec_Classe_Descriptions_en_detail = False;
external_help = "Impression des informations detaillees pour la classe ? 
Mettre a False dans les phases de demarrage du projet.";

external boolean ____avec_Attributs_et_Operations = True;
external_help = "Impression des informations au sujet des attributs et operations ?";

external boolean ______comme_la_Table_de_Classe = False;
external_help = "Impression des attributs et operations dans la table de classe ou sous forme purement textuelle ?";

external boolean ______avec_Information_d_Analyse = False;
external_help = "Impression des items d'analyse pour les attributs et operations ?";

external boolean ______avec_Information_de_Cxx = False;
external_help = "Impression des items C++ pour les attributs et operations ?";

external boolean ______avec_Information_de_Java = False;
external_help = "Impression des items Java pour les attributs et operations ?";

external boolean ______avec_Information_de_IDL = False;
external_help = "Impression des items IDL pour les attributs et operations ?";

external boolean ______avec_Information_de_Ada95 = False;
external_help = "Impression des items Ada95 pour les attributs et operations ?";

external boolean ____avec_Diagrammes_d_Interaction = True;
external_help = "Impression des diagrammes de sequence et collaboration avec la reference aux classes employees ?";

external boolean ____avec_Diagrammes_d_Etat = True;
external_help = "Impression des diagrammes d'etat et d'activite scopes a une classe ?";

external boolean ____avec_la_Table_des_Associations = True;
external_help = "Generer la table des associations d'une classe qui resume les liens d'association avec leurs cardianlites et leurs descriptions ?";

external boolean __Chapitre_de_la_Definition_des_Stereotypes = True;
external_help = "Generer le chapitre avec la definition des stereotypes ?";

external boolean ____avec_Diagrammes_de_Stereotype = False;
external_help = "Imprimer les diagrammes de stereotype au debut du chapitre sur les stereotypes";

external boolean __Chapitre_du_Glossaire = False;
external_help = "Generer un glossaire a la fin du document ?";

external boolean ____seulement_pour_Objets_du_Rapport = False;
external_help = "Limiter le glossaire aux items associes aux diagrammes et symboles mentionnes dans le rapport ?";

external string LIMITE_L_EVALUATION____________ = "Pas evalue"; 
external_help = "-------------------";

external string __au_Prefix_general = "*";
external_help = "Limiter le rapport aux fichiers et symboles correspondant a l'expression reguliere. (Voir \"Help on Script\" pour la syntaxe exacte). ";

external string __au_nom_des_Packages = "*";
external_help = "Limiter le rapport aux diagrammes et symboles impliquant la liste de packages indiques (Voir \"Help on Script\" pour la syntaxe exacte)";

external string __au_nom_des_Classes = "*";
external_help = "Limiter le rapport sur les classes a la liste indiquee. (Voir \"Help on Script\" pour la syntaxe exacte)";

external string __au_nom_des_Stereotypes = "*";
external_help = "Limiter le chapitre sur la definition des stereotypes a la liste indiquee. (Voir \"Help on Script\" pour la syntaxe exacte)";

external string __aux_Tables_d_Exigences = "*";
external_help = "Limiter le chapitre des exigences a la liste indiquee. (Voir \"Help on Script\" pour la syntaxe exacte).";

external string __aux_Diagrammes_des_Use_Cases = "*";
external_help = "Limiter le chapitre de la vue utilisateur a la liste indiquee. (Voir \"Help on Script\" pour la syntaxe exacte)";

external string __aux_Diagrammes_d_Implementation = "*";
external_help = "Limiter le chapitre sur l'architecture physique a la liste indiquee pour les diagrammes de de composant et de deploiement. (Voir \"Help on Script\" pour la syntaxe exacte)";

 
//*****************************************************************************
// MapExternals: This function maps all the french externals to their language
//               independent globaly defined equivalents
//*****************************************************************************
void MapExternals()
{
    // Document Information Section
    e_COMPANY               = __Nom_de_la_Compagnie;
    e_DIVISION              = __Nom_de_la_Section;
    e_DOC_AUTHOR            = __Nom_d_Auteur;
    e_DOC_TITLE             = __Titre_du_Document;
    e_DOC_SUBTITLE          = __Soustitre_du_Document;
    e_DOC_VERSION           = __Version_du_Document;
    e_DOC_STATUS            = __Etat_du_Document;

    // Document Layout Section
    e_DEFAULT_FORMAT        = __Fichier_de_Format;
    e_DOCUMENT_LANGUAGE     = to_upper(__Langue_du_Document);
    e_GEN_SINGLE_HTML_FILE  = __Generer_un_fichier_HTML;
    e_OPTIMIZED_PRINTING    = __Optimized_Diagram_Printing;
	e_PRINT_SETTING         = __Use_Print_Setting;
	e_NO_DISPLAYMARKS       = StripInfo(__Reprime_Displaymarks);
    e_PRINT_ALPHA_ORDER     = __Symbols_en_Ordre_Alphabetique;
    e_GEN_INTRODUCTIONS     = __avec_Introductions;
    e_GEN_DESCRIPTIONS      = __avec_Descriptions;
    e_MISSING_DESCRIPTION   = ____Averte_Descriptions_Manqees;
    e_MULTIPLE_DESCRIPTION  = ____Imprinte_descriptions_multiples;
    e_GENERATION_INFO       = __avec_Information_du_Generation;

    if (!e_GEN_DESCRIPTIONS)
       e_MISSING_DESCRIPTION = False;

    // Document Chapter Section

    e_GEN_REQUIREMENTS      = __Chapitre_des_Exigences;
    e_IN_RQ_TABLE_FORM      = ____comme_la_Table_d_Exigence;
    e_SHOW_UNALLOCATED      = ____Marque_Exigences_pas_accomplit;
    e_SUPERSEDED_REQ        = __avec_Exigences_intermediaires;

    e_GEN_USERS_VIEW        = __Chapitre_de_la_Vue_Utilisateur;
    e_UV_DETAILS            = ____avec_Use_Case_Descriptions_en_detail;
    e_SCENARIOS             = ____avec_Use_Case_Scenarios;

    e_GEN_SYS_ARCH          = __Chapitre_d_Architecture_du_Systeme;
    e_LOG_ARCH              = ____avec_l_Architecture_logique;
    e_PHYS_ARCH             = ____avec_l_Architecture_physique;
    e_IMPL_DETAILS          = ______avec_Architecture_Description_en_detail;

    if (!e_LOG_ARCH && !e_PHYS_ARCH)
       e_GEN_SYS_ARCH = False;

    e_GEN_PACKAGE_REPORT    = __Chapitre_du_Presentation_des_Packages;
    e_PACK_DETAILS          = ____avec_Package_Descriptions_en_detail;
    e_PACK_DIAGRAMS         = ____avec_Diagrammes_de_Packages_et_Classes;

    e_GEN_CLASS_REPORT      = __Chapitre_du_Presentation_des_Classes;
    e_CLASS_DETAILS         = ____avec_Classe_Descriptions_en_detail;
    e_CL_ATT_OP             = ____avec_Attributs_et_Operations;
    e_IN_CL_TABLE_FORM      = ______comme_la_Table_de_Classe;
    e_WITH_ANALYSIS_INFO    = ______avec_Information_d_Analyse;
    e_WITH_CXX_INFO         = ______avec_Information_de_Cxx;
    e_WITH_JAVA_INFO        = ______avec_Information_de_Java;
    e_WITH_IDL_INFO         = ______avec_Information_de_IDL;
    e_WITH_ADA_INFO         = ______avec_Information_de_Ada95;
    e_CL_INTERACTION        = ____avec_Diagrammes_d_Interaction;
    e_CL_BEHAVIOR           = ____avec_Diagrammes_d_Etat;
    e_GEN_ASSOC_SUM_TBL	    = ____avec_la_Table_des_Associations;

    e_GEN_STEREOTYPES       = __Chapitre_de_la_Definition_des_Stereotypes;
    e_STEREOTYPE_FILES      = ____avec_Diagrammes_de_Stereotype;

    e_GEN_GLOSSARY          = __Chapitre_du_Glossaire;
    e_REFD_GLOSSARY         = ____seulement_pour_Objets_du_Rapport;

    // Reduce Evaluation Section
    e_PREFIX              = __au_Prefix_general;
    e_PACKAGES            = __au_nom_des_Packages;
    e_CLASSES             = __au_nom_des_Classes;
    e_STEREOTYPES         = __au_nom_des_Stereotypee;
    e_RQTBL_FILES         = __aux_Tables_d_Exigences;
    e_USECASE_FILES       = __aux_Diagrammes_des_Use_Cases;
    e_IMPL_FILES          = __aux_Diagrammes_d_Implementation;

    return;
}
