--#ACD# M(UDIF) includes
   -- user defined code to be added here ...
--#end ACD#
private package String_Item_Type_pkg.PrivateAccessOps is

    --Attribute operations----------------------------------------------------------
    function getItem(this : String_Item_Type_ptr) return String;
        -- pragma inline(getItem);
    procedure setItem(acc_this : String_Item_Type_ptr; value : String);
        -- pragma inline(setItem);
    function getLength(this : String_Item_Type_ptr) return Integer;
        -- pragma inline(getLength);

end String_Item_Type_pkg.PrivateAccessOps;
