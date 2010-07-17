#include "rules/qrl/gde_qrl_std.inc"

const string ANCHOR_SYMBOL = "Anchor";

/* 
    Deletes any anchor points which have no attached arcs.
*/
void
DeleteUnconnectedAnchors()
{
    list    	anchors;
    gde_node 	current_anchor;
    int	    	ix, count;

    anchors = gde_symbols_find_with_types(gde_all_symbols(), 
    	    	    	    	    	  string_to_list(ANCHOR_SYMBOL, ","));

    for (ix = 0, count = list_count(anchors); ix < count; ix = ix + 1)
    {
    	current_anchor = to_gde_node(list_get(anchors, ix));
    	if (list_count(gde_node_inarcs(current_anchor)) == 0
    	    && list_count(gde_node_outarcs(current_anchor)) == 0)
    	    gde_node_delete(current_anchor);
    }
}
