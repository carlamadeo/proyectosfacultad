generic

    type item is private;

package generic_set is 

    type set is private;

    procedure add       (s: set; i: item);
    procedure remove    (s: set; i: item);
    function  is_member (s: set; i: item) return boolean;

private

    type set_type;
    type set is access set_type;

end generic_set;

