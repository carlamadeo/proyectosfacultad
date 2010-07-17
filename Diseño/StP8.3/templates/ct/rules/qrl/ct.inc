const int       MAXINT = 2147483647;


string
names_to_oms_query_disjunction(string names, string separators)
{
    int             i, n;
    list            name_list;
    string          name_restrictor = "name='";

    if (names != NULL)
    {
	if (separators == NULL)
	    separators = " ";
	name_list = string_to_list(names, separators);
	for (i = 0, n = list_count(name_list); i < n; i = i + 1)
	{
	    name_restrictor = name_restrictor + to_oms_string(list_get(name_list, i));
	    if (i < n - 1)
		name_restrictor = name_restrictor + "'||name='";
	}
    }
    return name_restrictor + "'";
}

// returns a node from an id, or NULL if there isn't any

const string node_from_oid_query = "node[id = ${oid}]";
node
node_from_oid(int oid)
{
  node	r;

  r = find_by_query(node_from_oid_query);
  return(r);
}

// returns a link from an id, or NULL if there isn't any

const string link_from_oid_query = "link[id = ${oid}]";
link
link_from_oid(int oid)
{
  link	r;

  r = find_by_query(link_from_oid_query);
  return(r);
}

// returns a cntx from an id, or NULL if there isn't any

const string cntx_from_oid_query = "cntx[id = ${oid}]";
cntx
cntx_from_oid(int oid)
{
  cntx	r;

  r = find_by_query(cntx_from_oid_query);
  return(r);
}

// returns a file from an id, or NULL if there isn't any
const string file_from_oid_query = "file[id = ${oid}]";

file
file_from_oid(int oid)
{
  file	r;

  r = find_by_query(file_from_oid_query);
  return(r);
}

int
object_annot_fileid(int oid)
{
    node	n;
    link	l;
    cntx	c;
    file	f;

    n = node_from_oid(oid);
    if (n != NULL)
    {
	return(n.annot_file_id);
    }
    l = link_from_oid(oid);
    if (l != NULL)
    {
	return(l.annot_file_id);
    }
    c = cntx_from_oid(oid);
    if (c != NULL)
    {
	return(c.annot_file_id);
    }
    f = file_from_oid(oid);
    if (f != NULL)
    {
	return(f.annot_file_id);
    }
    return(0);
}

const string node_scope_node_query = "node[id = ${onode.scope_node_id}]";
node
node_get_scope_node(node onode)
{
  // returns the scope node of a node, or NULL if there isn't any
  node result;

  if (onode.scope_node_id == 0)
    return NULL;

  result = find_by_query(node_scope_node_query);
  return(result);
}

void
node_build_scope_chain(int oid, list scope_chain)
{

  // returns the scope chain of a node in the list scope_chain
  // order of nodes is outermost first

  node onode;
  onode = node_from_oid(oid);
  while (onode != NULL) {

    if (list_count(scope_chain) == 0)
      list_append(scope_chain, onode);
    else
      list_insert(scope_chain, 0, onode);

    // get the next element in the scope chain
    onode = node_get_scope_node(onode);
  }
}


node
node_toplevel_scope_node(node n)
{
    node	onode = n;

    while(onode.scope_node_id != 0) 
    {
	onode = find_by_query(node_scope_node_query);
	if (onode == NULL)
	{
	    break;
	}
    }
    return(onode);
}

const string    FILE_NAME_SCOPE_SEPARATOR = "__";
const string    FILE_NAME_BLANK_REPLACEMENT = "_";
const	int	MAX_DIAG_NAME_LEN   = 70; // source DIAG_LEN
const int	FILE_NAME_BASE_LENGTH = 65;

/*
 * make a file name from the scope chain
 * If the scope chain is longer than the limit (arg lim) we make
 * the name unique.
 */

string
make_filename_from_scope_chain(list nodes, int lim)
{
    node	node;
    string	filename = "";
    int		i;

    if (list_count(nodes) == 0)
	return "";

    node = list_get(nodes, 0);
    filename = node.name;
    if (string_length(filename) > FILE_NAME_BASE_LENGTH)
    {
	filename = string_extract(filename, 0, FILE_NAME_BASE_LENGTH);
    }


    for (i = 1; i < list_count(nodes) && i < lim; i = i + 1)
    {
	node = list_get(nodes, i);	
	filename = filename + FILE_NAME_SCOPE_SEPARATOR + node.name;
    }

    if (list_count(nodes) > lim)
    {
	filename = create_new_file_in_sequence(filename, NULL);
    }

    return string_search_and_replace(filename, " ", FILE_NAME_BLANK_REPLACEMENT); 
}

list
make_scope_chain_from_filename(string fn)
{
    string	nfn, sno;
    list	l;
    
    nfn = string_search_and_replace(fn, FILE_NAME_SCOPE_SEPARATOR, "\n");

    l = string_to_list(nfn, "\n");
    if (list_count(l) > 0)
    {
	sno = list_get(l, list_count(l)-1);
	if (string_converts(sno, "int") == True)
	{
	    list_delete(l, list_count(l)-1);
	}
    }
    return(l);
}

list 
sort_string_list(list l)
{
    if (l == NULL)
    {
	l = list_create("string", 0);
    }
    return(list_sort(l));
}
 
list 
item_list_to_string_list(list items)
{
    item	one_item;
    int		i;
    list	res;

    res = list_create("string", 0);


    for (i = 0; i < list_count(items); i = i + 1)
    {
	one_item = list_get(items, i);

	if (one_item.value == "")
	    continue;
	list_append(res, one_item.value);
    }

    return res;
}

string
strip_whitespace(string name)
{
  /* return a string consisting of only non-blank chars */

  return (replace_whitespace(name, ""));
} 

string
replace_whitespace(string s, string repl)
{

  /* return a string with sequences of newlines and blanks replaced by repl */

  if (s == NULL)
    return (NULL);

  return (list_to_string(string_to_list(s, " \n\r"), repl));
}

string
replace_newlines(string s, string repl)
{

  /* return a string with sequences of newlines replaced by repl */

  if (s == NULL)
    return (NULL);

  return (list_to_string(string_to_list(s, "\n"), repl));
}

/* return the ceil of numerator/denominator */
int
ceil(int numerator, int denominator)
{
    return (numerator + (denominator - 1)) / denominator;
}

/*
 * center the string, returning a string of length `length` and using `pad`
 * as the padding character
 */
string
string_center(string string, int length, string pad)
{
    string          result;
    int             left, right;

    if (string == NULL || length < 0 || string_length(string) == length)
	return string;

    if (length == 0)
	return "";

    if (string_length(string) < length)
    {
	if (pad == NULL || pad == "")
	    pad = " ";
	if (string_length(pad) > 1)
	    pad = string_extract(pad, 0, 1);
	left = (length - string_length(string)) / 2;
	right = ceil(length - string_length(string), 2);
	result = string_extract(string_repeat(pad, left) + string + string_repeat(pad, right), 0, length);
    } else
    {
	left = (string_length(string) - length) / 2;
	result = string_extract(string, left, length);
    }
    return result;
}

/*
 * right justify the string, returning a string of length `length` and using
 * `pad` as the padding character
 */
string
string_right(string string, int length, string pad)
{
    string          result;

    if (string == NULL || length < 0 || string_length(string) == length)
	return string;

    if (length == 0)
	return "";

    if (string_length(string) < length)
    {
	if (pad == NULL || pad == "")
	    pad = " ";
	if (string_length(pad) > 1)
	    pad = string_extract(pad, 0, 1);
	result = string_repeat(pad, length - string_length(string)) + string;
    } else
    {
	result = string_extract(string, string_length(string) - length, length);
    }
    return result;
}

/*
 * overlay the string `new` onto the string `target` starting at position `n`
 */
string
string_overlay(string new, string target, int n)
{
    string          result;
    int             length_result;
    int             length_new = string_length(new);
    int             length_target = string_length(target);

    if (new == NULL || target == NULL || n < 0 || n >= length_target || length_new == 0)
	return target;
    result = string_extract(target, 0, n) + new;
    length_result = string_length(result);
    if (length_result >= length_target)
	return result;
    return result + string_extract(target, n + length_new, length_target - length_result);
}

int
string_count_char(string s, string c)
{
    int             i, n;
    int             count = 0;

    if (s == NULL || s == "" || c == NULL || c == "")
	return 0;

    if (string_length(c) > 1)
	c = string_extract(c, 0, 1);

    for (i = 0, n = string_length(s); i < n; i = i + 1)
	if (string_extract(s, i, 1) == c)
	    count = count + 1;
    return count;
}

/* min and max */
int
list_min_int(list integers)
{
    int             i, min = MAXINT;

    for (i = 0; i < list_count(integers); i = i + 1)
        min = min(min, list_get(integers, i));
    return min;
}

int
list_max_int(list integers)
{
    int             i, max = -MAXINT;

    for (i = 0; i < list_count(integers); i = i + 1)
        max = max(max, list_get(integers, i));
    return max;
}

int 
min(int x, int y)
{
  if (x <= y)
    return x;
  else
    return y;
}

int 
max(int x, int y)
{
  if (x >= y)
    return x;
  else
    return y;
}

// test if the file dir/name exist that they're readable/writeable
 
list            test_file_access__filedirs = list_create("string", 0);
list            test_file_access__filenames = list_create("string", 0);

boolean
test_file_access(string filedir, string filename, boolean create_dir)
{
    if (list_find(test_file_access__filedirs, 0, filedir) == list_count(test_file_access__filedirs))
    {
	list_append(test_file_access__filedirs, filedir);

 	if (!file_exists(filedir) && create_dir)
	    mkdir(filedir);
	
	if ((!file_exists(filedir)) || (!read_file_access(filedir)) || (!write_file_access(filedir)))
	{
	    print_error("Cannot access output directory '" + filedir + "' -- aborting...", NULL);
	    return False;
	}
    }
    
    if (list_find(test_file_access__filenames, 0, filename) == list_count(test_file_access__filenames))
    {
	list_append(test_file_access__filenames, filename);
	if (file_exists(filename) && ((!read_file_access(filename)) || (!write_file_access(filename))))
	{
	    print_error("Cannot read or write existing output file '" + filename + "' -- aborting...", NULL);
	    return False;
	}
    }
    
    return True;
}

// given a list of files to be appended to and the current file to write to,
// write the appending string

void
file_append(list appending_files, string current_file, string appending_string)
{
    string          file_contents;

    if (current_file != NULL && current_file != "")
    {
	if (appending_files != NULL)
	{
	    if (list_find(appending_files, 0, current_file) !=
		list_count(appending_files))
	    {
		file_contents = read_file(current_file);
		if (file_contents != NULL)
		    appending_string = file_contents + appending_string;
	    } else
		list_append(appending_files, current_file);
	}
	write_file(current_file, appending_string);
    } else
    {
	if (appending_files != NULL && current_file != NULL)
	    list_append(appending_files, current_file);
	print(appending_string);
    }
    return;
}

list
set_to_list(set set)
{
    list            list;
    int             i, n;

    if (set == NULL)
	return NULL;

    list = list_create(set_get_type(set), 0);
    for (i = 0, n = set_count(set); i < n; i = i + 1)
	list_append(list, set_get_element(set, i));

    return list;
}

set
list_to_set(list list)
{
    set             set;
    int             i, n;

    if (list == NULL)
	return NULL;

    set = set_create(list_get_type(list));
    for (i = 0, n = list_count(list); i < n; i = i + 1)
	set_add(set, list_get(list, i));

    return set;
}

list
project_id_list_from_link_list(list links)
{
    int             i;
    link            link;
    list            ids = list_create("int", 0);

    for (i = 0; i < list_count(links); i = i + 1)
    {
	link = list_get(links, i);
	list_append(ids, link.id);
    }
    return ids;
}

list
project_string_attribute_from_se_list(list objects, string attribute)
{
    int             i;
    node            node;
    link            link;
    cntx            cntx;
    string          se;
    list            projection = list_create("string", 0);

    for (i = 0; i < list_count(objects); i = i + 1)
    {
	se = list_get_type(objects, i);
	if (se == "node")
	    node = list_get(objects, i);
	else if (se == "link")
	    link = list_get(objects, i);
	else if (se == "cntx")
	    cntx = list_get(objects, i);
	else
	    continue;
	qrl_eval("list_append(projection, to_string(" + se + "." + attribute + "));");
    }
    return projection;
}

// returns a node from a name and type, or NULL if there isn't any

const string node_from_name_type_query = "node[name=='${name}' && ${type}]";
node
node_from_name_type(string name, string type)
{
  node	r;

  r = find_by_query(node_from_name_type_query);
  return(r);
}

// returns a link from a name and type, or NULL if there isn't any

const string link_from_name_type_query = "link[name=='${name}' && ${type}]";
link
link_from_name_type(string name, string type)
{
  link	r;

  r = find_by_query(link_from_name_type_query);
  return(r);
}

// returns a cntx from a name and type, or NULL if there isn't any

const string cntx_from_name_type_query = "cntx[name=='${name}' && ${type}]";
cntx
cntx_from_name_type(string name, string type)
{
  cntx	r;

  r = find_by_query(cntx_from_name_type_query);
  return(r);
}

// returns a file from a name and type, or NULL if there isn't any
const string file_from_name_type_query = "file[name=='${name}' && ${type}]";

file
file_from_name_type(string name, string type)
{
  file	r;

  r = find_by_query(file_from_name_type_query);
  return(r);
}


const	string	FIND_NXT_FILE_QUERY = "file[${type} && name $ '${fn}*']";

string
create_new_file_in_sequence(string basename, string type)
{
    string	fn, tmp;
    file	f;
    int		scno = 1, v, len;

    if (type == NULL)
    {
	type = current_editor_apptype();
    }
    if ((string_length(basename)+5) > MAX_DIAG_NAME_LEN)
    {
	basename = string_extract(basename, 0, MAX_DIAG_NAME_LEN-5);
    }

    // ECR 5230 begin
    //
    // Replace invalid file name characters with underscores. We have
    // to do it __BEFORE__ running the OMS query because in the other
    // case it will not find anything. This translation map is the same
    // as the one in the source file
    // source/editors.src/libs/tools/ti_filesys.C, function
    // edit_name_is_legal(),
    // which is used to create file system file names, and respectively
    // PDM files name attribute.
    //
    basename = string_translate(basename,
                                " `!#$^&*[]|\\:;\"',<>?/",
                                "_____________________");
    len = string_length(basename);
    if (len > 0 && string_extract(basename, 0, 1) == "-")
        basename = "_" + string_extract(basename, 1, len - 1);
    // ECR 5230 end
    fn = basename;
    len = string_length(basename);
    if (len >= 2)
    {
	if (string_extract(basename, len - 2, 2) != FILE_NAME_SCOPE_SEPARATOR)
	    fn = basename + FILE_NAME_SCOPE_SEPARATOR;
    } else {
	fn = basename + FILE_NAME_SCOPE_SEPARATOR;
    }
    len = string_length(fn);
    //message("DEBUG create_new_file_in_sequence:: len='"+len+"' fn='"+fn+"' basename='"+basename+"' type='"+type+"'");
    int len1 = len;
    for_each_in_select(FIND_NXT_FILE_QUERY, f)
    {
        // the query can return files which are actually shorter than or
        // the same length as the 
        // base name + separator (e.g., Message_1 vs Message__
        // causing qrl run time error
        //message("DEBUG create_new_file_in_sequence:: f.name='"+f.name+"'");
        len1 = string_length(f.name);
        if(len1>len)
        {
            tmp = string_extract(f.name, len, len1 - len);
            if (string_converts(tmp, "int") == True)
            {
                v = to_int(tmp);
                if (v >= scno)
                    scno = v + 1;
            }
        }
        //else message("DEBUG create_new_file_in_sequence:: f.name='"+f.name+"' is too SHORT!! len="+len+" len1="+len1);
    }
    fn = fn + scno;
    return(fn);
}

// do sorting here instead of doing:  
//	sort by node_refs[node[OMTAttribute && scope_node[OMTClass && name = '${classname}']] && file[OMTClassTable]].ycoord 

struct node_noderef
{
    node            node;
    node_ref        node_ref;
};

list
list_select_node_sort_by_table_row(string node_query)
{
    list            nodes_noderefs = list_create("node_noderef", 0);
    node_noderef    node_noderef;
    node            node;
    string          query_string;

    if ( is_product_uml())
        query_string = "node_ref[node_id=${node.id} & file[UmlClassTable]]";
    if ( is_product_omt())
        query_string = "node_ref[node_id=${node.id} & file[OMTClassTable]]";
    /*
     * get a single table reference for each node (sort is safe if there
     * isn't one)
     */
    for_each_in_select(node_query, node)
    {
	node_noderef.node = node;
	node_noderef.node_ref = find_by_query(query_string);
	list_append(nodes_noderefs, node_noderef);
    }

    /* sort the nodes by their references */
    nodes_noderefs = list_sort(nodes_noderefs, "list_select_node_sort_by_table_row_compare_fn");

    /* extract the node list and return it */
    int             i, n;
    list            nodes = list_create("node", 0);
    for (i = 0, n = list_count(nodes_noderefs); i < n; i = i + 1)
    {
	node_noderef = list_get(nodes_noderefs, i);
	list_append(nodes, node_noderef.node);
    }
    return nodes;
}

int
list_select_node_sort_by_table_row_compare_fn(node_noderef n1, node_noderef n2)
{
    int             r1 = MAXINT, r2 = MAXINT;

    if (n1.node_ref != NULL)
	r1 = n1.node_ref.ycoord;
    if (n2.node_ref != NULL)
	r2 = n2.node_ref.ycoord;

    return (r1 - r2);
}

string
field_by_query(string q, string field, string default)
{
    /* given a query, return desired field */
    /*
     * like find_by_query, but will return the desired field, in string
     * format, of the object, default value if no object (WARNING:  NO
     * CHECKING IS DONE FOR THE DESIRED FIELD -- YOU BETTER BE CAREFUL!
     */

    /* get a single object and its type */
    list            l = list_select(q);
    if (l == NULL || list_count(l) == 0)
	return default;
    string          oms_type = list_get_type(l, 0);

    /* polymorphism in qrl! */
    if (oms_type == "node")
    {
	node            o;
    } else if (oms_type == "link")
    {
	link            o;
    } else if (oms_type == "cntx")
    {
	cntx            o;
    } else if (oms_type == "node_ref")
    {
	node_ref        o;
    } else if (oms_type == "link_ref")
    {
	link_ref        o;
    } else if (oms_type == "cntx_ref")
    {
	cntx_ref        o;
    } else if (oms_type == "note")
    {
	note            o;
    } else if (oms_type == "item")
    {
	item            o;
    } else if (oms_type == "file")
    {
	file            o;
    } else if (oms_type == "file_hist")
    {
	file_hist       o;
    } else if (oms_type == "file_lock")
    {
	file_lock       o;
    } else if (oms_type == "viewpoint")
    {
	viewpoint       o;
    }
    o = list_get(l, 0);
    qrl_eval("default = to_string(o." + field + ");");
    return default;
}

string
field_by_type(list l, string type, string field, string default)
{
    /* given a type, return desired field */

    /* get a single object and its type */
    if (l == NULL || list_count(l) == 0)
	return default;
    string          oms_type = list_get_type(l, 0);

    /* polymorphism in qrl! */
    if (oms_type == "node")
    {
	node            o;
    } else if (oms_type == "link")
    {
	link            o;
    } else if (oms_type == "cntx")
    {
	cntx            o;
    } else if (oms_type == "note")
    {
	note            o;
    } else if (oms_type == "item")
    {
	item            o;
    } else if (oms_type == "file")
    {
	file            o;
    } else if (oms_type == "file_hist")
    {
	file_hist       o;
    } else if (oms_type == "file_lock")
    {
	file_lock       o;
    } else if (oms_type == "viewpoint")
    {
	viewpoint       o;
    }
    int             i;
    for (i = 0; i < list_count(l); i = i + 1)
    {
	o = list_get(l, i);
	if (o.type == type)
	{
	    qrl_eval("default = to_string(o." + field + ");");
	    break;
	}
    }
    return default;
}

string
fields_by_type(list l, string type, string field, string default, string sep)
{
    /* given a type, return desired fields, separated by sep */

    /* get a single object and its type */
    if (l == NULL || list_count(l) == 0)
	return default;
    string          oms_type = list_get_type(l, 0);

    string          result = "";

    /* polymorphism in qrl! */
    if (oms_type == "node")
    {
	node            o;
    } else if (oms_type == "link")
    {
	link            o;
    } else if (oms_type == "cntx")
    {
	cntx            o;
    } else if (oms_type == "note")
    {
	note            o;
    } else if (oms_type == "item")
    {
	item            o;
    } else if (oms_type == "file")
    {
	file            o;
    } else if (oms_type == "file_hist")
    {
	file_hist       o;
    } else if (oms_type == "file_lock")
    {
	file_lock       o;
    } else if (oms_type == "viewpoint")
    {
	viewpoint       o;
    }
    int             i;
    for (i = 0; i < list_count(l); i = i + 1)
    {
	o = list_get(l, i);
	if (o.type == type)
	{
	    if (result != "")
		result = result + sep;
	    qrl_eval("result = result + to_string(o." + field + ");");
	}
    }
    if (result == "")
	result = default;
    return result;
}

string
node_name(node node)
{
    if (node == NULL)
	return "";
    return node.name;
}

string
string_identity(string string)
{
    return string;
}

int
strpbrk(string s1, string s2)
{

    /*
     * strpbrk() returns a  pointer  to  the  first  occurrence  in string s1
     * of any character from string s2, or a NULL pointer if no character
     * from s2 exists in s1.
     */

    int             s1l, s2l, i, x;

    if (s1 == NULL || s2 == NULL || (s1l = string_length(s1)) == 0 || (s2l = string_length(s2)) == 0)
	return 0;

    for (i = 0; i < s2l; i = i + 1)
    {
	x = string_find(s1, 0, string_extract(s2, i, 1));
	if (x != s1l)
	    return x + 1;
    }
    return 0;
}

int
strrchr(string s1, string c)
{

    /*
     * strrchr() returns  a  pointer to the last occurrence of c.
     */

    int             s1l, i, x = 0;

    if (s1 == NULL || c == NULL || (s1l = string_length(s1)) == 0 || string_length(c) != 1)
	return 0;

    for (i = 0; i < s1l; i = i + 1)
    {
	if (string_extract(s1, i, 1) == c)
	    x = i + 1;
    }
    return x;
}

string
class_elaborated_app_type(string classname)
{
    node            elaborated_type;

    elaborated_type = find_by_query("node[(Metaclass|ParameterizedClass|InstantiatedClass|ClassUtility|ParameterizedClassUtility|InstantiatedClassUtility) & name='" + to_oms_string(classname) + "' & node_refs]");
    if (elaborated_type != NULL)
	return elaborated_type.type;

    return "OMTClass";
}

////////////////////////////////////////////////////////////////////////////////
//
// ngoae class utility

#include "ct/qrl/include/opt_displaymark.inc"

enum OAECommandTypes
{
    NoteUpdate
};

struct OAECommand
{
    OAECommandTypes type;
    int             oid;
    int             sid;
    int             ooid;
    list            stpem_commands;
};

void
OAECommand__ctorNoteUpdate(OAECommand c, int oid, int sid, string note, list items)
{
    c.type = NoteUpdate;
    c.oid = oid;
    c.sid = 0; /*sid*/  /* do refresh myself, otherwise too many from oae */
    c.ooid = 0;
    c.stpem_commands = items;
    list_insert(c.stpem_commands, 0, note);
    list_append(c.stpem_commands, OAE_CMD_FILE_SAVE);
    list_append(c.stpem_commands, OAE_CMD_ANNOT_UNLOAD);
    return;
}

void
OAECommand__send(list commands, string callback)
{
    int             i, j;
    int             ngoae;
    OAECommand      command;
    string          stpem_command;

    for (i = 0; i < list_count(commands); i = i + 1)
    {
	command = list_get(commands, i);
	ngoae = send_start_edit_annot(command.oid);
	for (j = 0; j < list_count(command.stpem_commands); j = j + 1)
	{
	    stpem_command = list_get(command.stpem_commands, j);
	    editor_send_msg(NGOAE_CLASS, stpem_command, ngoae);
	}
    }
    if (list_count(commands) > 0 && callback != NULL && callback != "")
    {
	editor_send_msg(NGOAE_CLASS, "EditorQrlEval \"editor_send_msg(\\\"" + editor_get_msgd_name() + "\\\",\\\"EditorQrlEvalNoSave " + callback + "\\\"," + editor_get_msgd_handle() + ");\"", ngoae);
    }
    return;
}

void
annlist_append_value(list l, string item_type, string item_value, string item_value_del)
{
    list_append(l, OAE_CMD_ITEM_SEL + item_type);
    if (item_value_del != NULL && item_value == item_value_del)
	list_append(l, OAE_CMD_ITEM_DEL);
    else
	list_append(l, OAE_CMD_ITEM_SETVAL + stpem_string_val(item_value));
    return;
}

void
annlist_append_values(list l, string item_type, list item_value_adds, list item_value_dels)
{
    int             i;

    for (i = 0; i < list_count(item_value_dels); i = i + 1)
    {
	list_append(l, OAE_CMD_ITEM_SEL + item_type);
	list_append(l, OAE_CMD_ITEM_DEL);
    }
    for (i = 0; i < list_count(item_value_adds); i = i + 1)
    {
	list_append(l, "ItemAdd " + item_type);
	list_append(l, OAE_CMD_ITEM_SETVAL + stpem_string_val(string_strip(list_get(item_value_adds, i), "B", " ")));
    }
    return;
}

////////////////////////////////////////////////////////////////////////////////
//
// prop sheet class utility

string          PropSheet__editor = NULL;
string
PropSheet__editor()
{
    if (PropSheet__editor == NULL)
	PropSheet__editor = editor_get_msgd_name();
    return PropSheet__editor;
}

string          PropSheet__editor_type = NULL;
string
PropSheet__editor_type()
{
    if (PropSheet__editor_type == NULL)
    {
	string          editor_type = PropSheet__editor();
	if (editor_type == "cte")
	    PropSheet__editor_type = "gte";
	else if (editor_type == "uclasst")
	    PropSheet__editor_type = "gte";
	else if (editor_type == "ustatet")   // ECR 8008
	    PropSheet__editor_type = "gte";
	else if (editor_type == "uclass")
	    PropSheet__editor_type = "gde";
	else if (editor_type == "class")
	    PropSheet__editor_type = "gde";
	else if (editor_type == "ome")
	    PropSheet__editor_type = "gde";
	else if (editor_type == "bach")
	    PropSheet__editor_type = "gde";
	else if (editor_type == "chen")
	    PropSheet__editor_type = "gde";
	else
	    PropSheet__editor_type = "gde";
    }
    return PropSheet__editor_type;
}

void
PropSheet__show(string name)
{
    if (PropSheet__editor_type() == "gte")
    {
	gte_stpem("PropertySheetShow " + name);
    } else if (PropSheet__editor_type() == "gde")
    {
	gde_defer_action_completed(1);
	gde_builtin("PropertySheetShow " + name);
	gde_defer_action_completed(0);
    }
    return;
}

void
PropSheet__print_error(string m)
{
    if (PropSheet__editor_type() == "gte")
	gte_print_error(m);
    else if (PropSheet__editor_type() == "gde")
	gde_print_error(m);
    return;
}

list
PropSheet___selected_symbols()
{
    if (PropSheet__editor_type() == "gte")
	return gte_selected_symbols();
    if (PropSheet__editor_type() == "gde")
	return gde_selected_symbols();
    return NULL;
}

list
PropSheet___selected_class_symbols()
{
    if (PropSheet__editor() == "uclassd")
	return uml_selected_class_symbols();
    if (PropSheet__editor() == "cte")
	return gte_selected_class_symbols();
    if (PropSheet__editor() == "uclass")
	return booch_selected_class_symbols();
    if (PropSheet__editor() == "class")
	return booch_selected_class_symbols();
    if (PropSheet__editor() == "ome")
	return omt_selected_class_symbols();
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////


int
generateProbablyUniqueId(string namedGenerator, string itemTypeName)
{
    int             id;

    id = sys_id_update(namedGenerator);
    if ( find_by_query("item[${itemTypeName} && value='${id}']") != NULL )
    {
	// need to do some remedial actions here
	item the_item;
	int max_id = id, x;
	for_each_in_select( "item[${itemTypeName} && value > '${id}']",
			      the_item )
	  {
	    x = to_int( the_item.value );
	    if ( x > max_id )
	      max_id = x;
	  }
	id = sys_id_correct(namedGenerator, max_id + 1);
    }

    return id;
}

// ECR 3791 begin
boolean
us_papersizes() {
    // the default is to return true (if the TI var is not set)
    if( toolinfo_variable( "use_us_papersizes" ) == "False" )
        return False;
    else
        return True;
}
// ECR 3791 end