const string COMMENT = "Comment"; 
const string ANCHOR =	"Anchor";

const string DATA_DIAGRAMS = "(BachDiagram || ChenDiagram || DseDiagram)";
const string ERE_TYPE = "EntityGroup";
const list DSE_TYPE_LIST = string_to_list("Sequence,Iteration,Selection", ",");
const string DSE_TYPES = "(" + list_to_string(DSE_TYPE_LIST, " || ") + ")";

enum direction {IN, OUT};

void
print_time(string title)
{
   message(title + " " + time_to_string(time_now(), NULL));
}
