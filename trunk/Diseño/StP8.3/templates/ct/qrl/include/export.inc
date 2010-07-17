////////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = %W% %Y% %D%
//      StP/Core
//      Confidential property of Aonix
//      Copyright (c) 1992-97
//      All rights reserved
//
//      Utilities for exporting OO model.
//
// 	Public Functions:
//
//		Export
//
//
////////////////////////////////////////////////////////////////////////////////

#include "ct/qrl/include/export_types.inc"
#include "ct/qrl/include/parse.inc"

const boolean exportDebug = False;

// main entry point for Export writer

int
Export(list files, string ofname)
{
    int     i;
    list    fileStrings = list_create("string", 0);
    string  query = "file[(" + euUseCaseDiag + " || " + euClassDiag + ")]";

    if (exportDebug)
    {
	xferLookupTableDump();
    }
    if (files == NULL || list_count(files) == 0)
    {
	// export entire model

	files = list_select(query);
    }
    if (list_count(files) == 0)
    {
	print_message("No files to export.");
	return(0);
    }
    for (i = 0; i < list_count(files); i++)
    {
	exportFileAny(list_get(files, i), fileStrings);
    }
    if (list_count(fileStrings) == 0)
    {
	print_message("No files to export.");
	return(0);
    } 
    if (list_count(fileStrings) == 1)
    {
	print_message("Writing 1 file to '" + ofname + "'...");
    } else
    {
	print_message("Writing " +
		      list_count(fileStrings) +
		      " files to '" + ofname + "'...");
    }
    if (exportWrite(fileStrings, ofname) == 0)
    {
	print_message("Export Succeeded.");
	return(0);
    }
    print_error("Export Failed.");
    return(-1);
}

// get format version to n decimal places

string
floatString(float f, int n)
{
    return(string_extract(to_string(f), 0, 2 + n + to_int(f / 10)));
}

// take the export output, assemble it into one string and blast it
// out to the specified file.

int
exportWrite(list strs, string ofname)
{
    string    output;
    int       i;

    if (file_exists(ofname) && !write_file_access(ofname))
    {
	print_error("Could not write '" + ofname + "'");
	print_message("Check permissions.");
	return(-1);
    }
    output = "ExportFile\n{\n    { Format \"" + 
	     floatString(exportFormat, 1) + 
	     "\" }\n}\n\n";

//    for (i = 0; i < list_count(strs); i++)
//    {
//	output += list_get(strs, i);
//    }
    output += list_to_string(strs, "");
    write_file(ofname, output);

    if (exportDebug)
    {
	print_message("Verifying...");
    }
    if (output != read_file(ofname))
    {
	print_error("Write error, check available disk space.");
	return(-1);
    }
    return(0);
}

// take any of the supported file types and append their export
// substring to the list (ready for export)

const string exportSdQuery = "node_ref[file_id == ${file.id} " +
			     "&& node[${euScenarioNode}]]";

int
exportFileAny(file file, list strs)
{
    if (file.type == euUseCaseDiag)
    {
	if (find_by_query(exportSdQuery) != NULL)
	{
	    return(exportFileSd(file, strs));
	} 
	return(exportFileUcd(file, strs));
    } else if (file.type == euClassDiag)
    {
	return(exportFileCd(file, strs));
    } else if (file.type == euReqTable)
    {
	return(exportFileRt(file, strs));
    } 
    print_message(file.type + 
		  " '" + file.name + "' " +
		  "could not be exported (unsupported type).");
    return(0);
}

// generate export format string for UseCaseDiagram

int
exportFileUcd(file file, list strs)
{
    string    op;

    print_message("Exporting " + ecUseCaseDiag + " '" + file.name + "'...");

    op = exportFileHeader(file, ecUseCaseDiag) + 
	 exportSymbols(file, ecUseCaseDiag) + 
	 "}\n\n";

    list_append(strs, op);
    return(0);
}

// generate export format string for ScenarioDiagram

int
exportFileSd(file file, list strs)
{
    string    op;

    print_message("Exporting " + ecScenarioDiag + " '" + file.name + "'...");

    op = exportFileHeader(file, ecScenarioDiag) + 
	 exportSymbols(file, ecScenarioDiag) + 
	 "}\n\n";

    list_append(strs, op);
    return(0);
}

// generate export format string for ClassDiagram

int
exportFileCd(file file, list strs)
{
    string    op;

    print_message("Exporting " + ecClassDiag + " '" + file.name + "'...");

    op = exportFileHeader(file, ecClassDiag) + 
	 exportSymbols(file, ecClassDiag) + 
	 "}\n\n";

    list_append(strs, op);
    return(0);
}

// generate export format string for Requirement Table

int
exportFileRt(file file, list strs)
{
    string    op;

    print_message("Exporting " + ecReqTable + " '" + file.name + "'...");

    op = exportFileHeader(file, ecReqTable) + 
	 exportSymbols(file, ecReqTable) + 
	 "}\n\n";

    list_append(strs, op);
    return(0);
}

// generate export format substring for File 

string
exportFileHeader(file f, string t)
{
  return("File\n{\n    { Name \"" + f.name + "\" }\n    { Type " + t + " }\n");
}

// generate export format substring for Node & Links in file

string
exportSymbols(file file, string ftype)
{
    string    output = "";
    node_ref  node_ref;
    link_ref  link_ref;

    for_each_in_select("node_ref[file_id == ${file.id}]", node_ref)
    {
	output += exportNode(ftype, node_ref);
    }
    for_each_in_select("link_ref[file_id == ${file.id}]", link_ref)
    {
	output += exportArc(ftype, link_ref);
    } 
    return(output);
}

string
assocName(node n)
{
    list      nodes = string_to_list(n.name, ":,");

    if (list_count(nodes) == 3)
    {
        return(list_get(nodes, 0));
    }
    return("");
}

enum FromTo { From, To };

int
assocFromTo(node_ref nr, node n, FromTo which)
{
    list      nodes = string_to_list(n.name, ":,");
    string    name;
    node      class;
    link      role;
    link_ref  lr;
    int       pos;

    if (which == To)
    {
        pos = 1;
    } else
    {
	pos = 0;
    }
    if (list_count(nodes) == 3)
    {
        pos += 1;
    }
    name = list_get(nodes, pos);

    class = find_by_query("node[OMTClass && name == '${name}']");

    role = find_by_query("link[to_node_id == ${n.id} && " +
		         "from_node_id == ${class.id}]");

    lr = find_by_query("link_ref[link_id == ${role.id} && " +
                       "to_node_ref_id == ${nr.id}]");

    return(lr.from_node_ref_id);
}

int
assocFrom(node_ref nr, node n)
{
    return(assocFromTo(nr, n, From));
}

int
assocTo(node_ref nr, node n)
{
    return(assocFromTo(nr, n, To));
}

string
exportNode(string ftype, node_ref nr)
{
    node      comp, n = find_by_query("node[${nr.node_id}]");
    node_ref  cnr;
    string    extra = "";

    if (exportIsValid(ftype, n.type))
    {
        if (n.type == euAssociationLink)
        {
            string name;
	    list comps = string_to_list(n.name, ":");

	    if (list_count(comps) == 2)
	    {
		name = list_get(comps, 1);
	    } else
	    {
		name = list_get(comps, 0);
	    }
	    return("    { LinkSymbol\n" + 
		   "        { Id " + nr.id + " }\n" +
		   "        { Type " + xferOmtLookup(ftype, n.type) + " }\n" +
		   "        { Name \"" + name + "\" }\n" +
		   "        { FromNode " + assocFrom(nr, n) + " }\n" +
		   "        { ToNode " + assocTo(nr, n) + " }\n" +
		   "        { AssociationName \"" + assocName(n) + "\" }\n" +
		   "    }\n");
        }
        if (n.type == euClassNode)
	{
	    for_each_in_select("node_ref[scope_node_ref_id == ${nr.id}]", cnr)
	    {
	        comp = find_by_query("node[${cnr.node_id}]");
		extra += exportClassComponent(comp);
	    }
	}
	return("    { NodeSymbol\n" + 
	       "        { Id " + nr.id + " }\n" +
	       "        { Type " + xferOmtLookup(ftype, n.type) + " }\n" +
	       "        { Name \"" + n.name + "\" }\n" +
	       "        { Loc " + nr.xcoord + " " + nr.ycoord + " }\n" +
	       extra +
	       "    }\n");
    }
    if (exportDebug)
    {
	print_message("Node " + n.type + " is not valid for export");
    }
    return("");
}

string
exportClassComponent(node comp)
{
    string res;

    if (comp.type == "OMTAttribute")
    {
        res = "        { " + ecClassAttr + "\n";
    } else if (comp.type == "OMTOperation")
    {
        res = "        { " + ecClassOp + "\n";
    } else
    {
        return("");
    }
    res += "            { Name \"" + comp.name + "\" }\n";
    return(res + "        }\n");
}

string
exportArc(string ftype, link_ref lr)
{
    link l = find_by_query("link[${lr.link_id}]");

    if (exportIsValid(ftype, l.type))
    {
	return("    { LinkSymbol\n" + 
	       "        { Id " + lr.id + " }\n" +
	       "        { Type " + xferOmtLookup(ftype, l.type) + " }\n" +
	       "        { Name \"" + l.name + "\" }\n" +
	       "        { FromNode " + lr.from_node_ref_id + " }\n" +
	       "        { ToNode " + lr.to_node_ref_id + " }\n" +
	       "    }\n");
	return("");
    }
    if (exportDebug)
    {
	print_message("Link " + l.type + " is not valid for export");
    }
    return("");
}

int
exportFromNode(Rule sym)
{
    RuleAttribute  attr = RuleAttr(sym, "From");

    if (attr != NULL)
    {
	attr = AttrAttr(attr, "ID");
	return(RuleAttrIntVal(attr));
    }
    return(0);
}

int
exportToNode(Rule sym)
{
    RuleAttribute  attr = RuleAttr(sym, "To");

    if (attr != NULL)
    {
	attr = AttrAttr(attr, "ID");
	return(RuleAttrIntVal(attr));
    }
    return(0);
}

boolean
exportIsValid(string ftype, string objtype)
{
    if (xferOmtLookup(ftype, objtype) != NULL)
    {
	return(True);
    }
    return(False);
}

// return the corresponding export "type"

string
exportType(string ftype, string otype)
{
    string s = xferOmtLookup(ftype, otype);

    if (s == NULL)
    {
	// should never happen since exportIsValid uses the same lookup

	print_error("Internal Error: exportType(" + otype + ")");
    }
    return(s);
}

string
exportSymbolLoc(Rule sym)
{
    RuleAttribute  attr = RuleAttr(sym, "Loc");
    return(RuleAttrIntVal(attr) + " " + RuleAttrOtherIntVal(attr));
}

string
exportSymbolScale(Rule sym)
{
    RuleAttribute  attr = RuleAttr(sym, "Scale");
    return(RuleAttrFloatVal(attr) + " " + RuleAttrOtherFloatVal(attr));
}

string
exportSymbolName(Rule sym)
{
    int            i;
    RuleAttribute  attr = RuleAttr(sym, "Label");
    string         s = "";

    if (attr != NULL)
    {
	for (i = 0; i < list_count(attr.value); i++)
	{
	    if (i > 0)
	    {
		s += "\\n";
	    }
	    s += RuleAttrStringVal(list_get(attr.value, i));
	}
    }
    return("\"" + s + "\"");
}

