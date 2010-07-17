////////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = %W% %Y% %D%
//      StP/Core
//      Confidential property of Aonix
//      Copyright (c) 1992-97
//      All rights reserved
//
//      Object types for import/export.
//
////////////////////////////////////////////////////////////////////////////////

// eu prefix = object type in OMT repository
// ed prefix = symbol type in OMT diagram
// ec prefix = object type in common export format

// when a new object type is added the corresponding lookup entry
// must be added to the table in xferInit()

const float exportFormat = 1.0;

////////////////////////////////////////////////////////////////
//
// file types
//
////////////////////////////////////////////////////////////////

const string euUseCaseDiag = "OMTDiagram";
const string ecUseCaseDiag = "UseCaseDiagram";

const string euClassDiag = "OMTObjectDiagram";
const string ecClassDiag = "ClassDiagram";

const string euScenarioDiag = "OMTDiagram";
const string ecScenarioDiag = "ScenarioDiagram";

const string euReqTable = "REQTable";
const string ecReqTable = "RequirementModel";

////////////////////////////////////////////////////////////////
//
// node and link object types (UseCase Diagrams)
//
////////////////////////////////////////////////////////////////

const string euSystemNode = "OMTOOANode";
const string edSystemNode = "System";
const string ecSystemNode = "TheSystem";

const string euUseCaseNode = "OMTUseCase";
const string edUseCaseNode = "UseCase";
const string ecUseCaseNode = "UseCase";

const string euUseCasePre = "OMTUseCasePreCond";
const string edUseCasePre = "UseCasePreCond";
const string ecUseCasePre = "PreCondition";

const string euUseCasePost = "OMTUseCasePostCond";
const string edUseCasePost = "UseCasePostCond";
const string ecUseCasePost = "PostCondition";

const string euUseCaseExc = "OMTUseCaseException";
const string edUseCaseExc = "UseCaseException";
const string ecUseCaseExc = "Exception";

const string euActorNode = "OMTUseCaseActor";
const string edActorNode = "UseCaseActor";
const string ecActorNode = "Actor";

const string euInteractionLink = "OMTUseCaseInteraction";
const string edInteractionLink = "UseCaseInteraction";
const string ecInteractionLink = "Interaction";

const string euActorInheritanceLink = "OMTUseCaseActorInheritance";
const string edActorInheritanceLink = "UseCaseActorInheritance";
const string ecActorInheritanceLink = "ActorInheritance";

const string euUseCaseUsesLink = "OMTUseCaseUses";
const string edUseCaseUsesLink = "UseCaseUses";
const string ecUseCaseUsesLink = "UseCaseUses";

////////////////////////////////////////////////////////////////
//
// node and link object types (Scenario Diagrams)
//
////////////////////////////////////////////////////////////////

const string euScenarioNode = "OMTScenarioName";
const string edScenarioNode = "ScenarioName";
const string ecScenarioNode = "ScenarioName";

const string euEventLink = "OMTEventTrace";
const string edEventLink = "EventTrace";
const string ecEventLink = "EventTrace";

////////////////////////////////////////////////////////////////
//
// node and link object types (Class Diagrams)
//
////////////////////////////////////////////////////////////////

const string euClassNode = "OMTClass";
const string edClassNode = "Class";
const string ecClassNode = "Class";

const string euObjectInstNode = "OMTObjectInstance";
const string edObjectInstNode = "ObjectInstance";
const string ecObjectInstNode = "Object";

const string euInheritanceLink = "Inheritance";
const string edInheritanceLink = "Inheritance";
const string ecInheritanceLink = "Inheritance";

const string euAggregationLink = "AggregationLink";
const string edAggregationLink = "AggregationLink";
const string ecAggregationLink = "Aggregation";

const string euInstantiationLink = "Instantiation";
const string edInstantiationLink = "Instantiation";
const string ecInstantiationLink = "Instantiation";

// Attributes and Operations are special cases so don't
// put the regular OMT names in here.

const string euClassAttr = "Attribute";
const string edClassAttr = "Attribute";
const string ecClassAttr = "Attribute";

// Attributes and Operations are special cases so don't
// put the regular OMT names in here.

const string euClassOp = "Operation";
const string edClassOp = "Operation";
const string ecClassOp = "Operation";

const string euAssociationLink = "OMTAssociation";
const string edAssociationLink = "AssociationLink";
const string ecAssociationLink = "ClassAssociation";

////////////////////////////////////////////////////////////////
//
// node and link object types (Requirement Tables)
//
////////////////////////////////////////////////////////////////

const string euRequirement = "REQRequirement";
const string edRequirement = "REQRequirement";
const string ecRequirement = "Requirement";

////////////////////////////////////////////////////////////////////////////////
//
// DO NOT CHANGE OBJECT NAMES BELOW THIS LINE.
//
// If mappings change in either product then the corresponding changes
// should be made to the string constants above. Everything below this
// line (and in other export related code) refers only to the string
// constants by their variable names.
//
////////////////////////////////////////////////////////////////////////////////

// lookup table (list) structure for mapping names between OMT & export

struct xferLookup {
    string omt;			// object name in OMT repository
    string diagram;		// symbol name in OMT diagram
    string ftype;		// file that it is valid in
    string common;		// object name in export file
};

enum reposOrDiagram {
    Repository,
    Diagram
};

list xferLookupTable = list_create("xferLookup", 0);

// xferInit populates the lookup table for mapping names between
// OMT product and export file format.

void
xferInit()
{
    if (list_count(xferLookupTable) != 0)
    {
	// already done
	return;
    }
    ////////////////////////////////////////////////////////////////
    //
    // add lookup entries for symbols in Scenario Diagrams...
    //
    ////////////////////////////////////////////////////////////////

    list_append(xferLookupTable, xferLookupCreate(euScenarioNode, 
						  edScenarioNode, 
						  ecScenarioDiag, 
						  ecScenarioNode));

    list_append(xferLookupTable, xferLookupCreate(euClassNode, 
    						  edClassNode,
						  ecScenarioDiag, 
						  ecClassNode));

    list_append(xferLookupTable, xferLookupCreate(euEventLink, 
    						  edEventLink,
						  ecScenarioDiag, 
						  ecEventLink));

    ////////////////////////////////////////////////////////////////
    //
    // add lookup entries for symbols in UseCase Diagrams...
    //
    ////////////////////////////////////////////////////////////////

    list_append(xferLookupTable, xferLookupCreate(euSystemNode, 
						  edSystemNode, 
						  ecUseCaseDiag, 
						  ecSystemNode));

    list_append(xferLookupTable, xferLookupCreate(euUseCaseNode, 
						  edUseCaseNode,
						  ecUseCaseDiag, 
						  ecUseCaseNode));

    list_append(xferLookupTable, xferLookupCreate(euActorNode, 
						  edActorNode,
						  ecUseCaseDiag, 
						  ecActorNode));

    list_append(xferLookupTable, xferLookupCreate(euUseCasePre, 
						  edUseCasePre,
						  ecUseCaseDiag, 
						  ecUseCasePre));

    list_append(xferLookupTable, xferLookupCreate(euUseCasePost, 
						  edUseCasePost,
						  ecUseCaseDiag, 
						  ecUseCasePost));

    list_append(xferLookupTable, xferLookupCreate(euUseCaseExc, 
						  edUseCaseExc,
						  ecUseCaseDiag, 
						  ecUseCaseExc));

    list_append(xferLookupTable, xferLookupCreate(euInteractionLink, 
						  edInteractionLink,
						  ecUseCaseDiag, 
						  ecInteractionLink));

    list_append(xferLookupTable, xferLookupCreate(euActorInheritanceLink, 
						  edActorInheritanceLink,
						  ecUseCaseDiag, 
						  ecActorInheritanceLink));

    list_append(xferLookupTable, xferLookupCreate(euUseCaseUsesLink, 
						  edUseCaseUsesLink,
						  ecUseCaseDiag, 
						  ecUseCaseUsesLink));

    ////////////////////////////////////////////////////////////////
    //
    // add lookup entries for symbols in Class Diagrams...
    //
    ////////////////////////////////////////////////////////////////

    list_append(xferLookupTable, xferLookupCreate(euClassNode, 
						  edClassNode,
						  ecClassDiag, 
						  ecClassNode));

    list_append(xferLookupTable, xferLookupCreate(euObjectInstNode, 
						  edObjectInstNode,
						  ecClassDiag, 
						  ecObjectInstNode));

    list_append(xferLookupTable, xferLookupCreate(euInheritanceLink, 
						  edInheritanceLink,
						  ecClassDiag, 
						  ecInheritanceLink));

    list_append(xferLookupTable, xferLookupCreate(euAggregationLink, 
						  edAggregationLink,
						  ecClassDiag, 
						  ecAggregationLink));

    list_append(xferLookupTable, xferLookupCreate(euInstantiationLink, 
						  edInstantiationLink,
						  ecClassDiag, 
						  ecInstantiationLink));

    list_append(xferLookupTable, xferLookupCreate(euClassAttr, 
						  edClassAttr,
						  ecClassDiag, 
						  ecClassAttr));

    list_append(xferLookupTable, xferLookupCreate(euClassOp, 
						  edClassOp,
						  ecClassDiag, 
						  ecClassOp));

    list_append(xferLookupTable, xferLookupCreate(euAssociationLink, 
						  edAssociationLink,
						  ecClassDiag, 
						  ecAssociationLink));
}

xferLookup
xferLookupCreate(string a, string b, string c, string d)
{
    xferLookup l;

    l.omt = a;
    l.diagram = b;
    l.ftype = c;
    l.common = d;
    return(l);
}

// map OMT object name to an export object (qulaified by the filetype)

string
xferOmtLookup(string f, string s)
{
    xferLookup res;
    int        i;

    xferInit();

    for (i = 0; i < list_count(xferLookupTable); i++)
    {
	res = list_get(xferLookupTable, i);

	if (res.omt == s && res.ftype == f)
	{
	    return(res.common);
	}
    }
    return(NULL);
}

// map a common object name to an OMT object (qulaified by the filetype)

string
xferCommonLookup(string f, string s, reposOrDiagram which)
{
    xferLookup res;
    int        i;

    xferInit();

    for (i = 0; i < list_count(xferLookupTable); i++)
    {
	res = list_get(xferLookupTable, i);

	if (res.common == s && res.ftype == f)
	{
	    if (which == Repository)
	    {
	        return(res.omt);
	    } else
	    {
		return(res.diagram);
	    }
	}
    }
    return(NULL);
}

// map a common export filetype to the OMT equivalent

string
xferOmtFiletype(string s)
{
    if (s == ecUseCaseDiag)
	return(euUseCaseDiag);
    if (s == ecClassDiag)
	return(euClassDiag);
    if (s == ecScenarioDiag)
	return(euScenarioDiag);
    return("");
}

// dump the contents of the lookup table (for debugging)

void
xferLookupTableDump()
{
    xferLookup res;
    int        i;
    string     last = "";

    xferInit();
    print_message("--------------------------------------------------------");

    for (i = 0; i < list_count(xferLookupTable); i++)
    {
	res = list_get(xferLookupTable, i);

	if (res.ftype != last)
	{
	    print_message(res.ftype);
	    last = res.ftype;
	}
	print_message("    " + res.omt + " == " + res.common);
    }
    print_message("--------------------------------------------------------");
}
