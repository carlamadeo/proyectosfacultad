with Ada.Finalization;
generic
    type Element_Cptr is private;
    null_value : Element_Cptr;
    -- We expect Element_Cptr to be an access type but don't really care for the referenced type.

    with procedure free (item : in out Element_cptr);

package ACD_Runtime.Smart_Pointer_pkg is

    type Smart_Pointer is new Ada.Finalization.Controlled with private;

    function Create(Contents : in Element_Cptr) return Smart_Pointer;

    procedure Move (From : in out Element_Cptr; To : out Smart_Pointer);
    -- Move(P,S) is a shortcut for S:=Create(X); X := null;
    -- This was introduced to make clear that the original value X must not be used after converting
    -- to a smart pointer.

    function Read(This : Smart_Pointer) return Element_Cptr;
    function "=" (L : Smart_Pointer; R : Element_Cptr) return Boolean;
    function "+"(This : Smart_Pointer) return Element_Cptr renames Read;

    procedure Adjust   (Object : in out Smart_Pointer);
    procedure Finalize (Object : in out Smart_Pointer);

private
    type Counted_Data;
    type Counted_Data_Ptr is access Counted_Data;
    type Smart_Pointer is new Ada.Finalization.Controlled with
        record
            Ptr : Counted_Data_Ptr;
        end record;
end ACD_Runtime.Smart_Pointer_pkg;
