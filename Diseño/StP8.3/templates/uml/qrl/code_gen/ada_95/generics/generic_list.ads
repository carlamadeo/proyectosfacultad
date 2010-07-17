generic

    type item is private;

package generic_list is 

    type list is private;

private

    type list_type;
    type list is access list_type;

end generic_list;

