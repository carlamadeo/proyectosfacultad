--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#
package body String_Item_Type_pkg.PrivateAccessOps is

    --Attribute operations----------------------------------------------------------
    function getItem(this : String_Item_Type_ptr) return String is
    begin
        return this.Item;

    end getItem;

    procedure setItem(acc_this : String_Item_Type_ptr; value : String) is
    begin
        acc_this.Item := value;
    end setItem;

    function getLength(this : String_Item_Type_ptr) return Integer is
    begin
        return this.Length;

    end getLength;


end String_Item_Type_pkg.PrivateAccessOps;
