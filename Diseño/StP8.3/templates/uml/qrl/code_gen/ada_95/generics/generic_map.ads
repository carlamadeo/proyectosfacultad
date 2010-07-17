generic

    type key is private;
    type value is private;

package generic_map is

    type map is private;

    procedure bind     (m: map; k: key; v: value);
    procedure unbind   (m: map; k: key; v: value);
    function  is_bound (m: map; k: key) return boolean;
    function  value_of (m: map; k: key) return value;

private

    type map_type;
    type map is access map_type;

end generic_map;

