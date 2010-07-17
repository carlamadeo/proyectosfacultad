with unchecked_deallocation;
with ACD_Runtime.Synchronized_counter_pkg;
with ada.text_io;
with ada.exceptions;

package body ACD_Runtime.Smart_Pointer_pkg is

    type Counted_Data is record
        Counter : aliased Synchronized_counter_pkg.counter;
        Data    : Element_Cptr := null_value;
    end record;
    
    procedure Free is new Unchecked_Deallocation (Counted_Data, Counted_Data_Ptr);
    
    procedure Adjust (Object : in out Smart_Pointer) is
    begin
        if Object.Ptr /= null then
            Synchronized_counter_pkg.increment(Object.Ptr.Counter'access);
        end if;
    end Adjust;
    
    procedure Finalize (Object : in out Smart_Pointer) is
    begin
        if Object.Ptr /= null then
            if Synchronized_counter_pkg.decrement_returns_zero(Object.Ptr.Counter'access) then
                begin
                    Free(Object.Ptr.Data); -- implies: Finalize (Object.Ptr.Data);
                exception -- just for easier debugging        
                    when Ex : others =>
                        Ada.Text_Io.Put_Line ("Element_Smart_Cptr.Finalize " & Ada.Exceptions.Exception_Name(Ex));
                        Ada.Text_Io.Put_Line ("Info " & Ada.Exceptions.Exception_Information(Ex));
                end;
                Free(Object.Ptr);
            end if;
        end if;
    end Finalize;

    function Create(Contents : in Element_Cptr) return Smart_Pointer is
        Result : Smart_Pointer := (Ada.Finalization.Controlled with Ptr => null);
    begin
        if Contents /= null_value then
            Result.Ptr := new Counted_Data; -- Ref counter is implicitly 1.
            Result.Ptr.Data := Contents;
        end if; -- This way, equality for Smart_Pointer makes sense: Create(null) = Create(null)
        return Result;
    end Create;

    procedure Move (From : in out Element_Cptr; To : out Smart_Pointer) is
    begin
        To   := Create (from);
        From := null_value;
    end Move;

    function Read(This : Smart_Pointer) return Element_Cptr is
        Result : Element_Cptr;
    begin
        if This.Ptr = null
        then Result := null_value;
        else Result := This.Ptr.Data;
        end if;
        return Result;
    end Read;

    function "=" (L : Smart_Pointer; R : Element_Cptr) return Boolean is
    begin
        return Read(L) = R;
    end;

end ACD_Runtime.Smart_Pointer_pkg;
