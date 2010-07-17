////////////////////////////////////////////////////////////////////////////////
//
//      sccsid[] = %W% %Y% %D%
//      StP/Core
//      Confidential property of Aonix
//      Copyright (c) 1992-97
//      All rights reserved
//
//      Utilities for importing OO model.
//
// 	Public Functions:
//
//		Import
//
//
////////////////////////////////////////////////////////////////////////////////

#include "ct/qrl/include/export_types.inc"
#include "ct/qrl/include/parse.inc"

const boolean importDebug = False;

struct importFileObj
{
    string	name;
    string	type;
    list	nodes;
    list	links;
};

///////////////////////////////////////////////////////////////////////////
//
// main entry point for Import reader
//
// takes the pathname of the import file
//
///////////////////////////////////////////////////////////////////////////

int
Import(string ifname)
{
    list             rules = RuleParseFile(ifname);
    list             files = list_create("importFileObj", 0);
    int              i, rc = 0;
    float            format;
    string           fs, cfs;
    importFileObj    file;

    if (rules == NULL || list_count(rules) == 0)
    {
	print_error("Nothing to Import.");
	return(-1);
    }
    format = importFormat(rules);

    if (format != exportFormat)
    {
	fs = floatString(format, 1);
	cfs = floatString(exportFormat, 1);

	if (format < 0)
	{
	    print_error(ifname + " is not a valid Import file.");
	    return(-1);
	} else if (format < exportFormat)
	{
	    print_message("Importing version " + fs + " export file.");
	    print_message("Current version is " + cfs + ".");
	} else
	{
	    print_error("Cannot import version " + fs + " file.");
	    print_message("Current version is " + cfs + ".");
	    return(-1);
	}
    }
    rc += importFiles(rules, files);

    if (rc < 0)
    {
	print_error("File '" + ifname + "' contains errors");
	return(-1);
    }
    if (importConfirm(files))
    {
	for (i = 0; i < list_count(files); i++)
	{
	    rc += importOneFile(list_get(files, i));
	}
	print_message("Imported " + list_count(files) + " file(s)");
    } else
    {
	print_message("Import Cancelled.");
    }
    system("stputil -p ${projdir} -s ${system} -Force -C 'undef all'");

    for (i = 0; i < list_count(files); i++)
    {
	file = list_get(files, i);

	print_message("Updating " + file.type + " '" + file.name + "'...");

	system("dupdate -replace -update -p ${projdir} -s ${system} -e " +
	       list_get(file_editor_names(xferOmtFiletype(file.type)), 0) +
	       " ${file.name}");
    }
    print_message("Imported " + list_count(files) + " file(s)");
    return(rc);
}

// confirm overwrite if any conflicts

boolean
importConfirm(list files)
{
    return(True);
}

// get the files to import

int
importFiles(list rules, list files)
{
    int     i, rc = 0;
    Rule    rl;

    for (i = 0; i < list_count(rules); i++)
    {
	rl = list_get(rules, i);

	if (rl.name == "ExportFile")
	{
	    continue;
	} 
	if (rl.name == "File")
	{
	    rc += importFile(rl, files);

	    if (rc < 0)
	    {
		break;
	    }
	} else
	{
	    print_error("Unrecognized rule '" + 
			rl.name + 
			"' at line " +
			rl.line);
	    rc -= 1;
	}
    }
    return(rc);
}

// get the import file format

float
importFormat(list rules)
{
    int              i, j;
    Rule             rl;
    RuleAttribute    attr;
    
    for (i = 0; i < list_count(rules); i++)
    {
	rl = list_get(rules, i);

	if (rl.name == "ExportFile")
	{
	    for (j = 0; j < list_count(rl.attrs); j++)
	    {
		attr = list_get(rl.attrs, j);

		if (attr.name == "Format")
		{
		    if (string_converts(list_get(attr.value, 0), "float"))
		    {
			return(to_float(list_get(attr.value, 0)));
		    } 
		}
	    }
	}
    }
    return(-1.0);
}

// take a File rule and create an importFileObj

int
importFile(Rule rl, list files)
{
    int              i;
    RuleAttribute    attr;
    importFileObj    file;

    file.nodes = list_create("RuleAttribute", 0);
    file.links = list_create("RuleAttribute", 0);
    
    for (i = 0; i < list_count(rl.attrs); i++)
    {
	attr = list_get(rl.attrs, i);

	if (attr.name == "Name")
	{
	    file.name = RuleAttrStringVal(attr);
	} else if (attr.name == "Type")
	{
	    file.type = RuleAttrStringVal(attr);
	} else if (attr.name == "NodeSymbol")
	{
	    list_append(file.nodes, attr);
	} else if (attr.name == "LinkSymbol")
	{
	    list_append(file.links, attr);
	} else
	{
	    print_error("Unrecognized File Attribute '" + 
			attr.name + 
			"' at line " +
			attr.line);
	    return(-1);
	}
    }
    if (file.name == "" || file.type == "")
    {
	print_error("Invalid File Rule at line " + rl.line);
	return(-1);
    } 
    list_append(files, file);
    return(0);
}

struct importDiag
{
    string filename;
    string contents;
};

// do the actual import

int diagUniqueId;

int
importOneFile(importFileObj file)
{
    importDiag diag;
    string     ftype = xferOmtFiletype(file.type);

    if (ftype == "")
    {
	print_error("'" + file.type + "' is not a supported file type.");
	return(-1);
    }
    print_message("Importing " + file.type + " '" + file.name + "'...");

    diag.filename = current_projdir() + 
		    current_system() + "/" + 
		    file_filestore(ftype) + "/" + 
		    file.name +
		    file_suffix(ftype);

    if (importDebug)
    {
	print_message("  filename is '" + diag.filename + "'");
    }
    diagUniqueId = 0;

    diag.contents = importDiagHeader(file, ftype) +
		    importDiagNodes(file.nodes, file.type) +
		    importDiagLinks(file.links, file.type);

    return(importWrite(diag));
}

// write the diagram file out

int
importWrite(importDiag diag)
{
    if (file_exists(diag.filename)) 
    {
	if (read_file_access(diag.filename))
	{
	    // backup existing file
	    write_file(diag.filename + "%", read_file(diag.filename));

	    if (read_file(diag.filename + "%") != read_file(diag.filename))
	    {
		print_error("Backup '" + diag.filename + "' failed.");
		print_message("Check directory permissions and disk space");
		return(-1);
	    } else
	    {
		print_message(diag.filename + 
			      " copied to " + 
			      diag.filename + "\%");
	    }
	} 
	if (!write_file_access(diag.filename))
	{
	    print_error("Could not write '" + diag.filename + "'.");
	    print_message("Check file permissions");
	    return(-1);
	} 
    } 
    write_file(diag.filename, diag.contents);

    if (importDebug)
    {
	write_file(diag.filename + ".pre", read_file(diag.filename));
	print_message("Verifying...");
    }
    if (read_file(diag.filename) != diag.contents)
    {
	print_error("Error writing '" + diag.filename + "'.");
	print_message("Check available disk space");
	return(-1);
    }
    return(0);
}

// return the appropriate header for the diagram file

string
importDiagHeader(importFileObj file, string ftype)
{
    return("Diagram " + ftype + "\n{\n" +
	   "    { Name \"" + file.name + "\" }\n" + 
	   "    { Format \"1.5\" }\n" +
	   "    { Scale 1.00 }\n" +
	   "    { Alignment 32 }\n" +
	   "    { Summary\n" +
	   "        { Nodes " + list_count(file.nodes) + " }\n" +
	   "        { Arcs " + list_count(file.links) + " }\n" +
	   "    }\n}\n\n");
}

// return all the appropriate Node rules that should go in the file

string
importDiagNodes(list nodes, string ftype)
{
    int      i;
    string   s;

    for (i = 0, s = ""; i < list_count(nodes); i++)
    {
	s += importDiagNode(list_get(nodes, i), ftype);
    }
    return(s);
}

string
importLabel(RuleAttribute attr)
{
    string val = list_get(attr.value, 0);

    if (val == "")
	return(val);

    return("    { Label\n        { Text \"" + val + "\" }\n    }\n");
}

string
importParent(RuleAttribute attr, list links)
{
    RuleAttribute a;
    int 	  i, id = RuleAttrIntVal(AttrAttr(attr, "Id"));

    for (i = 0; i < list_count(links); i++)
    {
	a = AttrAttr(list_get(links, i), "ToNode");

	if (RuleAttrIntVal(a) == id)
	{
	    return("    { IsIncluded True }\n    { Parent " + 
		   RuleAttrIntVal(AttrAttr(list_get(links, i), "FromNode")) + 
		   " }\n");
	}
    }
    return("");
}

string
importDiagNode(RuleAttribute attr, string ftype)
{
    int            i, id;
    string         s, hdr, t, extra = "";
    RuleAttribute  sattr;

    for (i = 0, s = ""; i < list_count(attr.value); i++)
    {
	sattr = list_get(attr.value, i);

	if (sattr.name == "Id")
	{
	    id = list_get(sattr.value, 0);
	    hdr = "Node " + id + "\n{\n";
	} else if (sattr.name == "Type")
	{
	    t = list_get(sattr.value, 0);
	    s += "    { Type " + xferCommonLookup(ftype, t, Diagram) + " }\n";
	} else if (sattr.name == "Name")
	{
	    s += importLabel(sattr);
	} else if (sattr.name == "Loc")
	{
	    s += "    { Loc " +
		 list_get(sattr.value, 0) + " " +
		 list_get(sattr.value, 1) + " }\n";
	} else if (sattr.name == "Scale")
	{
	    s += "    { Scale " +
		 list_get(sattr.value, 0) + " " +
		 list_get(sattr.value, 1) + " }\n";
        } else if (sattr.name == ecClassAttr || sattr.name == ecClassOp)
	{
	    extra += importClassComponents(sattr, id);
	} else
	{
	    print_error("Invalid attribute '" + sattr.name + 
			"' at line " + sattr.line + ".");
	}
    }
    return(hdr + s + "}\n\n" + extra);
}

string
importClassComponents(RuleAttribute attr, int classId)
{
    string         s = "";
    int            i, id;
    RuleAttribute  sattr;

    s = "Node " + ++diagUniqueId + "\n{\n";
    id = diagUniqueId;

    if (attr.name == ecClassAttr)
    {
	s += "    { Type " + edClassAttr + " }\n";
    } else
    if (attr.name == ecClassOp)
    {
	s += "    { Type " + edClassOp + " }\n";
    } else
    {
        return("");
    }
    for (i = 0; i < list_count(attr.value); i++)
    {
	sattr = list_get(attr.value, i);

	if (sattr.name == "Type")
	{
	    // add component type annotation
	} else if (sattr.name == "Name")
	{
	    s += importLabel(sattr);
	} else
	{
	    print_error("Invalid attribute '" + sattr.name + 
			"' at line " + sattr.line + ".");
	}
    }
    s += "    { IsIncluded True }\n" +
         "    { Parent " + classId + " }\n}\n\n" +
         "Arc " + ++diagUniqueId + "\n{\n" +
	 "    { Type ClassComponentLink }\n" +
	 "    { From\n        { ID " + classId + " }\n    }\n" +
	 "    { To\n        { ID " + id + " }\n    }\n" +
	 "    { IsInclusion True }\n}\n\n";
    return(s);
}

// return all the appropriate Link rules that should go in the file

string
importDiagLinks(list links, string ftype)
{
    int      i;
    string   s;

    for (i = 0, s = ""; i < list_count(links); i++)
    {
	s += importDiagLink(list_get(links, i), ftype);
    }
    return(s);
}

string
importDiagLink(RuleAttribute attr, string ftype)
{
    int            i;
    string         s, hdr, t;
    RuleAttribute  sattr;

    for (i = 0, s = ""; i < list_count(attr.value); i++)
    {
	sattr = list_get(attr.value, i);

	if (sattr.name == "Id")
	{
	    hdr = "Arc " + list_get(sattr.value, 0) + "\n{\n";
	} else if (sattr.name == "Type")
	{
	    t = list_get(sattr.value, 0);
	    s += "    { Type " + xferCommonLookup(ftype, t, Diagram) + " }\n";
	} else if (sattr.name == "Name")
	{
	    if (t != ecAssociationLink)
	    {
		s += importLabel(sattr);
	    }
	} else if (sattr.name == "FromNode")
	{
	    s += "    { From\n" +
		 "        { ID " + list_get(sattr.value, 0) + " }\n" +
		 "        { Offset 0 0 }\n    }\n";
	} else if (sattr.name == "ToNode")
	{
	    s += "    { To\n" +
		 "        { ID " + list_get(sattr.value, 0) + " }\n" +
		 "        { Offset 0 0 }\n    }\n";
	} else if (sattr.name == "AssociationName")
        {
	    s += importLabel(sattr);
	} else
	{
	    print_error("Invalid attribute '" + sattr.name + 
			"' at line " + sattr.line + ".");
	}
    }
    return(hdr + s + "}\n\n");
}

